#include "VarCondition.h"

void VarCondition :: Load(rapidxml::xml_node<char> *node)
{
	statement.clear();
	if(NodeValid(node,false))
	{
		for(rapidxml::xml_node<char> *n = node->first_node(); n != NULL; n = n->next_sibling())
		{
			Condition co;
			LoadStr(co.var,"var",n);

			std::string str;
			LoadStr(str,"rel",n);
			if(str == "or")
				co.op = OP_OR;
			else
				co.op = OP_AND;

			LoadStr(str,"prefix",n);
			if(str == "!")
				co.negate = true;
			else
				co.negate = false;

			statement.push_back(co);
		}
	}
}

void VarCondition :: Calc(VarMap &varmap)
{
	if(statement.empty() == false)
	{
		RelOp next_op = OP_AND;
		result = true;
		bool cur_result = false;

		for(std::vector<Condition>::iterator i = statement.begin(); i != statement.end(); ++i)
		{
			if(i->negate)
				cur_result = (varmap.count(i->var) == 0);
			else
				cur_result = (varmap.count(i->var) > 0);

			if(next_op == OP_AND)
				result = cur_result && result;
			else
				result = cur_result || result;

			next_op = i->op;
		}
	}
	else
		result = true;
}