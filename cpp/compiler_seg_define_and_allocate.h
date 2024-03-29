#pragma once

#include <Windows.h>
#include <iostream>
#include "NtDefines.h"

#pragma section(".LDATA", read, write)
#pragma section(".LTEXT", read, write, execute)

#pragma data_seg(".LDATA$1")
#pragma data_seg(".LDATA$2")
#pragma data_seg(".LDATA$3")
#pragma data_seg()

#pragma code_seg(".LTEXT$1")
#pragma code_seg(".LTEXT$2")
#pragma code_seg(".LTEXT$3")
#pragma code_seg()

__declspec( allocate( ".LDATA$1" ) ) static char Np_DataStart = 0x0;
__declspec( allocate( ".LDATA$3" ) ) static char Np_DataEnd = 0x0;

__declspec( allocate( ".LTEXT$1" ) ) static char Np_TextStart = 0x0;
__declspec( allocate( ".LTEXT$3" ) ) static char Np_TextEnd = 0x0;


#define NON_PAGED_DATA  __declspec( allocate( ".LDATA$2" ) )
#define NON_PAGED_CODE __declspec( code_seg( ".LTEXT$2" ) ) __declspec(noinline)
#define NON_PAGED_LAMBDA(...)  []( __VA_ARGS__ ) NON_PAGED_CODE

// Mini non-paged crt
#define Np_memcpy(dst, src, size) __movsb( ( BYTE* ) dst, ( const BYTE* ) src, size )
#define Np_memset(dst, val, size) __stosb( ( BYTE* ) dst, val, size)
#define Np_ZeroMemory(dst, size) __stosb( ( BYTE* ) dst, 0, size)

#pragma comment(linker,"/MERGE:.LDATA=.data")
#pragma comment(linker,"/MERGE:.LTEXT=.text")

// Routines to lock the pages
static BOOL Np_TryIncreaseWorkingSetSize( SIZE_T Size )
{
	SIZE_T Min, Max;
	if ( !GetProcessWorkingSetSize( NtCurrentProcess(), &Min, &Max ) )
		return FALSE;
	if ( !SetProcessWorkingSetSize( NtCurrentProcess(), Min + Size, Max + Size ) )
		return FALSE;
	printf( "[+] Increasing working set (%d KB, %d KB) -> (%d KB, %d KB)!\n", Min / 1024, Max / 1024, ( Min + Size ) / 1024, ( Max + Size ) / 1024 );
	return TRUE;
}

static BOOL Np_TryLockPage( PVOID Page )
{
	if ( !Np_TryIncreaseWorkingSetSize( 0x1000 ) )
		return FALSE;
	if ( VirtualLock( Page, 0x1000 ) )
		return TRUE;
	if ( !Np_TryIncreaseWorkingSetSize( 0x2000 ) )
		return FALSE;
	return VirtualLock( Page, 0x1000 );
}

static BOOL Np_LockRange( PVOID From, PVOID To )
{
	PBYTE FromPageAligned = ( PBYTE ) ( ( uintptr_t ) ( From ) & ( ~0xFFF ) );
	PBYTE ToPageAligned = ( PBYTE ) ( ( uintptr_t ) ( To ) & ( ~0xFFF ) );

	for ( PBYTE Current = FromPageAligned; Current <= ToPageAligned; Current += 0x1000 )
	{
		if ( !Np_TryLockPage( Current ) )
		{
			printf( "[+] Failed locking %16llx!\n", Current );
			return FALSE;
		}
		else
		{
			printf( "[+] Locked %16llx successfully!\n", From );
		}
	}
	return TRUE;
}

static BOOL Np_LockSections()
{
	printf( "[+] .LDATA: %16llx -> %16llx!\n", &Np_DataStart, &Np_DataEnd );
	printf( "[+] .LTEXT: %16llx -> %16llx!\n", &Np_TextStart, &Np_TextEnd );

	return
		Np_LockRange( &Np_DataStart, &Np_DataEnd ) &&
		Np_LockRange( &Np_TextStart, &Np_TextEnd );
}