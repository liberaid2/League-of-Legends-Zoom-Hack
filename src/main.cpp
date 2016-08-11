#include <iostream>
#include "MemoryControlLib.h"

int main(){
	MemoryControlLib *mcLib = new MemoryControlLib("League of Legends.exe");
	mcLib->Open();
	
	MODULEENTRY32 game = {0};
	mcLib->GetModuleInfo("", &game, true);
	
	DWORD sign = mcLib->FindSignature((DWORD)game.modBaseAddr, game.modBaseSize, (PBYTE)"\x0F\x2F\xC8\x8B\x86\x00\x00\x00\x00\xF3\x0F\x10\x50\x24\x77\x0D\x0F\x2F\xD1\x76\x05\x0F\x28\xC2\xEB\x03\x0F\x28\xC1", "xxxxx????xxxxxxxxxxxxxxxxxxxx");
	
	printf("Found signature: 0x%X\n", sign);
	
	if(sign)
	{
		byte nopBuffer[] = {0x90, 0x90, 0x90};
		mcLib->Write(nopBuffer, sign, 3);
	} else {
		printf("League of Legends game not found!\n");
	}
	
	mcLib->Close();
	system("PAUSE >nul");
	return 0;
}