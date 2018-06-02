#include "p2Defs.h"
#include "Application.h"
#include "Log.h"
#include "FileSystem.h"
#include "PhysFS/include/physfs.h"
#include "SDL/include/SDL.h"


#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

FileSystem::FileSystem() : Module()
{
	name = "file_system";

	// need to be created before Awake so other modules can use it
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
	SDL_free(base_path);

	// By default we include executable's own directory
	// without this we won't be able to find config.xml :-(
	
	AddPath(".");
	AddPath("../Game");
	AddPath("Resources.zip");
}

// Destructor
FileSystem::~FileSystem()
{
	PHYSFS_deinit();
}

// Called before render is available
bool FileSystem::Awake(pugi::xml_node& fsNode)
{
	LOG("Loading File System");
	bool ret = true;

	// Add all paths in configuration in order
	for(pugi::xml_node path = fsNode.child("path"); path; path = path.next_sibling("path"))
	{
		AddPath(path.child_value());
	}

	// Ask SDL for a write dir
	char* write_path = SDL_GetPrefPath(App->GetOrganization(), App->GetTitle());

	if(PHYSFS_setWriteDir(write_path) == 0)
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());
	else
	{
		// We add the writing directory as a reading directory too with special mount point
		LOG("Writing directory is %s\n", write_path);
		AddPath(write_path, "Saves");
	}

	SDL_free(write_path);

	return ret;
}

// Called before quitting
bool FileSystem::CleanUp()
{
	if (PHYSFS_deinit() == 0)
		LOG("Cleaning FileSystem Error:%s", PHYSFS_getLastError());

	return true;
}

// Add a new zip file or folder
bool FileSystem::AddPath(const char* path_or_zip, const char* mount_point)
{
	bool ret = false;

	if(PHYSFS_mount(path_or_zip, mount_point, 1) == 0)
		LOG("File System error while adding a path or zip(%s): %s\n", path_or_zip, PHYSFS_getLastError());

	else
		ret = true;

	return ret;
}

// Check if a file exists
bool FileSystem::Exists(const char* file) const
{
	return PHYSFS_exists(file) != 0;
}

// Check if a file is a directory
bool FileSystem::IsDirectory(const char* file) const
{
	return PHYSFS_isDirectory(file) != 0;
}

// Read a whole file and put it in a new buffer
unsigned int FileSystem::Load(const char* file, char** buffer) const
{
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);

	if(fs_file != NULL)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fs_file);

		if(size > 0)
		{
			*buffer = new char[(uint)size];
			PHYSFS_sint64 readed = PHYSFS_readBytes(fs_file, *buffer, (PHYSFS_sint64)size);
			if(readed != size)
			{
				LOG("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE(*buffer);
			}
			else
				ret = (uint)readed;
		}

		if(PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

// Read a whole file and put it in a new buffer
SDL_RWops* FileSystem::Load(const char* file) const
{
	char* buffer;
	int size = Load(file, &buffer);

	if(size > 0)
	{
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size);
		if(r != NULL)
			r->close = close_sdl_rwops;

		return r;
	}
	else
		return NULL;
}

int close_sdl_rwops(SDL_RWops *rw)
{
	RELEASE(rw->hidden.mem.base);
	SDL_FreeRW(rw);
	return 0;
}

// Save a whole buffer to disk
unsigned int FileSystem::Save(const char* file, const char* buffer, unsigned int size) const
{
	unsigned int ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openWrite(file);

	if(fs_file != NULL)
	{
		PHYSFS_sint64 written = PHYSFS_writeBytes(fs_file, (const void*)buffer, size);
		if(written != size)
			LOG("File System error while writing to file %s: %s\n", file, PHYSFS_getLastError());
		else
			ret = (uint) written;

		if(PHYSFS_close(fs_file) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret == size;
}

bool FileSystem::deleteSavedGame()
{
	PHYSFS_File* file = PHYSFS_openWrite("savedgame.xml");
	PHYSFS_flush(file);	

	return PHYSFS_fileLength(file) == 0;
}

bool FileSystem::isGameSaved()
{
	std::string fileDir = savesFolder + std::string("savedgame.xml");
	PHYSFS_File* file = PHYSFS_openRead(fileDir.data());

	if (!file)
		return false;

	return PHYSFS_fileLength(file) != 0;
}

bool FileSystem::isInputSettingSaved() const
{
	std::string fileDir = savesFolder + std::string("inputSettings.xml");
	PHYSFS_File* file = PHYSFS_openRead(fileDir.data());

	if (!file)
		return false;

	return PHYSFS_fileLength(file) != 0;
}