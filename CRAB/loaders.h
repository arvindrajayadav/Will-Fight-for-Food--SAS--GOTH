#ifndef LOADERS_H
#define LOADERS_H

#ifdef __GNUC__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include "rapidxml/rapidxml.hpp"
#include "numstr.h"

//Function to check if node is valid
//return true for valid, false otherwise
bool NodeValid(rapidxml::xml_node<char> *node, bool echo = true);

//Functions to load attributes from xml files
//return true on success, false on failure
bool LoadStr(std::string &val, std::string name, rapidxml::xml_node<char> *node, bool echo = true);

template <typename T>
bool LoadNum(T &val, std::string name, rapidxml::xml_node<char> *node, bool echo = true);

//Functions to load various type of objects
bool LoadRect(SDL_Rect &rect, rapidxml::xml_node<char> *node, bool echo = true);
bool LoadColor(SDL_Color &col, rapidxml::xml_node<char> *node, std::string r_name="r", std::string g_name="g", std::string b_name="b", bool echo = true);
bool LoadXY(Sint16 &x, Sint16 &y, rapidxml::xml_node<char> *node, bool echo = true);

#endif //LOADERS_H