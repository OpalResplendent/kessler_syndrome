#include <SDL.h>
#undef main // stupid sdl hack
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#undef main // stupid sdl hack
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

// sound effects
std::vector<Mix_Chunk*> gSounds;
Mix_Chunk *gShot = NULL;
Mix_Chunk *gExplosion = NULL;
Mix_Chunk *gThruster = NULL;
Mix_Chunk *gDeath = NULL;

bool loadSound(std::string soundname)
{
	std::string fullpath;
	//fullpath = "../data/KS sound/" + soundname + ".wav";
	fullpath = soundname + ".wav";
	Mix_Chunk *tempsound = Mix_LoadWAV( fullpath.c_str() );
	gSounds.push_back(tempsound);
	return true;
}

enum SOUNDS{SHOT,THRUSTER,DEATH,UFO};

bool loadMedia()
{
	bool Return = true;

	loadSound("shot");
	loadSound("thruster");
	loadSound("death");
	loadSound("ufo");

	return(Return);
}

bool initSDL()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	} else {
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Kessler Syndrome", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		} else {
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			} else {
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
			}
		}
		gFont = TTF_OpenFont( "C:/Windows/Fonts/arial.ttf", 18 );
		if( gFont == NULL )
		{
			printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
			success = false;
		}

		//Initialize SDL Audio
	    if( SDL_Init(  SDL_INIT_AUDIO ) < 0 )
	    {
	        printf( "SDL audio could not initialize! SDL Error: %s\n", SDL_GetError() );
	        success = false;
	    } else {
	    	//Initialize SDL_mixer
        	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
            {
                printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                success = false;
            }
        }

        loadMedia();

	}

	return success;
}

void closeSDL()
{
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	for(int ii = 0; ii < gSounds.size(); ++ii)
	{
		Mix_FreeChunk( gSounds[ii] );
	}
	gSounds.clear();

	//Quit SDL subsystems
	SDL_Quit();
}