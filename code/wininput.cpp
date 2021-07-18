
static const int MOUSE_BUTTONS = 5;
static const int BUTTONS = 4;
static const int EXTRA_BUTTONS = 4;

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

// Windows implementation
void input_update()
{
	if(GetKeyState(VK_ESCAPE) & 0x8000)
	{
		gRunning = false;
		return;
	}

	for(int ii = 0; ii < BUTTONS; ++ii)
	{
		inputstate.buttons[ii].downprevious = inputstate.buttons[ii].down;
	}

	inputstate.buttons[0].down = GetKeyState(VK_UP) & 0x8000 || GetKeyState('W') & 0x8000
								|| GetKeyState('X') & 0x8000;
	inputstate.buttons[1].down = GetKeyState(VK_LEFT) & 0x8000 || GetKeyState('A') & 0x8000;
	inputstate.buttons[2].down = GetKeyState(VK_RIGHT) & 0x8000 || GetKeyState('D') & 0x8000;
	inputstate.buttons[3].down = GetKeyState(VK_SPACE) & 0x8000 || GetKeyState(VK_LCONTROL) & 0x8000 
								|| GetKeyState('Z') & 0x8000 || GetKeyState('C') & 0x8000
								|| GetKeyState(VK_RCONTROL) & 0x8000;

	for (int ii = 0; ii < BUTTONS; ++ii)
	{
		inputstate.buttons[ii].wentdown = (inputstate.buttons[ii].down == true && inputstate.buttons[ii].downprevious == false);
		inputstate.buttons[ii].wentup = (inputstate.buttons[ii].down == false && inputstate.buttons[ii].downprevious == true);
	}

	for(int ii = 0; ii < EXTRA_BUTTONS; ++ii)
	{
		inputstate.extrabuttons[ii].downprevious = inputstate.extrabuttons[ii].down;
	}

	inputstate.extrabuttons[0].down = !!(GetKeyState('1') & 0x8000);
	inputstate.extrabuttons[1].down = !!(GetKeyState('2') & 0x8000);
	inputstate.extrabuttons[2].down = !!(GetKeyState('3') & 0x8000);
	inputstate.extrabuttons[3].down = !!(GetKeyState('0') & 0x8000);

	for (int ii = 0; ii < EXTRA_BUTTONS; ++ii)
	{
		inputstate.extrabuttons[ii].wentdown = (inputstate.extrabuttons[ii].down == true && inputstate.extrabuttons[ii].downprevious == false);
		inputstate.extrabuttons[ii].wentup = (inputstate.extrabuttons[ii].down == false && inputstate.extrabuttons[ii].downprevious == true);
	}

	WinProcessMessages();

}

