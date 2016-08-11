#include <iostream>
#include "MemoryControlLib.h"

int main()
{
	/* Setup memory control lib. Get game's PID */
	MemoryControlLib *mcLib = new MemoryControlLib("League of Legends.exe");
	
	/* If we can't get PID then there is no League of Legends.exe */
	if(!mcLib->getPID())
	{
		prtinf("League of Legends game not found\n");
		system("PAUSE >nul");
		return 0;
	}
	
	/* Get LoL Handle */
	mcLib->Open();
	
	/* Get league of legends.exe memory address and size */
	MODULEENTRY32 game = {0};
	mcLib->GetModuleInfo("", &game, true);
	
	/* Find some asm code */
	DWORD sign = mcLib->FindSignature((DWORD)game.modBaseAddr, game.modBaseSize, (PBYTE)"\x0F\x2F\xC8\x8B\x86\x00\x00\x00\x00\xF3\x0F\x10\x50\x24\x77\x0D\x0F\x2F\xD1\x76\x05\x0F\x28\xC2\xEB\x03\x0F\x28\xC1", "xxxxx????xxxxxxxxxxxxxxxxxxxx");
	
	printf("Found signature: 0x%X\n", sign);
	
	/* If we find this then replace zoom comparer with nops */
	if(sign)
	{
		byte nopBuffer[] = {0x90, 0x90, 0x90};
		mcLib->Write(nopBuffer, sign, 3);
	} else 
	{
		printf("Zoom hack cannot be activated\n");
	}
	
	/* Close Handle */
	mcLib->Close();
	system("PAUSE >nul");
	return 0;
}