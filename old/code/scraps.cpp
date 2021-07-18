


if(!player_dead)
		{
			renderplayer(player_position, player_direction);
		} else if(death_timer < 30)
		{
			float mult = 1/(death_timer+1.67f);
			SetDrawColor((unsigned char)(drawR * mult), (unsigned char)(drawG * mult), (unsigned char)(drawB * mult));
			renderplayer(player_position, player_direction);
			UnsetDrawColor();
		} else if(beatlevel && beatleveltimer < 60)
		{
			float mult = (float)beatleveltimer/60.0f;
			SetDrawColor((unsigned char)(drawR * mult), (unsigned char)(drawG * mult), (unsigned char)(drawB * mult));
			renderplayer(player_position, player_direction);
			UnsetDrawColor();
		}





float dot(v2 inca, v2 incb)
{
	float Return = 0;

	Return += inca.x+incb.x;
	Return += inca.y+incb.y;

	return(Return);
}

bool complex_collide_asteroid(v2 incpoint, asteroid targetasteroid)
{
	bool Return = false;

	float size = (targetasteroid.size) * sqrt(targetasteroid.size);
	float rotation = targetasteroid.rotation;
	float tempcos = cos(rotation);
	float tempsin = sin(rotation);

	v2 tempposition;

	// normalize to 0,0
	v2 tempvecA = {0, 0};

	tempposition = targetasteroid.points[0];
	rotatev2(tempposition, tempsin, tempcos);
	v2 tempvecB = {tempposition.x * size, tempposition.y * size};

	tempposition = targetasteroid.points[1];
	rotatev2(tempposition, tempsin, tempcos);
	v2 tempvecC = {tempposition.x * size, tempposition.y * size};

	v2 tempvecP = {incpoint.x - targetasteroid.position.x, incpoint.y - targetasteroid.position.y};

	// Compute vectors
	v2 vec0 = {tempvecC.x - tempvecA.x,
				tempvecC.y - tempvecA.y};
	v2 vec1 = {tempvecB.x - tempvecA.x,
				tempvecB.y - tempvecA.y};
	v2 vec2 = {tempvecP.x - tempvecA.x,
				tempvecP.y - tempvecA.y};

	// Compute dot products
	float dot00 = dot(vec0, vec0);
	float dot01 = dot(vec0, vec1);
	float dot02 = dot(vec0, vec2);
	float dot11 = dot(vec1, vec1);
	float dot12 = dot(vec1, vec2);

	// Compute barycentric coordinates
	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	Return = ((u >= 0) && (v >= 0) && ((u + v) < 1));

 	if(Return && !hit)
	{
		debugvecA = targetasteroid.position;
		debugvecB = tempvecB + targetasteroid.position;
		debugvecC = tempvecC + targetasteroid.position;
		debugvecP = tempvecP + targetasteroid.position;
		hit = true;
	}

	return(Return);
}


float distance = 9;

	//float angle = 145.0f*(M_PI/180);
	float angle = 2.53073f;
	float tempcos = cos(angle);
	float tempsin = sin(angle);
	playership[1] = {distance*(tempcos-tempsin),distance*(tempcos+tempsin)};

	//angle = -145.0f*(M_PI/180);
	angle = -angle;
	clamp_wrap(angle,0,2.0f*M_PI);
	tempcos = cos(angle);
	tempsin = sin(angle);
	playership[2] = {distance*(tempcos-tempsin),distance*(tempcos+tempsin)};


	distance = 6;

	//angle = 140*(M_PI/180);
	angle = 2.44346f;
	tempcos = cos(angle);
	tempsin = sin(angle);
	playership[3] = {distance*(tempcos-tempsin),distance*(tempcos+tempsin)};


	angle = -angle;
	clamp_wrap(angle,0,2.0f*M_PI);
	tempcos = cos(angle);
	tempsin = sin(angle);
	playership[4] = {distance*(tempcos-tempsin),distance*(tempcos+tempsin)};

	distance = 10;

	angle = (float)M_PI;
	tempcos = cos(angle);
	tempsin = sin(angle);
	playership[5] = {distance*(tempcos-tempsin),distance*(tempcos+tempsin)};

	distance = 5;

	//angle = 20*(M_PI/180);
	angle = 0.349066f;
	tempcos = cos(angle);
	tempsin = sin(angle);
	playership[6] = {distance*(tempcos-tempsin),distance*(tempcos+tempsin)};
	//angle = -20*(M_PI/180);
	angle = -angle;
	clamp_wrap(angle,0,2.0f*M_PI);
	tempcos = cos(angle);
	tempsin = sin(angle);
	playership[7] = {distance*(tempcos-tempsin),distance*(tempcos+tempsin)};


#if 0
	float distance = 9;

	//float angle = 145.0f*(M_PI/180);
	float angle = 2.53073f;
	float tempcos = cos(angle);
	float tempsin = sin(angle);
	playership[1] = {distance*(tempcos-tempsin),distance*(tempcos+tempsin)};

	//angle = -145.0f*(M_PI/180);
	angle = -angle;
	clamp_wrap(angle,0,2.0f*M_PI);
	tempcos = cos(angle);
	tempsin = sin(angle);
	playership[2] = {distance*(tempcos-tempsin),distance*(tempcos+tempsin)};


	distance = 6;

	//angle = 140*(M_PI/180);
	angle = 2.44346f;
	tempcos = cos(angle);
	tempsin = sin(angle);
	playership[3] = {distance*(tempcos-tempsin),distance*(tempcos+tempsin)};


	angle = -angle;
	clamp_wrap(angle,0,2.0f*M_PI);
	tempcos = cos(angle);
	tempsin = sin(angle);
	playership[4] = {distance*(tempcos-tempsin),distance*(tempcos+tempsin)};

	distance = 10;

	angle = (float)M_PI;
	tempcos = cos(angle);
	tempsin = sin(angle);
	playership[5] = {distance*(tempcos-tempsin),distance*(tempcos+tempsin)};

	distance = 5;

	//angle = 20*(M_PI/180);
	angle = 0.349066f;
	tempcos = cos(angle);
	tempsin = sin(angle);
	playership[6] = {distance*(tempcos-tempsin),distance*(tempcos+tempsin)};
	//angle = -20*(M_PI/180);
	angle = -angle;
	clamp_wrap(angle,0,2.0f*M_PI);
	tempcos = cos(angle);
	tempsin = sin(angle);
	playership[7] = {distance*(tempcos-tempsin),distance*(tempcos+tempsin)};
#endif


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

