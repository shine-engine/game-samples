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

#include "Pong.h"

ShEntity2 * ball = shNULL;

ShEntity2 * r1 = shNULL;
ShEntity2 * r2 = shNULL;

CShVector3 velocity(-2.0f, 2.0f, 0.0f);

ShInput * up = shNULL;
ShInput * down = shNULL;

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
	CShIdentifier table("table"); // this is the level name

	//
	// Load the level
	bool loaded = ShLevel::Load(table);
	SH_ASSERT(loaded);

	//
	// Get the entities
	ball	= ShEntity2::Find(table, CShIdentifier("balle"));
	r1		= ShEntity2::Find(table, CShIdentifier("r1"));
	r2		= ShEntity2::Find(table, CShIdentifier("r2"));

	//
	// Create inputs
	up		= ShInput::CreateInputPressed(ShInput::e_input_device_keyboard, ShInput::e_input_device_control_pc_key_up, 0.5f);
	down	= ShInput::CreateInputPressed(ShInput::e_input_device_keyboard, ShInput::e_input_device_control_pc_key_down, 0.5f);
}

/**
 * Called on each frame, before the engine update
 */
void OnPreUpdate (float dt)
{
	bool bDown	= ShInput::IsTrue(down);
	bool bUp	= ShInput::IsTrue(up);

	//
	// Update r1 position according to user inputs
	if (bDown)
	{
		ShEntity2::Translate(r1, CShVector3(0.0f, -10.0f, 0.0f));
	}

	if (bUp)
	{
		ShEntity2::Translate(r1, CShVector3(0.0f, 10.0f, 0.0f));
	}

	//
	// Update ball position
	ShEntity2::Translate(ball, velocity);
}

/**
 * Called on each frame, after the engine update
 */
void OnPostUpdate(float dt)
{
	//
	// Get the position of every object
	const CShVector2 & position = ShEntity2::GetPosition2(ball);
	const CShVector2 & r1_position = ShEntity2::GetPosition2(r1);
	const CShVector2 & r2_position = ShEntity2::GetPosition2(r2);
	
	//
	// Get the scale of every object
	const CShVector3 & scale = ShEntity2::GetScale(ball);
	const CShVector3 & r1_scale = ShEntity2::GetScale(r1);
	const CShVector3 & r2_scale = ShEntity2::GetScale(r2);

	//
	// Compute half height & half width
	float r1_half_height = ShEntity2::GetHeight(r1) * 0.5f * r1_scale.m_y;
	float r2_half_height = ShEntity2::GetHeight(r2) * 0.5f * r2_scale.m_y;

	float r1_half_width = ShEntity2::GetWidth(r1) * 0.5f * r1_scale.m_x;
	float r2_half_width = ShEntity2::GetWidth(r2) * 0.5f * r2_scale.m_x;

	float radius = ShEntity2::GetWidth(ball) * 0.5f * scale.m_x;

	//
	// First handle collisions with the top wall
	if ((position.m_y + radius) > 180.0f)
	{
		velocity.m_y = - velocity.m_y;
	}

	//
	// Then handle collisions with the bottom wall
	if ((position.m_y - radius) < -180.0f)
	{
		velocity.m_y = - velocity.m_y;
	}

	//
	// Check collision with r1
	if ((position.m_x - radius) < (r1_position.m_x + r1_half_width) && (position.m_x - radius) > (r1_position.m_x - r1_half_width))
	{
		if (position.m_y < (r1_position.m_y + r1_half_height) && position.m_y > (r1_position.m_y - r1_half_height))
		{
			velocity.m_x = - velocity.m_x;
		}
	}

	//
	// Check collision with r1
	if ((position.m_x + radius) > (r2_position.m_x - r2_half_width) && (position.m_x + radius) < (r2_position.m_x + r2_half_width))
	{
		if (position.m_y < (r2_position.m_y + r2_half_height) && position.m_y > (r2_position.m_y - r2_half_height))
		{
			velocity.m_x = - velocity.m_x;
		}
	}
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
