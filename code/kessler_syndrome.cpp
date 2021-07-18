
//#include <windows.h>
//#include <dsound.h>
#include <cmath> // cos sin sqrt abs
#include <vector>
#include <time.h> // time.h and random until i get something better
//#include <iacaMarks.h>

//#include <stdio.h> // errors etc... file handling later?
#if 0
	// msvc 2008 doesn't have std::tostring
#include <sstream>
template <typename T>
std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}
#endif

// useful assert macro
#define ASSERT(Expression) if(!(Expression)) {*(int *)0 = 0;}

#include "math.cpp"
#include "globals.cpp"
#include "random.cpp"
#include "lineclipper.cpp"

#include "winplatform.cpp"
#include "winabstraction.cpp"
#include "wininput.cpp"

#include "update.cpp"
#include "render.cpp"
#include "userinterface.cpp"

void buildplayership()
{
	playership[0].x = 0;
	playership[0].y = 9;
	playership[5].x = 0;
	playership[5].y = -9;

	playership[1].x = 5.5f;
	playership[1].y = -7;
	playership[3].x = 3.9f;
	playership[3].y = -3;
	playership[6].x = 2;
	playership[6].y = -3;

	playership[2].x = -playership[1].x;
	playership[2].y = playership[1].y;
	playership[4].x = -playership[3].x;
	playership[4].y = playership[3].y;
	playership[7].x = -playership[6].x;
	playership[7].y = playership[6].y;

	// increase size for reasons
	for(int ii = 0; ii < 8; ii++)
	{
		playership[ii] = playership[ii] * playershipsize;
	}
}

void buildUFO()
{
	ufo[0].x = 1.5;
	ufo[0].y = -3;
	ufo[2].x = 2.5;
	ufo[2].y = -1;
	ufo[4].x = 6;
	ufo[4].y = 1;
	ufo[6].x = 3;
	ufo[6].y = 3;

	// increasing size of even points since we copy these for symmetry
	for(int ii = 0; ii < 4; ii++)
	{
		ufo[ii*2] = ufo[ii*2] * 4;
	}

	ufo[1].x = -ufo[0].x;
	ufo[1].y = ufo[0].y;
	ufo[3].x = -ufo[2].x;
	ufo[3].y = ufo[2].y;
	ufo[5].x = -ufo[4].x;
	ufo[5].y = ufo[4].y;
	ufo[7].x = -ufo[6].x;
	ufo[7].y = ufo[6].y;
}

//int main( int argc, char* args[] )
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	rand_seed();

	// Start up Windows Platform
	if(WindowsInit(hInstance,"KESSLER SYNDROME",SCREEN_WIDTH,SCREEN_HEIGHT))
	{
		printf( "Windows Platform failed to initialize!\n" );
	}
	else
	{

		//Main loop flag
		bool quit = false;

		buildplayership();
		buildUFO();

		game_over = true;
		for(int ii = 0; ii < 10; ++ii)
		{
			new_asteroid();
		}

		//While application is running
		while( gRunning )
		{

			input_update();
			//ui_update();

			ks_update();

			ClearFramebuffer();
			ks_render();

			//ks_debugrenderborders();
			//ks_debugrender();

			ks_renderui();

			SwapBuffer();
			Wait();
		}
		timeEndPeriod(1);
	}

	return 0;
}
