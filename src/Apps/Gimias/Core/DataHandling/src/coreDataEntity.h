/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataEntity_H
#define coreDataEntity_H

#include "gmDataHandlingWin32Header.h"
#include "coreObject.h"
#include "coreDataEntityMetadata.h"
#include "coreDataEntityImpl.h"
#include "coreDataEntityImplFactory.h"
#include "coreDataEntityTypes.h"
#include <string>
#include <vector>
#include <typeinfo>
#include <boost/signals.hpp>
#include <boost/any.hpp>

namespace Core{

class RenDataBuilder;

typedef boost::any AnyProcessingData;

/**
This helper function tries to cast \a data to T, and store the result 
in \a result.
Returns true iff the cast could be made.

\ingroup gmDataHandling
\author Maarten Nieber
\date 15 sep 2008
*/

template< class T >
bool CastAnyProcessingData(const AnyProcessingData& data, T& result);

typedef size_t TimeStepIndex;

/** 
\brief A DataEntity is a class that represents "data" that can be processed 
by GIMIAS, such as an image, mesh, simulation result, etc.

DataEntity is the basic unit for data processing, mapping and interaction 
for the Core. Every image, mesh, etc going in and out of the Core should 
be cast to a DataEntity

Every DataEntity has a header with meta-info required to understand what 
it holds: Core::DataEntityMetadata. It has also a set of processing data 
objects, that holds the data in its original representation: Core::DataEntityImpl.

The DataEntity also holds a rendering representation (boost::any) so as to 
allow displaying and rendering the original data. The rendering data object 
holds a rendering representation of the original 
data object, called also processing data (the type used when processing 
it with algorithms). Therefore, whenever the original data is modified, 
a Builder must be called to convert that data into its rendering 
representation. Rendering Data Builders are responsible for converting 
original data into its rendering representation. 

DataEntity contains a set of TimeSteps. Each time step stores the data in 
a concrete time instant. The DataEntityImpl class follows the Bridge Pattern: 
The intent of Bridge pattern is to decouple an abstraction from its 
implementation so that the two can vary independently. This allows to 
switch the current implementation of a data object on the fly, depending 
on the used filter. Imagine that the filter1 uses vtkPolyData surface 
meshes and the filter2 uses Netgen surface meshes. When the filter1 is 
called, the data implementation will switch to vtkPolydata automatically. 
When the filter2 is called, the data implementation will be switched 
to Netgen automatically.

Each new data type should extend this hierarchy. A plugin, for example, 
can extend the possible surface mesh implementations supported by the 
framework. Imagine you have a new data type called NetgenMesh. To add 
this implementation to the framework, you need to create a subclass of 
SurfaceTimeStepImpl and register this new class into the framework. Each 
time a processor needs a NetgenMesh, the current surface mesh 
implementation will be automatically converted.

\author Juan A. Moya
\date 26 Mar 2008
\ingroup gmDataHandling
*/
class GMDATAHANDLING_EXPORT DataEntity : public Core::SmartPointerObject
{
public:

	typedef boost::signal1<void, Core::DataEntity*> DataEntityDestroyedSignal;

	coreDeclareSmartPointerTypesMacro(DataEntity, Core::SmartPointerObject)
	coreClassNameMacro(DataEntity)
	coreFactorylessNewMacro(DataEntity)
	coreFactorylessNewMacro1Param(DataEntity, DataEntityType)
	coreFactorylessNewMacro2Param(DataEntity, DataEntityType, ModalityType)

	//! Typedefef for keeping references to all the children of the data Entity
	typedef std::vector<Core::DataEntity*> ChildrenListType;

	int GetId() const;
	DataEntityType GetType() const;
	void SetType(DataEntityType type);
	//! Returns data entity cast to ImageDataEntity (if the modality is image) or NULL otherwise.
	bool IsImage() const;
	//! Convenience function.
	bool IsSurfaceMesh() const;
	//! Convenience function.
	bool IsVolumeMesh() const;
	//! Convenience Function
	bool IsSkeletonMesh() const;
	//! Convenience Function
	bool IsROI( ) const;
	//! Convenience Function
	bool IsMeasurement( ) const;
	//! Convenience Function
	bool IsPointSet( ) const;
	//! Convenience Function
	bool IsSignal() const;
	//! Convenience Function
	bool IsContour( ) const;
	//! Convenience Function
	bool IsNumericData() const;
	//! Convenience Function
	bool IsVectorField() const;
	//! Convenience Function
	bool IsTransform() const;
	//! Convenience Function
	bool IsMITKData() const;
	//! Convenience Function
	bool IsTensor() const;


	//!
	boost::any GetRenderingData( const std::type_info& type );
	//!
	void SetRenderingData(boost::any x);
	
	/** Templated GET/SET method. 
	\param [in] switchImplementation If type of T is not the type of the 
	internal pointer, it will try to change to another implementation 
	*/
	template <typename T> bool GetProcessingData(
		T& processingDataByRef, 
		TimeStepIndex timeStep = 0,
		bool switchImplementation = false);

	//!
	bool AddTimeStep(
		boost::any processingData, 
		RealTime realTime = DataEntityImpl::CONSTANT_IN_TIME);

	//!
	template <typename T> 
	bool AddTimeSteps( std::vector<T> &processingData);

	//!
	size_t GetNumberOfTimeSteps() const;

	//! Reset time steps to 0
	void ResetTimeSteps( );

	//! Resize to size time steps of type
	bool Resize( int size, const std::type_info &type );

	//!
	double GetTimeAtTimeStep(TimeStepIndex timeStep) const;

	//! set real time for every timestep
	void SetTimeAtTimeStep (TimeStepIndex timeStep, double timeVal);

	//! Find closest timestep to a real time 
	TimeStepIndex FindTimeStep( Core::RealTime time );

	//! set real time for every timestep
	void SetTimeForAllTimeSteps (std::vector <float>& timesteps);

	/** Replace timeStep with this new data and call UpdateRenderingRepresentation( )
	If the size if m_TimeSteps is <= timeStep -> throw an exception
	*/
	void SetTimeStep(
						boost::any processingDataObj, 
						TimeStepIndex timeStep = 0,
						RealTime realTime = DataEntityImpl::CONSTANT_IN_TIME );

	//! \note Return empty pointer if timeStep > GetTimeDimension( )
	boost::any GetProcessingData(TimeStepIndex timeStep = 0);

	/** 
	Because a DataEntity is referenced by a mitk::GenericProperty, 
	it has to implement the following members:
	- an operator<< so that the properties value can be put into a std::stringstream
    - an operator== so that two properties can be checked for equality
	
	Read the documentation of Core::DataEntityProperty, you can find 
	its definition at coreMitkProperties.h
	*/
	virtual std::string operator*(void);

	/** Templated subject-observer methods (using boost signals). 
	\note Remember to include coreDataEntity.txx
	*/
	template <class T> boost::signals::connection AddObserverOnDestroyDataEntity(
		T* observer, 
		void (T::*slotFunction)(Core::DataEntity*));
	
	//! Set The father of the Data Enity
	/*! A children data entity is a Data Entity that is associated to 
	another Data Entity, e.g. an ECG associated	to a image
	 \param father -[in] the Father Data Entity
	*/
	bool SetFather(DataEntity* father);

	//! Returns the father. If it's doesn't have a father, returns null
	Pointer GetFather();

	/** Returns the children list. 
	If the data entity doesn't have any child, the list is empty
	*/
	ChildrenListType GetChildrenList();

	//!
	Core::DataEntityMetadata::Pointer GetMetadata() const;

	//!
	Core::DataEntityImpl::Pointer GetTimeStep( int timeStepIndex = -1 );

	//!
	bool SwitchImplementation( const std::type_info &type );

	//!
	bool Copy( const DataEntity::Pointer& de, ImportMemoryManagementType mem = gmCopyMemory );

protected:
	DataEntity(DataEntityType type = UnknownTypeId, ModalityType modality = UnknownModality);
	virtual ~DataEntity();

private:
	coreDeclareNoCopyConstructors(DataEntity);

	//! Add a data entity to the list of children
	void RegisterChild(Core::DataEntity* child);

	//! Removes the child that has the coreDataEntityId from the list. 
	/*! Returns true if success,false otherwise
	\param coreDataEntityId -[in] The Id of the child dataEntity to be removed
	*/
	bool RemoveChildFromList(int coreDataEntityId);

	std::list<boost::any>::iterator FindRenderingData( const std::type_info& type );

private:
	virtual void NotifyObserversOfDestruction(void) const;

	//! This a Core TAG that cannot be modified and will be created by Core
	DataEntityType m_Type;
	
	//! Id: This is a Core TAG that cannot be modified and will be created by Core
	unsigned int m_Id;

	//!
	DataEntityImpl::Pointer m_DataEntityImpl;

	/** \todo Use DataEntityUmpl instead of boost::any
	Different implementations of rendering data
	*/
	std::list<boost::any> m_RenderingDataList;

	// SignalTypeId handling
	DataEntityDestroyedSignal m_OnDestroyedSignal;

	// ID count of already created DataEntities
	static unsigned int dataEntityIdCount;

	//! A list containing all the children of a data entity
	ChildrenListType m_ChildrenList;

	//! The Father of the Data Entity. Each data Entity can have only one father
	DataEntity* m_Father;

	//!
	Core::DataEntityMetadata::Pointer m_Metadata;

};

} // end namespace Core

#include "coreDataEntity.txx"
#include "coreDataEntityHolder.h"

#endif

