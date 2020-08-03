#ifndef XMLDOC_H
#define XMLDOC_H

#include <string>
#include "rapidxml/rapidxml.hpp"

class XMLDoc
{
	rapidxml::xml_document<char> confxml;
	char *xmltext;

public:
	XMLDoc();
	XMLDoc(std::string  filename);

	//Load the text from the specified file into the rapidxml format
	//Each function that references it must parse it there
	void Load(std::string  filename);

	//Check if document is ready for parsing
	bool ready() const { return xmltext != NULL;}

	const rapidxml::xml_document<>* Doc() const;
	char* text() { return xmltext; }
};

#endif //XMLDOC_H