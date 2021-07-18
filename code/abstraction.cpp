
void DrawLine(v2 inca, v2 incb)
{
	SDL_RenderDrawLine(gRenderer,(int)inca.x,(int)inca.y,(int)incb.x,(int)incb.y);
}

void DrawPoint( v2 incpoint )
{
	SDL_RenderDrawPoint(gRenderer, (int)incpoint.x, (int)incpoint.y);
}

unsigned char cr;
unsigned char cg;
unsigned char cb;
unsigned char ca;
unsigned char pr;
unsigned char pg;
unsigned char pb;
unsigned char pa;

void SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SDL_SetRenderDrawColor(gRenderer,r,g,b,a);
	pr = cr; pg = cg; pb = cb; pa = ca;
	cr = r; cg = g; cb = b; pa = a;
}

void SetDrawColor(unsigned char r, unsigned char g, unsigned char b)
{
	SetDrawColor(r, g, b, (unsigned char)255);
}

void UnsetDrawColor()
{
	SetDrawColor(pr,pg,pb,pa);
}

void ClearFramebuffer()
{
	SetDrawColor(0x00,0x00,0x00,0xff);
	SDL_RenderClear( gRenderer );
	SetDrawColor(0xff,0xff,0xff,0xff);
}

void SwapBuffer()
{
	SDL_RenderPresent( gRenderer );
}

void PlaySound(int soundnumber)
{
	Mix_PlayChannel( -1, gSounds[soundnumber], 0 );
}

int LoopSound(int soundnumber)
{
	int Return;
	Return = Mix_PlayChannel( -1, gSounds[soundnumber], -1 );
	return(Return);
}

void SoundVolume(int channelnumber, float soundvolume)
{
	int volume = (int)(MIX_MAX_VOLUME * soundvolume);
	Mix_Volume(channelnumber, volume);
}

void FadeOutSound(int channelnumber)
{
	Mix_FadeOutChannel(channelnumber, 40);
}

void StopSounds()
{
	Mix_HaltChannel(-1);
}
