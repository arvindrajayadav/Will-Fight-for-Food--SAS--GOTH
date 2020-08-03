#include "ImageData.h"

//------------------------------------------------------------------------
// Purpose: Load texture
//------------------------------------------------------------------------
bool ImageData :: Load(SDL_Surface *surface)
{
	if(surface == NULL)
		return false;

	GLenum texture_format = GL_RGBA;
	GLint nOfColors = surface->format->BytesPerPixel;
	w = static_cast<float>(surface->w);
	h = static_cast<float>(surface->h);

	if(nOfColors == 4) // contains an alpha channel
	{
		if(surface->format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_BGRA;
	}
	else if (nOfColors == 3) // no alpha channel
	{
		if(surface->format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
	}

	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0, texture_format, GL_UNSIGNED_BYTE, surface->pixels );

	return true;
}

bool ImageData :: Load(std::string filename)
{
	SDL_Surface *surface = load_image(filename.c_str());
	bool result = Load(surface);
	SDL_FreeSurface(surface);
	return result;
}

//------------------------------------------------------------------------
// Purpose: Draw a texture to the screen without cropping
//------------------------------------------------------------------------
void ImageData:: Draw(float x, float y, SDL_Rect *clip)
{
	/*if(clip == NULL)
		DrawTexture(texture, Vectorf(x, y), Vectorf(w, h), Vectorf(0,0),Vectorf((1.0f*w)/tw,(1.0f*h)/th));
	else*/
		DrawTexture(texture, Vectorf(x, y), Vectorf(w, h), clip);
}

//------------------------------------------------------------------------
// Purpose: Delete texture data
//------------------------------------------------------------------------
void ImageData :: Delete()
{
	glDeleteTextures(1, &texture);
}