/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blStoragePolicy_H
#define blStoragePolicy_H

//! ublas storage
#include <boost/numeric/ublas/storage.hpp> 
#include <boost/numeric/ublas/functional.hpp>
#include "boost/filesystem.hpp"
#include <boost/iterator/iterator_facade.hpp>

#include <blSingleton.h>

#include "CILabExceptionMacros.h"
#include "CILabNamespaceMacros.h"
#include "CILabBoostMacros.h"

#include <exception>
#include <string.h>

#define BLSTORAGE_POLICY_MESSAGES

cilabDeclareExceptionMacro(DiskStoragePolicyException, std::exception)

template <typename ValueType>

//forward declarations
class blDiskStoragePolicy;

CILAB_BEGIN_NAMESPACE(ublas)

//! Set the folder where blDiskStoragePolicy stores its matrices.
//! \arg createFolder - Flag: create the folder if \a folderName does not exist.
void SetDiskFileStorageFolder( const std::string folderName, const bool createFolder = false );

//! Returns the current maximum size of a matrix that is still stored in memory. Matrices larger than this size
//! will be stored on disk.
size_t GetMaxSizeOfAMatrixStoredInMemoryInMb();

//! Sets the current maximum size (in MB) of a matrix that is still stored in memory. Matrices larger than this size
//! will be stored on disk.
void SetMaxSizeOfAMatrixStoredInMemoryInMb(size_t valueInMegaBytes);

//! This function returns a new filename for storing an ublas matrix on disk
std::string GenerateUblasMatrixPath();

CILAB_END_NAMESPACE(ublas)

/**
This class is a "smart reference" to a matrix value that is stored using the DiskStoragePolicy class.

\author Maarten Nieber
\date 09 jun 2008
*/

template< class ValueType >
class blDiskStoragePolicyReference
{
public:
	typedef blDiskStoragePolicy<ValueType> StoragePolicyType;

	//! When creating the smart reference, you have to pass the position in the matrix-storage for the 
	//! matrix element that is referenced.
	blDiskStoragePolicyReference(StoragePolicyType* storagePolicy, int pos)
	{
		this->m_StoragePolicy = storagePolicy;
		this->m_Pos = pos;
	}

	//! Return the value that is referenced.
	operator ValueType() const
	{
		return this->m_StoragePolicy->Get(this->m_Pos);
	}

	//! Set the value that is referenced.
	blDiskStoragePolicyReference& operator=(const ValueType& rhs)
	{
		this->m_StoragePolicy->Set(this->m_Pos, rhs);
		return *this;
	}

	//! Divide the value that is referenced by a factor.
	template< class F>
	void operator/=(const F factor)
	{
		this->m_StoragePolicy->Set(this->m_Pos, this->m_StoragePolicy->Get(this->m_Pos) / factor);
	}

	//! Multiply the value that is referenced by a factor.
	template< class F>
	void operator*=(const F factor)
	{
		this->m_StoragePolicy->Set(this->m_Pos, this->m_StoragePolicy->Get(this->m_Pos) * factor);
	}

	//! Add a term to the value that is referenced.
	template< class F>
	void operator+=(const F factor)
	{
		this->m_StoragePolicy->Set(this->m_Pos, this->m_StoragePolicy->Get(this->m_Pos) + factor);
	}

	//! Subtract a term from the value that is referenced.
	template< class F>
	void operator-=(const F factor)
	{
		this->m_StoragePolicy->Set(this->m_Pos, this->m_StoragePolicy->Get(this->m_Pos) - factor);
	}

private:
	//! Storage of all matrix values.
	StoragePolicyType* m_StoragePolicy;
	//! Position within m_StoragePolicy of the referenced value.
	int m_Pos;
};

/**
This class is similar to blDiskStoragePolicyReference, except that the reference is const (meaning you can only
read it).

\author Maarten Nieber
\date 04 jul 2008
*/

template< class ValueType >
class blDiskStoragePolicyConstReference
{
public:
	typedef blDiskStoragePolicy<ValueType> StoragePolicyType;
	blDiskStoragePolicyConstReference(const StoragePolicyType* storagePolicy, int pos)
	{
		this->m_StoragePolicy = storagePolicy;
		this->m_Pos = pos;
	}

	//!
	operator ValueType() const
	{
		return this->m_StoragePolicy->Get(this->m_Pos);
	}

private:
	//! Storage of all matrix values.
	const StoragePolicyType* m_StoragePolicy;
	//! Position within m_StoragePolicy of the referenced value.
	int m_Pos;
};

/**
This class implements a memory-cache that can be used with blDiskStoragePolicy.
Only the currently used matrix row is cached. When you move to a new row, the cache is only filled the second
time you use the new row (otherwise, traversing a column would become very slow).
In experiments so far, the cache seemed to slow down the program (with cache 180 secs, without 120 secs).
However, the code is here in case someone comes up with an improvement.

\author Maarten Nieber
\date 04 jul 2008
*/

template< class ValueType >
class blDiskStorageCache
{
public:
	typedef blDiskStorageCache<ValueType> Self;
	
	//! \arg stream - The disk stream used by the blDiskStoragePolicy that this cache is associated with. The cache
	//! needs the stream to fill/write the cache contents.
	blDiskStorageCache(std::fstream* stream)
	{
		this->m_Stream = stream;
		this->m_TotalNrOfCacheHits = this->m_CurrentNrOfCacheHits = 0;
		this->m_CacheMisses = 0;
		this->m_CurrentColIdx = this->m_CurrentRowIdx = -1;
	}

	//! The cache needs to know the number of columns to see which matrix row it has to cache.
	void SetNumberOfColumns(int x);

	//!
	void operator=(const blDiskStorageCache& rhs)
	{
		this->m_CachedRow = rhs.m_CachedRow;
		this->m_CurrentRowIdx = rhs.m_CurrentRowIdx;
		this->m_CurrentColIdx = rhs.m_CurrentColIdx;
		this->m_CurrentNrOfCacheHits = rhs.m_CurrentNrOfCacheHits;
	}

	//! Swaps the contents of two matrices
	void swap( blDiskStorageCache& rhs )
	{
		std::swap(this->m_Stream, rhs.m_Stream);
		std::swap(this->m_CurrentRowIdx, rhs.m_CurrentRowIdx);
		std::swap(this->m_CurrentColIdx, rhs.m_CurrentColIdx);
		std::swap(this->m_TotalNrOfCacheHits, rhs.m_TotalNrOfCacheHits);
		std::swap(this->m_CurrentNrOfCacheHits, rhs.m_CurrentNrOfCacheHits);
		std::swap(this->m_CacheMisses, rhs.m_CacheMisses);
		std::swap(this->m_CachedRow, rhs.m_CachedRow);
	}

	//! Updates statistics about the cache (pos either hits or misses the cache)
	//! Returns true if the cache was hit.
	bool UpdateCache( int pos ) const;

	//! Returns the number of columns in the matrix.
	inline int GetNumberOfColumns() const
	{
		return this->m_CachedRow.size();
	}

	//! Helper function.
	char* GetStartOfCacheBuffer() const
	{
		return reinterpret_cast<char*>(&(this->m_CachedRow[0]));
	}

	//! Writes current cache contents to the disk
	void WriteCacheToDisk() const;

	//!
	void PrintCacheStatistics() const;

private:
	//!
	mutable std::vector<ValueType> m_CachedRow;
	//!
	mutable int m_CurrentRowIdx;
	//!
	mutable int m_CurrentColIdx;
	//! For studing the cache performance afterwards
	mutable int m_TotalNrOfCacheHits;
	//! For studing the cache performance afterwards
	mutable int m_CacheMisses;
	//! Nr of times the currently cached row is hit (this member is reset to zero when you move to a new row).
	mutable int m_CurrentNrOfCacheHits;
	//! See constructor.
	std::fstream* m_Stream;
};

/**
Class for storage a container of matrices on disk if the size to be saved is over a threshold

\author Mart�n Bianculli & Maarten Nieber
\date 11 jun 2008
*/
template <typename ValueType>
class blDiskStoragePolicy
{
public:
	friend class blDiskStoragePolicyReference<ValueType>;
	friend class blDiskStoragePolicyConstReference<ValueType>;
	friend class blStoragePolicyTest;

	typedef blDiskStoragePolicy<ValueType> Self;
	typedef blDiskStoragePolicyReference<ValueType> reference;
	typedef blDiskStoragePolicyConstReference<ValueType> const_reference;
	typedef unsigned int size_type;
	typedef int difference_type;
	typedef const ValueType* const_pointer;
	typedef ValueType* pointer;
    typedef const_pointer const_iterator;
	typedef pointer iterator;

	//!
	blDiskStoragePolicy();

	//!
	blDiskStoragePolicy(const blDiskStoragePolicy& rhs)
	{
		this->Make(0);
		this->DeepCopyFrom(rhs);
	}

	//! Doesn't do something at the moment. But don't remove, in the future we may need it.
	void SetNumberOfColumns(int x)
	{

	}

	//! Constructs storage of \a size elements.
	blDiskStoragePolicy( std::streamsize size);

	//! Fill constructor. Creates n initialized element with copies of x.
	blDiskStoragePolicy( std::streamsize size, ValueType x );

	//! Destructor. The matrix file is deleted.
	~blDiskStoragePolicy();

	//!
	const blDiskStoragePolicy& operator=(const blDiskStoragePolicy& rhs)
	{
		this->DeepCopyFrom(rhs);
		return *this;
	}
	
	//! The container may be reallocated if its size changes. Existing element values are preserved,
	//! \additional elements are copies of \a initValue. 
	void resize(std::streamsize size, ValueType initValue = 0 );

	//!
	std::streamsize size() const;

	//! zero-based index element access
	reference operator[](int i);

	//! zero-based index element access
	const_reference operator[](int i) const;
	
	//! Swaps the contents of two matrices
	void swap( blDiskStoragePolicy& diskStoragePolicy ); 

	//! Returns true if currently using disk, and false if using memory.
	bool IsUsingDisk() const;

private:
	//! Return element at position \a pos.
	ValueType Get(int pos) const;
	
	//! Set element at position \a pos to \a x.
	void Set(int pos, ValueType x);

	//! Helper function for the constructor
	void Make(int size, ValueType initValue = 0);

	//!
	void DeepCopyFrom(const blDiskStoragePolicy& rhs)
	{
		this->resize(rhs.size());
		for( int i = 0; i < this->m_Size; ++i )
			this->Set(i, rhs.Get(i));
	}

private:
	std::streamsize m_Size;
	std::fstream* m_Stream;
	std::string m_Path;
	bool m_UsingDiskStorage;
	std::vector<ValueType> m_MemoryStorage;
};

/**
 * Boost methods (boost/numeric/ublas/functional.hpp) specific implementation for the
 * blDiskStoragePolicyReference type.
 */
namespace boost { namespace numeric { namespace ublas {

template<>
struct scalar_assign<blDiskStoragePolicyReference<double>, double>:
	public scalar_binary_assign_functor<blDiskStoragePolicyReference<double>, double>
{
	typedef blDiskStoragePolicyReference<double> T1;
	typedef double T2;
	typedef T1 argument1_type;
	//typedef typename scalar_binary_assign_functor<T1, T2>::argument1_type argument1_type;
	typedef scalar_binary_assign_functor<T1,T2>::argument2_type argument2_type;
	//typedef typename scalar_binary_assign_functor<T1, T2>::argument2_type argument2_type;
#if BOOST_WORKAROUND( __IBMCPP__, <=600 )
	static const bool computed ;
#else
	static const bool computed = false ;
#endif

	static BOOST_UBLAS_INLINE
	void apply (argument1_type t1, argument2_type t2)
	{
		t1 = t2;
	}

	template<class U1, class U2>
	struct rebind
	{
		typedef scalar_assign<U1, U2> other;
	};
};

template<>
struct scalar_multiplies_assign<blDiskStoragePolicyReference<double>, double>:
	public scalar_binary_assign_functor<blDiskStoragePolicyReference<double>, double>
{
	typedef blDiskStoragePolicyReference<double> T1;
	typedef double T2;
	typedef T1 argument1_type;
	//typedef typename scalar_binary_assign_functor<T1, T2>::argument1_type argument1_type;
	typedef scalar_binary_assign_functor<T1, T2>::argument2_type argument2_type;
	//typedef typename scalar_binary_assign_functor<T1, T2>::argument2_type argument2_type;
	static const bool computed = true;

	static BOOST_UBLAS_INLINE
	void apply (argument1_type t1, argument2_type t2)
	{
		t1 *= t2;
	}

	template<class U1, class U2>
	struct rebind
	{
		typedef scalar_multiplies_assign<U1, U2> other;
	};
};

}}}

#include "blStoragePolicy.txx"

#endif //blStoragePolicy
