#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <string>
#include "graphics.h"

//Image data used in Asset Manager
class ImageData
{
	//The dimensions of the image
	float w, h;

	//The width and height of texture, these will be a power of 2
	Uint16 th, tw;

	//Relative depth of texture (UNUSED)
	float z;

	//The openGL texture associated with image
	GLuint texture;

public:
	ImageData()
	{
		w = 0.0;
		h = 0.0;
		tw = 0;
		th = 0;
		z = 0.0;
		texture = NULL;
	}
	~ImageData(){}

	//Load an openGL texture
	bool Load(std::string filename);
	bool Load(SDL_Surface *surface);

	//Draw the openGL texture
	void Draw(float x, float y, SDL_Rect *clip = NULL);

	//Delete the openGL texture
	void Delete();

	float W() { return w; }
	float H() { return h; }
};

#endif //IMAGEDATA_H