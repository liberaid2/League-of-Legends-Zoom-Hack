#include "MemoryControlLib.h"

MemoryControlLib::MemoryControlLib(char* procName, size_t bufferSize)
{
	int len = strlen(procName) + 1;
	m_pName = new char[len];
	strcpy_s(m_pName, len, procName);
	
	m_pBuffer = new byte[bufferSize];
	InitPID();
}

MemoryControlLib::~MemoryControlLib()
{
	delete[] m_pName;
	delete[] m_pBuffer;
}

void MemoryControlLib::InitPID()
{
	m_pID = 0;
	
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pInfo;
	pInfo.dwSize = sizeof(PROCESSENTRY32);
	
	if(Process32First(snapshot, &pInfo))
	{
		while(Process32Next(snapshot, &pInfo))
		{
			if(_stricmp(pInfo.szExeFile, m_pName) == 0)
			{
				m_pID = pInfo.th32ProcessID;
				break;
			}
		}
	}
	CloseHandle(snapshot);
}

void MemoryControlLib::Open(DWORD accessRights) {m_hProc = OpenProcess(accessRights, false, m_pID);}
void MemoryControlLib::Close() {CloseHandle(m_hProc);}
void MemoryControlLib::Write(void* ptr, DWORD to, size_t size, DWORD memProtect)
{
	DWORD oldProtect = 0;
	VirtualProtectEx(m_hProc, (LPVOID)to, size, memProtect, &oldProtect);
	WriteProcessMemory(m_hProc, (LPVOID)to, ptr, size, NULL);
	VirtualProtectEx(m_hProc, (LPVOID)to, size, oldProtect, NULL);
}
MemoryControlLib& MemoryControlLib::Read(DWORD from, size_t size, DWORD memProtect)
{
	memset(m_pBuffer, 0, sizeof(m_pBuffer));
	DWORD oldProtect = 0;
	VirtualProtectEx(m_hProc, (LPVOID)from, size, memProtect, &oldProtect);
	ReadProcessMemory(m_hProc, (LPCVOID)from, m_pBuffer, size, NULL);
	VirtualProtectEx(m_hProc, (LPVOID)from, size, oldProtect, NULL);
	return *this;
}

DWORD MemoryControlLib::ReadPointer(DWORD base, DWORD* offsets, int count)
{
	for (int i = 0; i < count - 1; i++)
		base = Read(base + offsets[i], 4).toUINT32();
	return base + offsets[count - 1];
}

DWORD MemoryControlLib::Alloc(DWORD place, DWORD memProtect) { return (DWORD)VirtualAllocEx(m_hProc, NULL, place, MEM_COMMIT | MEM_RESERVE, memProtect);}
void MemoryControlLib::Free(DWORD ptr) {VirtualFreeEx(m_hProc, (LPVOID)ptr, 0, MEM_RELEASE);}

DWORD MemoryControlLib::FindSignature(DWORD base, DWORD size, byte* sign, char* mask)
{
	MEMORY_BASIC_INFORMATION mbi = {0};
	DWORD offset = 0;
	
	while(offset < size)
	{
		VirtualQueryEx(m_hProc, (LPCVOID)(base+offset), &mbi, sizeof(MEMORY_BASIC_INFORMATION));
		if(mbi.State != MEM_FREE)
		{
			byte* buffer = new byte[mbi.RegionSize];
			
			ReadProcessMemory(m_hProc, mbi.BaseAddress, buffer, mbi.RegionSize, NULL);
			for(int i = 0; i < mbi.RegionSize; i++)
			{
				if(DataComparer(buffer + i, sign, mask))
				{
					delete[] buffer;
					return (DWORD)mbi.BaseAddress + i;
				}
			}
			
			delete[] buffer;
		}
		
		offset += mbi.RegionSize;
	}
	
	return 0;
}

bool MemoryControlLib::DataComparer(byte* data, byte* sign, char* mask)
{
	for(; *mask; mask++, sign++, data++)
		if(*mask == 'x' && *data != *sign)
			return false;
		
	return true;
}

void MemoryControlLib::GetModuleInfo(char* moduleName, MODULEENTRY32* mInfo, bool aboutProcess)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_pID);
	mInfo->dwSize = sizeof(MODULEENTRY32);

	if (Module32First(snapshot, mInfo))
	{
		if (aboutProcess){CloseHandle(snapshot);return;	}
		while (Module32Next(snapshot, mInfo))
		{
			if (_stricmp(mInfo->szModule, moduleName) == 0)
				return;
		}
	}
	CloseHandle(snapshot);
	memset(mInfo, 0, sizeof(MODULEENTRY32));
}

double MemoryControlLib::toDouble()
{
	double v = 0;
	memcpy(&v, m_pBuffer, sizeof(double));
	return v;
}
float MemoryControlLib::toFloat()
{
	float v = 0;
	memcpy(&v, m_pBuffer, sizeof(float));
	return v;
}

UINT64 MemoryControlLib::toUINT64()
{
	UINT64 v = 0;
	memcpy(&v, m_pBuffer, sizeof(UINT64));
	return v;
}
INT64 MemoryControlLib::toINT64()
{
	INT64 v = 0;
	memcpy(&v, m_pBuffer, sizeof(INT64));
	return v;
}

UINT32 MemoryControlLib::toUINT32()
{
	UINT32 v = 0;
	memcpy(&v, m_pBuffer, sizeof(UINT32));
	return v;
}
INT32 MemoryControlLib::toINT32()
{
	INT32 v = 0;
	memcpy(&v, m_pBuffer, sizeof(INT32));
	return v;
}

UINT16 MemoryControlLib::toUINT16()
{
	UINT16 v = 0;
	memcpy(&v, m_pBuffer, sizeof(UINT16));
	return v;
}
INT16 MemoryControlLib::toINT16()
{
	INT16 v = 0;
	memcpy(&v, m_pBuffer, sizeof(INT16));
	return v;
}

UINT8 MemoryControlLib::toUINT8()
{
	UINT8 v = 0;
	memcpy(&v, m_pBuffer, sizeof(UINT8));
	return v;
}
INT8 MemoryControlLib::toINT8()
{
	INT8 v = 0;
	memcpy(&v, m_pBuffer, sizeof(INT8));
	return v;
}

char* MemoryControlLib::toStringA()
{
	return (char *)m_pBuffer;
}
wchar_t* MemoryControlLib::toStringW()
{
	return (wchar_t *)m_pBuffer;
}
