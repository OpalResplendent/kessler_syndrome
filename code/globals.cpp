
// TODO: remove const
int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


const int BOARD_WIDTH = 800;
const int BOARD_HEIGHT = 600;
const v2 BOARD_SIZE = v2{(float)BOARD_WIDTH,(float)BOARD_HEIGHT};
const v2 BOARD_HALF_SIZE = v2{(float)BOARD_WIDTH/2,(float)BOARD_HEIGHT/2};
const int BOARD_BORDER = 20;
const int ASTEROID_RESOLUTION = 12;

// colors for changing
unsigned char drawR = 255;
unsigned char drawG = 255;
unsigned char drawB = 255;
unsigned char drawA = 255;

bool gRunning = true;	// input needs this for quick quitting
bool gPaused = false;

int THRUSTERCHANNEL;
int DEATH;
int THRUSTER;

struct bullet
{
	v2 position;
	v2 velocity;
	bool active;
	int timer;
};

struct line
{
	v2 points[2];
	v2 velocity;
};

// points[2], position, velocity, direction, rotational_speed
struct line_segment
{
	v2 points[2];
	v2 position;
	v2 velocity;
	float direction;
	float rotational_speed;
};

// da lines, timer; lines: points[2], velocity
struct death_animation
{
	std::vector<line_segment> line_segments;
	int timer;
};

struct explosion
{
	v2 position;
	int timer;
};

// asteroid: v2 pos, v2 vel, f rot, f drot, i size, v2[ASTEROID_RESOLUTION] points
struct asteroid
{
	v2 position;
	v2 velocity;
	float rotation;		// this will be 0 and 2*pi
	float drotation;
	float size;
	v2 points[ASTEROID_RESOLUTION];
};

std::vector<bullet> bullets;
std::vector<asteroid> asteroids;
std::vector<explosion> explosions;
std::vector<death_animation> death_animations;

float player_direction;
v2 player_position;
v2 player_velocity;
unsigned long long player_score;
unsigned long long high_score = 9999;
unsigned long long next_life;
int player_lives;
bool player_dead;
int death_timer;
bool respawning;
bool game_over;
const int MIN_DIFFICULTY = 4;
const int MAX_DIFFICULTY = 10;
int difficulty = 4;
int selected_difficulty = 4;
bool beatlevel;
int beatleveltimer;

float thrust = 0.06;
float friction = 0.004;
float maxspeed = 1;
float turnspeed = 0.075;
float turnvelocity;
float turnacceleration = 0.019;
float turnfriction = 0.14;
float maxturnvelocity = 1;//0.16;
v2 playership[8] = {};
float playershipsize = 1.2;

float bullet_speed = 4;
const float SHOT_INERTIA = 1.0f;//0.5f;
const int MAX_BULLETS = 4;

v2 ufo[8] = {};
v2 ufo_position;
v2 ufo_velocity;
std::vector<bullet> ufo_bullets;
bool ufo_active;
int ufo_timer;
int ufo_shot_timer;
const int MAX_UFO_BULLETS = 2;

bool funky_mode = false;
bool funky_centered = false;
