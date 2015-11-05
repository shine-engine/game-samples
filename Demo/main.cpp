#include <ShSDK/ShSDK.h>

#include "Demo.h"

#if SH_PC
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
#else
int main(int argc, char ** argv)
#endif
{
	ShDisplayProperties displayProperties;
	
	displayProperties.m_bLandscape		= true;
	displayProperties.m_width           = 1280;
	displayProperties.m_height          = 720;

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