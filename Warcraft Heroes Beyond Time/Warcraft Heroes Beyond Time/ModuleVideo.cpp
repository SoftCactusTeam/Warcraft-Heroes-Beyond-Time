#include "Log.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePrinter.h"
#include "ModuleAudio.h"

#include "ModuleVideo.h"

ModuleVideo::ModuleVideo()
{
	name = "video";
}

ModuleVideo::~ModuleVideo()
{
}

bool ModuleVideo::Awake(pugi::xml_node &)
{
	return true;
}

bool ModuleVideo::Start()
{
	Initialize("video/Intro_Soft_Cactus.avi");
	App->audio->PlayFx(App->audio->introVideoFX);
	return true;
}

bool ModuleVideo::Update(float dt)
{
	if (!isVideoFinished)
		App->video->GrabAVIFrame();

	return true;
}

bool ModuleVideo::CleanUp()
{
	CloseAVI();
	return true;
}

void ModuleVideo::Initialize(char* path)
{
	OpenAVI(path);                  // Open The AVI File
}

void ModuleVideo::OpenAVI(LPCSTR path)
{
	AVIFileInit();                          // Opens The AVIFile Library

											/*TODO 2.1: Use AVIStreamOpenFromFile(...) to open a single stream from the AVI file.
											- The first parameter is a pointer to a buffer that receives the stream handle.
											- The second parameter is the path to the file.
											- The third parameter is the type of stream we want to open (in this case streamtypeVIDEO).
											- The fourth parameter is which video stream we want (there can be more than one), in this case: 0.
											*/
	if (AVIStreamOpenFromFile(&pavi, path, streamtypeVIDEO, 0, OF_READ, NULL) != 0) // Opens The AVI Stream
		LOG("Failed To Open The AVI Stream");



	// Uncomment this when finished TODO 2

	AVIStreamInfo(pavi, &psi, sizeof(psi));					// Reads Information About The Stream Into psi
	width = psi.rcFrame.right - psi.rcFrame.left;           // Width Is Right Side Of Frame Minus Left
	height = psi.rcFrame.bottom - psi.rcFrame.top;          // Height Is Bottom Of Frame Minus Top

	
	lastFrame = AVIStreamLength(pavi);						// The Last Frame Of The Stream

															// TODO 3.1: Use AVIStreamFrameOpen(...) to decompress video frames from the AVI file. On the second parameter you can pass AVIGETFRAMEF_BESTDISPLAYFMT to select the best display format.
															// Hint: this function returns a PGETFRAME
	pgf = AVIStreamGetFrameOpen(pavi, (LPBITMAPINFOHEADER)AVIGETFRAMEF_BESTDISPLAYFMT);              // Create The PGETFRAME Using Our Request Mode
	if (pgf == NULL)
		LOG("Failed To Open The AVI Frame");

	isVideoFinished = false;
}

bool ModuleVideo::GrabAVIFrame()
{
	//Uncomment this after you have finished TODO 3.
	LPBITMAPINFOHEADER lpbi;													 // Holds The Bitmap Header Information
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf, frame);					// Grab Data From The AVI Stream
	pdata = (char *)lpbi + lpbi->biSize + lpbi->biClrUsed * sizeof(RGBQUAD);    // Pointer To Data Returned By AVIStreamGetFrame
																				// (Skip The Header Info To Get To The Data)
																				// Convert Data To Requested Bitmap Format

																				/*TODO 5.1: Create a surface using the bitmap data we have above this TODO, and create the texture of the frame with that surface (use LoadSurface from textures module)
																				- pdata holds the texture data (pixels)
																				- biBitCount holds the depht in bits and is contained in the LPBITMAPINFOHEADER structure
																				- pitch is the length of a row of pixels in bytes (widht x 3)
																				*/
	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pdata, width, height, lpbi->biBitCount, width * 3, 0, 0, 0, 0);
	SDL_Texture* texture = App->textures->LoadSurface(surface);

	//TODO 6.1: Blit the texture of the frame.
	//App->render->Blit(texture, 0, 0, NULL, SDL_FLIP_VERTICAL);
	//App->printer->PrintSprite({ 0, 0 }, texture, { 0, 0, width, height }, 10000);
	App->render->BlitVideo(texture, 0, 0, NULL, SDL_FLIP_VERTICAL, 0);



	//TODO 7: Limit the change of the frame to one out of two times.
	// Hint: We want to blit a diferent frame only when our counter, i, is an even number.
	if (i % 2 == 0)
	{
		frame++;
	}
	i++;
	if (frame >= lastFrame)
	{
		frame = 0;
		isVideoFinished = true;
	}

	// TODO 5.2: Unload the texture and free the surface after the blit.
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);


	// BONUS. Window title with information about our video.
	//char   title[100];
	//mpf = AVIStreamSampleToTime(pavi, lastFrame) / lastFrame;        // Calculate Rough Milliseconds Per Frame
	//wsprintf(title, "AVI Player: Width: %d, Height: %d, Frames: %d, Miliseconds per frame: %d", width, height, lastFrame, mpf);
	//App->window->SetTitle(title);

	return true;
}

void ModuleVideo::CloseAVI()
{
	// TODO 3.2: Use AVIStreamGetFrameClose to deallocate the getframe resources.
	AVIStreamGetFrameClose(pgf);                // Deallocates The GetFrame Resources

												//TODO 2.2: Use AVIStreamRelease(...) to release the stream.
	AVIStreamRelease(pavi);                     // Release The Stream

	AVIFileExit();                              // Release The File
}