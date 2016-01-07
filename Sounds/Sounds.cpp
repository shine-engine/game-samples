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

#include "Sounds.h"

#define SFX_COUNT 6

static const CShIdentifier musicIdentifier("shoot_battle_2");

static const CShIdentifier aResourceIdentifiers [SFX_COUNT] =
{
	CShIdentifier("player_warrior_attack_01"),
	CShIdentifier("player_warrior_attack_02"),
	CShIdentifier("player_warrior_attack_03"),
	CShIdentifier("player_warrior_attack_04"),
	CShIdentifier("player_warrior_attack_05"),
	CShIdentifier("player_warrior_attack_06"),
};

static const ShInput::EInputDeviceControl aKeys [SFX_COUNT] = 
{
	ShInput::e_input_device_control_pc_key_a,
	ShInput::e_input_device_control_pc_key_z,
	ShInput::e_input_device_control_pc_key_e,
	ShInput::e_input_device_control_pc_key_r,
	ShInput::e_input_device_control_pc_key_t,
	ShInput::e_input_device_control_pc_key_y
};

int soundResourceHandleMusic = -1;
int soundResourceHandleSfx [SFX_COUNT];

ShInput * aInputs [SFX_COUNT];

ShSoundInstance::Handle streamHandle;

ShEntity2 * pPlayButton = shNULL;
ShEntity2 * pStopButton = shNULL;

static bool bTouchInside = false;

/**
 * Called when a mouse button is clicked or when a finger touch the screen
 */
static void OnTouchDown(int iTouch, float positionX, float positionY)
{
	if (0 == iTouch)
	{
		float distance = shDistance(CShVector3(ShEntity2::GetPosition2(pPlayButton), 0.0f), CShVector3(positionX, positionY, 0.0f));
		float halfWidth = ShEntity2::GetWidth(pPlayButton)*0.5f;

		if (distance < halfWidth)
		{
			bTouchInside = true;
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
		if (bTouchInside)
		{
			float distance = shDistance(CShVector3(ShEntity2::GetPosition2(pPlayButton), 0.0f), CShVector3(positionX, positionY, 0.0f));
			float halfWidth = ShEntity2::GetWidth(pPlayButton)*0.5f;

			if (distance < halfWidth)
			{
				if (ShSoundInstance::IsPlaying(streamHandle))
				{
					ShSoundInstance::Stop(streamHandle);
					ShEntity2::SetShow(pStopButton, false);
					ShEntity2::SetShow(pPlayButton, true);
				}
				else
				{
					ShSoundInstance::PlayMusic(soundResourceHandleMusic, streamHandle);
					ShEntity2::SetShow(pStopButton, true);
					ShEntity2::SetShow(pPlayButton, false);
				}
			}
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
	ShInput::AddOnTouchDown(OnTouchDown);
	ShInput::AddOnTouchUp(OnTouchUp);
	ShInput::AddOnTouchMove(OnTouchMove);
}

/**
 * Called after engine initialization
 */
void OnPostInitialize(void)
{
	CShIdentifier levelIdentifier("sounds"); // this is the level name

	//
	// Load the level
	bool loaded = ShLevel::Load(levelIdentifier);
	SH_ASSERT(loaded);

	//
	// Get Sprites
	pPlayButton = ShEntity2::Find(levelIdentifier, CShIdentifier("sprite_sounds_button_green_play"));
	SH_ASSERT(shNULL != pPlayButton);

	pStopButton = ShEntity2::Find(levelIdentifier, CShIdentifier("sprite_sounds_button_green_stop"));
	SH_ASSERT(shNULL != pStopButton);
	ShEntity2::SetShow(pStopButton, false);

	//
	// Sound Resources
	soundResourceHandleMusic = ShSoundResource::Find(musicIdentifier);
	SH_ASSERT(-1 != soundResourceHandleMusic);

	for (int i = 0; i < SFX_COUNT; ++i)
	{
		soundResourceHandleSfx[i] = ShSoundResource::Find(aResourceIdentifiers[i]);
		SH_ASSERT(-1 != soundResourceHandleSfx[i]);
	}

	//
	// Inputs
	for (int i = 0; i < SFX_COUNT; ++i)
	{
		aInputs[i] = ShInput::CreateInputJustPressed(ShInput::e_input_device_keyboard, aKeys[i], 0.5f);
		SH_ASSERT(shNULL != aInputs[i]);
	}
}

/**
 * Called on each frame, before the engine update
 */
void OnPreUpdate(float deltaTimeInMs)
{
	for (int i = 0; i < SFX_COUNT; ++i)
	{
		if (ShInput::IsTrue(aInputs[i]))
		{
			ShSoundInstance::Handle h;
			ShSoundInstance::PlaySFX(soundResourceHandleSfx[i], h, false);
		}
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
