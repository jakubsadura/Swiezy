/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blSystemInfo.h"

#include <list>
#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER 
    #include <windows.h>
    #include <psapi.h>
#else
	#ifdef __APPLE__
		#include <sys/param.h>
		#include <sys/types.h>
		#include <mach/vm_statistics.h>
		#include <mach/mach_types.h> 
		#include <mach/mach_init.h>
		#include <mach/mach_host.h>
		#include <getopt.h>
		#include <mach/mach.h>

		#include <sys/sysctl.h> 
        #include <sys/mount.h>
    #else
        #include <sys/sysinfo.h>
    #endif 
    #include <unistd.h>
#endif //_MSC_VER 

const unsigned int oneMb = 1048576;

size_t base::SystemInfo::GetAvailableVirtualMemoryInMb()
{
#ifdef _MSC_VER
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	int retVal = GlobalMemoryStatusEx (&statex);
	return size_t(statex.ullAvailVirtual/oneMb);
#endif

	return 0;
}

size_t base::SystemInfo::GetAvailablePhysicalMemoryInMb()
{
#ifdef _MSC_VER
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	int retVal = GlobalMemoryStatusEx (&statex);
	return size_t(statex.ullAvailPhys/oneMb);
#elif defined(__APPLE__) 
	struct statfs stats;
	if (0 == statfs("/", &stats))
		return (u_int64_t)stats.f_bsize * stats.f_bfree / oneMb;
	else
		return 0;
#else 
	struct sysinfo info;
	if ( ! sysinfo( &info ) )
		return info.freeram * info.mem_unit / oneMb;
#endif
	
	return 0;
}

size_t base::SystemInfo::GetTotalSizeOfPhysicalRam()
{
#ifdef _MSC_VER 
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);
	return (size_t) statex.ullTotalPhys;
#elif defined(__APPLE__) 
	struct statfs stats;
	if (0 == statfs("/", &stats))
		return (u_int64_t)stats.f_bsize * stats.f_bfree;
	else
		return 0;
#else
	struct sysinfo info;
	if ( ! sysinfo( &info ) )
		return info.totalram * info.mem_unit;
#endif 
}


size_t base::SystemInfo::GetProcessMemoryUsage( bool getVirtualMemory )
{
#ifdef _MSC_VER 
	size_t size = 0;
	DWORD pid = GetCurrentProcessId();
	PROCESS_MEMORY_COUNTERS pmc;
	HANDLE hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid );
	if ( hProcess == NULL )
		return 0;
	if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
	{
		if ( !getVirtualMemory )
		{
			size = pmc.WorkingSetSize;
		}
		else
		{
			size = pmc.PagefileUsage;
		}
	}
	CloseHandle( hProcess );
	return size;
#else
#ifndef __APPLE__
	int size, res, shared, text, sharedLibs, stack, dirtyPages;
	if ( ! ReadStatmFromProcFS( &size, &res, &shared, &text, &sharedLibs, &stack, &dirtyPages ) )
		return (size_t) size * getpagesize();
	else
		return 0;
#else  //Apple version
	
	struct task_basic_info t_info;
	mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

	if (KERN_SUCCESS == task_info(mach_task_self(),
		TASK_BASIC_INFO, (task_info_t)&t_info, 
		&t_info_count))
			return t_info.resident_size;
			//return t_info.virtual_size; // if one wants virtual size instead!
	else 
		return 0;

#endif //__APPLE__
#endif 
	return 0;
}

#ifndef _MSC_VER 
#ifndef __APPLE__
int base::SystemInfo::ReadStatmFromProcFS( 
	int* size, int* res, int* shared, int* text, int* sharedLibs, int* stack, int* dirtyPages ) 
{
	int ret = 0;
	FILE* f;
	f = fopen( "/proc/self/statm", "r" );
	if( f ) {
		if (fscanf( f, "%d %d %d %d %d %d %d", size, res, shared, text, sharedLibs, stack, dirtyPages )
		    != 7) 
			ret = -1; 
		fclose( f );
	} else {
		ret = -1;
	}
	return ret;
}
#endif //__APPLE__
#endif //_MSC_VER

size_t base::SystemInfo::ComputeProcessMaxMemory( )
{

	size_t limitInBytes = GetTotalSizeOfPhysicalRam();
	size_t dCurrentMemory = 0;
	size_t lMemoryStepInBytes = 100 * 1024 * 1024;
	std::list<void*> memoryBlocks;
	bool bMallocFailed = false;

// In debug it takes a lot of time to allocate memory
#if defined( _WIN32) && defined(_DEBUG )
	return GetAvailableVirtualMemoryInMb( )*oneMb;
#else
	try
	{
		// Allocate memory until bad_allocation exception
		while ( !bMallocFailed && dCurrentMemory < limitInBytes )
		{
			void* memoryBlock = malloc( lMemoryStepInBytes );
			if ( memoryBlock != NULL )
			{
				memoryBlocks.push_back( memoryBlock );
				dCurrentMemory += lMemoryStepInBytes;
			}
			else
			{
				bMallocFailed = true;
			}
		}
	}
	catch (std::bad_alloc& e)
	{
		e.what();
	}

	// Free memory
	while ( memoryBlocks.size( ) )
	{
		free( memoryBlocks.front() );
		memoryBlocks.pop_front();
	}
#endif

	return dCurrentMemory + GetProcessMemoryUsage( );
}
