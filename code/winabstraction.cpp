
union WinColor
{
	struct{ unsigned char b, g, r, x; };
	struct{ unsigned char b, g, r, a; };
	unsigned char data[4];
	unsigned int color;
};

WinColor CurrentColor;
WinColor PreviousColor;
WinColor Saved_CurrentColor;
WinColor Saved_PreviousColor;

void SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	PreviousColor = CurrentColor;

	CurrentColor.r = r;
	CurrentColor.g = g;
	CurrentColor.b = b;
	CurrentColor.a = a;
}

void SetDrawColor(unsigned char r, unsigned char g, unsigned char b)
{
	SetDrawColor(r, g, b, (unsigned char)255);
}

void UnsetDrawColor()
{
	SetDrawColor(PreviousColor.r,PreviousColor.g,PreviousColor.b,PreviousColor.a);
}

void Debug_SaveDrawColors()
{
	Saved_CurrentColor = CurrentColor;
	Saved_PreviousColor = PreviousColor;
}

void Debug_LoadDrawColors()
{
	CurrentColor = Saved_CurrentColor;
	PreviousColor = Saved_PreviousColor;
}

int
Debug_DrawLine(v2* inca, v2* incb)
{
	int Result;
	//*inca = *inca + BOARD_OFFSET;
	//*incb = *incb + BOARD_OFFSET;
	Result = clip_line(inca,incb);

	if(Result)
	WinSimpleDrawLine(CurrentColor.color,
					(int)inca->x,(int)inca->y,
					(int)incb->x,(int)incb->y);
	return(Result);
}

#if 0
int
Debug_DrawLine(v2 inca, v2 incb)
{
	int Result;
	v2 outa = inca;
	v2 outb = incb;
#if 0
	WinSimpleDrawLine(0x55555555,
					(int)outa.x + BOARD_X_OFFSET,(int)outa.y + BOARD_Y_OFFSET,
					(int)outb.x + BOARD_X_OFFSET,(int)outb.y + BOARD_Y_OFFSET);
#endif
	Result = Debug_DrawLine(&outa,&outb);

	outa = outa - BOARD_OFFSET;
	outb = outb - BOARD_OFFSET;
#if 0
	if((int)inca.x != (int)outa.x || (int)inca.y != (int)outa.y)
	{
		v2 tempouta = inca;
		v2 tempoutb = outa;

		clamp_wrap(tempouta.x, (float)0, (float)BOARD_WIDTH);
		clamp_wrap(tempoutb.x, (float)0, (float)BOARD_WIDTH);
		clamp_wrap(tempouta.y, (float)0, (float)BOARD_HEIGHT);
		clamp_wrap(tempoutb.y, (float)0, (float)BOARD_HEIGHT);

		//DrawRealLine(tempouta,tempoutb);
	}
#endif
	return(Result);
}

#if 0
void DrawLine(v2i inca, v2i incb)
{
	WinSimpleDrawLine(CurrentColor.color,inca.x,inca.y,incb.x,incb.y);
}
#endif

#endif

void DrawLine(v2 inca, v2 incb)
{
	v2 vec0 = inca;
	v2 vec1 = incb;

	//vec0 += BOARD_OFFSET;
	//vec1 +=  BOARD_OFFSET;

	int status = clip_line(&vec0,&vec1);

	if(status != 0)
	{
		WinSimpleDrawLine(CurrentColor.color,(int)vec0.x,(int)vec0.y,(int)vec1.x,(int)vec1.y);
	}

	// BUG: this will fuck up if lines are off the screen on opposite sides
	//		ie: off left side and off right side
	// NOTE: this shouldn't happen though so we should be fine
	// wraparound
	if( status != 1 )
	{
		float tempx = 0;
		float tempy = 0;

		int rerender_x = false;
		int rerender_y = false;

		if(inca.x < 0 || incb.x < 0)
		{
			rerender_x = -1;
		}

		if(inca.x > BOARD_WIDTH || incb.x > BOARD_WIDTH)
		{
			rerender_x = 1;
		}

		if(inca.y < 0 || incb.y < 0)
		{
			rerender_y = -1;
		}

		if(inca.y > BOARD_HEIGHT || incb.y > BOARD_HEIGHT)
		{
			rerender_y = 1;
		}

		if(rerender_x != 0)
		{
			tempx = 0.0f - rerender_x * BOARD_WIDTH;

			vec0 = inca;
			vec1 = incb;
			//vec0 = inca + BOARD_OFFSET;
			//vec1 = incb + BOARD_OFFSET;

			vec0.x += tempx;
			vec1.x += tempx;

			if(clip_line(&vec0,&vec1))
				WinSimpleDrawLine(CurrentColor.color,(int)vec0.x,(int)vec0.y,(int)vec1.x,(int)vec1.y);
		}

		if(rerender_y != 0)
		{
			tempy = 0.0f - rerender_y * BOARD_HEIGHT;

			vec0 = inca;
			vec1 = incb;
			//vec0 = inca + BOARD_OFFSET;
			//vec1 = incb + BOARD_OFFSET;

			vec0.y += tempy;
			vec1.y += tempy;

			if(clip_line(&vec0,&vec1))
				WinSimpleDrawLine(CurrentColor.color,(int)vec0.x,(int)vec0.y,(int)vec1.x,(int)vec1.y);
		}

		if(rerender_x && rerender_y)
		{
			vec0 = inca;
			vec1 = incb;
			//vec0 = inca + BOARD_OFFSET;
			//vec1 = incb + BOARD_OFFSET;

			vec0.x += tempx;
			vec1.x += tempx;
			vec0.y += tempy;
			vec1.y += tempy;

			if(clip_line(&vec0,&vec1))
				WinSimpleDrawLine(CurrentColor.color,(int)vec0.x,(int)vec0.y,(int)vec1.x,(int)vec1.y);
		}

	}
}

void DrawRealLine(v2 inca, v2 incb)
{
	WinSimpleDrawLine(CurrentColor.color,(int)inca.x,(int)inca.y,(int)incb.x,(int)incb.y);
}

void DrawPoint( v2 incpoint )
{
	WinSimpleDrawPoint(CurrentColor.color, (int)incpoint.x, (int)incpoint.y);
}

void ClearFramebuffer()
{
	SetDrawColor(0x00,0x00,0x00,0xff);
	WinSimpleClearBuffer();
	UnsetDrawColor();
}

void SwapBuffer()
{
	WinSimpleSwapBuffer();
}

void Wait()
{
	WinVsync();
}

void PlaySound(int soundnumber)
{

}

int LoopSound(int soundnumber)
{
	return 0;
}

void SoundVolume(int channelnumber, float soundvolume)
{

}

void FadeOutSound(int channelnumber)
{

}

void StopSounds()
{

}
