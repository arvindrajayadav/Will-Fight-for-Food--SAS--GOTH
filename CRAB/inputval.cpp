#include "inputval.h"
#include "rapidxml/rapidxml_print.hpp"
#include <fstream>

InputVal gInputVal[I_TOTAL];

void LoadInputVal(std::string filename)
{
	XMLDoc control_list(filename);
	if(control_list.ready())
	{
		rapidxml::xml_node<char> *node = control_list.Doc()->first_node("controls");
		if(NodeValid(node))
		{
			int i=0;
			for(rapidxml::xml_node<char> *subnode = node->first_node(); subnode != NULL && i < I_TOTAL; subnode = subnode->next_sibling(), ++i)
			{
				gInputVal[i].name = subnode->name();
				gInputVal[i].key = (SDLKey)StringToNumber<Sint16>(subnode->first_attribute("key")->value());
				gInputVal[i].alt = (SDLKey)StringToNumber<Sint16>(subnode->first_attribute("alt")->value());
			}
		}
	}
}

void WriteInputVal(std::string filename)
{
	rapidxml::xml_document<char> doc;

	// xml declaration
	rapidxml::xml_node<char> *decl = doc.allocate_node(rapidxml::node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

	// root node
	rapidxml::xml_node<char> *root = doc.allocate_node(rapidxml::node_element, "controls");
	doc.append_node(root);

	// child node
	rapidxml::xml_node<char> *child[I_TOTAL];
	std::string keytext[I_TOTAL], alttext[I_TOTAL];
	for(int i=0; i<I_TOTAL; i++)
	{
		child[i] = doc.allocate_node(rapidxml::node_element, gInputVal[i].name.c_str());

		keytext[i] = NumberToString<Sint16>(gInputVal[i].key);
		child[i]->append_attribute(doc.allocate_attribute("key", keytext[i].c_str()));

		alttext[i] = NumberToString<Sint16>(gInputVal[i].alt);
		child[i]->append_attribute(doc.allocate_attribute("alt", alttext[i].c_str()));

		root->append_node(child[i]);
	}

	std::string xml_as_string;
	rapidxml::print(std::back_inserter(xml_as_string), doc);

	std::ofstream save(filename, std::ios::out);
	if(save.is_open())
	{
		save << xml_as_string;
		save.close();
	}

	doc.clear();
}