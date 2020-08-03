#include "LoadingScreen.h"

void LoadingScreen :: Init()
{
	background.Load("res/gfx/load/loading.png");

	anim[0].Load("res/gfx/load/load_1.png");
	anim[1].Load("res/gfx/load/load_2.png");
	anim[2].Load("res/gfx/load/load_3.png");
	anim[3].Load("res/gfx/load/load_4.png");

	w = anim[0].W()/2;
	h = anim[0].H() + 30;
}

void LoadingScreen :: Draw()
{
	background.Draw(0.0f, 0.0f);
	anim[frame].Draw(gScreenSettings.Width()/2 - w, gScreenSettings.Height() - h);

	//Update the screen
	SDL_GL_SwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT);

	if(!timer.Started())
		timer.start();

	if(timer.get_ticks() > 125)
	{
		timer.stop();
		frame = (frame + 1) % 4;
	}
}

void LoadingScreen :: Quit()
{
	background.Delete();

	anim[0].Delete();
	anim[1].Delete();
	anim[2].Delete();
	anim[3].Delete();
}

LoadingScreen gLoadScreen;