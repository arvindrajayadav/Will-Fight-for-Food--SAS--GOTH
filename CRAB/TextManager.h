#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>

#ifdef __APPLE__
#include <SDL_ttf.h>
#elif __GNUC__
#include <SDL/SDL_ttf.h>
#else
#include <SDL_ttf.h>
#endif

#include "inputval.h"
#include "GameParam.h"
#include "ImageData.h"

enum TextAlign { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT };

extern const SDL_Color black, white, yellow, red, green, blue;

class TextManager
{
	//The collection of stored fonts
	std::vector<TTF_Font*> font;

	//The data stored in our cache - text and texture
	struct TextCacheUnit
	{
		std::string text;
		SDL_Color col;
		Uint16 font;

		ImageData img;
		bool empty;

		TextCacheUnit() { empty = true; col = black; }

		bool EqualCol(SDL_Color color) { return (col.r == color.r && col.g == color.g && col.b == color.b ); }
	};

	//Text Cache - done to avoid having to render a texture every time text is drawn
	std::vector<TextCacheUnit> cache;

	//The oldest element in the text cache
	int oldest;

	int Search(std::string text, SDL_Color col, Uint16 fontid);
	int FindFreeSlot();

public:
	TextManager(void){ oldest = 0; }
	~TextManager(void){}

	void Init();
	void Quit();

	TTF_Font* GetFont(const Uint16 fontid) { return font[fontid]; }
	SDL_Surface* RenderTextBlended(const Uint16 font, std::string text, SDL_Color col);

	void Draw(Sint16 x, Sint16 y, std::string text, SDL_Color color, Uint16 fontid = 0, TextAlign alignment = ALIGN_LEFT);
	void Draw(Sint16 x, Sint16 y, std::string text, SDL_Color color, Uint16 fontid, Uint16 line_width, Uint16 line_height);
};

extern TextManager gTextManager;

#endif //TEXTMANAGER_H