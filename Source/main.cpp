
#include <stdio.h>
#include <windows.h>		
//#include "Object3D.h"
#include "Camera.h"
#include <math.h>
#include <string>
#include <time.h>
#include <fstream>
#include <sstream>
#include "ModelCollection.h"
using namespace::std;

HGLRC		rc=NULL;	
HDC		dc=NULL;		
HWND		wnd=NULL;		
HINSTANCE	hInstance;		
bool	ready=TRUE;		

GLuint ballTX[4];
GLuint wallTX[7];
GLuint background[3];	
GLuint monster[2];
GLuint brick[1];

int length(64),width(64),height(50);
int npcHealth(5),playerHealth(100),projectileHealth(100);
bool transparentSectors=true;
int numOfCubes=0;
int numOfNpc=-1;
int divisions=1;
int period=60;



GLuint	texture[3];
vector<int> stats;
TopContainer *bf;
TextureCollection bal;
TextureCollection wal;
TextureCollection bak;
TextureCollection mon;
TextureCollection brik;
GLUquadricObj *quadratic;	// Storage For Our Quadratic Objects ( NEW )

GLfloat LightAmbient[]=		{ 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]=	{20,20,20,transparentSectors};

GLuint	filter;
Camera camera;

int InitGL(GLvoid);
int DrawGLScene(GLvoid);
void Keyboard_Input();
GLuint readtexture(char *loc,GLuint& TexHandle,int type);
GLvoid resize(GLsizei width, GLsizei height);
BOOL startglwindow(char* title, int width, int height, int bits);
GLvoid KillGLWindow(GLvoid);
AUX_RGBImageRec *LoadBMP(char *Filename) ;
int WINAPI WinMain(	HINSTANCE	hInstance,HINSTANCE	hPrevInstance,LPSTR	lpCmdLine,int nCmdShow);
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ModelCollection npcmodels=ModelCollection();

int InitGL(GLvoid)				
{	
	//read params
	ifstream input;
	string garbage;
	input.open("params.txt",ios::in);
	input	>>garbage>>length
			>>garbage>>width
			>>garbage>>height
			>>garbage>>divisions
			>>garbage>>numOfCubes
			>>garbage>>numOfNpc
			>>garbage>>npcHealth
			>>garbage>>playerHealth
			>>garbage>>projectileHealth
			>>garbage>>period
			>>garbage>>transparentSectors;
	input.close();
	LightPosition[3]=transparentSectors;
	
	

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

	

	if(transparentSectors)
		glEnable (GL_LIGHTING );
	


	quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords ( NEW )

	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //transperancy


	glGenTextures(6,wallTX);
	readtexture("TX/wall1.bmp",wallTX[0],2);
	wal=TextureCollection(wallTX[0]);
	readtexture("TX/wall2.bmp",wallTX[1],2);
	wal.addTX(wallTX[1]);
	readtexture("TX/wall3.bmp",wallTX[2],2);
	wal.addTX(wallTX[2]);
	readtexture("TX/wall4.bmp",wallTX[3],2);
	wal.addTX(wallTX[3]);
	readtexture("TX/wall5.bmp",wallTX[4],2);
	wal.addTX(wallTX[4]);
	readtexture("TX/wall6.bmp",wallTX[5],2);
	wal.addTX(wallTX[5]);


	glGenTextures(3,ballTX);
	readtexture("TX/ball1.bmp",ballTX[0],2);
	bal=TextureCollection(ballTX[0]);
	readtexture("TX/ball2.bmp",ballTX[1],2);
	bal.addTX(ballTX[1]);
	readtexture("TX/ball3.bmp",ballTX[2],2);
	bal.addTX(ballTX[2]);

	glGenTextures(2,background);
	readtexture("TX/stars.bmp",background[0],2);
	bak=TextureCollection(background[0]);
	readtexture("TX/ground.bmp",background[1],2);
	bak.addTX(background[1]);

	glGenTextures(1,monster);
	readtexture("TX/monster1.bmp",monster[0],2);
	mon=TextureCollection(monster[0]);
	readtexture("TX/monster2.bmp",monster[1],2);
	mon.addTX(monster[1]);

	glGenTextures(1,brick);
	readtexture("TX/brick.bmp",brick[0],2);
	brik=TextureCollection(brick[0]);

	char *a;
	for(int i=0;i<8;i++)
	{
		string s;
		stringstream out;
		out << "data/gargoyle/animation3/"<<i<<".ms3d";
		s = out.str();
		a=new char[s.size()+1];
		a[s.size()]=0;
		memcpy(a,s.c_str(),s.size());

		npcmodels.addModel(a);
		delete a;
	}

	Terrain *tr=new Terrain();
	tr->pos.dy=-height/2;
	tr->pos.dx=-length;
	tr->pos.dz=-width;
	tr->loadModel("data/terrain8080.ms3d");
	//GLuint tst=readtexture("TX/ground.bmp",background[1],2);
	//tr->TX.addTX(tst);
	

	bf=new TopContainer(divisions,length,height,width,bak,!transparentSectors);
	bf->addObject(tr);
	camera=Camera(bf->pos+Vector3D(bf->dim.dx*0.5,0.5,bf->dim.dz*0.5),Vector3D(0,0,-10),Vector3D(0,1,0),0.1,bf,playerHealth);
	srand(time(NULL));

	glBindTexture(GL_TEXTURE_2D,NULL);

	return TRUE;
}

int DrawGLScene(GLvoid) 
{
	unsigned int before=GetTickCount();

	if(numOfCubes>0){
		wal.nextTX();
		bf->addObject(new Object3D(rand()%(int)bf->dim.dx-((int)bf->dim.dx/2),rand()%(int)bf->dim.dy-((int)bf->dim.dy/2),rand()%(int)bf->dim.dz-((int)bf->dim.dz/2),2,2,2,wal));
		//bf->addObject(new Stackable(rand()%(int)bf->dim.dx-((int)bf->dim.dx/2),rand()%(int)bf->dim.dy-((int)bf->dim.dy/2),rand()%(int)bf->dim.dz-((int)bf->dim.dz/2),2,2,2,wal));
		numOfCubes--;
	}
	if(numOfNpc>0){
		mon.nextTX();
		bf->addObject(new NPC(rand()%(int)bf->dim.dx-((int)bf->dim.dx/2),rand()%(int)bf->dim.dy-((int)bf->dim.dy/2),rand()%(int)bf->dim.dz-((int)bf->dim.dz/2),2,2,2,mon,camera.body,npcHealth,0.1,&npcmodels));
		numOfNpc--;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	camera.refresh();
	bf->update();
	bf->draw();

	SYSTEMTIME  after;
	GetSystemTime(&after);
	while(GetTickCount()-before<period)
	{
		Sleep(period/7);

	}
	return TRUE; 
}
bool spacepressed=false;
bool controlpressed=false;
bool shiftpressed=false;
void Keyboard_Input()
{
	if((GetKeyState('W') & 0x80))
	{	
		camera.go(1);
	}

	if((GetKeyState('S') & 0x80))
	{
		camera.go(-1);
	}

	if((GetKeyState('A') & 0x80))
	{	
		//camera.rotate(-0.1);
		camera.strafe(1);
	}

	if((GetKeyState('D') & 0x80))
	{
		//camera.rotate(0.1);
		camera.strafe(-1);
	}

	if((GetKeyState(VK_SPACE) & 0x80) && !spacepressed)
	{
		spacepressed=true;
		Vector3D ud=(camera.arm-camera.center()).normalize();
  		bf->addObject(new Sphere3D(0.2,bf,bal,quadratic,ud.dx,ud.dy,ud.dz,camera.center().dx+ud.dx*1.5,camera.center().dy+ud.dy*1.5,camera.center().dz+ud.dz*1.5,3,0.4,projectileHealth));
		
	}
	else if(!(GetKeyState(VK_SPACE) & 0x80))
		spacepressed=false;

	if((GetKeyState(VK_CONTROL) & 0x80) && !controlpressed)
	{
		bf->addObject(new Stackable(camera.arm.dx,camera.arm.dy,camera.arm.dz,1,1,1,brik));
		//ModelFactory

		//bf->addObject();
		controlpressed=true;
	}
	else if(!(GetKeyState(VK_CONTROL) & 0x80))
		controlpressed=false;

	if((GetKeyState(VK_LSHIFT) & 0x80) && !shiftpressed)
	{
		camera.jump();
		shiftpressed=true;
	}
	else if(!(GetKeyState(VK_LSHIFT) & 0x80))
		shiftpressed=false;
	if((GetKeyState('T') & 0x80) )
		camera.drawArm();
	if((GetKeyState(VK_UP) & 0x80) )
		camera.lengthOfArm+=0.4;
	if((GetKeyState(VK_DOWN) & 0x80) )
		camera.lengthOfArm-=0.4;
}

 
GLuint readtexture(char *loc,GLuint& TexHandle,int type) 								// Load Bitmaps And Convert To Textures
{

	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP(loc))
	{

		glGenTextures(3, &texture[0]);					// Create Three Textures
		if(type==0){
			// Create Nearest Filtered Texture
			glBindTexture(GL_TEXTURE_2D, TexHandle);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		}else if (type==1){
			// Create Linear Filtered Texture
			glBindTexture(GL_TEXTURE_2D, TexHandle);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		}else if (type==2){
			// Create MipMapped Texture
			glBindTexture(GL_TEXTURE_2D, TexHandle);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		}
		//glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (TextureImage[0])								// If Texture Exists
	{
		if (TextureImage[0]->data)						// If Texture Image Exists
		{
			free(TextureImage[0]->data);				// Free The Texture Image Memory
		}

		free(TextureImage[0]);							// Free The Image Structure
	}
	return 0;//GLuint(TexHandle);
									
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
    startglwindow("AlienShooter",1355,690,32);



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
				camera.goToMouse(1355,690);
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