/**
 * Copyright (c) 2015 Shine Research http://shine-research.com/
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <ShSDK/ShSDK.h>

#include "Memory.h"

ShEntity2 * g_pWinEntity = shNULL;

#define WIDTH 4
#define HEIGHT 4
#define PIECES (WIDTH*HEIGHT)

SH_STATIC_ASSERT(PIECES%2 == 0, "Nombre de pièces impair");

static CShIdentifier aIdentifier [] = 
{
	CShIdentifier("recto_carre"),
	CShIdentifier("recto_triangle"),
	CShIdentifier("recto_rond"),
	CShIdentifier("recto_croix"),
	CShIdentifier("recto_carre_vert"),
	CShIdentifier("recto_triangle_vert"),
	CShIdentifier("recto_rond_vert"),
	CShIdentifier("recto_croix_vert"),
};

SH_STATIC_ASSERT((sizeof(aIdentifier)/sizeof(aIdentifier[0])) == (PIECES/2), "Identifier manquant");

struct Card
{
	ShEntity2 * pEntityRecto;
	ShEntity2 * pEntityVerso;
	int type;
};

enum EState
{
	e_state_default,
	e_state_wait,
	e_state_choosen,
	e_state_win
};

Card aCards [PIECES];

ShCamera * g_pCamera = shNULL;

EState currentState = e_state_default;

int firstCard = -1;
int secondCard = -1;

float currentStateTime = 0.0f;

static void shuffle()
{
	for (int i = 0; i < PIECES; ++i)
	{
		int j = rand()%PIECES;

		CShVector3 pos1 = ShEntity2::GetPosition(aCards[i].pEntityRecto);
		CShVector3 pos2 = ShEntity2::GetPosition(aCards[j].pEntityRecto);

		ShEntity2::SetPosition(aCards[i].pEntityRecto, pos2);
		ShEntity2::SetPosition(aCards[j].pEntityRecto, pos1);

		ShEntity2::SetPosition(aCards[i].pEntityVerso, pos2);
		ShEntity2::SetPosition(aCards[j].pEntityVerso, pos1);
	}
}

/**
 * Called when a mouse button is clicked or when a finger touch the screen
 */
static void OnTouchDown(int iTouch, float positionX, float positionY)
{
	if (0 == iTouch)
	{
		switch (currentState)
		{
			case e_state_default:
			{
				for (int i = 0; i < PIECES; ++i)
				{
					const CShVector2& pos = ShEntity2::GetPosition2(aCards[i].pEntityRecto);
					float half_height = ShEntity2::GetHeight(aCards[i].pEntityRecto) * 0.5f;
					float half_width = ShEntity2::GetWidth(aCards[i].pEntityRecto) * 0.5f;

					if (positionX > pos.m_x - half_width && positionX < pos.m_x + half_width && positionY > pos.m_y - half_height && positionY < pos.m_y + half_height)
					{
						if (ShEntity2::IsShow(aCards[i].pEntityVerso))
						{
							firstCard = i;
						
							ShEntity2::SetShow(aCards[i].pEntityRecto, true);
							ShEntity2::SetShow(aCards[i].pEntityVerso, false);
						
							currentState = e_state_wait;
							currentStateTime = 0.0f;
						}

						break;
					}
				}
			}
			break;

			case e_state_wait:
			{
				for (int i = 0; i < PIECES; ++i)
				{
					const CShVector2& pos = ShEntity2::GetPosition2(aCards[i].pEntityRecto);
					float half_height = ShEntity2::GetHeight(aCards[i].pEntityRecto) * 0.5f;
					float half_width = ShEntity2::GetWidth(aCards[i].pEntityRecto) * 0.5f;

					if (positionX > pos.m_x - half_width && positionX < pos.m_x + half_width && positionY > pos.m_y - half_height && positionY < pos.m_y + half_height)
					{
						if (ShEntity2::IsShow(aCards[i].pEntityVerso))
						{
							secondCard = i;
						
							ShEntity2::SetShow(aCards[i].pEntityRecto, true);
							ShEntity2::SetShow(aCards[i].pEntityVerso, false);
						
							currentState = e_state_choosen;
							currentStateTime = 0.0f;
						}

						break;
					}
				}
			}
			break;

			case e_state_choosen:
			{

			}
			break;

			case e_state_win:
			{
				currentState = e_state_default;

				for (int i = 0; i < PIECES; ++i)
				{
					ShEntity2::SetShow(aCards[i].pEntityRecto, false);
					ShEntity2::SetShow(aCards[i].pEntityVerso, true);
				}

				ShEntity2::SetShow(g_pWinEntity, false);

				shuffle();
			}
			break;
		}
	}
}

/**
 * Called when a mouse button is released or when a finger leave the screen
 */
static void OnTouchUp(int iTouch, float positionX, float positionY)
{
	if (0 == iTouch)
	{
		switch (currentState)
		{
		}
	}
}

/**
 * Called when the mouse moves or when a finger moves on the screen
 */
static void OnTouchMove(int iTouch, float positionX, float positionY)
{
	// ...
}

/**
 * Called before engine initialization
 */
void OnPreInitialize(void)
{
	ShInput::SetOnTouchDown(OnTouchDown);
	ShInput::SetOnTouchUp(OnTouchUp);
	ShInput::SetOnTouchMove(OnTouchMove);

	srand(time(NULL));
}

/**
 * Called after engine initialization
 */
void OnPostInitialize(void)
{
	CShIdentifier levelIdentifier("memory");

	bool loaded = ShLevel::Load(levelIdentifier);
	SH_ASSERT(loaded);

	//
	// Create Camera
	g_pCamera = ShCamera::Create(GID(global), GID(camera_free), false);
	SH_ASSERT(NULL != g_pCamera);
	ShCamera::SetPosition(g_pCamera, CShVector3(0, 0.0f, 1000.0f));
	ShCamera::SetTarget(g_pCamera, CShVector3(0.0f, 0.0f, 0.0f));
	ShCamera::SetFarPlaneDistance(g_pCamera, 3000.0f);
	ShCamera::SetViewport(g_pCamera, 256*WIDTH, 256*HEIGHT);
	ShCamera::SetProjectionOrtho(g_pCamera);

	ShCamera::SetCurrent2D(g_pCamera);

	g_pWinEntity = ShEntity2::Find(levelIdentifier, CShIdentifier("sprite_memory_win_001"));
	SH_ASSERT(shNULL != g_pWinEntity);
	ShEntity2::SetShow(g_pWinEntity, false);

	//
	// Create all sprites
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			CShVector3 pos;
			pos.m_x = (256.0f * j) - (128.0f + (((WIDTH/2)-1) * 256.0f));
			pos.m_y = (256.0f * i) - (128.0f + (((HEIGHT/2)-1) * 256.0f));

			int c = i*WIDTH+j;

			aCards[c].pEntityRecto = ShEntity2::Create(levelIdentifier, CShIdentifier(), GID(layer_default), CShIdentifier("memory"), aIdentifier[c/2], pos, CShEulerAngles(0.0f, 0.0f, 0.0f), CShVector3(1.0f, 1.0f, 1.0f));
			SH_ASSERT(shNULL != aCards[c].pEntityRecto);

			aCards[c].pEntityVerso = ShEntity2::Create(levelIdentifier, CShIdentifier(), GID(layer_default), CShIdentifier("memory"), CShIdentifier("verso"), pos, CShEulerAngles(0.0f, 0.0f, 0.0f), CShVector3(1.0f, 1.0f, 1.0f));
			SH_ASSERT(shNULL != aCards[c].pEntityVerso);

			aCards[c].type = c/2;
		}
	}

	for (int i = 0; i < PIECES; ++i)
	{
		ShEntity2::SetShow(aCards[i].pEntityRecto, false);
	}

	shuffle();
}

/**
 * Called on each frame, before the engine update
 */
void OnPreUpdate(float dt)
{
	currentStateTime += dt;

	switch (currentState)
	{
		case e_state_default:
		{

		}
		break;

		case e_state_wait:
		{

		}
		break;

		case e_state_choosen:
		{
			if (aCards[firstCard].type == aCards[secondCard].type)
			{
				// TODO : animation

				int count = 0;

				for (int i = 0; i < PIECES; ++i)
				{
					if (ShEntity2::IsShow(aCards[i].pEntityRecto))
					{
						++count;
					}
				}

				if (PIECES == count)
				{
					ShEntity2::SetShow(g_pWinEntity, true);
					currentState = e_state_win;
				}
				else
				{
					currentState = e_state_default;
				}

				firstCard = -1;
				secondCard = -1;
			}
			else
			{
				if (currentStateTime > 2.0f)
				{
					ShEntity2::SetShow(aCards[firstCard].pEntityRecto, false);
					ShEntity2::SetShow(aCards[secondCard].pEntityRecto, false);
					
					ShEntity2::SetShow(aCards[firstCard].pEntityVerso, true);
					ShEntity2::SetShow(aCards[secondCard].pEntityVerso, true);

					currentState = e_state_default;

					firstCard = -1;
					secondCard = -1;
				}
			}
		}
		break;

		case e_state_win:
		{
			// ...
		}
		break;
	}
}

/**
 * Called on each frame, after the engine update
 */
void OnPostUpdate(float deltaTimeInMs)
{
	// nothing here
}

/**
 * Called before the engine release
 */
void OnPreRelease(void)
{
	// nothing here
}

/**
 * Called after the engine release
 */
void OnPostRelease(void)
{
	// nothing here
}
