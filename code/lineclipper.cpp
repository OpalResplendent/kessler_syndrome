
// ABRL = Above, Below, Right, Left
const unsigned char LC_ABOVE = 0x8;
const unsigned char LC_BELOW = 0x4;
const unsigned char LC_RIGHT = 0x2;
const unsigned char LC_LEFT = 0x1;

/*
const float XMIN = BOARD_X_OFFSET;
const float XMAX = BOARD_WIDTH + BOARD_X_OFFSET;
const float YMIN = BOARD_Y_OFFSET;
const float YMAX = BOARD_HEIGHT + BOARD_Y_OFFSET;
*/

const float XMIN = 0;
const float XMAX = BOARD_WIDTH-1;
const float YMIN = 0;
const float YMAX = BOARD_HEIGHT-1;

#define Outcode unsigned char

unsigned char
get_ABRL(v2* point)
{
	unsigned char Result = 0;

	if( point->x < XMIN )
		Result += LC_LEFT;
	else if( point->x > XMAX )
		Result += LC_RIGHT;

	// backwards?
	if( point->y < YMIN)
		Result += LC_BELOW;
	else if( point->y > YMAX )
		Result += LC_ABOVE;

	return(Result);
}

int
clip_line(v2* pointa, v2* pointb)
{
	int Result = 0; // whether any part of the line appears on screen

	Outcode outcodes[2];
	outcodes[0] = get_ABRL(pointa);
	outcodes[1] = get_ABRL(pointb);

	if( !(outcodes[0] || outcodes[1]) )
	{
		Result = 1;
	} else if( (outcodes[0] & outcodes[1]) )
	{
		// if both are outside and have a common outside don't draw
		Result = 0;
	} else {

		v2* pointers[2];
		pointers[0] = pointa;
		pointers[1] = pointb;

		v2* currentpointer;
		v2* otherpointer;

		int loopsleft = !!outcodes[0] + !!outcodes[1];

		Result = loopsleft + 1;

		float slope = 0;
		if(pointa->x != pointb->x)
		{
			slope = (pointa->y - pointb->y)/(pointa->x - pointb->x); // calculating here since we use it everywhere
		}

		while(loopsleft > 0)
		{
			// select the pointer
			int current;

			if(outcodes[0])
			{
				current = 0;
				currentpointer = pointers[0];
				otherpointer = pointers[1];
			} else {
				current = 1;
				currentpointer = pointers[1];
				otherpointer = pointers[0];
			}

			switch(outcodes[current])
			{
				// clip below left
				case 0x5:
				{
					v2 tempa;
					v2 tempb;
					float dista = 2000;
					float distb = 2000;

					// checkleft
					tempa.x = XMIN;
					tempa.y = otherpointer->y + (XMIN - otherpointer->x) * slope;

					// checkbelow
					if(!!slope)
						tempb.x = otherpointer->x + (YMIN - otherpointer->y) / slope;
					else
						tempb.x = currentpointer->x;
					tempb.y = YMIN;

					dista = sqrt(pow((otherpointer->x - tempa.x),2)+pow((otherpointer->y - tempa.y),2));
					distb = sqrt(pow((otherpointer->x - tempb.x),2)+pow((otherpointer->y - tempb.y),2));

					*currentpointer = ( (dista < distb) ) ? tempa : tempb;
				}break;

				// clip below right
				case 0x6:
				{
					v2 tempa;
					v2 tempb;
					float dista = 2000;
					float distb = 2000;

					// checkright
					tempa.x = XMAX;
					tempa.y = otherpointer->y + (XMAX - otherpointer->x) * slope;

					// checkbelow
					if(!!slope)
						tempb.x = otherpointer->x + (YMIN - otherpointer->y) / slope;
					else
						tempb.x = currentpointer->x;
					tempb.y = YMIN;

					dista = sqrt(pow((otherpointer->x - tempa.x),2)+pow((otherpointer->y - tempa.y),2));
					distb = sqrt(pow((otherpointer->x - tempb.x),2)+pow((otherpointer->y - tempb.y),2));

					*currentpointer = ( (dista < distb) ) ? tempa : tempb;

				}break;

				// clip above left
				case 0x9:
				{
					v2 tempa;
					v2 tempb;
					float dista = 2000;
					float distb = 2000;

					// checkleft
					tempa.x = XMIN;
					tempa.y = otherpointer->y + (XMIN - otherpointer->x) * slope;

					// checkabove
					if(!!slope)
						tempb.x = otherpointer->x + (YMAX - otherpointer->y) / slope;
					else
						tempb.x = currentpointer->x;
					tempb.y = YMAX;

					dista = sqrt(pow((otherpointer->x - tempa.x),2)+pow((otherpointer->y - tempa.y),2));
					distb = sqrt(pow((otherpointer->x - tempb.x),2)+pow((otherpointer->y - tempb.y),2));

					*currentpointer = ( (dista < distb) ) ? tempa : tempb;
				}break;

				// clip above right
				case 0xA:
				{
					v2 tempa;
					v2 tempb;
					float dista = 2000;
					float distb = 2000;

					// checkright
					tempa.x = XMAX;
					tempa.y = otherpointer->y + (XMAX - otherpointer->x) * slope;

					// checkabove
					if(!!slope)
						tempb.x = otherpointer->x + (YMAX - otherpointer->y) / slope;
					else
						tempb.x = currentpointer->x;
					tempb.y = YMAX;

					dista = sqrt(pow((otherpointer->x - tempa.x),2)+pow((otherpointer->y - tempa.y),2));
					distb = sqrt(pow((otherpointer->x - tempb.x),2)+pow((otherpointer->y - tempb.y),2));

					*currentpointer = ( (dista < distb) ) ? tempa : tempb;
				}break;

				// clip left
				case 0x1:
				{
					currentpointer->x = XMIN;
					currentpointer->y = otherpointer->y + (XMIN - otherpointer->x) * slope;
				}break;

				// clip right
				case 0x2:
				{
					currentpointer->x = XMAX;
					currentpointer->y = otherpointer->y + (XMAX - otherpointer->x) * slope;
				}break;

				// clip below
				case 0x4:
				{
					if(!!slope)
					{
						currentpointer->x = otherpointer->x + (YMIN - otherpointer->y) / slope;
					} else {
						currentpointer->x = currentpointer->x;
					}
					currentpointer->y = YMIN;
				}break;

				// clip above
				case 0x8:
				{
					if(!!slope)
					{
						currentpointer->x = otherpointer->x + (YMAX - otherpointer->y) / slope;
					} else {
						currentpointer->x = currentpointer->x;
					}
					currentpointer->y = YMAX;
				}break;

				default:
				{
					// shouldn't happen
					ASSERT(false);
				}break;

			}

			if(!get_ABRL(pointers[current]))
			{
				loopsleft--;
				outcodes[current] = 0;
			} else {
				Result = 0;
				return(Result);
			}

		}

	}

	return(Result);
}
