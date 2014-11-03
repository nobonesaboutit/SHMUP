#ifndef IMPROV_H
#define IMPROV_H
																			//SPAGHETTI BOX
#include "AIE.h"
#include "MyMaths.h"
#include "KeyStater.h"
#include <iostream>
#include "Ship.h"
#include <math.h>
#include "Enemy.h"
#include "Bullet.h"
#include <iostream>
#include <fstream>

using namespace std;

enum GAMESTATE
{
	SPLASHSCREEN,
	MAINMENU,
	LOADGAME,
	RELOADGAME,
	GAMEPLAY,
	HISCORE,
	QUIT,
	UNLOAD
};

float frameTime = 0;
float totalTime = 0;

struct GameStateHandler
{
	GAMESTATE CurrentState, LastState;

	bool hasChanged = false;

	GameStateHandler(GAMESTATE aState)
	{
		CurrentState = aState;
		LastState = CurrentState;
	}

	void ChangeState(GAMESTATE aState)
	{
		totalTime = 0;
		hasChanged = true;
		LastState = CurrentState;
		CurrentState = aState;
	}

	void Update()
	{
		hasChanged = false;
	}
};

GameStateHandler Game(SPLASHSCREEN);

bool pause = false;
bool newHi = false;
int sWidth = 800;
int sHeight = 600;
int spriteCount = 0;

const int splasmaCount = 500;
int fireTime = 0;

int s1Lives = 3;

const int alienCount = 5;
int alienMark = alienCount;

KeyStater Keys;
Ship Player1;
Bullet SPlasma[splasmaCount];
Enemy Alien[alienCount];

/*
Ship Player2;
Ship Player3;
Ship Player4;
*/

void AlienPlayerCollision()
{
	for (int i = 0; i < alienCount; i++)
	{
		if (CheckCircleCircle(Player1.x, Player1.y, Player1.r, Alien[i].x, Alien[i].y, Alien[i].r) && Alien[i].active)															//FIX THIS SHIT
		{
			for (int i = 0; i < alienCount; i++)
			{
				Alien[i].active = true;
				Alien[i].SetPos((i + 3) * 0.1f * sWidth, 0.9f * sHeight, 15);
			}
				alienMark = alienCount;
				Player1.SetPos(100, 100, 15);
				s1Lives--;
				if (s1Lives == 0)
					totalTime = 0;
		}
	}
}


bool AlienActiveCheck()
{
	int a = alienCount;
	for (int i = 0; i < alienCount; i++)
	{
		if (!Alien[i].active)
			a--;
	}
	if (a == 0)
		return false;
	alienMark = a;
	return true;
}

void BREAK()
{
	sWidth = sWidth;
}

void SplashScreen(), MainMenu(), LoadGame(), ReloadGame(), GamePlay(), GameDraw(), HiScore(), Quit(), Unload();

int MakeSprite(char *atext, int awidth, int aheight, int &asprites)
{
	asprites++;
	return CreateSprite(atext, awidth, aheight, true, SColour(255, 255, 255, 255));
}

int GetNextBullet()
{
	for (int i = 0; i < splasmaCount; i++)
	{
		if (!SPlasma[i].active)
			return i;
	}
	return -1;
}

bool Shoot()
{
	if (fireTime > 0)
		fireTime -= frameTime;
	if ((Keys.IsPressed(VK_SPACE) || Keys.IsDown(VK_SPACE)) && fireTime == 0)
	{
		fireTime = 1.5; 
		if (GetNextBullet() == -1)
			return false;
		SPlasma[GetNextBullet()].x = Player1.x;
		SPlasma[GetNextBullet()].y = Player1.y;
		SPlasma[GetNextBullet()].active = true;
	}
	return true;
}

float ReadF();
void Write(float);

void AlienSPlasmaCollision()
{
	for (int i = 0; i < splasmaCount; i++)
	{
		if (SPlasma[i].active)
		{
			for (int ii = 0; ii < alienCount; ii++)
			{
				if (Alien[ii].active)
				{
					if (CheckCircleCircle(Alien[ii].x, Alien[ii].y, Alien[ii].r, SPlasma[i].x, SPlasma[i].y, SPlasma[i].r))
					{
						alienMark--;
						if (alienMark == 0)
						{
							if (totalTime < ReadF())
							{
								newHi = true;
								Write(totalTime);
							}
							totalTime = 0;
						}
						Alien[ii].active = false;
					}
				}
			}
		}
	}
}

void DrawUI()
{
	char a[10], b[10], c[30];
	_itoa(alienMark, a, 10);
	_itoa(alienCount, b, 10);
	strcat(a, "/");
	strcat(a, b);
	DrawString(a, 0.1f * sWidth, 0.9f * sHeight, SColour(255, 255, 255, 255));
	_itoa(s1Lives, a, 10);
	_itoa(totalTime, c, 10);
	//DrawString(c, 0.8f * sWidth, 0.9f * sHeight, SColour(255, 255, 255, 255));

}

void DrawNum(float a, float x, float y)
{
	char b[10];
	_itoa(a, b, 10);
	DrawString(b, x * sWidth, y * sHeight, SColour(255, 255, 255, 255));
}

void DrawChar(char *a, float x, float y)
{
	DrawString(a, x * sWidth, y * sHeight, SColour(255, 255, 255, 255));
}

//FILE I/O
void Write(char writeme)//WRITING
{
	fstream fscores;
	fscores.open("fscores.txt", ios_base::out);
	fscores << writeme << " " << endl;
	fscores.close();
}

void Write(char *writeme)//WRITING ARRAY OVERLOAD
{
	fstream fscores;
	fscores.open("fscores.txt", ios_base::out);
	fscores << writeme << " " << endl;
	fscores.close();
}

void Write(float writeme)//WRITING FLOAT OVERLOAD
{
	fstream fscores;
	fscores.open("fscores.txt", ios_base::out);
	fscores << writeme << " " << endl;
	fscores.close();
}

void DrawScore(float x, float y)//READING AS CHAR
{
	char ca[6];
	fstream fscores;
	fscores.open("fscores.txt", ios_base::in);
	fscores.getline(ca, 6);
	fscores.close();
	DrawString(ca, x * sWidth, y * sHeight, SColour(255, 255, 255, 255));
}

float ReadF()//READING AS FLOAT
{
	char ca[6];
	float fa;
	fstream fscores;
	fscores.open("fscores.txt", ios_base::in);
	fscores.getline(ca, 6);
	fscores.close();
	fa = atoi(ca);
	return fa;
}
#endif