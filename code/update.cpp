
void constrain_to_board(v2 *inc)
{
	clamp_wrap(inc->x, (float)0, (float)(BOARD_WIDTH));
	clamp_wrap(inc->y, (float)0, (float)(BOARD_HEIGHT));
}

// NOTE: wonderful hack <3
void move_everything(v2 velocity)
{
	ufo_position = ufo_position + velocity;

	for(unsigned int ii = 0; ii < bullets.size(); ++ii)
	{
		bullets[ii].position = bullets[ii].position + velocity;
	}

	for(unsigned int ii = 0; ii < ufo_bullets.size(); ++ii)
	{
		ufo_bullets[ii].position = ufo_bullets[ii].position + velocity;
	}

	for(unsigned int ii = 0; ii < asteroids.size(); ++ii)
	{
		asteroids[ii].position = asteroids[ii].position + velocity;
	}

	for(unsigned int ii = 0; ii < explosions.size(); ++ii)
	{
		explosions[ii].position = explosions[ii].position + velocity;
	}

	for(unsigned int ii = 0; ii < death_animations.size(); ++ii)
	{
		for(unsigned int jj = 0; jj < death_animations[ii].line_segments.size(); ++jj)
		{
			death_animations[ii].line_segments[jj].position = death_animations[ii].line_segments[jj].position + velocity;
		}
	}
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

	v2 pointA = tempasteroid.points[ASTEROID_RESOLUTION-1];
	v2 pointB;

	for(int ii = 0; ii < ASTEROID_RESOLUTION; ++ii)
	{
		pointB = pointA;
		pointA = tempasteroid.points[ii];

		// shrink the edges by a bit
		temp_ls.position = (((pointA + pointB) / 2));
		temp_ls.points[0] = (pointA + temp_ls.position)/2;
		temp_ls.points[1] = (pointB + temp_ls.position)/2;
		temp_ls.points[0] = temp_ls.points[0] - temp_ls.position;
		temp_ls.points[1] = temp_ls.points[1] - temp_ls.position;

		// slow down the velocity a bit for effect
		temp_ls.velocity = temp_ls.position / 80;

		// calculate the normal and orient by scaled rotation
		v2 tempnormal = {-temp_ls.position.y,temp_ls.position.x};
		tempnormal = tempnormal * (tempasteroid.drotation);

		// add normal and inherit velocity from parent (should)
		temp_ls.velocity = temp_ls.velocity + tempnormal + tempasteroid.velocity;

		// we can calulate based on distance from center at each point and their relative distances or something
		// but this seems reasonable as well
		temp_ls.rotational_speed = temp_ls.points[1].x - temp_ls.points[0].x + temp_ls.points[1].y - temp_ls.points[0].y;
		temp_ls.rotational_speed = temp_ls.rotational_speed / 80;// + tempasteroid.drotation;

		temp_ls.position = tempasteroid.position + temp_ls.position;
		temp_ls.direction = tempasteroid.rotation;

		tempdeath.line_segments.push_back(temp_ls);
	}

	death_animations.push_back(tempdeath);
}

line_segment
segment_player(v2 inca, v2 incb) // should be val
{
	// points[2], position, velocity, direction, rotational_speed
	line_segment Result = {};

	// get velocity
	Result.velocity = (((inca + incb) / 2));
	Result.points[0] = (inca - Result.velocity);
	Result.points[1] = (incb - Result.velocity);

	Result.position = player_position + Result.velocity;

	// slow down the velocity a bit for effect
	Result.velocity = Result.velocity / 20;

	// calculate the normal and orient by scaled rotation
	v2 normal = {-Result.velocity.y,Result.velocity.x};
	normal = normal * (turnvelocity * 50);

	// add normal and inherit velocity from parent (should)
	Result.velocity = Result.velocity + normal;
	Result.velocity = Result.velocity + player_velocity;

	Result.rotational_speed = turnvelocity;

	return(Result);
}

void
explode_ship()
{
	death_animation tempdeath;
	tempdeath.timer = 100;

	float tempsin = sin(player_direction);
	float tempcos = cos(player_direction);

	v2 vec0;
	v2 vec1;

	vec0 = playership[0];
	vec1 = playership[1];

	rotatev2(vec0, tempsin, tempcos);
	rotatev2(vec1, tempsin, tempcos);
	tempdeath.line_segments.push_back(segment_player(vec0,vec1));

	vec1 = playership[2];

	rotatev2(vec1, tempsin, tempcos);
	tempdeath.line_segments.push_back(segment_player(vec0,vec1));

	vec0 = playership[3];
	vec1 = playership[4];

	rotatev2(vec0, tempsin, tempcos);
	rotatev2(vec1, tempsin, tempcos);
	tempdeath.line_segments.push_back(segment_player(vec0,vec1));

	death_animations.push_back(tempdeath);

	PlaySound(DEATH);
	SoundVolume(THRUSTERCHANNEL,0.0f);

	explosion tempexplosion = {player_position.x, player_position.y, 60};
	explosions.push_back(tempexplosion);

}

line_segment
segment_ufo(v2 inca, v2 incb)
{
	// points[2], position, velocity, direction, rotational_speed
	line_segment Result = {};

	// get velocity
	Result.velocity = (((inca + incb) / 2));
	Result.points[0] = (inca - Result.velocity)/2;
	Result.points[1] = (incb - Result.velocity)/2;

	Result.position = ufo_position + Result.velocity;

	// slow down the velocity a bit for effect
	Result.velocity = Result.velocity / 20;

	// add normal and inherit velocity from parent (should)
	Result.velocity = Result.velocity + ufo_velocity;

	return(Result);
}

void
explode_UFO()
{
	death_animation tempdeath;
	tempdeath.timer = 60;

	tempdeath.line_segments.push_back(segment_ufo(ufo[0], ufo[1]));
	tempdeath.line_segments.push_back(segment_ufo(ufo[0], ufo[2]));
	tempdeath.line_segments.push_back(segment_ufo(ufo[1], ufo[3]));
	tempdeath.line_segments.push_back(segment_ufo(ufo[2], ufo[3]));
	tempdeath.line_segments.push_back(segment_ufo(ufo[2], ufo[4]));
	tempdeath.line_segments.push_back(segment_ufo(ufo[3], ufo[5]));
	tempdeath.line_segments.push_back(segment_ufo(ufo[4], ufo[5]));
	tempdeath.line_segments.push_back(segment_ufo(ufo[4], ufo[6]));
	tempdeath.line_segments.push_back(segment_ufo(ufo[5], ufo[7]));
	tempdeath.line_segments.push_back(segment_ufo(ufo[6], ufo[7]));

	death_animations.push_back(tempdeath);
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
		tempposition.x = frand((float)0,(float)(BOARD_WIDTH));
		tempposition.y = frand((float)0,(float)(BOARD_HEIGHT));
	} while ( within_distance(&player_position, &tempposition, 200.0f) );

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

		asteroids.back().points[ii] = tempposition;
	}
}

void
addscore( int adding )
{
	player_score += adding;

	if(player_score > next_life)
	{
		player_lives++;
		next_life += 2500 * next_life / 8000;
	}

	if(player_score > high_score)
	{
		high_score = player_score;
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
			addscore((int)(size*size*10 + size * 50 + difficulty * 10));
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
				float direction = frand(0, M_PI * 2.0f);
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

void restartgame()
{

	StopSounds();
	THRUSTERCHANNEL = LoopSound(THRUSTER);
	SoundVolume(THRUSTERCHANNEL,0.0f);

	player_position.x = (BOARD_WIDTH) / 2;
	player_position.y = (BOARD_HEIGHT) / 2;
	player_velocity.x = 0;
	player_velocity.y = 0;
	player_dead = true;
	death_timer = 60;
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

void
ks_update()
{
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

			if(!funky_mode)
			{
				player_position = player_position + player_velocity;
				constrain_to_board(&player_position);
			} else {
				// funky shit mode:
				move_everything(v2{0,0}-player_velocity);
			}

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
				SoundVolume(THRUSTERCHANNEL,0.4f);
			}
			if(inputstate.buttons[0].wentup)
			{
				SoundVolume(THRUSTERCHANNEL,0.0f);
			}

			if(inputstate.buttons[3].wentdown)
			{
				if(bullets.size() < MAX_BULLETS)
				{
//					IACA_VC64_START;
					bullet tempbullet;
					tempbullet.active = true;
					//tempbullet.position = player_position + 10.0f * psvec;
					tempbullet.position = playership[0];
					rotatev2(tempbullet.position,tempsin,tempcos);
					tempbullet.position = tempbullet.position + player_position;
					tempbullet.velocity = player_velocity * SHOT_INERTIA + bullet_speed * psvec;
					tempbullet.timer = 120;
					bullets.push_back(tempbullet);

					PlaySound(0);

				}
//				IACA_VC64_END;
			}
		} else {
			if(inputstate.buttons[3].down && death_timer == 0)
			{
				respawning = true;

				player_position.x = (BOARD_WIDTH) / 2;
				player_position.y = (BOARD_HEIGHT) / 2;
			}
		}
	}

	if(bullets.size() > 0)
	{
		for(unsigned int ii = 0; ii < bullets.size(); ++ii)
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

			float magnitude = frand(3.0,4.0);

			if(game_over || player_dead)
			{
				float direction = frand(0,M_PI * 2.0f);
				tempbullet.velocity.x = 0;
				tempbullet.velocity.y = magnitude;
				rotatev2(tempbullet.velocity,sin(direction),cos(direction));
				ufo_shot_timer = 200 + irand(0,300);
			} else {
				if(frand(-1,4) > 0)
				{
					// dumb shot (4/5)
					tempbullet.velocity.x = player_position.x - ufo_position.x + frand(-20,20);
					tempbullet.velocity.y = player_position.y - ufo_position.y + frand(-20,20);
				} else {
					// slightly less dumb shot (1/5)
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
			ufo_position.x = frand((float)0,(float)(BOARD_WIDTH));
			ufo_position.y = frand((float)0,(float)(BOARD_HEIGHT));
		} while ( within_distance(&player_position, &ufo_position, 300.0f) );

		ufo_velocity.x = player_position.x - ufo_position.x;
		ufo_velocity.y = player_position.y - ufo_position.y;

		float magnitude = frand(0.5,2.5);
		clamp_v2magnitude(ufo_velocity,magnitude,magnitude);
	}

	// this is copy pasted from player bullets, should roll into funciton?
	if(ufo_bullets.size() > 0)
	{
		for(unsigned int ii = 0; ii < ufo_bullets.size(); ++ii)
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
	for(unsigned int ii = 0; ii < bullets.size(); ++ii)
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
						addscore(1000);
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
		for(unsigned int ii = 0; ii < ufo_bullets.size(); ++ii)
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
			death_timer = 120;
			player_position.x = BOARD_WIDTH / 2;
			player_position.y = BOARD_HEIGHT / 2;
			if(--player_lives == 0)
			{
				game_over = true;
				death_timer = 200;

				// not sure if we should commit high score here or on every score
				if(player_score > high_score)
				{
					high_score = player_score;
				}

				PlaySound(DEATH);
			}
		}
	}

	for(unsigned int ii = 0; ii < explosions.size(); ++ii)
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

	for(unsigned int ii = 0; ii < death_animations.size(); ++ii)
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
			for(unsigned int jj = 0; jj < death_animations[ii].line_segments.size(); ++jj)
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
			while ( within_distance(&player_position, &asteroids[ii].position, 88.0f) )
			{
				asteroids[ii].position.x = frand((float)0,(float)(BOARD_WIDTH));
				asteroids[ii].position.y = frand((float)0,(float)(BOARD_HEIGHT));
			}
		}
	}

	if( beatlevel == false && ufo_active == false && beatleveltimer == 0 && asteroids.size() == 0 )
	{
		beatlevel=true;
		beatleveltimer = 210;
	}

	if(beatleveltimer > 0)
	{
		beatleveltimer--;
	}

	if(beatlevel && beatleveltimer == 0 )//&& inputstate.buttons[3].wentdown)
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
		player_score = 0;
		next_life = 8500;
		player_lives = 3;
		restartgame();
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
	if(inputstate.extrabuttons[3].wentdown)
	{
		funky_mode = !funky_mode;
		funky_centered = false;
	}

	if(funky_mode && !funky_centered)
	{
		v2 tempvec =  v2{BOARD_WIDTH/2, BOARD_HEIGHT/2} - player_position;
		clamp_v2magnitude(tempvec,-10,10);
		move_everything(tempvec);
		player_position = player_position + tempvec;

		if((int)player_position.x == BOARD_WIDTH/2 && (int)player_position.y == BOARD_HEIGHT/2)
		{
			funky_centered = true;
		}
	}
}
