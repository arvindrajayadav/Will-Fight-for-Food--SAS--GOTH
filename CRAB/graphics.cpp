#include "graphics.h"

//------------------------------------------------------------------------
// Purpose: Image Loader Function
//------------------------------------------------------------------------
SDL_Surface *load_image(const char *filename)
{
	//The image that's loaded
	SDL_Surface* loadedImage = NULL;

	//The optimized surface that will be used
	SDL_Surface* optimizedImage = NULL;

	//Load the image
	loadedImage = IMG_Load(filename);

	//If the image loaded
	if (loadedImage != NULL)
	{
#ifdef __APPLE__
		SDL_PixelFormat pf;
		pf.palette = 0;
		pf.BitsPerPixel = 32;
		pf.BytesPerPixel = 4;
		pf.alpha = 255;
		pf.Rshift = pf.Rloss = pf.Gloss = pf.Bloss = pf.Aloss = pf.colorkey = 0;
		pf.Rmask = 0x000000ff;
		pf.Gshift = 8;
		pf.Gmask = 0x0000ff00;
		pf.Bshift = 16;
		pf.Bmask = 0x00ff0000;
		pf.Ashift = 24;
		pf.Amask = 0xff000000;

		optimizedImage = SDL_ConvertSurface(loadedImage, &pf, SDL_SWSURFACE);
#else
		//Create an optimized surface
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
#endif
		//Free the old surface
		SDL_FreeSurface(loadedImage);

		//If the surface was optimized
		if (optimizedImage != NULL)
		{
			//Color key surface
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
		}
	}

	//Return the optimized surface
	return optimizedImage;
}
//------------------------------------------------------------------------
// Purpose: Blits teh surface
//------------------------------------------------------------------------
void apply_surface(Sint16 x, Sint16 y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
	//Holds offsets
	SDL_Rect offset;

	//Get offsets
	offset.x = x;
	offset.y = y;

	//Blit
	SDL_BlitSurface(source, clip, destination, &offset);
}

//------------------------------------------------------------------------
// Purpose: Check for collision between 2 rectangles
//------------------------------------------------------------------------
bool CollideBoundingBox(SDL_Rect a, SDL_Rect b)
{
	if (b.x + b.w < a.x)	return false;	//just checking if their
	if (b.x > a.x + a.w)	return false;	//bounding boxes even touch

	if (b.y + b.h < a.y)	return false;
	if (b.y > a.y + a.h)	return false;

	return true; //bounding boxes intersect
}

bool InsideBoundingBox(Sint16 x, Sint16 y, SDL_Rect box)
{
	if (x > box.x &&	x < box.x + box.w && y > box.y && y < box.y + box.h)
		return true;

	return false;
}

void Clamp(int &val, int low, int high)
{
	if (val < low)
		val = low;
	else if (val > high)
		val = high;
}

//void ToggleFullScreen(SDL_Surface *screen)
//{
//	Uint32 flags = screen->flags; /* Save the current flags in case toggling fails */
//	screen = SDL_SetVideoMode(gScreenSettings.Width(), gScreenSettings.Height(), SCREEN_BPP, screen->flags | SDL_FULLSCREEN); /*Toggles FullScreen Mode */
//
//	if(screen == NULL) /* If toggle FullScreen failed, then switch back */
//		screen = SDL_SetVideoMode(gScreenSettings.Width(), gScreenSettings.Height(), SCREEN_BPP, flags);
//
//	if(screen == NULL)  /* If you can't switch back for some reason, then epic fail */
//		exit(1);
//}

template <typename T>
T GetPowerOf2(T num)
{
	--num;
	num |= num >> 1;
	num |= num >> 2;
	num |= num >> 4;
	num |= num >> 8;
	num |= num >> 16;
	++num;
	return num;
}

//------------------------------------------------------------------------
// Purpose: Draw a texture to the screen
//
// sourceID: the GLuint for the texture loaded in memory
// position: position on the screen to start drawing the texture
// sourceDimensions: the source texture's actual height and width
// clip: SDL-style crop window
//------------------------------------------------------------------------
void DrawTexture(GLuint sourceID, Vectorf position, Vectorf sourceDimensions, SDL_Rect* clip)
{
	Vectorf texOffset;
	Vectorf texCrop;
	Vectorf drawDims;

	if (clip == NULL)
	{
		texOffset = Vectorf(0, 0);
		texCrop = Vectorf(1, 1);
		drawDims = sourceDimensions;
	}
	else
	{
		//math our texture percentages
		float sourceW = sourceDimensions.X(), sourceH = sourceDimensions.Y();

		texOffset.X(clip->x / sourceW);
		texOffset.Y(clip->y / sourceH);

		//normalize offsets
		while (texOffset.X() > sourceW)
			texOffset.X(texOffset.X() - sourceW);

		while (texOffset.Y() > sourceH)
			texOffset.Y(texOffset.Y() - sourceH);

		texCrop.X(clip->w / sourceW);
		texCrop.Y(clip->h / sourceH);

		//assert(clip->h == static_cast<int>( floor(texCrop.Y() * sourceH + 0.5) ));

		//we will need to pass in a scale parameter eventually.  clip.w and .h are fine for now though
		Vectorf drawScale = Vectorf(clip->w / sourceW, clip->h / sourceH);

		drawDims.X(sourceW*drawScale.X());
		drawDims.Y(sourceH*drawScale.Y());
	}

	DrawTexture(sourceID, position, drawDims, texOffset, texCrop);
}

//------------------------------------------------------------------------
// Purpose: Draw a texture to the screen
//
// sourceID: the GLuint for the texture loaded in memory
// position: position on the screen to start drawing the texture
// drawDimensions: the height and width of which we want to draw the texture on the screen, not necessarily the actual dimensions
// textureOffset: CURRENTLY: offset in percent IDEALLY: where on the texture to start drawing (from the top left) in pixels
// textureCropWindow: CURRENTLY: window in percent IDEALLY: height and width on the texture of what to draw in pixels
//------------------------------------------------------------------------
void DrawTexture(GLuint sourceID, Vectorf position, Vectorf drawDimensions, Vectorf textureOffset, Vectorf textureCropWindow)
{
	// Bind the texture to which subsequent calls refer to
	glBindTexture(GL_TEXTURE_2D, sourceID);

	// Note: using arrays rather than immediate mode (glBegin / glEnd) because immediate mode is deprecated

	GLfloat positions[4 * 2];
	GLfloat texcoords[4 * 2];

	int posidx = 0;
	int texidx = 0;

	//Bottom-left vertex (corner)
	positions[posidx++] = position.X();
	positions[posidx++] = position.Y();
	texcoords[texidx++] = textureOffset.X();
	texcoords[texidx++] = textureOffset.Y();

	//Bottom-right vertex (corner)
	positions[posidx++] = (position.X() + drawDimensions.X());
	positions[posidx++] = position.Y();
	texcoords[texidx++] = (textureOffset.X() + textureCropWindow.X());
	texcoords[texidx++] = textureOffset.Y();

	//Top-right vertex (corner)
	positions[posidx++] = (position.X() + drawDimensions.X());
	positions[posidx++] = (position.Y() + drawDimensions.Y());
	texcoords[texidx++] = (textureOffset.X() + textureCropWindow.X());
	texcoords[texidx++] = (textureOffset.Y() + textureCropWindow.Y());

	//Top-left vertex (corner)
	positions[posidx++] = position.X();
	positions[posidx++] = (position.Y() + drawDimensions.Y());
	texcoords[texidx++] = textureOffset.X();
	texcoords[texidx++] = (textureOffset.Y() + textureCropWindow.Y());

	glVertexPointer(2, GL_FLOAT, 0, positions);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	glDrawArrays(GL_QUADS, 0, 4);
}