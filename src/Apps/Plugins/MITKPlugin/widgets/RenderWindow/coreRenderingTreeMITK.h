/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef coreRenderingTreeMITK_H
#define coreRenderingTreeMITK_H

#include "gmWidgetsWin32Header.h"
#include "coreRenderingTree.h"
#include "mitkDataStorage.h"
#include "blMITKUtils.h"

namespace Core
{
/**
\brief Specialization of RenderingTree for MITK

\ingroup gmKernel
\author Xavi Planes
\date Oct 2010
*/
class PLUGIN_EXPORT RenderingTreeMITK : public Core::RenderingTree
{
public:
	coreDeclareSmartPointerClassMacro(RenderingTreeMITK, Core::RenderingTree);
	
	//! Redefined
	boost::any Add( DataEntity::Pointer dataEntity, bool bShow = true, bool bInitializeViews = true );
	//! Redefined
	bool Remove(unsigned int dataEntityId, bool bInitializeViews = true);
	//! Redefined
	void RemoveAll();
	//! Redefined
	long GetNumberOfDataEntities( );
	//! Redefined
	boost::any GetNode(Core::DataEntity::Pointer dataEntity);
	//! Redefined
	boost::any GetNode(unsigned int dataEntityId);
	//! Redefined
	std::vector<DataEntity::Pointer> GetAllDataEntities( );
	//! Redefined
	void Show( DataEntity::Pointer dataEntity, bool bShow, bool initializeViews = true );
	//! Redefined
	bool IsDataEntityRendered(Core::DataEntity* dataEntity);
	//! Redefined
	bool IsDataEntityShown(Core::DataEntity* dataEntity);
	//! Redefined
	void UpdateMetadata( Core::DataEntity::Pointer dataEntity );
	//! Redefined
	void UpdateRenderingProperties( Core::DataEntity::Pointer dataEntity );
	//! Redefined
	void SetProperty( Core::DataEntity::Pointer, blTag::Pointer property );
	//! Redefined
	blTag::Pointer GetProperty( Core::DataEntity::Pointer dataEntity, const std::string& name );

	//!
	mitk::DataStorage::Pointer GetDataStorage() const;
	void SetDataStorage(mitk::DataStorage::Pointer tree);

	/**
	\brief Update the node in the tree . 
	Set active scalars and get point data (or get cell data) 
	to render it through ApplyLookUpTable
	*/
	void UpdateNode(mitk::DataTreeNode::Pointer node,
		blMITKUtils::LUT_SCALAR_MODE scalarMode=blMITKUtils::LUT_SCALAR_MODE_NONE);

	//!
	mitk::DataStorage::SetOfObjects::ConstPointer GetAllDataEntityNodes( );

	//! Get the data entity from a node
	static Core::DataEntity* GetDataEntity( mitk::DataTreeNode::Pointer node );
	//!
	bool HasNode(mitk::DataTreeNode::Pointer node);
	//!
	mitk::DataTreeNode::Pointer GetRootNode(void);
	//!
	bool Remove(mitk::DataTreeNode::Pointer node, bool bInitializeViews = true);
	//!
	bool HasDataEntityOfType(Core::DataEntityType type);

protected:

	//!
	RenderingTreeMITK(void);
	virtual ~RenderingTreeMITK(void);

	//!
	void OnModifiedDataEntity(Core::SmartPointerObject* dataEntity);

	//!
	void ConnectToOnModifiedAnyNodeSignal(Core::DataEntity::Pointer dataEntity);

	//!
	void DisconnectToOnModifiedAnyNodeSignal(Core::DataEntity::Pointer dataEntity);

	//!
	mitk::DataTreeNode::Pointer CreateRenderingNodeWithDefaultProperties(
		Core::DataEntity::Pointer dataEntity);
	//!
	void SetVolumeImageNodeDefaultProperties( 
		mitk::DataTreeNode::Pointer newNode );

	//!
	void SetSurfaceMeshNodeDefaultProperties( 
		mitk::DataTreeNode::Pointer newNode );

	//!
	void SetContourNodeDefaultProperties( 
		mitk::DataTreeNode::Pointer newNode );

	//!
	void SetPointSetNodeDefaultProperties( 
		mitk::DataTreeNode::Pointer newNode);

	//!
	void SetMeasurementNodeDefaultProperties( 
		mitk::DataTreeNode::Pointer newNode );

	//!
	void SetVolumeMeshNodeDefaultProperties( 
		mitk::DataTreeNode::Pointer newNode );

	//!
	void SetROINodeDefaultProperties( 
		mitk::DataTreeNode::Pointer newNode );


	//! Apply LUT to the node
	void ApplyLUTToFirstScalarsVector( mitk::DataTreeNode::Pointer node,
		blMITKUtils::LUT_SCALAR_MODE scalarMode=blMITKUtils::LUT_SCALAR_MODE_NONE );

	//! compute size of the point
	double ComputeSizeOfPoint( mitk::DataTreeNode::Pointer node);

	//!
	void OnModifiedRenderingTreeNode( );

	coreDeclareNoCopyConstructors(RenderingTreeMITK);

	//!
	mitk::BaseProperty::Pointer ConvertToMITK( blTag::Pointer tag );

	//!
	blTag::Pointer ConvertFromMITK( const char* propertyKey, mitk::BaseProperty* prop );

private:
	//!
	mitk::DataStorage::Pointer m_DataStorage;

	//! Map Core::DataEntity ID with observer of property node TAG
	std::map<mitk::DataTreeNode::Pointer, unsigned long> m_MapObserversNode;
};

} // namespace Core

#endif

