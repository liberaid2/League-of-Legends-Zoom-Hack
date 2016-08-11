#pragma once
#ifndef MEMORYCONTROLLIB_H
#define MEMORYCONTROLLIB_H

#include <Windows.h>
#include <TlHelp32.h>
#include <string>

/* 
 * Basic C++ class that can: 
 * - Open process
 * - Close process 
 * - Write smth to memory 
 * - Read smth from memory 
 * - Allocate memory
 * - Release allocated memory
 * */
class MemoryControlLib
{
	public:
		MemoryControlLib(char* procName, size_t bufferSize = 32);
		~MemoryControlLib();
	
		void Open(DWORD accessRight = PROCESS_ALL_ACCESS);
		void Close();
		
		void Write(void *ptr, DWORD to, size_t size, DWORD memProtect = NULL);
		MemoryControlLib& Read(DWORD from, size_t size, DWORD memProtect = NULL);
		DWORD ReadPointer(DWORD base, DWORD *offsets, int count);
		
		DWORD Alloc(DWORD place = 0x1000, DWORD memProtect = PAGE_EXECUTE_READWRITE);
		void Free(DWORD ptr);
		
		DWORD FindSignature(DWORD base, DWORD size, byte* sing, char* mask);
		
		void GetModuleInfo(char* moduleName, MODULEENTRY32* mInfo, bool aboutProcess = false);
		
		DWORD getPID() {return m_pID;}
		
		double toDouble();
		float toFloat();

		UINT64 toUINT64();
		INT64 toINT64();

		UINT32 toUINT32();
		INT32 toINT32();

		UINT16 toUINT16();
		INT16 toINT16();

		UINT8 toUINT8();
		INT8 toINT8();

		char* toStringA();
		wchar_t* toStringW();

		byte *getBuffer(){ return m_pBuffer; };
	
	private:
		char *m_pName;
		HANDLE m_hProc;
		DWORD m_pID;
		byte *m_pBuffer;
		size_t m_bufferSize;
		
		void InitPID();
		bool DataComparer(byte* data, byte* sign, char* mask);
};

#endif /* MEMORYCONTROLLIB_H */