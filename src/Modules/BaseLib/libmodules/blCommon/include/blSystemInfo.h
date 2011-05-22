/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLSYSTEMINFO_H
#define BLSYSTEMINFO_H

#include "string.h"

namespace base{

/** 
\brief System info utility functions
\ingroup blCommon
\date 07/11/2008
\author Maarten Nieber
*/
class SystemInfo
{
public:
	/** 
	\brief The size of the user-mode portion of the virtual address space 
	of the calling process, in bytes.
	\note On linux this function return 0.
	\note On Windows: This value depends on the type of process, the type 
	of processor, and the configuration of the operating system. For example, 
	this value is approximately 2 GB for most 32-bit processes on an x86 
	processor and approximately 3 GB for 32-bit processes that are large 
	address aware running on a system with 4-gigabyte tuning enabled.
	*/
	static size_t GetAvailableVirtualMemoryInMb();

	/**
	\brief Available memory size.
	\note On Windows: The amount of physical memory currently available, 
	in bytes. This is the amount of physical memory that can be immediately 
	reused without having to write its contents to disk first.
	*/
	static size_t GetAvailablePhysicalMemoryInMb();

	//! Returns the total size of physical memory in bytes
	static size_t GetTotalSizeOfPhysicalRam();

	/** 
	\brief Returns the memory usage of the current process in bytes.
	\note On Linux, this refers to the virtual memory allocated by 
	the process (the VIRT column in top).
	On windows, this refery to the size in bytes of the working 
	set pages (the "Mem Usage" column in the task manager).
	\param [in] getVirtualMemory Return the column VM Size
	of the task manager
	*/
	static size_t GetProcessMemoryUsage( bool getVirtualMemory = true );

	/**
	 \brief Compute max memory allowed by the current process.
	 This function tries to allocate memory blocks of 100 Mb until
	 the allocation call fails.
	 \param [in] lLimitInGb Upper limit in Gb that is allowed
	 \return max allowed memory in bytes
	 */
	static size_t ComputeProcessMaxMemory( );

protected:
#ifndef _MSC_VER 
	static int ReadStatmFromProcFS( 
		int* size, int* res, int* shared, int* text, int* sharedLibs, int* stack, int* dirtyPages );
#endif

};

} // namespace base

#endif //BLSYSTEMINFO_H
