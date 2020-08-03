#ifndef SENTENCEMAKER_H
#define SENTENCEMAKER_H

#include "AssetManager.h"
#include "TextManager.h"

struct SentenceSegment
{
	std::vector<std::string> sentence;
	Uint16 choice;

	SentenceSegment()
	{
		sentence.clear();
		choice = 65535;
	}

	void Reset() { choice = 65535; }

	bool Valid()
	{
		if(choice < sentence.size())
			return true;
		return false;
	}

	std::string SelectedSentence()
	{
		if(Valid())
			return sentence[choice];
		return "";
	}

	void Load(rapidxml::xml_node<char> *node);
};

class SentenceMaker
{
	SentenceSegment content, tone;
	std::string final;

	//The stuff needed to draw the sentence
	std::string bg;
	Sint16 bg_x, bg_y, text_x, text_y;
	Uint16 font;
	SDL_Color col;

	//The poses for the body language
	std::string body;
	Sint16 body_x, body_y;
	Uint16 body_choice;
	std::vector<SDL_Rect> clip;

public:
	SentenceMaker()
	{
		bg_x = 0;
		bg_y = 0;

		text_x = 0;
		text_y = 0;
		font = 0;
		col = black;

		body_x = 0;
		body_y = 0;
		body_choice = 0;
		clip.clear();
	}

	~SentenceMaker(){}

	void Load(std::string filename);
	void Draw();

	void MakeSentence(){ final = content.SelectedSentence() + tone.SelectedSentence(); }

	void SetContent(Uint16 choice) { content.choice = choice; }
	void SetTone(Uint16 choice) { tone.choice = choice; }
	void SetBody(Uint16 choice) { body_choice = choice + 1; }

	void Reset()
	{
		final.clear();
		content.Reset();
		tone.Reset();
		body_choice = 0;
	}
};

#endif //SENTENCEMAKER_H