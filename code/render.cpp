
// TODO: this could be a lot better if it was using matrices directly
void renderplayer(v2 position, float rotation)
{
	float tempcos = cos(rotation);
	float tempsin = sin(rotation);

	v2 vec0 = playership[0];
	rotatev2(vec0, tempsin, tempcos);
	vec0 = vec0 + position;

	v2 vec1 = playership[1];
	rotatev2(vec1, tempsin, tempcos);
	vec1 = vec1 + position;
	DrawLine(vec0,vec1);

	vec1 = playership[2];
	rotatev2(vec1, tempsin, tempcos);
	vec1 = vec1 + position;
	DrawLine(vec0,vec1);

	vec0 = playership[3];
	rotatev2(vec0, tempsin, tempcos);
	vec0 = vec0 + position;

	vec1 = playership[4];
	rotatev2(vec1, tempsin, tempcos);
	vec1 = vec1 + position;
	DrawLine(vec0,vec1);

	if(inputstate.buttons[0].down)
	{
		vec0 = playership[5];
		rotatev2(vec0, tempsin, tempcos);
		vec0 = vec0 + position;

		vec1 = playership[6];
		rotatev2(vec1, tempsin, tempcos);
		vec1 = vec1 + position;
		DrawLine(vec0,vec1);

		vec1 = playership[7];
		rotatev2(vec1, tempsin, tempcos);
		vec1 = vec1 + position;
		DrawLine(vec0,vec1);
	}
}

void renderasteroid(v2 position, size_t asteroidindex)
{
	asteroid *incasteroid = &asteroids[asteroidindex];
	float rotation = incasteroid->rotation;
	float tempcos = cos(rotation);
	float tempsin = sin(rotation);

	float size = incasteroid->size;
	size = (size) * sqrt(size) + 11/difficulty;

	v2 vec0 = incasteroid->points[ASTEROID_RESOLUTION-1];
	rotatev2(vec0, tempsin, tempcos);
	vec0 = vec0 * size;

	v2 vec1 = vec0;

	for(int jj = 0; jj < ASTEROID_RESOLUTION; ++jj)
	{
		vec1 = vec0;

		vec0 = incasteroid->points[jj];
		rotatev2(vec0, tempsin, tempcos);

		vec0 = vec0 * size;

		DrawLine(vec0 + position, vec1 + position);
	}
}

// TODO: same as player, just use matrix + rotation or just matrix, can be 1 render function for both
void renderUFO()
{
	v2 position = {BOARD_WIDTH/2,BOARD_HEIGHT/2 - 100};

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

void renderexplosion(v2 position)
{
	float pifrac = (M_PI*2.0f)/ASTEROID_RESOLUTION;
	float rotmod = frand(0,(M_PI*2.0f));
	float rotation = pifrac * (ASTEROID_RESOLUTION-1) + rotmod;
	float size = 4;

	v2 vec0;
	v2 vec1;

	vec0 = {0,frand(1,4)};
	rotatev2(vec0, sin(rotation), cos(rotation));
	vec0 = vec0 * size + position;

	v2 vecfirst = vec0;

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

void
ks_render()
{
	SetDrawColor(drawR,drawG,drawB);

	// render player
	if(!game_over)
	{
		float mult = 1;

		if(player_dead && death_timer < 35)
		{
			float mult = 1/(death_timer+1.67f);
			SetDrawColor((unsigned char)(drawR * mult), (unsigned char)(drawG * mult), (unsigned char)(drawB * mult));
			renderplayer(player_position, player_direction);
			UnsetDrawColor();
		} else if(beatlevel && beatleveltimer < 69)
		{
			if(beatleveltimer > 30)
			{
				float mult = (float)(beatleveltimer-29)/40.0f;
				SetDrawColor((unsigned char)(drawR * mult), (unsigned char)(drawG * mult), (unsigned char)(drawB * mult));
				renderplayer(player_position, player_direction);
				UnsetDrawColor();
			}

		} else if( !player_dead )
		{
			renderplayer(player_position, player_direction);
		}
	}

	// render ufo
	if(ufo_active == true)
	{
		renderUFO();
	}

	// render bullets
	for(unsigned int ii = 0; ii < bullets.size(); ++ii)
	{
		if(bullets[ii].active == true)
		{
			if(bullets[ii].timer < 20)
			{
				float mult = (bullets[ii].timer/20.0f);
				SetDrawColor((unsigned char)(drawR * mult), (unsigned char)(drawG * mult), (unsigned char)(drawB * mult));
			} else {
				SetDrawColor(drawR,drawG,drawB);
			}

			DrawLine(bullets[ii].position,bullets[ii].position - bullets[ii].velocity);

			UnsetDrawColor();
		}
	}

	// render ufo bullets
	for(unsigned int ii = 0; ii < ufo_bullets.size(); ++ii)
	{
		if(ufo_bullets[ii].active == true)
		{
			if(ufo_bullets[ii].timer < 20)
			{
				float mult = (ufo_bullets[ii].timer/20.0f);
				SetDrawColor((unsigned char)(drawR * mult), (unsigned char)(drawG * mult), (unsigned char)(drawB * mult));
			} else {
				SetDrawColor(drawR,drawG,drawB);
			}

			DrawLine(ufo_bullets[ii].position,ufo_bullets[ii].position - ufo_bullets[ii].velocity);

			UnsetDrawColor();
		}
	}

	// render asteroids
	for(unsigned int ii = 0; ii < asteroids.size(); ++ii)
	{
		renderasteroid(asteroids[ii].position, ii);
	}

	// render explosions
	for(unsigned int ii = 0; ii < explosions.size(); ++ii)
	{
		v2 position = explosions.at(ii).position;
		renderexplosion(position);
	}

	// render death animations
	for(unsigned int ii = 0; ii < death_animations.size(); ++ii)
	{
		if(death_animations[ii].timer < 20)
		{
			float mult = (death_animations[ii].timer/20.0f);
			SetDrawColor((unsigned char)(drawR * mult), (unsigned char)(drawG * mult), (unsigned char)(drawB * mult));
		} else {
			SetDrawColor(drawR,drawG,drawB);
		}

		for(unsigned int jj = 0; jj < death_animations[ii].line_segments.size(); ++jj)
		{
			// da lines, timer; lines: points[2], velocity

			float tempsin = sin(death_animations[ii].line_segments[jj].direction);
			float tempcos = cos(death_animations[ii].line_segments[jj].direction);

			v2 vec0 = death_animations[ii].line_segments[jj].points[0];
			v2 vec1 = death_animations[ii].line_segments[jj].points[1];

			rotatev2(vec0,tempsin,tempcos);
			rotatev2(vec1,tempsin,tempcos);

			DrawLine(vec0 + death_animations[ii].line_segments[jj].position, vec1 + death_animations[ii].line_segments[jj].position);
		}
		UnsetDrawColor();
	}
}
