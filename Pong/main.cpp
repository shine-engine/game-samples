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

#if SH_PC
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
#else
int main(int argc, char ** argv)
#endif
{
	//
	// Set the callbacks
	ShApplication::SetOnPreInitialize(OnPreInitialize);
	ShApplication::SetOnPostInitialize(OnPostInitialize);
	ShApplication::SetOnPreUpdate(OnPreUpdate);
	ShApplication::SetOnPostUpdate(OnPostUpdate);
	ShApplication::SetOnPreRelease(OnPreRelease);
	ShApplication::SetOnPostRelease(OnPostRelease);

	//
	// Shine Engine display properties
	ShDisplayProperties displayProperties;
	
	displayProperties.m_bLandscape		= true;
	displayProperties.m_width           = 640;
	displayProperties.m_height          = 360;

	displayProperties.m_bEnable3d       = false;
	displayProperties.m_bEnableZ        = false;
	displayProperties.m_bUseSpecular    = false;
	displayProperties.m_bUsePointLights = false;
	displayProperties.m_bUseShadows		= false;

#if SH_PC
	return(ShMain((void*)&hInstance, displayProperties));
#else
	return(ShMain(argc, argv, displayProperties));
#endif
}