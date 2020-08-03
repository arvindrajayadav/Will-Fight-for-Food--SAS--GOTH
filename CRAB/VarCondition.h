#ifndef VARCOND_H
#define VARCOND_H

#include <vector>
#include "XMLDoc.h"
#include "gameevent.h"

class VarCondition
{
	enum RelOp { OP_AND, OP_OR };

	struct Condition
	{
		//The variable are we checking
		std::string var;

		//The operator to chain the next sequence with
		RelOp op;

		//Is the condition !var or var - true if !var
		bool negate;

		Condition() { negate = false; op = OP_AND; }
	};

	std::vector<Condition> statement;

	bool result;
public:
	VarCondition(void){ result = true; }
	~VarCondition(void){}

	void Load(rapidxml::xml_node<char> *node);
	void Calc(VarMap &varmap);

	bool Result() { return result; }

	//This overrides whatever the normal result is, use calculate to find value after this
	void Result(bool val) { result = val; }
};

#endif //VARCOND_H