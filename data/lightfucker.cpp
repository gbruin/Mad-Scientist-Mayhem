#include <iostream>
#include <Windows.h>
#include "_DasHard.h"

using namespace std;


/////////////////////////////////////////////////////////////////////////
/////////////////////// DASHARD.DLL   USB ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
#include "_DasHard.h"
/**/
HINSTANCE g_dasusbdll = NULL;
typedef int (*DASHARDCOMMAND)(int, int, unsigned char*);
DASHARDCOMMAND  DasUsbCommand = NULL;
int ref_open = 0;
/**/
int HardDllOpen()
{
	g_dasusbdll = LoadLibrary("DasHard2006.dll");
	if (g_dasusbdll)
		DasUsbCommand  = (DASHARDCOMMAND)::GetProcAddress((HMODULE)g_dasusbdll, "DasUsbCommand");
	if (DasUsbCommand)
		return 1;
	return 0;
}

int HardDllClose()
{
	if (g_dasusbdll!=NULL)
		return	FreeLibrary(g_dasusbdll);
	return 0;
}

int HardDllCommand(int command, int param, unsigned char *bloc)
{
	if (DasUsbCommand)
		return (*DasUsbCommand)(command, param, bloc);
	return 0;
}

/*
int main()
{
	HardDllOpen();
	if(HardDllCommand(DHC_INIT, NULL, NULL) < 0)
	{
		cout << "Fuck.  Cannot init.";
		return -1;
	}
	if(HardDllCommand(DHC_OPEN, NULL, NULL) != DHE_OK)
	{
		cout << "Fuck.  Cannot open.";
		return -1;
	}
	unsigned char dmx[513];
	dmx[512] = 0;
	//POST-INITIALIZATION *
	for(int i = 0; i < 512; i++)
	{
		dmx[i] = 250; 
	}
	if(HardDllCommand(DHC_DMXOUT, 512, dmx) != DHE_OK)
	{
		cout << "FUCK SEND FAILED";
	}
	//TEAR-DOWN *
	if(HardDllCommand(DHC_CLOSE, NULL, NULL) != DHE_OK)
	{
		cout << "Fuck, cannot close.";
	}
	HardDllCommand(DHC_EXIT, NULL, NULL);
	HardDllClose();
}
*/
