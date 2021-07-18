
#include <SDL.h>

static const int MOUSE_BUTTONS = 5;
static const int BUTTONS = 4;
static const int EXTRA_BUTTONS = 4;

static const Uint8 *keyboardstate = SDL_GetKeyboardState(NULL);

struct input_mousebuttonstate
{
	bool downprevious;
	bool down;
	bool wentdown;
	bool wentup;
};

struct input_buttonstate
{
	bool down;
	bool downprevious;
	bool wentdown;
	bool wentup;
};

struct input_state
{
	input_buttonstate buttons[BUTTONS]; // up, left, right, shoot
	input_buttonstate extrabuttons[EXTRA_BUTTONS];
	int mousex;
	int mousey;
	int mousexprevious;
	int mouseyprevious;
	input_mousebuttonstate mousebutton[MOUSE_BUTTONS];
	int mousebuttondown;
	int mousebuttonwentdown;
	int mousebuttonwentup;
};

input_state inputstate;

// SDL implementation
void input_update()
{
	if(keyboardstate[SDL_SCANCODE_ESCAPE])
	{
		gRunning = false;
		return;
	}

	inputstate.mousexprevious = inputstate.mousex;
	inputstate.mouseyprevious = inputstate.mousey;
	SDL_GetMouseState( &inputstate.mousex, &inputstate.mousey );

	inputstate.mousebuttonwentdown = false;
	inputstate.mousebuttonwentup = false;

	for(int ii = 0; ii < MOUSE_BUTTONS; ++ii)
	{
		inputstate.mousebutton[ii].downprevious = inputstate.mousebutton[ii].down;
	}

	inputstate.mousebutton[0].down = ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0);
	inputstate.mousebutton[1].down = ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0);
	inputstate.mousebutton[2].down = ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
	inputstate.mousebutton[3].down = ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_X1)) != 0);
	inputstate.mousebutton[4].down = ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_X2)) != 0);
	inputstate.mousebuttondown = inputstate.mousebutton[0].down + inputstate.mousebutton[1].down + 
			inputstate.mousebutton[2].down + inputstate.mousebutton[3].down + inputstate.mousebutton[4].down;

	for (int ii = 0; ii < MOUSE_BUTTONS; ++ii)
	{
		inputstate.mousebutton[ii].wentdown = (inputstate.mousebutton[ii].down == true && inputstate.mousebutton[ii].downprevious == false);
		inputstate.mousebutton[ii].wentup = (inputstate.mousebutton[ii].down == false && inputstate.mousebutton[ii].downprevious == true);
		
		if(inputstate.mousebutton[ii].wentdown)
			inputstate.mousebuttonwentdown = true;
		if(inputstate.mousebutton[ii].wentup)
			inputstate.mousebuttonwentup = true;
	}

	for(int ii = 0; ii < BUTTONS; ++ii)
	{
		inputstate.buttons[ii].downprevious = inputstate.buttons[ii].down;
	}

	inputstate.buttons[0].down = keyboardstate[SDL_SCANCODE_UP] || keyboardstate[SDL_SCANCODE_W]
								|| keyboardstate[SDL_SCANCODE_X];
	inputstate.buttons[1].down = keyboardstate[SDL_SCANCODE_LEFT] || keyboardstate[SDL_SCANCODE_A];
	inputstate.buttons[2].down = keyboardstate[SDL_SCANCODE_RIGHT] || keyboardstate[SDL_SCANCODE_D];
	inputstate.buttons[3].down = keyboardstate[SDL_SCANCODE_SPACE] || keyboardstate[SDL_SCANCODE_LCTRL] 
								|| keyboardstate[SDL_SCANCODE_Z] || keyboardstate[SDL_SCANCODE_C]
								|| keyboardstate[SDL_SCANCODE_RCTRL];

	for (int ii = 0; ii < BUTTONS; ++ii)
	{
		inputstate.buttons[ii].wentdown = (inputstate.buttons[ii].down == true && inputstate.buttons[ii].downprevious == false);
		inputstate.buttons[ii].wentup = (inputstate.buttons[ii].down == false && inputstate.buttons[ii].downprevious == true);
	}

	for(int ii = 0; ii < EXTRA_BUTTONS; ++ii)
	{
		inputstate.extrabuttons[ii].downprevious = inputstate.extrabuttons[ii].down;
	}

	inputstate.extrabuttons[0].down = !!keyboardstate[SDL_SCANCODE_1];
	inputstate.extrabuttons[1].down = !!keyboardstate[SDL_SCANCODE_2];
	inputstate.extrabuttons[2].down = !!keyboardstate[SDL_SCANCODE_3];
	inputstate.extrabuttons[3].down = !!keyboardstate[SDL_SCANCODE_4];

	for (int ii = 0; ii < EXTRA_BUTTONS; ++ii)
	{
		inputstate.extrabuttons[ii].wentdown = (inputstate.extrabuttons[ii].down == true && inputstate.extrabuttons[ii].downprevious == false);
		inputstate.extrabuttons[ii].wentup = (inputstate.extrabuttons[ii].down == false && inputstate.extrabuttons[ii].downprevious == true);
	}


	SDL_Event event;

	while( SDL_PollEvent( &event ) != 0 )
	{
		if( event.type == SDL_QUIT )
		{
			gRunning = false;
			break;
		}
	}

}

