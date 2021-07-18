
enum NUMBER_JUSTIFICATION {NJ_LEFT = -1, NJ_MIDDLE = 0, NJ_RIGHT = 1};
enum TEXT_JUSTIFICATION {TJ_LEFT = -1, TJ_MIDDLE = 0, TJ_RIGHT = 1};

float SPACING_MULTIPLIER = 1.2f;

					//0     1      2      3     4      5
v2 NUMBER_MAP[6] = {{0,0},	{1,0},
					{1,0.5},{1,1},
					{0,1},	{0,0.5}};

void
rendernumber(long long number, float x, float y, float size, NUMBER_JUSTIFICATION justification)
{
	v2 position = {x,y};

	char numberstring[32];
	unsigned int characters = sprintf_s(numberstring, "%lld", number);

	switch(justification)
	{
		case NJ_LEFT:
		{

		} break;
		case NJ_RIGHT:
		{
			position.x -= size * characters * 1.2f;
		} break;
		case NJ_MIDDLE:
		{
			position.x -= (size * characters * 1.2f) / 2;
		} break;
		default:
		{

		} break;
	}

	for(size_t ii = 0; ii < characters; ++ii)
	{
		switch(numberstring[ii]-0x30)
		{
			case 0:
			{
				DrawLine(position + size * NUMBER_MAP[0], position + size * NUMBER_MAP[1]);
				DrawLine(position + size * NUMBER_MAP[1], position + size * NUMBER_MAP[3]);
				DrawLine(position + size * NUMBER_MAP[3], position + size * NUMBER_MAP[4]);
				DrawLine(position + size * NUMBER_MAP[4], position + size * NUMBER_MAP[0]);
				DrawLine(position + size * NUMBER_MAP[1], position + size * NUMBER_MAP[4]);
			} break;
			
			case 1:
			{
				position.x -= size/2.0f;
				DrawLine(position + size * NUMBER_MAP[1], position + size * NUMBER_MAP[3]);
				position.x += size/2.0f;
			} break;
			
			case 2:
			{
				DrawLine(position + size * NUMBER_MAP[0], position + size * NUMBER_MAP[1]);
				DrawLine(position + size * NUMBER_MAP[1], position + size * NUMBER_MAP[2]);
				DrawLine(position + size * NUMBER_MAP[2], position + size * NUMBER_MAP[5]);
				DrawLine(position + size * NUMBER_MAP[5], position + size * NUMBER_MAP[4]);
				DrawLine(position + size * NUMBER_MAP[4], position + size * NUMBER_MAP[3]);
			} break;
			
			case 3:
			{
				DrawLine(position + size * NUMBER_MAP[0], position + size * NUMBER_MAP[1]);
				DrawLine(position + size * NUMBER_MAP[1], position + size * NUMBER_MAP[3]);
				DrawLine(position + size * NUMBER_MAP[5], position + size * NUMBER_MAP[2]);
				DrawLine(position + size * NUMBER_MAP[4], position + size * NUMBER_MAP[3]);
			} break;
			
			case 4:
			{
				DrawLine(position + size * NUMBER_MAP[0], position + size * NUMBER_MAP[5]);
				DrawLine(position + size * NUMBER_MAP[5], position + size * NUMBER_MAP[2]);
				DrawLine(position + size * NUMBER_MAP[1], position + size * NUMBER_MAP[3]);
			} break;
			
			case 5:
			{
				DrawLine(position + size * NUMBER_MAP[0], position + size * NUMBER_MAP[1]);
				DrawLine(position + size * NUMBER_MAP[0], position + size * NUMBER_MAP[5]);
				DrawLine(position + size * NUMBER_MAP[2], position + size * NUMBER_MAP[5]);
				DrawLine(position + size * NUMBER_MAP[2], position + size * NUMBER_MAP[3]);
				DrawLine(position + size * NUMBER_MAP[4], position + size * NUMBER_MAP[3]);
			} break;
			
			case 6:
			{
				DrawLine(position + size * NUMBER_MAP[0], position + size * NUMBER_MAP[4]);
				DrawLine(position + size * NUMBER_MAP[2], position + size * NUMBER_MAP[5]);
				DrawLine(position + size * NUMBER_MAP[2], position + size * NUMBER_MAP[3]);
				DrawLine(position + size * NUMBER_MAP[4], position + size * NUMBER_MAP[3]);
			} break;
			
			case 7:
			{
				DrawLine(position + size * NUMBER_MAP[1], position + size * NUMBER_MAP[0]);
				DrawLine(position + size * NUMBER_MAP[1], position + size * NUMBER_MAP[3]);
			} break;
			
			case 8:
			{
				DrawLine(position + size * NUMBER_MAP[0], position + size * NUMBER_MAP[1]);
				DrawLine(position + size * NUMBER_MAP[3], position + size * NUMBER_MAP[1]);
				DrawLine(position + size * NUMBER_MAP[5], position + size * NUMBER_MAP[2]);
				DrawLine(position + size * NUMBER_MAP[3], position + size * NUMBER_MAP[4]);
				DrawLine(position + size * NUMBER_MAP[0], position + size * NUMBER_MAP[4]);
			} break;
			
			case 9:
			{
				DrawLine(position + size * NUMBER_MAP[1], position + size * NUMBER_MAP[0]);
				DrawLine(position + size * NUMBER_MAP[1], position + size * NUMBER_MAP[3]);
				DrawLine(position + size * NUMBER_MAP[5], position + size * NUMBER_MAP[2]);
				DrawLine(position + size * NUMBER_MAP[5], position + size * NUMBER_MAP[0]);
			} break;
			
			default:
			{

			} break;
		}
		position.x += size * 1.2f;
	}
}

v2 LETTERMAP[9] = { {0,0},	{0.5,0},	{1,0},	// 0 1 2
					{0,0.5},{0.5,0.5},	{1,0.5},// 3 4 5
					{0,1},	{0.5,1},	{1,1} };// 6 7 8
void
rendermapline(char a, char b, v2 position, v2 scale)
{
	DrawLine(position + scale * LETTERMAP[a], position + scale * LETTERMAP[b]);
}

void
rendercharacter(char character, v2 position, float size)
{
	v2 scale = {size,size};
	switch(character)
	{
		//letters
		case 'a':
		case 'A':
		{
			rendermapline(0,2,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(2,8,position,scale);
			rendermapline(3,5,position,scale);
		} break;

		case 'b':
		case 'B':
		{
			rendermapline(0,2,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(3,5,position,scale);
			rendermapline(6,8,position,scale);
			rendermapline(2,8,position,scale);
		} break;

		case 'c':
		case 'C':
		{
			rendermapline(0,2,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(6,8,position,scale);
		} break;

		case 'd':
		case 'D':
		{
			rendermapline(0,2,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(6,8,position,scale);
			rendermapline(2,8,position,scale);
		} break;

		case 'e':
		case 'E':
		{
			rendermapline(0,2,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(3,5,position,scale);
			rendermapline(6,8,position,scale);
		} break;

		case 'f':
		case 'F':
		{
			rendermapline(0,2,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(3,4,position,scale);
		} break;

		case 'g':
		case 'G':
		{
			rendermapline(0,2,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(6,8,position,scale);
			rendermapline(5,8,position,scale);
			rendermapline(5,4,position,scale);
		} break;

		case 'h':
		case 'H':
		{
			rendermapline(0,6,position,scale);
			rendermapline(3,5,position,scale);
			rendermapline(2,8,position,scale);
		} break;

		case 'i':
		case 'I':
		{
			rendermapline(0,2,position,scale);
			rendermapline(1,7,position,scale);
			rendermapline(6,8,position,scale);
		} break;

		case 'j':
		case 'J':
		{
			rendermapline(0,2,position,scale);
			rendermapline(1,7,position,scale);
			rendermapline(6,7,position,scale);
		} break;

		case 'k':
		case 'K':
		{
			rendermapline(0,6,position,scale);
			rendermapline(3,2,position,scale);
			rendermapline(3,8,position,scale);
		} break;

		case 'l':
		case 'L':
		{
			rendermapline(0,6,position,scale);
			rendermapline(6,8,position,scale);
		} break;

		case 'm':
		case 'M':
		{
			rendermapline(0,4,position,scale);
			rendermapline(2,4,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(8,2,position,scale);
		} break;

		case 'n':
		case 'N':
		{
			rendermapline(0,6,position,scale);
			rendermapline(0,8,position,scale);
			rendermapline(8,2,position,scale);
		} break;

		case 'o':
		case 'O':
		{
			rendermapline(0,2,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(6,8,position,scale);
			rendermapline(2,8,position,scale);
		} break;

		case 'p':
		case 'P':
		{
			rendermapline(0,2,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(2,5,position,scale);
			rendermapline(5,3,position,scale);
		} break;

		case 'q':
		case 'Q':
		{
			rendermapline(0,2,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(6,8,position,scale);
			rendermapline(2,8,position,scale);
			rendermapline(4,8,position,scale);
		} break;

		case 'r':
		case 'R':
		{
			rendermapline(0,2,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(2,5,position,scale);
			rendermapline(3,5,position,scale);
			rendermapline(3,8,position,scale);
		} break;

		case 's':
		case 'S':
		{
			rendermapline(0,2,position,scale);
			rendermapline(0,3,position,scale);
			rendermapline(3,5,position,scale);
			rendermapline(5,8,position,scale);
			rendermapline(6,8,position,scale);
		} break;

		case 't':
		case 'T':
		{
			rendermapline(0,2,position,scale);
			rendermapline(1,7,position,scale);
		} break;

		case 'u':
		case 'U':
		{
			rendermapline(0,6,position,scale);
			rendermapline(6,8,position,scale);
			rendermapline(2,8,position,scale);
		} break;

		case 'v':
		case 'V':
		{
			rendermapline(0,7,position,scale);
			rendermapline(2,7,position,scale);
		} break;

		case 'w':
		case 'W':
		{
			rendermapline(0,6,position,scale);
			rendermapline(6,4,position,scale);
			rendermapline(4,8,position,scale);
			rendermapline(2,8,position,scale);
		} break;

		case 'x':
		case 'X':
		{
			rendermapline(0,8,position,scale);
			rendermapline(2,6,position,scale);
		} break;

		case 'y':
		case 'Y':
		{
			rendermapline(0,4,position,scale);
			rendermapline(4,2,position,scale);
			rendermapline(4,7,position,scale);
		} break;

		case 'z':
		case 'Z':
		{
			rendermapline(0,2,position,scale);
			rendermapline(2,6,position,scale);
			rendermapline(6,8,position,scale);
		} break;

		// numbers
		case '0':
		{
			rendermapline(0,2,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(2,8,position,scale);
			rendermapline(6,8,position,scale);
			rendermapline(2,6,position,scale);
		} break;
		case '1':
		{
			rendermapline(0,1,position,scale);
			rendermapline(1,7,position,scale);
			rendermapline(6,8,position,scale);
		} break;
		case '2':
		{
			rendermapline(0,2,position,scale);
			rendermapline(2,5,position,scale);
			rendermapline(5,3,position,scale);
			rendermapline(6,3,position,scale);
			rendermapline(6,8,position,scale);
		} break;
		case '3':
		{
			rendermapline(0,2,position,scale);
			rendermapline(2,8,position,scale);
			rendermapline(6,8,position,scale);
			rendermapline(3,5,position,scale);
		} break;
		case '4':
		{
			rendermapline(0,3,position,scale);
			rendermapline(3,5,position,scale);
			rendermapline(2,8,position,scale);
		} break;
		case '5':
		{
			rendermapline(0,2,position,scale);
			rendermapline(0,3,position,scale);
			rendermapline(3,5,position,scale);
			rendermapline(5,8,position,scale);
			rendermapline(8,6,position,scale);
		} break;
		case '6':
		{
			rendermapline(0,2,position,scale);
			rendermapline(3,5,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(6,8,position,scale);
			rendermapline(5,8,position,scale);
		} break;
		case '7':
		{
			rendermapline(0,2,position,scale);
			rendermapline(2,6,position,scale);
		} break;
		case '8':
		{
			rendermapline(0,2,position,scale);
			rendermapline(2,8,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(6,8,position,scale);
			rendermapline(3,5,position,scale);
		} break;
		case '9':
		{
			rendermapline(0,2,position,scale);
			rendermapline(2,8,position,scale);
			rendermapline(0,3,position,scale);
			rendermapline(3,5,position,scale);
			rendermapline(8,6,position,scale);
		} break;

		default:
		{
			rendermapline(0,2,position,scale);
			rendermapline(0,6,position,scale);
			rendermapline(2,8,position,scale);
			rendermapline(6,8,position,scale);
			rendermapline(0,8,position,scale);
			rendermapline(6,2,position,scale);
		} break;
	}
}

void
renderstring(char* characters, v2 position, float size, TEXT_JUSTIFICATION justification)
{
	size_t index = 0;
	while(characters[index] != 0)
	{
		switch(characters[index])
		{
			case ' ':
			{
				// space
			} break;
			default:
			{
				rendercharacter(characters[index], position, size);
			} break;
		}
		
		++index;
		position.x += size * SPACING_MULTIPLIER;
	}
}

void
ks_renderui()
{
	
	// highscore
	rendernumber(high_score, SCREEN_WIDTH-30, 30, 14, NJ_RIGHT);
	// score
	rendernumber(player_score, 30, 30, 14, NJ_LEFT);
	

	// lives
	for(int ii = 0; ii < player_lives-1;++ii)
	{
		v2 temppos = { float((ii * 16) + SCREEN_WIDTH / 2),30};
		renderplayer(temppos,M_PI);
	}

	if(!game_over)
	{
		//
	} else if (beatlevel)
	{
		//
	} else 
	{
		if(!death_timer)
		{
			v2 position = { float(SCREEN_WIDTH / 2) - 60, SCREEN_HEIGHT - 80};
			renderstring("Difficulty",position,10,TJ_MIDDLE);
			position.y += 30;
			position.x = float(SCREEN_WIDTH / 2 - 15 * (difficulty-3));

			for(int ii = 0; ii < difficulty - 2; ++ii)
			{
				renderexplosion(position);
				position.x += 30.0f;
			}
		} else 
		{
			renderstring("GAME OVER",{300,300},20,TJ_MIDDLE);
		}
		
	}

#if 0
			// used to draw a the border here but we're full window now

			std::string output = "Score: " + to_string(player_score);
			testtext_render(output.c_str(), 10, 10, TJ_LEFT);

			output = "High Score: " + to_string(high_score);
			testtext_render(output.c_str(), SCREEN_WIDTH-10, 10, TJ_RIGHT);

			if(!game_over)
			{
				output = "Lives:";
				testtext_render(output.c_str(), SCREEN_WIDTH/2+10, 10, TJ_RIGHT);
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
					
					testtext_render(output.c_str(), SCREEN_WIDTH/2, SCREEN_HEIGHT - 80, TJ_MIDDLE);
					output = "Respawn: " + to_string(death_timer);
					testtext_render(output.c_str(), SCREEN_WIDTH/2, SCREEN_HEIGHT - 40, TJ_MIDDLE);
				}
			} else {
				//output = "GAME OVER";
				if(death_timer == 0)
				{
					output = "KESSLER SYNDROME";
					testtext_render(output.c_str(), SCREEN_WIDTH/2, 10, TJ_MIDDLE);
					output = "Press shoot to start.";
					testtext_render(output.c_str(), SCREEN_WIDTH/2, 30, TJ_MIDDLE);
				} else {
					output = "GAME OVER";
					testtext_render(output.c_str(), SCREEN_WIDTH/2, 10, TJ_MIDDLE);
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
				testtext_render(output.c_str(), SCREEN_WIDTH/2, SCREEN_HEIGHT - 80, TJ_MIDDLE);
				output = "Shoot: CTRL/Space/Z/C  Thrust: Up/W/X  Left/Right: LeftRight/AD";
				testtext_render(output.c_str(), SCREEN_WIDTH/2, SCREEN_HEIGHT - 40, TJ_MIDDLE);
			}

			if(beatlevel)
			{
				output = "Level clear!";
				testtext_render(output.c_str(), SCREEN_WIDTH/2, 120, TJ_MIDDLE);
				
				if(beatleveltimer == 0)
				{
					output = "Press shoot to go to next level.";
					testtext_render(output.c_str(), SCREEN_WIDTH/2, 80, TJ_MIDDLE);
				}

			}
#endif
}
