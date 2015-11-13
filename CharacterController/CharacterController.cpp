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

#include "CharacterController.h"

// A camera
ShCamera *					g_pCamera				= shNULL;

// The character entity
ShEntity3 *					g_pCharacter			= shNULL;

// Two animation : idle and run
ShAnimation *				pAnimationWarriorStop	= shNULL;
ShAnimation *				pAnimationWarriorRun	= shNULL;

// The three inputs to move, turn left and turn right
ShInput *					g_pInputUp				= shNULL;
ShInput *					g_pInputLeft			= shNULL;
ShInput *					g_pInputRight			= shNULL;

// The character controller
ShCharacterController *		g_pCharacterController	= shNULL;

// The speed and direction
float						g_speed					= 0.0f;
CShVector2					g_direction				= CShVector2(0.0f, -10.0f);

/**
 * Called before engine initialization
 */
void OnPreInitialize(void)
{
	// nothing here
}

/**
 * Called after engine initialization
 */
void OnPostInitialize(void)
{
	CShIdentifier levelIdentifier("character_controller"); // this is the level name

	//
	// Load level
	bool loaded = ShLevel::Load(levelIdentifier);
	SH_ASSERT(loaded);

	//
	// Create camera
	g_pCamera = ShCamera::Create(GID(global), GID(camera_free), false);
	SH_ASSERT(NULL != g_pCamera);
	ShCamera::SetPosition(g_pCamera, CShVector3(-300.0f,-1500.0f, 1000.0f));
	ShCamera::SetTarget(g_pCamera, CShVector3(0.0f, 0.0f, 100.0f));
	ShCamera::SetFarPlaneDistance(g_pCamera, 3000.0f);

	ShCamera::SetCurrent2D(g_pCamera);
	ShCamera::SetCurrent3D(g_pCamera);

	//
	// Find the character entity
	g_pCharacter = ShEntity3::Find(levelIdentifier, CShIdentifier("entitypc_warrior"));
	SH_ASSERT(shNULL != g_pCharacter);

	//
	// Initialize the character controller with the level, the identifier, the position, the radius, the direction, the speed.
	g_pCharacterController = ShCharacterController::Create(levelIdentifier, CShIdentifier("character_controller_character_001"), ShObject::GetPosition2(g_pCharacter), 50.0, g_direction, g_speed);
	SH_ASSERT(shNULL != g_pCharacterController);

	//
	// Create the moving input (arrow up).
	// Using JustPressed function in order to change each time the button is pressed and not continuously.
	g_pInputUp = ShInput::CreateInputPressed(ShInput::e_input_device_keyboard, ShInput::e_input_device_control_pc_key_up, 0.1f);
	SH_ASSERT(shNULL != g_pInputUp);

	//
	// Create the rotation inputs (right and left).
	// Using InputPressed function in order to change until the button is released.
	g_pInputLeft = ShInput::CreateInputPressed(ShInput::e_input_device_keyboard, ShInput::e_input_device_control_pc_key_left, 0.1f);
	SH_ASSERT(NULL != g_pInputLeft);
	g_pInputRight = ShInput::CreateInputPressed(ShInput::e_input_device_keyboard, ShInput::e_input_device_control_pc_key_right, 0.1f);
	SH_ASSERT(NULL != g_pInputRight);

	// Find tyhe two animations : warrior idle and warrior run.
	pAnimationWarriorStop = ShAnimation::Find(CShIdentifier("pc_warrior.pc_warrior.idle.01"));
	SH_ASSERT(NULL != pAnimationWarriorStop);

	pAnimationWarriorRun = ShAnimation::Find(CShIdentifier("pc_warrior.pc_warrior.run.01"));
	SH_ASSERT(NULL != pAnimationWarriorRun);

	// By default, we play the idle animation, allowing it to loop.
	ShEntity3::AnimationPlay(g_pCharacter, pAnimationWarriorStop, true);
}

/**
 * Called on each frame, before the engine update
 */
void OnPreUpdate(float deltaTimeInMs)
{
	// nothing here
}

/**
 * Called on each frame, after the engine update
 */
void OnPostUpdate(float deltaTimeInMs)
{
	//
	// Change the walk speed/direction
	if (ShInput::GetValue(g_pInputUp) > 0.2f)
	{
		ShEntity3::AnimationPlay(g_pCharacter, pAnimationWarriorRun, true);
		g_speed = 30.0f;
	}
	else
	{
		ShEntity3::AnimationPlay(g_pCharacter, pAnimationWarriorStop, true);
		g_speed = 0.0f;
	}
	
	if (ShInput::GetValue(g_pInputLeft) > 0.2f)
	{
		g_direction.Rotate(0.05f);
		ShEntity3::Rotate(g_pCharacter, CShEulerAngles(0, 0, 0.05f));
	}
	
	if (ShInput::GetValue(g_pInputRight) > 0.2f)
	{
		g_direction.Rotate(-0.05f);
		ShEntity3::Rotate(g_pCharacter, CShEulerAngles(0, 0, -0.05f));
	}
	
	ShCharacterController::SetWalkSpeed(g_pCharacterController, g_speed);
	ShCharacterController::SetWalkDirection(g_pCharacterController, g_direction);

	//
	// Update the character controller to change the position
	ShCharacterController::Update(g_pCharacterController);

	//
	// Change the position of the character according to the position of the character controller
	CShVector2 character_position = ShCharacterController::GetPosition(g_pCharacterController);
	ShEntity3::SetPositionX(g_pCharacter, character_position.m_x);
	ShEntity3::SetPositionY(g_pCharacter, character_position.m_y);
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

