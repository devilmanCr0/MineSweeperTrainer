#include "hTools.h"

void memoryAllocate(unsigned int size, BYTE* dst, BYTE* src)
{
	DWORD oldProtection;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtection);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldProtection, &oldProtection);

}
void memoryPatch(unsigned int size, BYTE* dst)
{
	DWORD protocol{};
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &protocol);
	memset(dst, 0x90, size); // sets all bytes at dst to nop
	VirtualProtect(dst, size, protocol, &protocol);
}
uintptr_t AddyAddress(uintptr_t address, std::vector<unsigned int> offsets)
{
	uintptr_t theaddress{ address };
	for (unsigned int off : offsets)
	{
		address = *(uintptr_t*)address;
		address += off;
	}

	return theaddress;
}

bool hook(void* toHook, void* ourFunc, int size)
{
	if (size < 5)
	{
		return false;
	}
	DWORD protocol{};
	VirtualProtect(toHook, size, PAGE_EXECUTE_READWRITE, &protocol);
	memset(toHook, 0x90, size);
	
	DWORD addressOffset{ (DWORD)ourFunc - (DWORD)toHook - 5 };

	*(BYTE*)toHook = 0xE9;
	*(DWORD*)((DWORD)toHook + 1) = addressOffset;

	VirtualProtect(toHook, size, protocol, &protocol);

	return true;
}

DWORD trampHook(void* toHook, void* ourFunc, int size)
{
	if (size < 5)
	{
		return 0;
	}

	BYTE* gateway{ (BYTE*)VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE) };

	
	memcpy_s(gateway, size, toHook, size);

	*(BYTE*)(gateway + size) = 0xE9;

	*(DWORD*)((DWORD)gateway + size + 1) = (DWORD)toHook - (DWORD)gateway - 5;

	hook(toHook, ourFunc, size);


	return (DWORD)gateway;
}


Hooking::Hooking(BYTE* src, BYTE* dst, BYTE* PtrToGateWay, uintptr_t len)
{
	this->src = src;
	this->dst = dst;
	this->len = len;
	this->PtrToGateWay = PtrToGateWay;
}
Hooking::Hooking(const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGateWay, uintptr_t len)
{
	HMODULE hModule = GetModuleHandleA(modName);

	
	this->src = (BYTE*)GetProcAddress(hModule, exportName);
	this->dst = dst;
	this->len = len;
	this->PtrToGateWay = PtrToGateWay;
}

void Hooking::Enable()
{
	memcpy(originalByte, src, len);
	*(uintptr_t*)PtrToGateWay = (uintptr_t)trampHook(src, dst, len);

	bStatus = true;

}
void Hooking::Disable()
{
	memoryAllocate(len, src, originalByte);
	bStatus = false;

}
void Hooking::Toggle()
{
	if (!bStatus) Enable();
	else Disable();
}