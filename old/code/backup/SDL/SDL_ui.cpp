
#include <SDL.h>
#include <SDL_ttf.h>
#include <String>

static int UI_NONE = 0;

struct TestUIState
{
	int hot_uiid;
	int active_uiid;
	int mouse_x;
	int mouse_y;
	bool mouse_down;
	bool mouse_down_previous;
	bool mouse_went_down;
	bool mouse_went_up;
};

TestUIState UIState;

void ui_update()
{
	UIState.hot_uiid = 0;
	UIState.mouse_down_previous = UIState.mouse_down;
	UIState.mouse_down = inputstate.mousebutton[0].down;
	UIState.mouse_went_down = inputstate.mousebutton[0].wentdown;
	UIState.mouse_went_up = inputstate.mousebutton[0].wentup;

	UIState.mouse_x = inputstate.mousex;
	UIState.mouse_y = inputstate.mousey;
}

bool testbutton_render(int uiid, char* text, int x, int y, int width, int height)
{
	bool Return = false;

	SDL_Rect rect = {x,y,width,height};

	// if mouse is over button
	if(UIState.mouse_x > x &&
		UIState.mouse_x < x+width &&
		UIState.mouse_y > y &&
		UIState.mouse_y < y+height)
	{
		if(!UIState.active_uiid || UIState.active_uiid == uiid )
			UIState.hot_uiid = uiid;
	}

	if(UIState.active_uiid == uiid)
	{
		if(inputstate.mousebuttonwentup)
		{
			if(UIState.hot_uiid == uiid)
			{
				Return = true;
			}
			UIState.active_uiid = 0;
		}
	} 
	else if (UIState.hot_uiid == uiid)
	{
		if(UIState.mouse_went_down)
			UIState.active_uiid = uiid;
	}

	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
	SDL_SetRenderDrawColor(gRenderer,0x55,0x55,0x55,0xff);
	SDL_RenderFillRect(gRenderer, &rect);

	int border = 4;
	rect.x = x+border;
	rect.y = y+border;
	rect.w = width-border*2;
	rect.h = height-border*2;
	if(UIState.hot_uiid == uiid)
		SDL_SetRenderDrawColor(gRenderer,0xff,0xff,0xff,0xff);
	else
		SDL_SetRenderDrawColor(gRenderer,0xcc,0xcc,0xcc,0xff);

	SDL_RenderFillRect(gRenderer, &rect);

	// using fast text because we aren't caching yet
	SDL_Color color_black = {0,0,0,255};
	SDL_Surface* tempsurface = TTF_RenderText_Blended( gFont, 
		text, color_black );

	SDL_Texture* temptext = SDL_CreateTextureFromSurface( gRenderer, tempsurface);

	///rect = SDL_Rect{x+(width - tempsurface->w) / 2, y + (height - tempsurface->h) / 2, tempsurface->w, tempsurface->h};
	rect.x = x+(width - tempsurface->w) / 2;
	rect.y = y + (height - tempsurface->h) / 2;
	rect.w = tempsurface->w;
	rect.h = tempsurface->h;

	SDL_RenderCopyEx( gRenderer, temptext, NULL, &rect,NULL,NULL,SDL_FLIP_NONE);

	SDL_FreeSurface( tempsurface );
	SDL_DestroyTexture( temptext );

	return(Return);
}

enum text_justification {tj_left, tj_right, tj_middle};
void testtext_render(const char* text, int x, int y, text_justification tj)
{
	// using fast text because we aren't caching yet
	SDL_Color color_white = {drawR,drawG,drawB,255};
	SDL_Surface* tempsurface = TTF_RenderText_Blended( gFont, text, color_white );

	SDL_Texture* temptext = SDL_CreateTextureFromSurface( gRenderer, tempsurface);
	SDL_Rect rect = {x,y,tempsurface->w,tempsurface->h};
	if(tj == tj_left)
	{
		rect.x = x;
	} else if(tj == tj_right)
	{
		rect.x = x-tempsurface->w;
	} else if(tj == tj_middle)
	{
		rect.x = x-(tempsurface->w/2);
	}

	//rect = SDL_Rect{x+(width - tempsurface->w) / 2, y + (height - tempsurface->h) / 2, tempsurface->w, tempsurface->h};

	SDL_RenderCopyEx( gRenderer, temptext, NULL, &rect,NULL,NULL,SDL_FLIP_NONE);

	SDL_FreeSurface( tempsurface );
	SDL_DestroyTexture( temptext );
}
