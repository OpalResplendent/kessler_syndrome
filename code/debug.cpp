
void
ks_debugrenderborders()
{
	SetDrawColor(100,100,100);

	//DrawLine( v2{0-BOARD_X_OFFSET,0}, v2{(float)(BOARD_WIDTH + BOARD_X_OFFSET * 2), 0} );
	//DrawLine( v2{0,0-BOARD_Y_OFFSET}, v2{0, (float)(BOARD_HEIGHT + BOARD_Y_OFFSET * 2)} );
	//DrawLine( v2{0-BOARD_X_OFFSET,BOARD_HEIGHT}, v2{(float)BOARD_WIDTH+BOARD_X_OFFSET,(float)BOARD_HEIGHT} );
	//DrawLine( v2{(float)BOARD_WIDTH,0-BOARD_Y_OFFSET}, v2{(float)BOARD_WIDTH,(float)BOARD_HEIGHT+BOARD_Y_OFFSET} );

	SetDrawColor(255,255,255);

	//DrawLine(v2{0,0},v2{(float)BOARD_WIDTH-1,0});
	//DrawLine(v2{0,0},v2{0,(float)BOARD_HEIGHT-1});
	//DrawLine(v2{(float)BOARD_WIDTH-1,(float)BOARD_HEIGHT-1},v2{0,(float)BOARD_HEIGHT-1});
	//DrawLine(v2{(float)BOARD_WIDTH-1,(float)BOARD_HEIGHT-1},v2{(float)BOARD_WIDTH-1,0});
}

void
ks_debugrender()
{
	Debug_SaveDrawColors();

	//v2 debugpointa = {210-BOARD_X_OFFSET,802-BOARD_Y_OFFSET};
	//v2 debugpointb = {183-BOARD_X_OFFSET,790-BOARD_Y_OFFSET};

	v2 debugpointa = {230-BOARD_X_OFFSET,810-BOARD_Y_OFFSET};
	v2 debugpointb = {300-BOARD_X_OFFSET,500-BOARD_Y_OFFSET};

	SetDrawColor(100,100,100);
	//DrawRealLine(debugpointa+BOARD_OFFSET,debugpointb+BOARD_OFFSET);
	SetDrawColor(255,0,0);
	DrawPoint(debugpointa+BOARD_OFFSET);
	DrawPoint(debugpointb+BOARD_OFFSET);

	SetDrawColor(100,100,255);
	Debug_DrawLine(&debugpointa,&debugpointb);

	//debugpointa = {210-BOARD_X_OFFSET,802-BOARD_Y_OFFSET};
	//debugpointb = {183-BOARD_X_OFFSET,790-BOARD_Y_OFFSET};

	debugpointa = {300-BOARD_X_OFFSET,500-BOARD_Y_OFFSET};
	debugpointb = {230-BOARD_X_OFFSET,810-BOARD_Y_OFFSET};

	SetDrawColor(100,100,100);
	//DrawRealLine(debugpointa+BOARD_OFFSET,debugpointb+BOARD_OFFSET);
	SetDrawColor(255,0,0);
	DrawPoint(debugpointa+BOARD_OFFSET);
	DrawPoint(debugpointb+BOARD_OFFSET);

	SetDrawColor(100,100,255);
	Debug_DrawLine(&debugpointb,&debugpointa);

	v2 debugpoint0 = {BOARD_WIDTH-50,50};
	v2 debugpoint1 = {BOARD_WIDTH+50,50};

	SetDrawColor(130,130,130);
	//DrawLine(debugpoint0,debugpoint1);
	SetDrawColor(255,0,255);
	Debug_DrawLine(&debugpoint0,&debugpoint1);

	debugpoint0 = {BOARD_WIDTH-50,50};
	debugpoint1.x = BOARD_WIDTH-50;
	debugpoint1.y = -50;

	SetDrawColor(130,130,130);
	//DrawLine(debugpoint0,debugpoint1);
	SetDrawColor(255,0,255);
	Debug_DrawLine(&debugpoint0,&debugpoint1);

	debugpoint0 = {BOARD_WIDTH-50,50};
	debugpoint1.x = BOARD_WIDTH+50;
	debugpoint1.y = -10;

	SetDrawColor(130,130,130);
	//DrawLine(debugpoint0,debugpoint1);
	SetDrawColor(255,0,255);
	Debug_DrawLine(&debugpoint0,&debugpoint1);
	
	debugpoint0 = {BOARD_WIDTH+10,45};
	debugpoint1 = {BOARD_WIDTH-50,-20};
	SetDrawColor(130,130,130);
	//DrawLine(debugpoint0,debugpoint1);
	SetDrawColor(255,0,255);
	Debug_DrawLine(&debugpoint0,&debugpoint1);

	debugpoint0 = {10,605};
	debugpoint1 = {-9,596};
	Debug_DrawLine(&debugpoint0,&debugpoint1);

	Debug_LoadDrawColors();
}