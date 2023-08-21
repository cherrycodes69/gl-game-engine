
#include <stdio.h>
#include <windows.h>		


#include "Object2D.h"

#include <math.h>
#include <string>
#include "TextureCollection.h"

using namespace::std;

 HGLRC		rc=NULL;	
 HDC		dc=NULL;		
 HWND		wnd=NULL;		
 HINSTANCE	hInstance;		
 bool	ready=TRUE;		

 GLuint ballTX[4];
 double playerpos=0;
 double pcpos=0;

 bool start=true;	


 TopContainer bf(1);


int DrawGLScene(GLvoid);


int InitGL(GLvoid);
GLvoid KillGLWindow(GLvoid);
GLvoid resize(GLsizei width, GLsizei height);
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL startglwindow(char* title, int width, int height, int bits);
AUX_RGBImageRec *LoadBMP(char *Filename) ;

void Keyboard_Input();

void readtexture(char *loc,GLuint TexHandle);
void drawTarget();

int WINAPI WinMain(	HINSTANCE	hInstance,HINSTANCE	hPrevInstance,LPSTR	lpCmdLine,int nCmdShow)			
{
	MSG		msg;									
	BOOL	done=FALSE;								
    startglwindow("Collision Detection",800,600,32);



	while(!done)									
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	
		{
			if (msg.message==WM_QUIT)				
			{
				done=TRUE;							
			}
			else									
			{
				TranslateMessage(&msg);				
				DispatchMessage(&msg);				
			}
		}
		else										
		{
				Keyboard_Input(); //here we are calling the method
			if (ready)					
			{
					DrawGLScene();					
					SwapBuffers(dc);				
			}
		}
	}
	KillGLWindow();									
	return (msg.wParam);							
}





int DrawGLScene(GLvoid) 
{

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
//glLoadIdentity(); 



Sleep(20);
//drawTarget();
bf.update();


glEnd();
return TRUE; 
}


void readtexture(char *loc,GLuint TexHandle) 
{
	AUX_RGBImageRec *TextureImage; 

	if (TextureImage=LoadBMP(loc)) //const_cast<char*>( loc.c_str())
	{
		glBindTexture(GL_TEXTURE_2D, TexHandle); //
		glTexImage2D(GL_TEXTURE_2D,
				 0,
				 3,
				 TextureImage->sizeX,
				 TextureImage->sizeY,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 TextureImage->data);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	}

	//glGenTextures(2, textures);
	
	//textureAt++;

}
GLvoid resize(GLsizei width, GLsizei height)	
{
	glViewport(0,0,width,height);					
	glMatrixMode(GL_PROJECTION);					
	glLoadIdentity();								
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);			
	glLoadIdentity();					
}



const int numOfCircles=5;


//GLuint targetTX[1];
int InitGL(GLvoid)				
{	
	
	



	//glGenTextures(1,targetTX);
	glGenTextures(4,ballTX);

	readtexture("0.bmp",ballTX[0]);
	readtexture("1.bmp",ballTX[1]);
	readtexture("2.bmp",ballTX[2]);
	readtexture("3.bmp",ballTX[3]);

	TextureCollection bal(ballTX,4,4);

	//readtexture("target.bmp",targetTX[1]);
	//readtexture("1.bmp");

	glEnable(GL_TEXTURE_2D); 
	glShadeModel(GL_SMOOTH); 

	glClearColor(0.5f, 1.0f, 1.0f, 0.0f); 
	glEnable(GL_DEPTH_TEST); 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 



	Circle2D *cir;
	//cir=new Object2dTX(2,&bf,texture,0.4,0.3,-13,16) ;
	
	for(int i=0;i<numOfCircles;i++)
	{
		bal.nextTX();
		cir=new Circle2D(1.0,&bf,bal,0.4,0.3,-13+i*3.0,-16.0+i) ;//(float radius, Field* containingField,TextureCollection _TX ,int startingTX,float _dx,float _dy,float _x,float _y)
		bf.addObject(*cir);
	}
	//cir=new Object2dTX(4,&bf,ball,0.4,0.3,-13,-16) ;
	//bf.addObject(cir);

	return TRUE;
}


GLvoid KillGLWindow(GLvoid)								
{

	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(rc);
	rc=NULL;	
	ReleaseDC(wnd,dc);	
	dc=NULL;					
	DestroyWindow(wnd);
	wnd=NULL;										
	UnregisterClass("OpenGL",hInstance);
	hInstance=NULL;	
	
}

BOOL startglwindow(char* title, int width, int height, int bits)
{
	
	GLuint		PixelFormat;			
	WNDCLASS	wc;						
	DWORD		dwExStyle;				
	DWORD		dwStyle;				
	RECT		WindowRect;				
	WindowRect.left=(long)0;			
	WindowRect.right=(long)width;		
	WindowRect.top=(long)0;				
	WindowRect.bottom=(long)height;		
			

	hInstance			= GetModuleHandle(NULL);	
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wc.lpfnWndProc		= (WNDPROC) WndProc;					
	wc.cbClsExtra		= 0;									
	wc.cbWndExtra		= 0;									
	wc.hInstance		= hInstance;							
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			
	wc.hbrBackground	= NULL;									
	wc.lpszMenuName		= NULL;									
	wc.lpszClassName	= "OpenGL";								

	RegisterClass(&wc);
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	
	dwStyle=WS_OVERLAPPEDWINDOW;					
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	

	wnd=CreateWindowEx(	dwExStyle,"OpenGL",title,dwStyle |	WS_CLIPSIBLINGS |WS_CLIPCHILDREN,0, 0,WindowRect.right-WindowRect.left,
								WindowRect.bottom-WindowRect.top,	
								NULL,								
								NULL,								
								hInstance,							
								NULL);							

	static PIXELFORMATDESCRIPTOR pfd= 
	{
		sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
		1, // Version Number
		PFD_DRAW_TO_WINDOW | // Format Must Support Window
		PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
		PFD_DOUBLEBUFFER, // Must Support Double Buffering
		PFD_TYPE_RGBA, // Request An RGBA Format
		bits, // Select Our Color Depth
		0, 0, 0, 0, 0, 0, // Color Bits Ignored
		0, // No Alpha Buffer
		0, // Shift Bit Ignored
		0, // No Accumulation Buffer
		0, 0, 0, 0, // Accumulation Bits Ignored
		16, // 16Bit Z-Buffer (Depth Buffer) 
		0, // No Stencil Buffer
		0, // No Auxiliary Buffer
		PFD_MAIN_PLANE, // Main Drawing Layer
		0, // Reserved
		0, 0, 0 // Layer Masks Ignored
	};
	
	dc=GetDC(wnd);							
	PixelFormat=ChoosePixelFormat(dc,&pfd);
	SetPixelFormat(dc,PixelFormat,&pfd);	
    rc=wglCreateContext(dc);		
	wglMakeCurrent(dc,rc);					
	ShowWindow(wnd,SW_SHOW);						
	SetForegroundWindow(wnd);						
	SetFocus(wnd);									
	resize(width, height);					
    InitGL();
	return TRUE;									
}





LRESULT CALLBACK WndProc(HWND wnd,UINT	uMsg,WPARAM	wParam,LPARAM lParam)			
{
	switch (uMsg)									
	{
		
		case WM_ACTIVATE:							
		{
			if (!HIWORD(wParam))					
			{
				ready=TRUE;						
			}
			else
			{
				ready=FALSE;						
			}

			return 0;								
		}

		case WM_SYSCOMMAND:							
		{
			switch (wParam)							
			{
				case SC_SCREENSAVE:					
				case SC_MONITORPOWER:				
				return 0;							
			}
			break;									
		}

		case WM_CLOSE:								
		{
			PostQuitMessage(0);						
			return 0;								
		}
		case WM_SIZE:								
		{
			resize(LOWORD(lParam),HIWORD(lParam)); 
			return 0;								
		}
	}

	
	return DefWindowProc(wnd,uMsg,wParam,lParam);
}






void Keyboard_Input()
{
	if((GetKeyState(VK_LEFT) & 0x80))
	{
//		bf.objects[0]->dir.dx-=0.1;
	}

	if((GetKeyState(VK_RIGHT) & 0x80))
	{
//		bf.objects[0]->dir.dx+=0.1;
	}
	if((GetKeyState(VK_UP) & 0x80))
	{
//		bf.objects[0]->dir.dy+=0.1;
	}

	if((GetKeyState(VK_DOWN) & 0x80))
	{
//		bf.objects[0]->dir.dy-=0.1;
	}

//	bf.objects[0]->dir.normalize();

	if(GetKeyState('A')& 0x80)
	{
		if(start)start=false;
		else start=true;
	}
}


AUX_RGBImageRec *LoadBMP(char *Filename) 
{
		FILE *File=NULL;

		if (!Filename) 
		{
		return NULL; 
		}

		File=fopen(Filename,"r"); 

		if (File) 
		{
		fclose(File); 
		return auxDIBImageLoad(Filename); 
		}

return NULL; 
}