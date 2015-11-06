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