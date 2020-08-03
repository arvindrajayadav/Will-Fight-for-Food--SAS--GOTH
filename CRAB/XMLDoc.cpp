#include "XMLDoc.h"
#include <cstdio>

#ifdef __GNUC__
int fopen_s(FILE** file, const char * filename, const char * mode)
{
	if(file)
	{
		*file = fopen(filename,mode);
		return *file == NULL;
	}
	return 0;
}
#endif

XMLDoc::XMLDoc()
{
	xmltext = NULL;
}

XMLDoc::XMLDoc(std::string filename)
{
	xmltext = NULL;
	Load(filename);
}

void XMLDoc::Load(std::string filename)
{
	if(xmltext != NULL)
	{
		delete[] xmltext;
		confxml.clear();
	}

	FILE* file;
	if( fopen_s(&file, filename.c_str(), "rb") )
	{
		std::string error_msg = "Couldn't open " + filename + "\n";
		fprintf(stderr, error_msg.c_str());
		xmltext = NULL;
	}
	else
	{
		fseek(file, 0L, SEEK_END);

		long int length = ftell(file);
		xmltext = new char[length+1];
		xmltext[length] = '\0';

		fseek(file,0,SEEK_SET);
		long int nread = fread(xmltext, 1, length, file);
		if (nread < length)
		{
			// An error has occurred
			delete[] xmltext;
			xmltext = NULL;
		}

		fclose(file);
	}
	if (xmltext != NULL)
	{
		confxml.parse<0>(xmltext);
	}
}

const rapidxml::xml_document<>* XMLDoc::Doc() const
{
	if (xmltext != NULL)
	{
		return &confxml;
	}
	else
	{
		return NULL;
	}
}