#pragma once

//ML Detection Extension
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

// SDL libs
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")

// OpenGL libs
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"Glu32.lib")

// SDL extension libs 
#pragma comment(lib, "SDL2_image.lib")  
#pragma comment(lib, "SDL2_ttf.lib") 
#pragma comment(lib, "SDL2_mixer.lib")  

// SDL and OpenGL Includes
#pragma warning(disable : 26812)
#pragma warning(disable : 4820)
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_ttf.h> 
#include <SDL_mixer.h> 
#include <SDL_image.h>

#pragma warning(default : 26812)
#include "structs.h"

