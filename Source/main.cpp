
#include <stdio.h>
#include <windows.h>		
#include "Object3D.h"
#include "Camera.h"
#include <math.h>
#include <string>
#include <time.h>
#include <fstream>
using namespace::std;

HGLRC		rc=NULL;	
HDC		dc=NULL;		
HWND		wnd=NULL;		
HINSTANCE	hInstance;		
bool	ready=TRUE;		

GLuint ballTX[4];

	
bool opaque=true;
int numOfSpheres=500;
int divisions=1;




GLuint	texture[3];
vector<int> stats;
TopContainer *bf;
TextureCollection bal;
GLUquadricObj *quadratic;	// Storage For Our Quadratic Objects ( NEW )

GLfloat LightAmbient[]=		{ 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]=	{20,20,20,opaque};

GLuint	filter;
Camera camera=Camera(Vector3D(0,0,0),Vector3D(0,0,-10),Vector3D(0,1,0),0.05);

int InitGL(GLvoid);
int DrawGLScene(GLvoid);
void Keyboard_Input();
void readtexture(char *loc,GLuint TexHandle);
GLvoid resize(GLsizei width, GLsizei height);
BOOL startglwindow(char* title, int width, int height, int bits);
GLvoid KillGLWindow(GLvoid);
AUX_RGBImageRec *LoadBMP(char *Filename) ;
int WINAPI WinMain(	HINSTANCE	hInstance,HINSTANCE	hPrevInstance,LPSTR	lpCmdLine,int nCmdShow);
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int InitGL(GLvoid)				
{	
	//read params
	ifstream input;
	input.open("params.txt",ios::in);
	input>>numOfSpheres>>divisions>>opaque;
	input.close();
	LightPosition[3]=opaque;
	bf=new TopContainer(divisions,40,40,40,!opaque);

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);

	

	if(opaque)
		glEnable (GL_LIGHTING );
	


	quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords ( NEW )

	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //transperancy


	glGenTextures(1,ballTX);
	readtexture("3dtx.bmp",ballTX[0]);
	bal=TextureCollection(ballTX,1,4);
	
	srand(time(NULL));

	

	return TRUE;
}

int DrawGLScene(GLvoid) 
{
	if(numOfSpheres>0){
		bf->addObject(new Sphere3D(0.5,bf,bal,quadratic,rand()%5-2,rand()%5-2,rand()%5-2,rand()%30-15,rand()%30-15,rand()%30-15,rand()%20,0.2));//(float radius, Field* containingField,TextureCollection _TX ,int startingTX,float _dx,float _dy,float _x,float _y));
		//bf->addObject(new Object3D(rand()%30-15,rand()%30-15,rand()%30-15,2,2,2));
		numOfSpheres--;
	}
	//bf->addObject(new Object3D(0,0,0,5,5,5));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	camera.refresh();

	//int before=time(NULL); //statistics

	bf->update();
	bf->draw();
	//Sleep(20);
	//int after=time(NULL);
	//stats.push_back(after-before);
	
	return TRUE; 
}
int keypressed=false;
void Keyboard_Input()
{
	if((GetKeyState(VK_UP) & 0x80))
	{	
		camera.go(1);
	}

	if((GetKeyState(VK_DOWN) & 0x80))
	{
		camera.go(-1);
	}

	if((GetKeyState(VK_LEFT) & 0x80))
	{	
		camera.rotate(-0.1);
	}

	if((GetKeyState(VK_RIGHT) & 0x80))
	{
		camera.rotate(0.1);
	}

	if((GetKeyState(VK_SPACE) & 0x80))
	{
		Vector3D ud=(camera.dir-camera.pos).normalize();
		bf->addObject(new Sphere3D(0.5,bf,TextureCollection(ballTX,1,4),quadratic,ud.dx,ud.dy,ud.dz,camera.pos.dx+ud.dx,camera.pos.dy+ud.dy,camera.pos.dz+ud.dz,3,0.3));
	}
	if((GetKeyState(VK_CONTROL) & 0x80))
	{
		Vector3D ud=(camera.dir-camera.pos).normalize();
		bf->addObject(new Object3D(camera.pos.dx+ud.dx,camera.pos.dy+ud.dy,camera.pos.dz+ud.dz,2,2,2));
	}

}

 
void readtexture(char *loc,GLuint TexHandle) 								// Load Bitmaps And Convert To Textures
{

	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP(loc))
	{

		glGenTextures(3, &texture[0]);					// Create Three Textures

		// Create Nearest Filtered Texture
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		// Create Linear Filtered Texture
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		// Create MipMapped Texture
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}

	if (TextureImage[0])								// If Texture Exists
	{
		if (TextureImage[0]->data)						// If Texture Image Exists
		{
			free(TextureImage[0]->data);				// Free The Texture Image Memory
		}

		free(TextureImage[0]);							// Free The Image Structure
	}

									
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
	gluDeleteQuadric(quadratic);
	
	//stats
	//double sum=0;	
	//for(vector<int>::iterator it=stats.begin();it!=stats.end();++it)
	//	sum+=*it;
	//sum/=stats.size();
	ofstream myfile;
	myfile.open ("count.txt");
	int sum=0;
	for(int i=0;i<bf->sectors.size();i++)
		sum+=bf->sectors[i].objects.size();
	myfile << sum;
	myfile.close();
	delete bf;

	
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

int WINAPI WinMain(	HINSTANCE	hInstance,HINSTANCE	hPrevInstance,LPSTR	lpCmdLine,int nCmdShow)			
{
	MSG		msg;									
	BOOL	done=FALSE;								
    startglwindow("3D Space Matrix",800,600,32);



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
				Keyboard_Input(); 
				camera.goToMouse(800,600);
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