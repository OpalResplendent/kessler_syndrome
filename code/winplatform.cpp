#include <windows.h>
#include <dsound.h>




struct win_offscreen_buffer{
	// windows pixels: 32-bit, upside-down, BB GG RR XX
	BITMAPINFO Info;
	void* Memory;
	unsigned int Width;
    unsigned int Height;
    unsigned int Pitch;
    unsigned int BytesPerPixel;
};

struct win_window_dimension
{
	int Width;
    int Height;
};

win_offscreen_buffer gBackBuffer;
HWND gWindowHandle;
WNDCLASSEX gWindowClass;

LARGE_INTEGER gLastFrame;
long long gFrameTime;

LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Return = 0;

	switch(uMsg)
	{
		case WM_DESTROY:
		case WM_CLOSE:
        {
            gRunning = false;
        } break;


		//WM_NCRBUTTONDOWN nHittest:HTCAPTION
		case WM_NCLBUTTONDBLCLK:	// doubleclick maximize
        case WM_NCRBUTTONDOWN:		// right click freezing
        case WM_CONTEXTMENU:		// right click menu
        {
        	// disable this stuff
        } break;

        case WM_SYSCOMMAND:
        case WM_COMMAND:
        {
        	WPARAM sysParam = wParam & 0xFFF0;
        	switch(sysParam)
        	{
        		case SC_MOUSEMENU:
        		case SC_KEYMENU:
        		{
        			//if(lParam)
        		} break;
        		default:
        			Return = DefWindowProc(hwnd, uMsg, wParam, lParam);
					break;
        	}

        } break;
		default:
			Return = DefWindowProc(hwnd, uMsg, wParam, lParam);
			break;
	}

	return(Return);
}

void
WinInitDSound(HWND WindowHandle)
{
	int BufferSize = 48000 * 2;
	// load dsound.dll
	void* dsoundlibrary = LoadLibrary("dsound.dll");
	if( dsoundlibrary != NULL)
	{
		LPDIRECTSOUND DSObject = {};
		if( DirectSoundCreate(0, &DSObject, 0 ) == ERROR_SUCCESS)
		{
			WAVEFORMATEX waveformat = {}; // have to assign these out of order because windows is awesome
			waveformat.wFormatTag 		= WAVE_FORMAT_PCM;
			waveformat.nChannels 		= 2;
			waveformat.nSamplesPerSec 	= 48000; // 48kHz
			waveformat.wBitsPerSample 	= 16; // 8 or 16
			waveformat.nBlockAlign 		= (waveformat.nChannels * waveformat.wBitsPerSample) / 8;
			waveformat.nAvgBytesPerSec 	= waveformat.nSamplesPerSec * waveformat.nBlockAlign;
			waveformat.cbSize 			= 0; // ignored but still set to 0

			if( DSObject->SetCooperativeLevel(gWindowHandle,DSSCL_PRIORITY) == ERROR_SUCCESS)
			{
				DSBUFFERDESC bufferdesc = {};
				bufferdesc.dwSize = sizeof(bufferdesc);
				bufferdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
				// because this is primary other things are set automatically

				LPDIRECTSOUNDBUFFER dsbuffer;
				if( DSObject->CreateSoundBuffer(&bufferdesc, &dsbuffer, 0) == ERROR_SUCCESS)
				{

					if( dsbuffer->SetFormat(&waveformat) == ERROR_SUCCESS)
					{
						// we deeed it! primary buffer online
						// need to create secondary buffer so we can actually do the sounds

					}
				}
			}

			// secondary buffer here?
			DSBUFFERDESC scondarybufferdesc;
			scondarybufferdesc.dwSize = sizeof(scondarybufferdesc);
			scondarybufferdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2;
			scondarybufferdesc.dwBufferBytes = BufferSize;
			scondarybufferdesc.lpwfxFormat = &waveformat;

			LPDIRECTSOUNDBUFFER secondarybuffer;
			if(DSObject->CreateSoundBuffer(&scondarybufferdesc, &secondarybuffer, 0) == ERROR_SUCCESS)
			{

			} else {
				MessageBox(NULL, "secondary buffer failed", "Error!",
            		MB_ICONEXCLAMATION | MB_OK);
			}

		}
	} else {
		MessageBox(NULL, "LoadLibrary(\"dsound.dll\") Failed", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
		//printf("LoadLibrary(\"dsound.dll\") Failed");
	}

	// create object w/ IDirectSound8::DirectSoundCreate8()
	// https://msdn.microsoft.com/en-us/library/microsoft.directx_sdk.reference.directsoundcreate8.aspx
	// HRESULT WINAPI DirectSoundCreate(LPGUID lpGuid, LPDIRECTSOUND* ppDS, LPUNKNOWN  pUnkOuter );
	//LPDIRECTSOUND DSObject = 0;
	//DirectSoundCreate(0, &DSObject, 0 ); // 0 autoselects, pointer we provided, and other thing is garbage

	// set cooperateive level w/ IDirectSound8::SetCooperativeLevel()
	// https://msdn.microsoft.com/en-us/library/microsoft.directx_sdk.idirectsound8.idirectsound8.setcooperativelevel.aspx
	//DSObject->SetCooperativeLevel(gWindowHandle,DSSCL_NORMAL);

	// create primary buffer w/ IDirectSound8::CreateSoundBuffer()
	// https://msdn.microsoft.com/en-us/library/microsoft.directx_sdk.idirectsound8.idirectsound8.createsoundbuffer.aspx

	// create secondary buffer w/ a ring buffer
	// http://c.learncodethehardway.org/book/ex44.html

	// play secondary buffer w/ IDirectSoundBuffer8::Play()

}

void
WinSimpleClearBuffer()
{
	unsigned int* pixels = (unsigned int *)gBackBuffer.Memory;

	unsigned int pixelssize = gBackBuffer.Width * gBackBuffer.Height;

	for(unsigned int ii = 0; ii < pixelssize; ++ii)
	{
		pixels[ii] = 0x00000000;
	}
}

void
WinDrawPoint(win_offscreen_buffer *Buffer,
				unsigned int value, int x0, int y0)
{
	if(x0 < 0 || y0 < 0
		|| x0 > (int)Buffer->Width - 1
		|| y0 > (int)Buffer->Height - 1)
		{return;}

	unsigned int* pixels = (unsigned int *)Buffer->Memory;

	pixels[x0 + y0 * gBackBuffer.Width] = value;
}

void
WinSimpleDrawPoint( unsigned int value, int x0, int y0 )
{
	y0 = gBackBuffer.Height - y0;
	WinDrawPoint( &gBackBuffer, value, x0, y0 );
}

void
WinDrawLine( win_offscreen_buffer *Buffer, unsigned int value,
			int x0, int y0, int x1, int y1 )
{
	y0 = Buffer->Height - 1 - y0;
	y1 = Buffer->Height - 1 - y1;

	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
	int err = (dx>dy ? dx : -dy)/2, e2;

	unsigned int* pixels = (unsigned int *)Buffer->Memory;

	for(;;)
	{
		//WinDrawPoint( Buffer, value, x0, y0);
		pixels[x0 + y0 * gBackBuffer.Width] = value;
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void
WinSimpleDrawLine(unsigned int value, int x0, int y0, int x1, int y1)
{

	WinDrawLine( &gBackBuffer,  value, x0, y0, x1, y1 );
}

// TODO: stopped here
/*
void
WinSimpleDrawLine(unsigned int value, float x0, float y0, float x1, float y1)
{

	WinDrawLine( &gBackBuffer,  value, x0, y0, x1, y1 );
}
*/

win_window_dimension
WinGetWindowDimension(HWND Window)
{
    win_window_dimension Result;

    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;

    return(Result);
}

void
WinResizeDIBSection( win_offscreen_buffer *Buffer, int Width, int Height )
{
	Buffer->Width = SCREEN_WIDTH;
	Buffer->Height = SCREEN_HEIGHT;
	Buffer->BytesPerPixel = 4;
	Buffer->Pitch = gBackBuffer.BytesPerPixel * gBackBuffer.Width;
}

void
WinSwapBuffer(win_offscreen_buffer *Buffer, HDC DeviceContext)
{
    StretchDIBits( DeviceContext,
    				0, 0, Buffer->Width, Buffer->Height,
    				0, 0, Buffer->Width, Buffer->Height,
    				gBackBuffer.Memory, &gBackBuffer.Info,
    				DIB_RGB_COLORS, SRCCOPY );
}

void
WinVsync()
{
	LARGE_INTEGER currentframe;
	long long sleeptime;

	QueryPerformanceCounter(&currentframe);

	sleeptime = gFrameTime - (currentframe.QuadPart - gLastFrame.QuadPart) / 100000;

	sleeptime = abs(sleeptime - 1);
	if(sleeptime > 15)
	{
		sleeptime = 15;
	}


	Sleep((DWORD)sleeptime);

	QueryPerformanceCounter(&gLastFrame);

}

void
WinSimpleSwapBuffer()
{
	HDC DeviceContext = GetDC(gWindowHandle);
    WinSwapBuffer(&gBackBuffer, DeviceContext);
    ReleaseDC(gWindowHandle, DeviceContext);
}

void
WinProcessMessages()
{
	MSG Message;
    while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
    {
        switch(Message.message)
        {
            case WM_QUIT:
            {
                gRunning = false;
            } break;

            case WM_SYSCOMMAND:
            case WM_COMMAND:
	        {
	        	WPARAM sysParam = Message.wParam & 0xFFF0;
	        	switch(sysParam)
	        	{
	        		case SC_KEYMENU:
	        		{
	        			//if(lParam)
	        		} break;
	        		case SC_MOUSEMENU:
	        		{
	        			//
	        		} break;
	        		default:
	        			TranslateMessage(&Message);
                		DispatchMessageA(&Message);
						break;
	        	}

	        } break;

            case WM_PAINT:
            {
            	//ClearWindow;

            	PAINTSTRUCT Paint;
            	HDC DeviceContext = BeginPaint(gWindowHandle, &Paint);
            	win_window_dimension Dimension = WinGetWindowDimension(gWindowHandle);
            	WinSwapBuffer( &gBackBuffer, DeviceContext );
            	EndPaint(gWindowHandle, &Paint);
            } break;

            default:
            {
                TranslateMessage(&Message);
                DispatchMessageA(&Message);
            } break;
        }
    }
}

int
WindowsInit( HINSTANCE hInstance, char* title, unsigned int Width, unsigned int Height )
{
	// register window class
	gWindowClass.style 			= CS_HREDRAW | CS_VREDRAW;
	gWindowClass.lpfnWndProc 	= WindowProc;
	gWindowClass.hInstance 		= hInstance;
	gWindowClass.hCursor 		= LoadCursor(0, IDC_ARROW);
	gWindowClass.hbrBackground 	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	gWindowClass.lpszClassName 	= "RandomWindowClass";

	// making sure everything is correct for ex
	gWindowClass.lpszMenuName 	= NULL;
	gWindowClass.hIcon 			= LoadIcon(NULL, IDI_APPLICATION);
	gWindowClass.cbSize 			= sizeof(WNDCLASSEX);
	gWindowClass.hIconSm 		= LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&gWindowClass))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    WinResizeDIBSection(&gBackBuffer,Width,Height);

	RECT lpRect = {100 , 100, (LONG)gBackBuffer.Width + 100, (LONG)gBackBuffer.Height + 100};
    DWORD dwStyle = (WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX)) | WS_VISIBLE;
    DWORD dwExStyle = 0;
    AdjustWindowRectEx( &lpRect, dwStyle, 0, dwExStyle);

	// create the window
	gWindowHandle = CreateWindowEx( dwExStyle,
						gWindowClass.lpszClassName,
						title,
						dwStyle,
						CW_USEDEFAULT, CW_USEDEFAULT,
						lpRect.right - lpRect.left,
						lpRect.bottom - lpRect.top,
						0, 0, hInstance, 0 );

	if(gWindowHandle == NULL)
    {
    	unsigned long error = GetLastError();
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

	HDC hdc = GetDC(NULL);
	long long WinRefreshRate = GetDeviceCaps(hdc, VREFRESH);
	ReleaseDC(NULL, hdc);
	gFrameTime = 1000/WinRefreshRate;

	timeBeginPeriod(1);
	QueryPerformanceCounter(&gLastFrame);

	int BitmapMemorySize = (gBackBuffer.Pitch * gBackBuffer.Height);
	gBackBuffer.Memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

	gBackBuffer.Info.bmiHeader.biSize = sizeof(gBackBuffer.Info.bmiHeader);
    gBackBuffer.Info.bmiHeader.biWidth = gBackBuffer.Width;
    gBackBuffer.Info.bmiHeader.biHeight = gBackBuffer.Height;
    gBackBuffer.Info.bmiHeader.biPlanes = 1;
    gBackBuffer.Info.bmiHeader.biBitCount = 32;
    gBackBuffer.Info.bmiHeader.biCompression = BI_RGB;

    WinInitDSound(gWindowHandle);

	return 0;
}

void WindowsKill()
{
	timeEndPeriod(1);
}
