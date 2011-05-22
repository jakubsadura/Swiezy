/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blStoragePolicy_txx
#define blStoragePolicy_txx

#include "blStoragePolicy.h"
#include "CILabAssertMacros.h"
#include <iostream>
#include <fstream>

template <typename ValueType>
blDiskStoragePolicy<ValueType>::blDiskStoragePolicy()
{
	this->Make(0);
}

template <typename ValueType>
blDiskStoragePolicy<ValueType>::blDiskStoragePolicy( std::streamsize size )
{
	this->Make(size);
}


/**
\Fill constructor
*/
template <typename ValueType>
blDiskStoragePolicy<ValueType>::blDiskStoragePolicy( std::streamsize size, ValueType initValue )
{
	this->Make(size, initValue);
}


/**
\Destructor
*/
template <typename ValueType>
blDiskStoragePolicy<ValueType>::~blDiskStoragePolicy()
{
	this->resize(0); // this will close and remove any open file
	delete this->m_Stream;
}

/**
\Resize the File to the specified number of bytes in size. 
*/
template <typename ValueType>
void blDiskStoragePolicy<ValueType>::resize(std::streamsize size, ValueType initValue)
{
	// A matrix of zero elements should always be stored in memory 
	// (the following code depends on this pre-condition).
	cilabAssertMacro(ublas::GetMaxSizeOfAMatrixStoredInMemoryInMb() > 0);

	//! using disk storage and size is above the threshold
	const int ONE_MEGA_BYTE = 1048576;
	const size_type sizeInBytes = size * sizeof(ValueType);
	if( sizeInBytes > ublas::GetMaxSizeOfAMatrixStoredInMemoryInMb() * ONE_MEGA_BYTE)
	{
#ifdef BLSTORAGE_POLICY_MESSAGES
		unsigned int sizeInMB = (unsigned int)(sizeInBytes / (1024 * 1024));
		std::cout << "\n*** Matrix stored on disk: " << sizeInMB << " MB\n";
#endif
		if( !this->m_UsingDiskStorage )
		{
			// open the stream
			this->m_UsingDiskStorage = true;
			this->m_Stream->open(this->m_Path.c_str(), std::ios::out | std::ios::in | std::ios::trunc | std::ios::binary);

			// copy from memory to stream
			int nrOfElementsToCopy = std::min(this->m_Size, size);
			if( nrOfElementsToCopy > 0 )
				this->m_Stream->write( (char*) &(this->m_MemoryStorage[0]), nrOfElementsToCopy * sizeof(ValueType) );
			this->m_Size = nrOfElementsToCopy;
		}

		// we are now using disk storage, enlarge the storage if necessary
		this->m_Stream->seekp(std::ios_base::end);
		while( this->m_Size < size )
		{
			this->m_Stream->write( (char*) &initValue, sizeof(ValueType) );
			this->m_Size++;
		}
	}
	else
	{
//#ifdef BLSTORAGE_POLICY_MESSAGES
//		if (size)
//		{
//			unsigned int sizeInMB = (unsigned int)(sizeInBytes / (1024 * 1024));
//			std::cout << "\nIn memory: " << sizeInMB << " MB\n";
//		}
//#endif
		this->m_MemoryStorage.resize(size, initValue);
		if( this->m_UsingDiskStorage )
		{
			this->m_UsingDiskStorage = false;
			int nrOfElementsToCopy = std::min(this->m_Size, size);
			if( nrOfElementsToCopy > 0 )
				this->m_Stream->read( (char*) &(m_MemoryStorage[0]), nrOfElementsToCopy * sizeof(ValueType) );
			this->m_Stream->close();
			boost::filesystem::remove(this->m_Path);
		}
		m_Size = size;
	}
}

//! Get Size
template <typename ValueType>
std::streamsize blDiskStoragePolicy<ValueType>::size() const
{
	return this->m_Size;
}

//!
template <typename ValueType>
typename blDiskStoragePolicy<ValueType>::reference blDiskStoragePolicy<ValueType>::operator[](int i)
{
	return reference(this, i);
} 

//!
template <typename ValueType>
typename blDiskStoragePolicy<ValueType>::const_reference blDiskStoragePolicy<ValueType>::operator[](int i) const
{
	return const_reference(this, i);
} 

//!
template <typename ValueType>
void blDiskStoragePolicy<ValueType>::Set( int pos, ValueType x )
{
	if( this->m_UsingDiskStorage )
	{
		cilabAssertMacro(pos >= 0); 
		cilabAssertMacro(pos < this->m_Size );

		//! set the position of the put pointer and write
		this->m_Stream->seekp(pos*sizeof(ValueType));
		this->m_Stream->write( reinterpret_cast<char*>(&x), sizeof(ValueType));	
	}
	else
	{
		this->m_MemoryStorage[pos] = x;
	}
}

//!
template <typename ValueType>
ValueType blDiskStoragePolicy<ValueType>::Get( int pos ) const
{
	cilabAssertMacro(pos >= 0); 
	cilabAssertMacro(pos < this->m_Size );

	if( this->m_UsingDiskStorage )
	{
        Self* This = const_cast<Self*>(this);
		ValueType returnValue;
		This->m_Stream->seekg(pos * sizeof(ValueType));
		This->m_Stream->read( reinterpret_cast<char*>(&returnValue), sizeof(ValueType));
		return returnValue;
	}
	else
	{
		return this->m_MemoryStorage[pos];
	}
}

//!
template <typename ValueType>
void blDiskStoragePolicy<ValueType>::swap( blDiskStoragePolicy& diskStoragePolicy )
{
	std::swap(this->m_MemoryStorage, diskStoragePolicy.m_MemoryStorage);
	std::swap(this->m_Path,diskStoragePolicy.m_Path);
	std::swap(this->m_Size,diskStoragePolicy.m_Size);
	std::swap(this->m_Stream,diskStoragePolicy.m_Stream);
	std::swap(this->m_UsingDiskStorage, diskStoragePolicy.m_UsingDiskStorage);
}

//!
template <typename ValueType>
bool blDiskStoragePolicy<ValueType>::IsUsingDisk() const
{
	return this->m_UsingDiskStorage;
}

template <typename ValueType>
void blDiskStoragePolicy<ValueType>::Make(int size, ValueType initValue)
{
	cilabAssertMacro( size >= 0 );
	this->m_Path = ublas::GenerateUblasMatrixPath();
	this->m_Stream = new std::fstream();
	this->m_UsingDiskStorage = false;
	this->m_Size = 0;
	this->resize(size, initValue);
}

template <typename ValueType>
void blDiskStorageCache<ValueType>::SetNumberOfColumns(int x)
{
	this->m_CachedRow.resize(x);
}

template <typename ValueType>
bool blDiskStorageCache<ValueType>::UpdateCache( int pos ) const
{
	cilabAssertMacro(this->GetNumberOfColumns() != 0);
    Self* This = const_cast<Self*>(this);
	const int currentRow = pos / this->GetNumberOfColumns();
	const int currentCol = pos - currentRow * this->GetNumberOfColumns();
	bool hit = this->m_CurrentRowIdx == currentRow;
	++(hit ? this->m_TotalNrOfCacheHits : this->m_CacheMisses);

	if( !hit )
	{
		this->WriteCacheToDisk();
		this->m_CurrentNrOfCacheHits = 0;
	}
	else
	{
		this->m_CurrentNrOfCacheHits++;
		if( this->m_CurrentNrOfCacheHits == 1 )
		{
			// Set the position of the get pointer and read the data
			This->m_Stream->seekg(currentRow * this->GetNumberOfColumns() * sizeof(ValueType));
			This->m_Stream->read( this->GetStartOfCacheBuffer(), this->GetNumberOfColumns() * sizeof(ValueType));
		}
	}

	this->m_CurrentRowIdx = currentRow;
	this->m_CurrentColIdx = currentCol;
	return this->m_CurrentNrOfCacheHits > 0;
}

template <typename ValueType>
void blDiskStorageCache<ValueType>::WriteCacheToDisk() const
{
	if( this->m_CurrentNrOfCacheHits > 0 )
	{
		this->m_Stream->seekp(this->m_CurrentRowIdx * this->GetNumberOfColumns() * sizeof(ValueType));
		this->m_Stream->write( this->GetStartOfCacheBuffer(), this->GetNumberOfColumns() * sizeof(ValueType));
	}
}

template <typename ValueType>
void blDiskStorageCache<ValueType>::PrintCacheStatistics() const
{
	std::cout << "Nr cache hits = " << this->m_TotalNrOfCacheHits << '\n';
	std::cout << "Nr cache misses = " << this->m_CacheMisses << '\n';
	if(const double totalCacheHitsAndMisses = this->m_CacheMisses + this->m_TotalNrOfCacheHits)
	{
		std::cout << "Cache hit percentage = " << (this->m_TotalNrOfCacheHits / totalCacheHitsAndMisses) << '\n';
	}
}

#endif //blStoragePolicy_txx
