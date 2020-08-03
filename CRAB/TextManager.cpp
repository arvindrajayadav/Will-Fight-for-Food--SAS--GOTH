#include "TextManager.h"

TextManager gTextManager;

const SDL_Color black = { 0, 0, 0 }, white = { 255, 255, 255 }, yellow = { 255, 216, 0 }, red = { 220, 10, 0 }, green = { 0, 127, 14 }, blue = { 0, 14, 145 };

//------------------------------------------------------------------------
// Purpose: Initialize, set cache etc
//------------------------------------------------------------------------
void TextManager :: Init()
{
	//Load the list of fonts
	XMLDoc font_list(gFilePath.font);
	if(font_list.ready())
	{
		rapidxml::xml_node<char> *node = font_list.Doc()->first_node("fonts");

		Uint16 num;
		if(LoadNum(num,"cache_size",node))
			cache.resize(num);
		else
			cache.resize(20);

		oldest = 0;

		for(rapidxml::xml_node<char> *n = node->first_node("font"); n != NULL; n = n->next_sibling("font"))
		{
			rapidxml::xml_attribute<char> *id, *path, *size;
			id = n->first_attribute("id");
			path = n->first_attribute("path");
			size = n->first_attribute("size");

			if(id != NULL && path != NULL && size != NULL)
			{
				Uint16 pos = StringToNumber<Uint16>(id->value());
				if(font.size() <= pos)
					font.resize(pos+1);
				font.at(pos) = TTF_OpenFont(path->value(),StringToNumber<Sint16>(size->value()));
#ifndef __GNUC__
				TTF_SetFontHinting(font.at(pos),TTF_HINTING_LIGHT);
#endif
			}
		}
	}
}

//------------------------------------------------------------------------
// Purpose: Search cache for rendered text
//------------------------------------------------------------------------
int TextManager :: Search(std::string text, SDL_Color col, Uint16 fontid)
{
	int pos = 0;
	for(std::vector<TextCacheUnit>::iterator i = cache.begin(); i != cache.end(); ++i, ++pos)
		if(i->empty == false && i->text == text && i->EqualCol(col) && i->font == fontid)
			return pos;

	return -1;
}

int TextManager :: FindFreeSlot()
{
	int pos = 0;
	for(std::vector<TextCacheUnit>::iterator i = cache.begin(); i != cache.end(); ++i, ++pos)
		if(i->empty)
			return pos;

	int ret = oldest;
	oldest = (oldest + 1) % cache.size();
	return ret;
}

//------------------------------------------------------------------------
// Purpose: Render the SDL surface for text
//------------------------------------------------------------------------
SDL_Surface* TextManager :: RenderTextBlended(Uint16 font, std::string text, SDL_Color col)
{
	if(text.empty())
		return TTF_RenderText_Blended(GetFont(font), " ", col);

	return TTF_RenderText_Blended(GetFont(font), text.c_str(), col);
}

//------------------------------------------------------------------------
// Purpose: Draw text
//------------------------------------------------------------------------
void TextManager :: Draw(Sint16 x, Sint16 y, std::string text, SDL_Color color, Uint16 fontid, TextAlign alignment)
{
	int pos = Search(text,color,fontid);

	if(pos == -1)
	{
		pos = FindFreeSlot();
		SDL_Surface *surf = RenderTextBlended(fontid,text,color);

		if(cache[pos].empty == false)
			cache[pos].img.Delete();
		else
			cache[pos].empty = false;

		cache[pos].text = text;
		cache[pos].col = color;
		cache[pos].font = fontid;

		cache[pos].img.Load(surf);

		SDL_FreeSurface(surf);
	}

	if(alignment == ALIGN_CENTER)
	{
		x -= cache[pos].img.W()/2;
		y -= cache[pos].img.H()/2;
	}
	else if(alignment == ALIGN_RIGHT)
	{
		x -= cache[pos].img.W();
	}

	cache[pos].img.Draw(x,y);
}

void TextManager :: Draw(Sint16 x, Sint16 y, std::string text, SDL_Color color, Uint16 fontid, Uint16 line_width, Uint16 line_height)
{
	Uint16 start_pos = 0, len = strlen(text.c_str());
	while(start_pos < len )
	{
		Uint16 end_pos = start_pos + 1;
		Sint16 last_interrupt = -1;
		std::string word;

		while (end_pos - start_pos <= line_width)
		{
			if( end_pos == len || text[end_pos] == '\n' )
			{
				last_interrupt = end_pos;
				break;
			}

			if( text[end_pos] == ' ' || text[end_pos] == ',' || text[end_pos] == '.')
				last_interrupt = end_pos;

			end_pos++;
		}

		int i;
		if ( last_interrupt >= 0) //wrap a word around
		{
			for( i=0; i < last_interrupt - start_pos; i++ )
				word += text[start_pos+i];

			start_pos = last_interrupt + 1;
		}
		else //word bigger than line, just thunk
		{
			for( i=0; i < end_pos - start_pos; i++ )
				word += text[start_pos+i];

			start_pos += line_width;
		}

		Draw(x,y,word,color,fontid);
		y += line_height;
	}
}

//------------------------------------------------------------------------
// Purpose: Quit
//------------------------------------------------------------------------
void TextManager :: Quit()
{
	for(std::vector<TTF_Font*>::iterator i = font.begin(); i != font.end(); ++i)
		TTF_CloseFont(*i);

	for(std::vector<TextCacheUnit>::iterator i = cache.begin(); i != cache.end(); ++i)
		if(i->empty ==  false)
			i->img.Delete();
}