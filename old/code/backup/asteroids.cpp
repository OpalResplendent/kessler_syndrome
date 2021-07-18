
/*
	TODO:
	Controller support
	
	FIX:
	sounds - thruster sound doesn't work right with sound setup
	death animation for player ship
	death animation for ufo
	
	Maybe:
	make add_to_score function
	make kill_player function (kind of already a thing in explode player)
	Seperate worldspace and screenspace
	change position to int + float?
	tail fin breaking off?
	Writing high score to file?

	DONE:
	FAke Wraparound drawing - just asteroids
	UFO
	Thruster Sound
*/

#include <string> // included in ui

	// msvc 2008 doesn't have std::tostring
#include <sstream>
template <typename T>
std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

#include <windows.h>
#include <vector>

#include <stdio.h> // errors etc... file handling later?
#include <cmath> // cos sin sqrt abs

#include "random.cpp"
#include "math.cpp"

// useful assert macro
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}

//Screen dimension constants (sdl would like this)
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
// const int SCREEN_FPS = 60;
// const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

// colors for changing
unsigned char drawR = 255;
unsigned char drawG = 255;
unsigned char drawB = 255;
unsigned char drawA = 255;

bool gRunning = true;	// input needs this for quick quitting

#include "SDL.cpp"
#include "abstraction.cpp" // currently v2 is in math
#include "input.cpp" // currently setup to take inputs from SDL
#include "ui.cpp"	// tied to SDL for font rendering

int SDL_THRUSTERCHANNEL;

const int BOARD_X_OFFSET = 0;
const int BOARD_Y_OFFSET = 0;
const int BOARD_WIDTH = SCREEN_WIDTH-BOARD_X_OFFSET;
const int BOARD_HEIGHT = SCREEN_HEIGHT-BOARD_Y_OFFSET;
const int BOARD_BORDER = 20;

const int ASTEROID_RESOLUTION = 12;

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
	std::vector<line> lines;
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
long player_score;
long high_score;
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
const int MAX_BULLETS = 4;

v2 ufo[8] = {};
v2 ufo_position;
v2 ufo_velocity;
std::vector<bullet> ufo_bullets;
bool ufo_active;
int ufo_timer;
int ufo_shot_timer;
const int MAX_UFO_BULLETS = 2;

float debug_max;
float debug_total;
float debug_number;

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

void explode_asteroid(asteroid tempasteroid) // actually rotating the temp asteroid
{
	death_animation tempdeath;
	tempdeath.timer = 100;

	float size = tempasteroid.size;
	size = (size) * sqrt(size) + 11/difficulty;

	float rotation = tempasteroid.rotation;
	float tempsin = sin(rotation);
	float tempcos = cos(rotation);
	
	for(int ii = 0; ii < ASTEROID_RESOLUTION; ++ii)
	{
		tempasteroid.points[ii] = tempasteroid.points[ii] * size;
		rotatev2(tempasteroid.points[ii], tempsin, tempcos);
	}
	
	// points[2], position, velocity, direction, rotational_speed
	line_segment temp_ls;

	line templine;
	v2 pointA = tempasteroid.points[ASTEROID_RESOLUTION-1];
	v2 pointB;

	for(int ii = 0; ii < ASTEROID_RESOLUTION; ++ii)
	{
		pointB = pointA;
		pointA = tempasteroid.points[ii];

		// shrink the edges by a bit
		templine.velocity = (((pointA + pointB) / 2));
		templine.points[0] = (pointA + templine.velocity)/2 + tempasteroid.position;
		templine.points[1] = (pointB + templine.velocity)/2 + tempasteroid.position;

		temp_ls.position = (((pointA + pointB) / 2));
		temp_ls.points[0] = (pointA + temp_ls.position)/2;
		temp_ls.points[1] = (pointB + temp_ls.position)/2;
		temp_ls.points[0] = temp_ls.points[0] - temp_ls.position;
		temp_ls.points[1] = temp_ls.points[1] - temp_ls.position;

		// slow down the velocity a bit for effect
		templine.velocity = templine.velocity / 80;

		temp_ls.velocity = temp_ls.position / 80;

		// calculate the normal and orient by scaled rotation
		v2 normal = {-templine.velocity.y,templine.velocity.x};
		normal = normal * (tempasteroid.drotation * 100);

		v2 tempnormal = {-temp_ls.position.y,temp_ls.position.x};
		tempnormal = tempnormal * (tempasteroid.drotation);

		// add normal and inherit velocity from parent (should)
		templine.velocity = templine.velocity + normal;
		templine.velocity = templine.velocity + tempasteroid.velocity;

		temp_ls.velocity = temp_ls.velocity + normal + tempasteroid.velocity;

		// we can calulate based on distance from center at each point and their relative distances or something
		// but this seems reasonable as well
		temp_ls.rotational_speed = temp_ls.points[1].x - temp_ls.points[0].x + temp_ls.points[1].y - temp_ls.points[0].y;
		temp_ls.rotational_speed = temp_ls.rotational_speed / 80;// + tempasteroid.drotation;
		
		tempdeath.lines.push_back(templine);

		temp_ls.position = tempasteroid.position + temp_ls.position;
		temp_ls.direction = tempasteroid.rotation;

		tempdeath.line_segments.push_back(temp_ls);
	}

	death_animations.push_back(tempdeath);
}

// bug this isn't correct ( too many links)
void explode_ship()
{
	death_animation tempdeath;
	tempdeath.timer = 100;

	float rotation = player_direction;
	float tempsin = sin(rotation);
	float tempcos = cos(rotation);

	int cycles = 6 + (2 * inputstate.buttons[0].down);

	line templine;
	v2 pointA = playership[cycles-1];
	rotatev2(pointA, tempsin, tempcos);
	v2 pointB;

	for(int ii = 0; ii < cycles; ++ii)
	{
		pointB = pointA;
		pointA = playership[ii];
		rotatev2(pointA, tempsin, tempcos);

		// shrink the edges by a bit
		templine.velocity = (((pointA + pointB) / 2));
		templine.points[0] = (pointA + templine.velocity)/2 + player_position;
		templine.points[1] = (pointB + templine.velocity)/2 + player_position;

		// slow down the velocity a bit for effect
		templine.velocity = templine.velocity / 80;

		// calculate the normal and orient by scaled rotation
		v2 normal = {-templine.velocity.y,templine.velocity.x};
		normal = normal * (turnvelocity * 100);

		// add normal and inherit velocity from parent (should)
		templine.velocity = templine.velocity + normal;
		templine.velocity = templine.velocity + player_velocity;
		
		tempdeath.lines.push_back(templine);
	}

	death_animations.push_back(tempdeath);
	PlaySound(DEATH);
	SoundVolume(SDL_THRUSTERCHANNEL,0.0f);

	//explosion tempexplosion = {player_position.x, player_position.y, 60};
	//explosions.push_back(tempexplosion);
				
}
// bug this isn't correct (not enough links)
void explode_UFO() // actually rotating the temp asteroid
{
	death_animation tempdeath;
	tempdeath.timer = 100;

	line templine;
	v2 pointA = ufo[7];
	v2 pointB;

	for(int ii = 0; ii < 8; ++ii)
	{
		pointB = pointA;
		pointA = ufo[ii];

		// shrink the edges by a bit
		templine.velocity = (((pointA + pointB) / 2));
		templine.points[0] = (pointA + templine.velocity)/2 + ufo_position;
		templine.points[1] = (pointB + templine.velocity)/2 + ufo_position;

		// slow down the velocity a bit for effect
		templine.velocity = templine.velocity / 80;

		// calculate the normal and orient by scaled rotation
		v2 normal = {-templine.velocity.y,templine.velocity.x};
		normal = normal * (turnvelocity * 100);

		// add normal and inherit velocity from parent (should)
		templine.velocity = templine.velocity + normal;
		templine.velocity = templine.velocity + ufo_velocity;
		
		tempdeath.lines.push_back(templine);
	}

	death_animations.push_back(tempdeath);
}

void renderplayer(v2 position, float rotation)
{
	float tempcos = cos(rotation);
	float tempsin = sin(rotation);

	v2 vec0 = playership[0];
	rotatev2(vec0, tempsin, tempcos);
	vec0 = vec0 + position;

	v2 vec1 = playership[1];
	rotatev2(vec1, sin(rotation), cos(rotation));
	vec1 = vec1 + position;
	DrawLine(vec0,vec1);

	vec1 = playership[2];
	rotatev2(vec1, sin(rotation), cos(rotation));
	vec1 = vec1 + position;
	DrawLine(vec0,vec1);

	vec0 = playership[3];
	rotatev2(vec0, sin(rotation), cos(rotation));
	vec0 = vec0 + position;

	vec1 = playership[4];
	rotatev2(vec1, sin(rotation), cos(rotation));
	vec1 = vec1 + position;
	DrawLine(vec0,vec1);

	if(inputstate.buttons[0].down)
	{
		vec0 = playership[5];
		rotatev2(vec0, sin(rotation), cos(rotation));
		vec0 = vec0 + position;

		vec1 = playership[6];
		rotatev2(vec1, sin(rotation), cos(rotation));
		vec1 = vec1 + position;
		DrawLine(vec0,vec1);

		vec1 = playership[7];
		rotatev2(vec1, sin(rotation), cos(rotation));
		vec1 = vec1 + position;
		DrawLine(vec0,vec1);
	}
}

void renderasteroid(v2 position, size_t incasteroid)
{
	asteroid *tempasteroid = &asteroids[incasteroid];
	float rotation = tempasteroid->rotation;
	float tempcos = cos(rotation);
	float tempsin = sin(rotation);

	float size = tempasteroid->size;
	size = (size) * sqrt(size) + 11/difficulty;

	v2 vec0 = tempasteroid->points[ASTEROID_RESOLUTION-1];
	rotatev2(vec0, tempsin, tempcos);
	vec0 = vec0 * size + position;

	v2 vec1;

	for(int jj = 0; jj < ASTEROID_RESOLUTION; ++jj)
	{
		vec1 = vec0;

		vec0 = tempasteroid->points[jj];
		rotatev2(vec0, tempsin, tempcos);

		vec0 = vec0 * size + position;

		DrawLine(vec0,vec1);
	}
}

void renderUFO()
{
	v2 position = {SCREEN_WIDTH/2,SCREEN_HEIGHT/2 - 100};

	DrawLine(ufo[0] + ufo_position, ufo[1] + ufo_position);
	DrawLine(ufo[0] + ufo_position, ufo[2] + ufo_position);
	DrawLine(ufo[1] + ufo_position, ufo[3] + ufo_position);
	DrawLine(ufo[2] + ufo_position, ufo[3] + ufo_position);
	DrawLine(ufo[2] + ufo_position, ufo[4] + ufo_position);
	DrawLine(ufo[3] + ufo_position, ufo[5] + ufo_position);
	DrawLine(ufo[4] + ufo_position, ufo[5] + ufo_position);
	DrawLine(ufo[4] + ufo_position, ufo[6] + ufo_position);
	DrawLine(ufo[5] + ufo_position, ufo[7] + ufo_position);
	DrawLine(ufo[6] + ufo_position, ufo[7] + ufo_position);
}

bool within_distance(v2 *inca, v2 *incb, float range)
{
	bool Return;
	float distance = sqrt(pow((inca->x-incb->x),2)+pow(inca->y-incb->y,2));

	Return = distance < range;

	return(Return);
}

v3 CrossProduct(v3 u, v3 v)
{
	v3 Return = { 	u.y * v.z - u.z * v.y,
					u.x * v.z - u.z * v.x,
					u.x * v.y - u.y * v.x };
	return(Return);
}

float DotProduct(v3 u, v3 v)
{
	float Return = u.x * v.x + u.y * v.y + u.z * v.z;
	return(Return);
}

bool SameSide(v3 p1, v3 p2, v3 a, v3 b)
{
	bool Return = false;
    v3 cp1 = CrossProduct(b-a, p1-a);
	v3 cp2 = CrossProduct(b-a, p2-a);
    if( DotProduct(cp1,cp2) >= 0)
    {
    	Return = true;
    }
    return(Return);
}

bool PointInTriangle(v2 incpoint, v2 incvecA, v2 incvecB, v2 incvecC)
{
	bool Return  = false;

	v3 p = {incpoint.x,incpoint.y, 0};
	v3 a = {incvecA.x,incvecA.y,0};
	v3 b = {incvecB.x,incvecB.y,0};
	v3 c = {incvecC.x,incvecC.y,0};

	if ( SameSide(p, a, b, c) && SameSide(p, b, a, c) && SameSide(p, c, a, b) )
    {
    	Return = true;
    }
    
    return(Return);
}

bool collide_asteroid(v2 incpoint, asteroid targetasteroid)
{
	bool Return = false;

	v2 position = targetasteroid.position;
	float size = (targetasteroid.size) * sqrt(targetasteroid.size) + 11.0f / difficulty;
	float bounds = size * 12; // usually distance is no more than 30 at size 3 but fuck it

	// let's move stuff near the far edges to close negative (point and asteroid)
	float distance = distancev2(incpoint, position);
	if(distance > 250)
	{
		clamp_wrap(incpoint.x, -200, BOARD_WIDTH-200);
		clamp_wrap(position.x, -200, BOARD_WIDTH-200);
		clamp_wrap(incpoint.y, -200, BOARD_HEIGHT-200);
		clamp_wrap(position.y, -200, BOARD_HEIGHT-200);
	}
	// check rough collide
	if( valueinside(incpoint.x,position.x-bounds,position.x+bounds) 
		&& valueinside(incpoint.y,position.y-bounds,position.y+bounds) )
	{
		float rotation = targetasteroid.rotation;
		float tempcos = cos(rotation);
		float tempsin = sin(rotation);

		// could probably find direction to player and infer which two points it should be between

		v2 tempvecA = targetasteroid.points[ASTEROID_RESOLUTION-1];
		rotatev2(tempvecA, tempsin, tempcos);
		tempvecA = tempvecA * size + position;

		v2 tempvecB;

		for(int ii = 0; ii < ASTEROID_RESOLUTION; ++ii)
		{
			tempvecB = tempvecA;

			tempvecA = targetasteroid.points[ii];
			rotatev2(tempvecA, tempsin, tempcos);
			tempvecA = tempvecA * size + position;

			if(PointInTriangle(incpoint, position, tempvecA, tempvecB))
			{
				Return = true;
				ii = ASTEROID_RESOLUTION;
			}
		}
	}
	return(Return);
}

void new_asteroid()
{
	// default size = 3->2->1 0 is dead
	//float size = frand(1,3);
	float size = 3;

	v2 tempposition;
	do
	{
		///tempposition = {frand((float)BOARD_Y_OFFSET,(float)(BOARD_WIDTH+BOARD_Y_OFFSET)), frand((float)BOARD_Y_OFFSET,(float)(BOARD_HEIGHT+BOARD_Y_OFFSET))};
		tempposition.x = frand((float)BOARD_X_OFFSET,(float)(BOARD_WIDTH+BOARD_X_OFFSET));
		tempposition.y = frand((float)BOARD_Y_OFFSET,(float)(BOARD_HEIGHT+BOARD_Y_OFFSET));
	} while ( within_distance(&player_position, &tempposition, 100.0f) );

	// asteroid: v2 pos, v2 vel, f rot, f drot, int size, v2[ASTEROID_RESOLUTION] points
	asteroid tempasteroid;
	tempasteroid.position = tempposition;
	tempasteroid.velocity.x = frand(-0.1,+0.1);
	tempasteroid.velocity.y = frand(-0.1,+0.1);
	tempasteroid.rotation = frand(-M_PI,M_PI);
	tempasteroid.drotation = frand(-0.02,+0.02);
	tempasteroid.size = size;

	asteroids.push_back(tempasteroid);

	float distance, rotmod, tempcos, tempsin;
	float pifrac = (M_PI*2.0f)/ASTEROID_RESOLUTION;

	for(int ii = 0; ii < ASTEROID_RESOLUTION; ++ii)
	{
		rotmod = pifrac*ii;
		tempcos = cos(rotmod);
		tempsin = sin(rotmod);

		distance = frand(2,7);
		tempposition.x = distance*(tempcos-tempsin);
		tempposition.y = distance*(tempcos+tempsin);
		//tempposition.x += frand()

		// debug distance
		v2 debugvec = {0,0};
		float debug_distance = distancev2(tempposition,debugvec) * size;
		if(debug_distance > debug_max)
			debug_max = debug_distance;
		debug_total += debug_distance;
		debug_number++;
		Assert(debug_distance < 30);

		asteroids.back().points[ii] = tempposition;
	}
}

void split_asteroid(size_t splitnumber)
{
	if(asteroids.size() > 0)
	{
		// totally arbitrary scoring system
		asteroid parentasteroid = asteroids.at(splitnumber);
		explode_asteroid(parentasteroid);
		float size = parentasteroid.size;

		if(!player_dead)
		{
			player_score += (int)(size*size*10 + size * 50 + difficulty * 10);
		}

		size--;

		if(size > 0)
		{
			// asteroid: v2 pos, v2 vel,
			// f rot, f drot, int size, v2[ASTEROID_RESOLUTION] points
			for(int ii = 0; ii < difficulty - size; ++ii)
			{
				//float variance = 2.0f; 
				//v2 dvelocity = {frand(-variance,+variance),frand(-variance,+variance)};
				//clamp_v2magnitude(dvelocity,0,variance);
				float variance = 2.0f + (difficulty / 7);
				float magnitude = frand(0.5f, variance);
				float direction = frand(0,M_PI * 2.0f);
				float tempsin = sin(direction);
				float tempcos = cos(direction);
				v2 dvelocity = {0,magnitude};
				rotatev2(dvelocity,tempsin,tempcos);

				asteroid tempasteroid;
				tempasteroid.position.x = parentasteroid.position.x+frand(-4,+4)+dvelocity.x;
				tempasteroid.position.y = parentasteroid.position.y+frand(-4,+4)+dvelocity.y;
				tempasteroid.velocity.x = parentasteroid.velocity.x+dvelocity.x;
				tempasteroid.velocity.y = parentasteroid.velocity.y+dvelocity.y;
				tempasteroid.rotation = parentasteroid.rotation + frand(-3,3);
				tempasteroid.drotation = parentasteroid.drotation + parentasteroid.drotation * frand(-0.5,1.5);
				tempasteroid.size = parentasteroid.size-1;

				asteroids.push_back(tempasteroid);

				for(int jj = 0; jj < ASTEROID_RESOLUTION; ++jj)
				{
					size_t back = asteroids.size()-1;
					asteroids[back].points[jj] = parentasteroid.points[jj] * frand(0.8,1.2);
				}
			}
		}

		std::swap(asteroids[splitnumber],asteroids[asteroids.size()-1]);
		asteroids.pop_back();
	}
}



void constrain_to_board(v2 *inc)
{
	// might be able to do this faster to do this with if statements, or with something hacky?
	// only called like 30 times per frame, so it should be fine
	clamp_wrap(inc->x, (float)BOARD_X_OFFSET, (float)(BOARD_WIDTH+BOARD_X_OFFSET));
	clamp_wrap(inc->y, (float)BOARD_Y_OFFSET, (float)(BOARD_HEIGHT+BOARD_Y_OFFSET));
}

void restartgame()
{
	StopSounds();
	SDL_THRUSTERCHANNEL = LoopSound(THRUSTER);
	SoundVolume(SDL_THRUSTERCHANNEL,0.0f);

	player_position.x = (BOARD_X_OFFSET + BOARD_WIDTH) / 2;
	player_position.y = (BOARD_Y_OFFSET + BOARD_HEIGHT) / 2;
	player_velocity.x = 0;
	player_velocity.y = 0;
	player_lives = 3;
	player_score = 0;
	player_dead = false;
	game_over = false;
	beatlevel = false;

	ufo_active = false;
	ufo_timer = 1800 + irand(0,600);

	if(difficulty == 0)
	{
		difficulty = selected_difficulty;
	}

	bullets.clear();
	ufo_bullets.clear();
	asteroids.clear();
	explosions.clear();
		
	for(int ii = 0; ii < 3 + difficulty; ++ii)
	{
		new_asteroid();
	}

}

//int main( int argc, char* args[] )
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	rand_seed();
	
	//Start up SDL and create window
	if( !initSDL() )
	{
		printf( "SDL failed to initialize!\n" );
	}
	else
	{

		//Main loop flag
		bool quit = false;

		buildplayership();
		buildUFO();

		//restartgame();

		game_over = true;
		for(int ii = 0; ii < 10; ++ii)
		{
			new_asteroid();
		}

		//While application is running
		while( gRunning )
		{

	//*******************//
	//*		INPUT		*//
	//*******************//

			input_update();
			ui_update();

	//*******************//
	//*		UPDATE		*//
	//*******************//
			
			if(player_score > high_score)
			{
				high_score = player_score;
			}

			if(!game_over)
			{
				//player_direction += turnspeed*(inputstate.buttons[2].down - inputstate.buttons[1].down);
				turnvelocity += turnacceleration * (inputstate.buttons[2].down - inputstate.buttons[1].down);
				turnvelocity = turnvelocity * (1 - turnfriction);
				if(inputstate.buttons[0].down)
				{
					turnvelocity = turnvelocity * (1 - turnfriction);
				}
				clamp(turnvelocity,-maxturnvelocity,maxturnvelocity);
				player_direction += turnvelocity;
				clamp_wrap(player_direction,-M_PI,M_PI);
	
				if(!player_dead)
				{
					player_velocity = player_velocity * (1-friction);
					player_position = player_position + player_velocity;
					constrain_to_board(&player_position);
	
					float tempsin = sin(player_direction);
					float tempcos = cos(player_direction);
	
					v2 psvec = {0,1};
					rotatev2(psvec,tempsin,tempcos);
	
					if(inputstate.buttons[0].down)
					{
						player_velocity = (player_velocity + (thrust * psvec));
					}

					if(inputstate.buttons[0].wentdown)
					{
						SoundVolume(SDL_THRUSTERCHANNEL,0.4f);
					}
					if(inputstate.buttons[0].wentup)
					{
						SoundVolume(SDL_THRUSTERCHANNEL,0.0f);
					}
	
					if(inputstate.buttons[3].wentdown)
					{
						if(bullets.size() < MAX_BULLETS)
						{
							bullet tempbullet;
							tempbullet.active = true;
							//tempbullet.position = player_position + 10.0f * psvec;
							tempbullet.position = playership[0];
							rotatev2(tempbullet.position,tempsin,tempcos);
							tempbullet.position = tempbullet.position + player_position;
							tempbullet.velocity = player_velocity * 0.5f + bullet_speed * psvec;
							tempbullet.timer = 120;
							bullets.push_back(tempbullet);
							PlaySound(0);
						}
					}
				} else {
					if(inputstate.buttons[3].down && death_timer == 0)
					{
						respawning = true;
						
						player_position.x = (BOARD_X_OFFSET + BOARD_WIDTH) / 2;
						player_position.y = (BOARD_Y_OFFSET + BOARD_HEIGHT) / 2;
					}
				}
			}

			if(bullets.size() > 0)
			{
				for(int ii = 0; ii < bullets.size(); ++ii)
				{
					if(--bullets[ii].timer > 0 && bullets[ii].active == true)
					{
						bullets[ii].position.x += bullets[ii].velocity.x;
						bullets[ii].position.y += bullets[ii].velocity.y;
						
						constrain_to_board(&bullets[ii].position);
							
					} else {
						bullets[ii].active = false;
						size_t back = bullets.size() - 1;
						if(ii < back)
						{
							bullets[back].position.x += bullets[back].velocity.x;
							bullets[back].position.y += bullets[back].velocity.y;
							constrain_to_board(&bullets[back].position);
							std::swap(bullets[ii],bullets[back]);
						}
						bullets.pop_back();
					}
				}
			}

			// update UFO
			if(ufo_active == true)
			{
				ufo_position = ufo_position + ufo_velocity;
				constrain_to_board(&ufo_position);
				if(--ufo_shot_timer < 0)
				{
					bullet tempbullet;
					ufo_shot_timer = 100 + irand(0,200);
					tempbullet.active = true;

					float magnitude = frand(3.0,5.0);

					if(game_over || player_dead)
					{
						float direction = frand(0,M_PI * 2.0f);
						float tempsin = sin(direction);
						float tempcos = cos(direction);
						tempbullet.velocity.x = 0;
						tempbullet.velocity.y = magnitude;
						rotatev2(tempbullet.velocity,tempsin,tempcos);
						ufo_shot_timer = 200 + irand(0,300);
					} else {
						// approx 1 in 5
						if(frand(-1,4) > 0)
						{
							// dumb shot
							tempbullet.velocity.x = player_position.x - ufo_position.x;
							tempbullet.velocity.y = player_position.y - ufo_position.y;
						} else {
							// slightly less dumb shot
							float xdist = player_position.x - ufo_position.x;
							float ydist = player_position.y - ufo_position.y;
							float distance = sqrt(xdist*xdist + ydist*ydist);
							float lead = distance / magnitude;
							tempbullet.velocity.x = player_position.x + player_velocity.x * lead - ufo_position.x;
							tempbullet.velocity.y = player_position.y + player_velocity.y * lead - ufo_position.y;
						}
						clamp_v2magnitude(tempbullet.velocity,magnitude,magnitude);
					}

					tempbullet.position = ufo_position + tempbullet.velocity * 2;
					tempbullet.timer = 160;
					ufo_bullets.push_back(tempbullet);
					PlaySound(0);

					float direction = frand(0,M_PI * 2.0f);
					float tempsin = sin(direction);
					float tempcos = cos(direction);
					float altmagnitude = frand(1,2);
					ufo_velocity.x = 0;
					ufo_velocity.y = altmagnitude;
					rotatev2(ufo_velocity,tempsin,tempcos);
				}
			} else if( !player_dead && !beatlevel && --ufo_timer < 1)
			{
				ufo_active = true;
				PlaySound(3);
				ufo_shot_timer = 30 + irand(0,90);
				do
				{
					ufo_position.x = frand((float)BOARD_Y_OFFSET,(float)(BOARD_WIDTH+BOARD_Y_OFFSET)); 
					ufo_position.y = frand((float)BOARD_Y_OFFSET,(float)(BOARD_HEIGHT+BOARD_Y_OFFSET));
				} while ( within_distance(&player_position, &ufo_position, 300.0f) );

				ufo_velocity.x = player_position.x - ufo_position.x;
				ufo_velocity.y = player_position.y - ufo_position.y;

				float magnitude = frand(0.5,2.5);
				clamp_v2magnitude(ufo_velocity,magnitude,magnitude);
			}

			// this is copy pasted from player bullets, should roll into funciton?
			if(ufo_bullets.size() > 0)
			{
				for(int ii = 0; ii < ufo_bullets.size(); ++ii)
				{
					if(--ufo_bullets[ii].timer > 0 && ufo_bullets[ii].active == true)
					{
						ufo_bullets[ii].position.x += ufo_bullets[ii].velocity.x;
						ufo_bullets[ii].position.y += ufo_bullets[ii].velocity.y;
						
						constrain_to_board(&ufo_bullets[ii].position);
							
					} else {
						ufo_bullets[ii].active = false;
						size_t back = ufo_bullets.size() - 1;
						if(ii < back)
						{
							ufo_bullets[back].position.x += ufo_bullets[back].velocity.x;
							ufo_bullets[back].position.y += ufo_bullets[back].velocity.y;
							constrain_to_board(&ufo_bullets[back].position);
							std::swap(ufo_bullets[ii],ufo_bullets[back]);
						}
						ufo_bullets.pop_back();
					}
				}
			}

			int asteroids_size = (int)asteroids.size();
			if(asteroids_size > 0)
			{
				for(int ii = 0; ii < asteroids_size; ++ii)
				{
					asteroid *temproid = &asteroids.at(ii);
					temproid->rotation += temproid->drotation;
					clamp_wrap(temproid->rotation,-M_PI,M_PI);
					temproid->position.x += temproid->velocity.x;
					temproid->position.y += temproid->velocity.y;
					constrain_to_board(&temproid->position);
				}
			}

			asteroids_size = (int)asteroids.size();
			for(int ii = 0; ii < bullets.size(); ++ii)
			{
				// should also check against wraparound asteroids?
				if(bullets[ii].active == true)
				{
					v2 bulletpos = bullets[ii].position;
					v2 bulletvel = bullets[ii].velocity;
					for(int jj = 0; jj < asteroids_size; ++jj)
					{
						if( collide_asteroid(bulletpos, asteroids[jj]) 
							|| collide_asteroid(bulletpos-bulletvel, asteroids[jj]) )
						{
							split_asteroid(jj);
							bullets[ii].active = false;
							asteroids_size = (int)asteroids.size();
							jj=asteroids_size;

							explosion tempexplosion = {bulletpos.x, bulletpos.y, 20};
							explosions.push_back(tempexplosion);
							PlaySound(2);
						}
					}

					//also check the ufo
					if( ufo_active && within_distance(&bulletpos, &ufo_position, 50))
					{
						v2 temppos = bulletpos - ufo_position;
						// ufo has 8 elements
						for(int jj = 2; jj < 8; ++jj)
						{
							if(PointInTriangle(temppos, ufo[jj-2], ufo[jj-1], ufo[jj]))
							{
								ufo_active = false;
								explode_UFO();
								bullets[ii].active = false;
								ufo_timer = 1200 + irand(0,600);
								player_score += 1000;
								jj = 7;

								explosion tempexplosion = {bulletpos.x, bulletpos.y, 20};
								explosions.push_back(tempexplosion);
								PlaySound(2);
							}
						}
					}
				}
			}

			if(!player_dead && !game_over)
			{
				// we will just check front point and two cross points
				// < |
				v2 checking[3];
				checking[0] = playership[0];
				checking[1] = playership[3];
				checking[2] = playership[4];

				float tempsin = sin(player_direction);
				float tempcos = cos(player_direction);

				rotatev2(checking[0],tempsin,tempcos);
				rotatev2(checking[1],tempsin,tempcos);
				rotatev2(checking[2],tempsin,tempcos);

				// check against ufo bullets
				for(int ii = 0; ii < ufo_bullets.size(); ++ii)
				{
					if(ufo_bullets[ii].active == true)
					{
						v2 bulletpos = ufo_bullets[ii].position;
						v2 bulletvel = ufo_bullets[ii].velocity;

						if( PointInTriangle(bulletpos-player_position, checking[0], checking[1], checking[2])
							|| PointInTriangle(bulletpos-bulletvel-player_position, checking[0], checking[1], checking[2]) )
						{
							player_dead = true;
							explode_ship();
							ufo_bullets[ii].active = false;
						}
					}
				}
				//check against ufo
				if( ufo_active && within_distance(&player_position, &ufo_position, 70))
				{
					// ufo has 8 elements
					for(int ii = 0; ii < 3; ++ii)
					{
						v2 temppos = checking[ii] + player_position;

						for(int jj = 2; jj < 8; ++jj)
						{
							v2 tempufo0 = ufo[jj-2] + ufo_position;
							v2 tempufo1 = ufo[jj-1] + ufo_position;
							v2 tempufo2 = ufo[jj] + ufo_position;

							if(PointInTriangle(temppos, tempufo0, tempufo1, tempufo2))
							{
								player_dead = true;
								explode_ship();
								
								jj = 8;
								ii = 3;
							}
						}
					}
				}

				// check against asteroids
				// should check against wraparound asteroids?
				// optimization: this is currently stupid, should be checking
				// 		asteroid points against the player's ship
				asteroids_size = (int)asteroids.size();
				for(int ii = 0; ii < 3; ++ii)
				{
					v2 tempposition = checking[ii] + player_position;
					for(int jj = 0; jj < asteroids_size; ++jj)
					{
						if( !player_dead && collide_asteroid(tempposition, asteroids[jj]) )
						{
							player_dead = true;
							explode_ship();
							split_asteroid(jj);
						}
					}
				}

				// since player can only die in this scope
				if(player_dead)
				{
					respawning = false;
					death_timer = 100;
					player_position.x = BOARD_X_OFFSET + BOARD_WIDTH / 2;
					player_position.y = BOARD_Y_OFFSET + BOARD_HEIGHT / 2;
					if(--player_lives == 0)
					{
						game_over = true;
						PlaySound(DEATH);
					}
				}
			}

			for(int ii = 0; ii < explosions.size(); ++ii)
			{
				explosions[ii].timer--;
				if( explosions[ii].timer == 0)
				{
					if(explosions.size()>1)
					{
						std::swap(explosions[ii], explosions[explosions.size()-1]);
						ii--;
					}
					explosions.pop_back();
				}
			}

			for(int ii = 0; ii < death_animations.size(); ++ii)
			{
				death_animations[ii].timer--;
				if( death_animations[ii].timer == 0)
				{
					if(death_animations.size()>1)
					{
						std::swap(death_animations[ii], death_animations[death_animations.size()-1]);
						ii--;
					}
					death_animations.pop_back();
				} else {
					for(int jj = 0; jj < death_animations[ii].lines.size(); ++jj)
					{
						death_animations[ii].lines[jj].points[0] = death_animations[ii].lines[jj].points[0] + death_animations[ii].lines[jj].velocity;
						death_animations[ii].lines[jj].points[1] = death_animations[ii].lines[jj].points[1] + death_animations[ii].lines[jj].velocity;
					}
					for(int jj = 0; jj < death_animations[ii].line_segments.size(); ++jj)
					{
						death_animations[ii].line_segments[jj].position = death_animations[ii].line_segments[jj].position + death_animations[ii].line_segments[jj].velocity;
						death_animations[ii].line_segments[jj].direction = death_animations[ii].line_segments[jj].direction + death_animations[ii].line_segments[jj].rotational_speed;
						clamp_wrap(death_animations[ii].line_segments[jj].direction, -M_PI, M_PI);
					}
				}
			}

			if(death_timer > 0)
			{
				death_timer--;
			} else if(respawning == 1 && death_timer == 0)
			{
				player_dead = false;
				respawning = false;

				float tempsin = sin(player_direction);
				float tempcos = cos(player_direction);

				v2 psvec = {0,1};
				rotatev2(psvec,tempsin,tempcos);

				//player_velocity = player_velocity * 0.25;
				player_velocity.x = 0;
				player_velocity.y = 0;

				if(inputstate.buttons[0].down)
				{
					// player_velocity = {0,0};
					// player_velocity = player_velocity * 0.25;
					// player_velocity = player_velocity + 1 * psvec;
					player_velocity = 0.5 * psvec;
				}

				for(size_t ii = 0; ii < asteroids.size(); ++ii)
				{
					while ( within_distance(&player_position, &asteroids[ii].position, 50.0f) )
					{
						asteroids[ii].position.x = frand((float)BOARD_Y_OFFSET,(float)(BOARD_WIDTH+BOARD_Y_OFFSET));
						asteroids[ii].position.y = frand((float)BOARD_Y_OFFSET,(float)(BOARD_HEIGHT+BOARD_Y_OFFSET));
					}
				}
			}

			if( beatlevel == false && ufo_active == false && beatleveltimer == 0 && asteroids.size() == 0 )
			{
				beatlevel=true;
				beatleveltimer = 120;
			} 

			if(beatleveltimer > 0)
			{
				beatleveltimer--;
			}

			if(beatlevel && beatleveltimer == 0 && inputstate.buttons[3].wentdown)
			{
				if(difficulty < MAX_DIFFICULTY)
				{
					difficulty++;
				}
				restartgame();
				beatlevel = false;
			}

			if(game_over && inputstate.buttons[3].wentdown && death_timer == 0)
			{
				difficulty = 0;
				restartgame();
			}

			if(game_over && death_timer == 0 && (inputstate.buttons[1].wentdown || inputstate.buttons[2].wentdown) )
			{
				difficulty += 0 - inputstate.buttons[1].wentdown + inputstate.buttons[2].wentdown;
				clamp(difficulty, 3, 5);
				selected_difficulty = difficulty;
			}

			if(game_over && death_timer == 0 && (inputstate.buttons[1].wentdown || inputstate.buttons[2].wentdown) )
			{
				difficulty += 0 - inputstate.buttons[1].wentdown + inputstate.buttons[2].wentdown;
				clamp(difficulty, 3, 5);
				selected_difficulty = difficulty;
			}

			if(inputstate.extrabuttons[0].wentdown)
			{
				int temp = drawR;
				temp += 51;
				clamp_wrap(temp, 0, 255);
				drawR = (unsigned char)temp;
			}
			if(inputstate.extrabuttons[1].wentdown)
			{
				int temp = drawG;
				temp += 51;
				clamp_wrap(temp, 0, 255);
				drawG = (unsigned char)temp;
			}
			if(inputstate.extrabuttons[2].wentdown)
			{
				int temp = drawB;
				temp += 51;
				clamp_wrap(temp, 0, 255);
				drawB = (unsigned char)temp;
			}


	//*******************//
	//*		RENDER		*//
	//*******************//

			ClearFramebuffer();
			SetDrawColor(drawR,drawG,drawB);
			
#if 0
			// not drawing border anymore
			//SDL_Rect temprect;
			temprect = SDL_Rect{BOARD_X_OFFSET-BOARD_BORDER,BOARD_Y_OFFSET-BOARD_BORDER,
								BOARD_WIDTH+2*BOARD_BORDER,BOARD_HEIGHT+2*BOARD_BORDER};
			SDL_SetRenderDrawColor(gRenderer,0x55,0x55,0x55,0xff);
			SDL_RenderFillRect(gRenderer, &temprect);
			SDL_SetRenderDrawColor(gRenderer,0x00,0x00,0x00,0xff);
			temprect = SDL_Rect{BOARD_X_OFFSET,BOARD_Y_OFFSET,BOARD_WIDTH,BOARD_HEIGHT};
			SDL_RenderFillRect(gRenderer, &temprect);
#endif

			// render player ship

			if(!game_over)
			{
				// should probably fake wrap-around drawing here as well but it's not a big deal
				if(!player_dead)
				{
					renderplayer(player_position, player_direction);
				} else if(death_timer < 30)
				{
					unsigned char color = (unsigned char)(153.0f * (31.0f/(death_timer+1)));
					SetDrawColor(color,color,color);
					renderplayer(player_position, player_direction);
					UnsetDrawColor();
				}
			}

			if(ufo_active == true)
			{
				renderUFO();
			}

			// render bullets

			for(int ii = 0; ii < bullets.size(); ++ii)
			{
				if(bullets[ii].active == true)
				{
					DrawLine(bullets[ii].position,bullets[ii].position - bullets[ii].velocity);
				}
			}

			// render ufo bullets
			for(int ii = 0; ii < ufo_bullets.size(); ++ii)
			{
				if(ufo_bullets[ii].active == true)
				{
					DrawLine(ufo_bullets[ii].position,ufo_bullets[ii].position - ufo_bullets[ii].velocity);
				}
			}

			// render asteroids
			for(int ii = 0; ii < asteroids.size(); ++ii)
			{
				renderasteroid(asteroids[ii].position, ii);

				// the rest of this is just faking wraparound drawing fake wraparound

				v2 temppos = asteroids[ii].position;
				bool rerender_x = false;
				bool rerender_y = false;
				float buffer = 50;

				if(temppos.x - buffer < BOARD_X_OFFSET)
				{
					rerender_x = true;
					temppos.x = temppos.x + (BOARD_WIDTH);
				} else if(temppos.x + buffer > BOARD_WIDTH + BOARD_X_OFFSET)
				{
					rerender_x = true;
					temppos.x = temppos.x - (BOARD_WIDTH + BOARD_X_OFFSET);
				}

				if(temppos.y - buffer < BOARD_Y_OFFSET)
				{
					rerender_y = true;
					temppos.y = temppos.y + (BOARD_HEIGHT + BOARD_Y_OFFSET);
				} else if(temppos.y + buffer > BOARD_HEIGHT + BOARD_Y_OFFSET)
				{
					rerender_y = true;
					temppos.y = temppos.y - (BOARD_HEIGHT + BOARD_Y_OFFSET);
				}

				if(rerender_x == true)
				{
					v2 temptemppos = {temppos.x,asteroids[ii].position.y};
					renderasteroid(temptemppos, ii);
				}

				if(rerender_y == true)
				{
					v2 temptemppos = {asteroids[ii].position.x,temppos.y};
					renderasteroid(temptemppos, ii);
				}

				if(rerender_x == true && rerender_y == true)
				{
					renderasteroid(temppos, ii);
				}
			}

			for(int ii = 0; ii < explosions.size(); ++ii)
			{
				v2 position = explosions.at(ii).position;
				float pifrac = (M_PI*2.0f)/ASTEROID_RESOLUTION;
				float rotmod = frand(0,(M_PI*2.0f));
				float rotation = pifrac * (ASTEROID_RESOLUTION-1) + rotmod;
				float tempcos = cos(rotation);
				float tempsin = sin(rotation);
				float size = 4;

				v2 vec0 = {0,frand(1,4)};
				rotatev2(vec0, sin(rotation), cos(rotation));
				vec0 = vec0 * size + position;

				v2 vecfirst = vec0;

				v2 vec1;

				for(int jj = 0; jj < ASTEROID_RESOLUTION-1; ++jj)
				{
					vec1 = vec0;

					vec0.x = 0;
					vec0.y = frand(1,3);

					rotation = pifrac * jj + rotmod;

					rotatev2(vec0, sin(rotation), cos(rotation));
					vec0 = vec0 * size + position;

					DrawLine(vec0,vec1);
				}

				DrawLine(vec0,vecfirst);
			}

			for(int ii = 0; ii < death_animations.size(); ++ii)
			{
#if 0
				for(int jj = 0; jj < death_animations[ii].lines.size(); ++jj) 
				{
					// da lines, timer; lines: points[2], velocity
					v2 vec0 = death_animations[ii].lines[jj].points[0];
					v2 vec1 = death_animations[ii].lines[jj].points[1];

					DrawLine(vec0,vec1);
				}
#endif
				for(int jj = 0; jj < death_animations[ii].line_segments.size(); ++jj) 
				{
					// da lines, timer; lines: points[2], velocity

					float tempsin = sin(death_animations[ii].line_segments[jj].direction);
					float tempcos = cos(death_animations[ii].line_segments[jj].direction);

					v2 vec0 = death_animations[ii].line_segments[jj].points[0];
					v2 vec1 = death_animations[ii].line_segments[jj].points[1];

					// we need to normalize these before we rotate :<
					rotatev2(vec0,tempsin,tempcos);
					rotatev2(vec1,tempsin,tempcos);

					DrawLine(vec0 + death_animations[ii].line_segments[jj].position, vec1 + death_animations[ii].line_segments[jj].position);
				}
			}

			// debugrender

	//***************************//
	//*		USER INTERFACE		*//
	//***************************//

			// used to draw a the border here but we're full window now

			std::string output = "Score: " + to_string(player_score);
			testtext_render(output.c_str(), 10, 10, tj_left);

			output = "High Score: " + to_string(high_score);
			testtext_render(output.c_str(), SCREEN_WIDTH-10, 10, tj_right);

			if(!game_over)
			{
				output = "Lives:";
				testtext_render(output.c_str(), SCREEN_WIDTH/2+10, 10, tj_right);
				for(int ii = 0; ii < player_lives-1;++ii)
				{
					v2 temppos = { float((25 + ii * 15) + SCREEN_WIDTH / 2),23};
					renderplayer(temppos,M_PI);
				}
	
				if(player_dead)
				{
					if(death_timer > 0)
					{
						output = "DEAD";
					} else {
						output = "Press shoot to respawn.";
					}
					
					testtext_render(output.c_str(), SCREEN_WIDTH/2, SCREEN_HEIGHT - 80, tj_middle);
					output = "Respawn: " + to_string(death_timer);
					testtext_render(output.c_str(), SCREEN_WIDTH/2, SCREEN_HEIGHT - 40, tj_middle);
				}
			} else {
				//output = "GAME OVER";
				if(death_timer == 0)
				{
					output = "KESSLER SYNDROME";
					testtext_render(output.c_str(), SCREEN_WIDTH/2, 10, tj_middle);
					output = "Press shoot to start.";
					testtext_render(output.c_str(), SCREEN_WIDTH/2, 30, tj_middle);
				} else {
					output = "GAME OVER";
					testtext_render(output.c_str(), SCREEN_WIDTH/2, 10, tj_middle);
				}

				output = "Press left or right to set level: ";
				switch(difficulty)
				{
					case 3:
						output = output + "Easy";
						break;
					case 4:
						output = output + "Medium";
						break;
					case 5:
						output = output + "Hard";
						break;
					default:
						output = output + to_string(difficulty);
						break;
				}
				testtext_render(output.c_str(), SCREEN_WIDTH/2, SCREEN_HEIGHT - 80, tj_middle);
				output = "Shoot: CTRL/Space/Z/C  Thrust: Up/W/X  Left/Right: LeftRight/AD";
				testtext_render(output.c_str(), SCREEN_WIDTH/2, SCREEN_HEIGHT - 40, tj_middle);
			}

			if(beatlevel)
			{
				output = "Level clear!";
				testtext_render(output.c_str(), SCREEN_WIDTH/2, 120, tj_middle);
				
				if(beatleveltimer == 0)
				{
					output = "Press shoot to go to next level.";
					testtext_render(output.c_str(), SCREEN_WIDTH/2, 80, tj_middle);
				}

			}

#if 0
			v2i refpos = {SCREEN_WIDTH-120, 0};
			if(testbutton_render(0x89442144,"TEST",refpos.x,refpos.y,120,50))
				split_asteroid((size_t)irand(0,(int)asteroids.size()-1));
#endif
	//***********************//
	//*		SWAP BUFFER		*//
	//***********************//

			SwapBuffer();
		}
	}

	//Free resources and close SDL
	closeSDL();

	return 0;
}