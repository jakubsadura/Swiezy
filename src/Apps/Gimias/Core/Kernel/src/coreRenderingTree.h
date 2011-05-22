/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef coreRenderingTree_H
#define coreRenderingTree_H

#include "gmKernelWin32Header.h"
#include "coreObject.h"
#include "coreDataEntity.h"
#include <boost/signals.hpp>
#include <map>

namespace Core
{
/**
\brief Holds all DataEntities rendered in a render window and their specific
rendering properties.

The RenderingTree also is involved in some of the behavior of the 
application, being the outer layer that provides application-wise 
behavior over the real internal tree (a mitk::DataTree). Namely, 
it provides convenience and helper functions fro working with the 
scene, and it will notify to its observers when it has been modified 
somehow, so they can update as well.

Each GIMIAS plug-in holds one or several rendering trees. Each node in 
the rendering tree is used for rendering an object. Each rendering tree 
node holds a smart pointer to a Data Entity, and contains the properties 
for rendering that Data Entity (there are a few exceptions to this rule though). 
The phrase "attaching a Data Entity to a rendering tree node" means that 
the rendering node will be used to "hold" and render this Data Entity.
Note that the rendering tree is not used for processing. 

\sa Core::DataEntity

\ingroup gmKernel
\author Juan Antonio Moya
\date 30 Jan 2008
*/
class GMKERNEL_EXPORT RenderingTree : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerTypesMacro(RenderingTree,Core::SmartPointerObject)
	coreClassNameMacro(RenderingTree)

	// Types for signals and slots
	typedef boost::signal0<void> TreeModifiedSignal;
	typedef TreeModifiedSignal::slot_function_type TreeModifiedSlot;

	/**
	\brief Adds the data entity to the tree as child of the root node. 
	
	A successful call triggers a notification OnModify to the 
	RenderingTree observers.
	\return The rendering node

	\note the "visible" property of the node will be changed to bShow
	\note Throws an exception if there's an error.
	\note If the dataEntity was in the rendering tree, This will not be 
	added again.
	The data of the node will be changed and the "visible" property will be
	set to bShow.
	*/
	virtual boost::any Add(
		DataEntity::Pointer dataEntity,
		bool bShow = true,
		bool bInitializeViews = true ) = 0;

	//!
	virtual bool Remove(DataEntity* dataEntity, bool bInitializeViews = true );
	//!
	virtual bool Remove(unsigned int dataEntityId, bool bInitializeViews = true) = 0;
	//! Remove all DataEntity nodes
	virtual void RemoveAll() = 0;

	//! Returns rendering node for \a dataEntity
	virtual boost::any GetNode(Core::DataEntity::Pointer dataEntity) = 0;
	//! Returns rendering node for \a dataEntity
	virtual boost::any GetNode(unsigned int dataEntityId) = 0;

	//! Count number of data entities
	virtual long GetNumberOfDataEntities( ) = 0;

	//!
	virtual std::vector<DataEntity::Pointer> GetAllDataEntities( ) = 0;

	//! Show or hide a data entity
	virtual void Show( DataEntity::Pointer dataEntity, bool bShow, bool initializeViews = true ) = 0;

	//! Indicates whether a data entity is in the rendering tree or not
	virtual bool IsDataEntityRendered(Core::DataEntity* dataEntity) = 0;

	//! Indicates whether a data entity is rendered
	virtual bool IsDataEntityShown(Core::DataEntity* dataEntity) = 0;

	//!
	virtual std::string GetName() const;
	virtual void SetName(std::string val);

	/** Templated subject-observer methods (using boost signals). 
	Include coreRenderingTree.txx
	*/
	template <class T> boost::signals::connection AddObserverOnModifiedAnyNode(
		T* observer, 
		void (T::*slotFunction)(void));

	//!
	virtual void UpdateMetadata( Core::DataEntity::Pointer dataEntity );

	//!
	virtual void UpdateRenderingProperties( Core::DataEntity::Pointer dataEntity );

	/** Modify rendering property of a data entity
	If the data entity is not rendered, set the property to the Metadata
	for later use
	*/
	virtual void SetProperty( Core::DataEntity::Pointer, blTag::Pointer property ) = 0;

	//!
	virtual blTag::Pointer GetProperty( Core::DataEntity::Pointer dataEntity, const std::string& name ) = 0;

protected:

	struct ConnectionInstance
	{
		ConnectionInstance(void) : refCount(0) {};
		friend class RenderingTree;	
		boost::signals::connection m_Connection;
		unsigned int refCount;
	};

	typedef std::map<unsigned int, ConnectionInstance> 
		OnDestroyDataEntityConnections;
	//!
	void OnDestroyDataEntity(Core::DataEntity* dataEntity);
	//!
	void ConnectToOnDestroySignal(Core::DataEntity::Pointer dataEntity);
	//!
	void DisconnectOnDestroyedSignal(unsigned int dataEntityId);
	//!
	void ConnectToOnModifiedSignal(Core::DataEntity::Pointer dataEntity);
	//!
	void DisconnectOnModifiedSignal(Core::DataEntity* dataEntity);
	//!
	virtual void ConnectToOnModifiedAnyNodeSignal(Core::DataEntity::Pointer dataEntity);
	//!
	virtual void DisconnectToOnModifiedAnyNodeSignal(Core::DataEntity::Pointer dataEntity);
	//!
	virtual void OnModifiedDataEntity(Core::SmartPointerObject* dataEntity) = 0;

	//!
	RenderingTree(void);
	virtual ~RenderingTree(void);

	coreDeclareNoCopyConstructors(RenderingTree);

protected:
	//!
	TreeModifiedSignal m_OnModifiedAnyNodeSignal;
	//!
	OnDestroyDataEntityConnections m_OnDestroyDataEntityConnections;
	//!
	std::string m_Name;
};

typedef DataHolder<RenderingTree::Pointer> RenderingTreeHolderType;

} // namespace Core

#include "coreRenderingTree.txx"

#endif

