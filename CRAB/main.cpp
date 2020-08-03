//The headers
#include "app.h"

//------------------------------------------------------------------------
// Purpose: The Main Function
//------------------------------------------------------------------------
int main( int argc, char* args[] )
{
	App app;

	if(app.Init())
		app.Run();

	return 0;
}