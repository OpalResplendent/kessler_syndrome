
unsigned int SEED;

void rand_seed()
{
	SEED = (unsigned int)time(NULL);
	srand(SEED);
}

float frand()
{
	float Return;

	Return = (float)((float)rand()/(float)RAND_MAX);

	return(Return);
}

float frand(float min, float max)
{
	float Return;

	// note: this seems bad
	Return = min + frand()*(max-min);

	return(Return);
}


int irand(int min, int max)
{
	int Return;

	// note: +1 since int is floor, so we want one above
	// note: this still seems bad
	Return = min + (int)(frand()*(max-min+1));

	return(Return);

}
