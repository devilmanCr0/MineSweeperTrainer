#pragma once
#include <windows.h>
#include<cstdint>
#include<cstdio>
#include<windows.h>
#include<Windows.h>
#include<TlHelp32.h>
#include<vector>
#include<iostream>
#include<stdio.h>
#include<string>

void memoryAllocate(unsigned int size, BYTE* dst, BYTE* src);
void memoryPatch(unsigned int size, BYTE* dst);
uintptr_t AddyAddress(uintptr_t address, std::vector<unsigned int> offsets);
DWORD trampHook(void* toHook, void* ourFunc, int size);
bool hook(void* toHook, void* ourFunc, int size);


struct Hooking
{
	bool bStatus{ false };

	BYTE* src{ nullptr };
	BYTE* dst{ nullptr };
	BYTE* PtrToGateWay{ nullptr };

	int len{ 0 };

	BYTE originalByte[15]{ 0 };

	Hooking(BYTE* src, BYTE* dst, BYTE* PtrToGateWay, uintptr_t len);
	Hooking(const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGateWay, uintptr_t len);

	void Enable();
	void Disable();
	void Toggle();
};