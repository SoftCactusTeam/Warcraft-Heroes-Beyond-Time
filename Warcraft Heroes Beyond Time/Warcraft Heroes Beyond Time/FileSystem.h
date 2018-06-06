#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Module.h"

struct SDL_RWops;

int close_sdl_rwops(SDL_RWops *rw);

static const char* savesFolder = "Saves/";

class FileSystem : public Module
{
public:

	FileSystem();

	// Destructor
	virtual ~FileSystem();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Utility functions
	bool AddPath(const char* path_or_zip, const char* mount_point = NULL);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;
	const char* GetSaveDirectory() const
	{
		return savesFolder;
	}
	bool deleteSavedGame();
	bool isGameSaved();
	bool AreSettingsSaved() const;

	// Open for Read/Write
	unsigned int Load(const char* file, char** buffer) const;
	SDL_RWops* Load(const char* file) const;

	//Save a file into disk
	unsigned int Save(const char* file, const char* buffer, unsigned int size) const;

private:

};

#endif // __FILESYSTEM_H__