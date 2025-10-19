#pragma once
#include<Windows.h>
typedef unsigned long long uint64_t;

template <class type>
static type Read(uint64_t Address)
{
	if (!IsBadReadPtr((PVOID)Address, sizeof(type))) { return *(type*)Address; }
	type buffer{};
	return buffer;
}
template <class type>
static void Write(uint64_t Address, type buffer)
{
	if (!IsBadWritePtr((PVOID)Address,sizeof(type)))
	{
		*(type*)Address = buffer;
	}
}