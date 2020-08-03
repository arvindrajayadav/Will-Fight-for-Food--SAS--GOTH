#ifndef VECTORF_H
#define VECTORF_H

//------------------------------------------------------------------------
// Purpose: A simple float vector class
//------------------------------------------------------------------------
class Vectorf
{
protected:
	float x,y;

public:
	Vectorf(float X = 0.0, float Y = 0.0){ x = X; y = Y;}

	float X()const{ return x; }
	void X(float value){ x = value; }

	float Y()const{ return y; }
	void Y(float value){ y = value; }

	//constants
	static Vectorf Zero() { return Vectorf(0,0); }
	static Vectorf One() { return Vectorf(1,1); }
};

#endif //VECTORF_H