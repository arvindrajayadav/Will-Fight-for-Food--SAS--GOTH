#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glew.h>
#define NO_SDL_GLEXT

#include "ScreenSettings.h"
#include "vectorf.h"

//SDL functions

SDL_Surface *load_image(const char *filename);
void apply_surface( Sint16 x, Sint16 y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
bool CollideBoundingBox(SDL_Rect a , SDL_Rect b);
bool InsideBoundingBox(Sint16 x, Sint16 y, SDL_Rect box);
void Clamp(int &val, int low, int high);

//DOES NOT WORK WITH OPENGL
//void ToggleFullScreen(SDL_Surface *screen);

//OpenGL functions

//Get nearest power of two, scaling up
template <typename T>
T GetPowerOf2(T num);

//Draw texture with cropping
void DrawTexture(GLuint sourceID, Vectorf position, Vectorf sourceDimensions, SDL_Rect* clip);
void DrawTexture(GLuint sourceID, Vectorf position, Vectorf drawDimensions, Vectorf textureOffset = Vectorf(0.0,0.0), Vectorf textureCropWindow = Vectorf(1.0,1.0) );

#endif //GRAPHICS_H