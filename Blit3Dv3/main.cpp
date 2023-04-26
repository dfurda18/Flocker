/**
	Flocking main file
*/
//memory leak detection
#define CRTDBG_MAP_ALLOC
#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>

#include "Blit3D.h"
#include "Flock.h"
#include "Bird.h"

#define backgroundWidth 1920
#define backgroundHeight 1080
#define POWER_UP_SIZE 25
#define BIRD_NUMBER 40

//GLOBAL DATA
enum GameState { GAME = 1, PAUSE = 2 };
// External resources
Blit3D* blit3D = NULL;
// Game Objects
Flock* flock = NULL;
// Sprites
Sprite* backgroundSprite = NULL; 

// Fonts
AngelcodeFont* electroliteFont = NULL;
AngelcodeFont* syneMonoFont = NULL;

// Game states
GameState gameState = GAME;
bool gameOver = false;
bool shoot = false;
// Game variables
int level = 0;
int score = 0;
int lastPowerUp = 0;
// Game Timers
double elapsedTime = 0;
float timeSlice = 1.f / 120.f;
float levelTitleTimer = 0;

/**
* This method initialices the scene.
*/
void Init()
{
	gameState = GAME;
	//turn cursor off
	blit3D->ShowCursor(false);
	gameOver = false;
	
	//load Sprites for background, shield icon, shot icon and power up
	backgroundSprite = blit3D->MakeSprite(0, 0, backgroundWidth, backgroundHeight, "Media\\Background.jpg");
	flock = new Flock();
	// Bird sprites
	for (int i = 0; i < 6; i++)
	{
		flock->AddSprite(blit3D->MakeSprite(i * 86, 0, 86, 52, "Media\\bird.png"));
	}
	for (int birdCounter = 0; birdCounter < BIRD_NUMBER; birdCounter++)
	{
		flock->MakeBird();
	}
	
	// Font
	electroliteFont = blit3D->MakeAngelcodeFontFromBinary32("Media\\fonts\\electrolite.bin");
	syneMonoFont = blit3D->MakeAngelcodeFontFromBinary32("Media\\fonts\\SyneMono.bin");
	//set the clear colour
	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);	//clear colour: r,g,b,a 
	
}
/**
* This method resotre's the scene initialization.
*/
void DeInit(void)
{
	// Eliminate the flock and birds
	if (flock != NULL)
	{
		flock->Destroy();
		delete flock;
	}
}
/**
* This method is called every update.
*/
void Update(double seconds)
{
	switch (gameState)
	{
	case GAME:
		//only update time to a maximun amount - prevents big jumps in 
		//the simulation if the computer "hiccups"
		if (seconds < 0.15)
			elapsedTime += seconds;
		else elapsedTime += 0.15;

		//update by a full timeslice when it's time
		while (elapsedTime >= timeSlice)
		{
			elapsedTime -= timeSlice;
			levelTitleTimer += timeSlice;
			if (levelTitleTimer > 5) levelTitleTimer = 5;
			// Update ship
			flock->Update(timeSlice);
		}
		break;
	case PAUSE:
		break;
	default:
		break;
	}
	
}
/**
* This method is called to draw all elements.
*/
void Draw(void)
{
	// Variables for the texts
	std::string text;
	float textWidth;
	float textHeight;
	float vMargin;
	float hMArgin;
	switch (gameState)
	{
	case GAME:
	case PAUSE:
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw the background in the middle of the screen
		backgroundSprite->Blit(1920.f / 2, 1080.f / 2);
		
		// Draw texts
		vMargin = 20.0f;
		hMArgin = 30.0f;
		
		flock->Draw();
		break;
	default:
		break;
	}
	if (gameState == PAUSE)
	{
		// Draw the paused message
		text = "PAUSED";
		textWidth = electroliteFont->WidthText(text);
		textHeight = 76.f;
		electroliteFont->BlitText(blit3D->screenWidth / 2 - textWidth / 2, blit3D->screenHeight / 2 + textHeight, text);
	}
}
/**
* This method handles the input.
*/
void DoInput(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		blit3D->Quit(); //start the shutdown sequence
	switch (gameState)
	{
	case GAME:
		if (!gameOver) {
			// Pause action
			if (key == GLFW_KEY_P && action == GLFW_RELEASE)
			{
				gameState = PAUSE;
			}
		}
		break;
	case PAUSE:
		// Unpause action
		if (key == GLFW_KEY_P && action == GLFW_RELEASE)
		{
			gameState = GAME;
		}
		break;
	default:
		break;
	}
}
/**
* This method is called at the start.
*/
int main(int argc, char *argv[])
{
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	blit3D = new Blit3D(Blit3DWindowModel::BORDERLESSFULLSCREEN_1080P, 1920, 1080);

	//set our callback funcs
	blit3D->SetInit(Init);
	blit3D->SetDeInit(DeInit);
	blit3D->SetUpdate(Update);
	blit3D->SetDraw(Draw);
	blit3D->SetDoInput(DoInput);

	//Run() blocks until the window is closed
	blit3D->Run(Blit3DThreadModel::SINGLETHREADED);
	if (blit3D) delete blit3D;
}