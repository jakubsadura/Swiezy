/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreDataEntityImpl_H
#define _coreDataEntityImpl_H

#include "gmDataHandlingWin32Header.h"
#include "coreObject.h"
#include <string>
#include <vector>
#include <boost/any.hpp>
#include "blTagMap.h"

namespace Core{

//! Type used for real time measurements, expressed in seconds.
typedef double RealTime;

enum ImportMemoryManagementType 
{ 
	//! Copy the source memory to destination memory (like image buffer)
	gmCopyMemory, 
	//! Reuse the source data buffer and free it when necessary
	gmManageMemory, 
	//! Reuse the source data buffer and never free it
	gmReferenceMemory 
};

/**
Stores the processing data at a certain time step. 
It is mutable (so that one client can change the Processing 
Data instance of another client).

\author Xavi Planes
\date 07 sept 2010
\ingroup gmDataHandling
*/
class GMDATAHANDLING_EXPORT DataEntityImpl : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerTypesMacro(DataEntityImpl,Core::SmartPointerObject) 
	coreClassNameMacro(DataEntityImpl)

	enum TimeTypes { CONSTANT_IN_TIME = -1 };


public:
	typedef std::vector<double> Point3D;
	typedef std::vector<int> SurfaceElement3D;

	typedef std::map<std::string, DataEntityImpl::Pointer > DataEntityImplMapType;

	typedef std::vector<DataEntityImpl::Pointer> DataEntityImplSetType;

public:
	//@{ 
	/// \name Interface for Multiple data

	//!
	virtual DataEntityImpl::Pointer GetAt( size_t num );

	//!
	virtual bool SetAt( 
		size_t pos, 
		DataEntityImpl::Pointer data, 
		ImportMemoryManagementType mem = gmCopyMemory );

	//!
	virtual size_t GetSize() const;

	//!
	virtual void SetSize(size_t size, DataEntityImpl::Pointer data = NULL ) const;

	//@}

	/** Set internal pointer
	\return false if type of val is not correct
	*/
	virtual bool SetDataPtr( boost::any val );

	//! Get internal pointer
	virtual boost::any GetDataPtr() const = 0;

	//!
	virtual void* GetVoidPtr( ) const;

	//!
	virtual void SetVoidPtr( void* );

	//! Remove all internal data
	virtual void ResetData( ) = 0;

	//! Return true if val is valid type to call SetDataPtr( )
	virtual bool IsValidType( const std::type_info &type );

	/** Copy the processing data val to internal pointer val
	\return false if type of val is not correct
	*/
	virtual bool DeepCopy( boost::any val, ImportMemoryManagementType mem = gmCopyMemory );

	/** Copy all the data
	\return false if type of val is not correct
	*/
	virtual bool DeepCopy( DataEntityImpl::Pointer input, ImportMemoryManagementType mem = gmCopyMemory );

	//! Returns true if the processing data has no real time point, but is constant in time.
	bool IsConstantInTime() const;

	//!
	Core::RealTime GetRealTime() const;
	void SetRealTime(Core::RealTime val);

	//!
	bool GetGenerateTemporalData() const;
	void SetGenerateTemporalData(bool val);

	//!
	virtual void CleanTemporalData( );

	/** Set all the data pointers
	\return false if data cannot be set
	*/
	virtual bool SetData( blTagMap::Pointer tagMap, ImportMemoryManagementType mem = gmCopyMemory );

	/** Get all the data pointers
	\return false if data cannot be retrieved
	*/
	virtual bool GetData( blTagMap::Pointer tagMap );

private:

	//! Cast any data and replace current internal data pointer
	virtual void SetAnyData( boost::any val ) = 0;

protected:
	//!
	DataEntityImpl( );

protected:
	//! The time associated with this data
	RealTime m_RealTime;

	//! Map of factories
	static DataEntityImplMapType m_DataEntityImplMap;

	//! Generate temporal data when calling GetData( )
	bool m_GenerateTemporalData;
};

} // end namespace Core

#endif // _coreDataEntityImpl_H

