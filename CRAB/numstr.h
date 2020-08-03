#ifndef NUMSTR_H
#define NUMSTR_H

#include <string>
#include <sstream>

template <typename T>
std::string NumberToString ( T Number )
{
	std::stringstream ss;
	ss << Number;
	return ss.str();
}

template <typename T>
T StringToNumber ( char *Text )//Text not by const reference so that the function can be used with a
{                               //character array as argument
	std::stringstream ss(Text);
	T result;
	return (ss >> result) ? result : 0;
}

template <typename T>
T StringToNumber (std::string Text )
{
	std::stringstream ss(Text.c_str());
	T result;
	return (ss >> result) ? result : 0;
}

#endif //NUMSTR_H