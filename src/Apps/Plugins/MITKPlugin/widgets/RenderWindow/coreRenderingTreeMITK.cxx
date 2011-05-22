/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreRenderingTreeMITK.h"
#include "coreAssert.h"
#include "coreReportExceptionMacros.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityInfoHelper.h"
#include "coreEnvironment.h"
#include "coreLogger.h"
#include "coreKernel.h"
#include "coreDataTreeMITKHelper.h"
#include "coreMitkProperties.h"
#include "coreRenDataFactory.h"
#include "coreVTKImageDataHolder.h"

#include "mitkRenderingManager.h"
#include "mitkDataStorage.h"
#include "mitkPointSet.h"
#include "mitkProperties.h"
#include "mitkStringProperty.h"
#include "mitkVtkResliceInterpolationProperty.h"
#include "mitkLevelWindowProperty.h"
#include "mitkMaterialProperty.h"
#include "mitkDataTreeHelper.h"
#include "mitkTransferFunctionProperty.h"
#include "mitkRenderingManager.h"
#include "mitkStandaloneDataStorage.h"
#include "mitkNodePredicateProperty.h"
#include "mitkUnstructuredGridVtkMapper3D.h"
#include "mitkPlanarPolygon.h"

#include "mitkStringVectorProperty.h"

#include "blMITKUtils.h"


#include <sstream>

using namespace Core;

//!
RenderingTreeMITK::RenderingTreeMITK(void)
{
	typedef mitk::GenericProperty<boost::any> DataEntityProperty;
	// Set the internal-tree
	this->m_DataStorage = mitk::StandaloneDataStorage::New();

	// Setup the defaults of the tree
	mitk::DataTreeNode::Pointer node;
	std::string nodeName = "<root>";
	
	try
	{
		// Each rendering tree should have one single root element, that holds an empty Data Entity.
		node = mitk::DataTreeNode::New();
		node->SetProperty("name", mitk::StringProperty::New(nodeName));
		node->SetProperty("renderingId", mitk::IntProperty::New(0));
		node->SetProperty("helper object", mitk::BoolProperty::New(true));
		node->SetProperty("dataEntity", Core::DataEntityProperty::New(NULL));
		m_DataStorage->Add( node );
	}
	coreCatchExceptionsAddTraceAndThrowMacro(RenderingTreeMITK::RenderingTreeMITK);
}

//!
RenderingTreeMITK::~RenderingTreeMITK(void)
{
	// The mitk::DataTree is still in memory because there are some references
	// from mitk::DataStorage (the reference count is not 0 here)
	// Some rendering data has a reference to the processing data of the 
	// DataEntity. We need to remove all references before destroying the 
	// DataEntities.
	RemoveAll( );

	// Remove the reference from the data storage
	m_DataStorage = NULL;
}

/** 
Sets the mitk tree internal representation to be held by the RenderingTreeMITK
*/
void RenderingTreeMITK::SetDataStorage(mitk::DataStorage::Pointer dataStorage)
{ 
	this->m_DataStorage = dataStorage;
}

/**
Returns true if the tree contains a node representing a data entity of a given type.
*/
bool RenderingTreeMITK::HasDataEntityOfType(Core::DataEntityType type)
{
	mitk::NodePredicateProperty::Pointer predicate;
	predicate = mitk::NodePredicateProperty::New("dataType", mitk::IntProperty::New(type));
	mitk::DataTreeNode::Pointer node;
	node = m_DataStorage->GetNode( predicate );
	bool found = node.IsNotNull();

	return found;
}

/**
*/
boost::any RenderingTreeMITK::Add(
					DataEntity::Pointer dataEntity,
					bool bShow /*= true*/,
					bool bInitializeViews /*= true*/ )
{
	if ( dataEntity.IsNull() )
	{
		throw Core::Exceptions::Exception( "RenderingTreeMITK::Add", 
			"The requested DataEntity cannot be NULL, if you plan to add it to the RenderingTreeMITK" );
	}
	coreAssertMacro(this->m_DataStorage.IsNotNull() 
		&& "The RenderingTreeMITK internal structures must be valid and initialized");
	mitk::DataTreeNode::Pointer node;
	try
	{
		Core::CastAnyProcessingData( GetNode( dataEntity ), node );
		bool bDataIsInRenderingTree = node.IsNotNull();

		// Create the node and add it to the rendering tree
		if ( !bDataIsInRenderingTree )
		{
			node = CreateRenderingNodeWithDefaultProperties(dataEntity);
			if(node.IsNotNull())
			{
				node->SetVisibility( bShow );

				// Get parent node
				DataEntity::Pointer parentDataEntity = dataEntity->GetFather();
				mitk::DataTreeNode::Pointer parentNode;
				Core::CastAnyProcessingData( GetNode( dataEntity ), parentNode );

				// Connect signal before adding the node, to avoid calling the factory twice
				ConnectToOnModifiedSignal(dataEntity);

				m_DataStorage->Add( node, parentNode );

				ConnectToOnModifiedAnyNodeSignal(dataEntity);

				UpdateRenderingProperties( dataEntity );

				// Add the connections so as to enable removing the nodes from the tree 
				// when the dataentity is destroyed
				this->ConnectToOnDestroySignal(dataEntity);

				// Notify that the tree has been modified (tree browser or MultiRenderWindow)
				this->Modified();
			}
		}
		else
		{
			// Build rendering data
			Core::RenDataFactory::Pointer builder = Core::RenDataFactory::New( );
			builder->SetInputDataEntity( 0, dataEntity );
			builder->Update();

			// Update rendering node with the new rendering data
			mitk::BaseData::Pointer mitkData;
			boost::any anyData = dataEntity->GetRenderingData( typeid( mitkData ) );
			Core::CastAnyProcessingData( anyData, mitkData );
			node->SetData( mitkData );

			node->SetVisibility( bShow );

			mitk::RenderingManager::GetInstance()->ForceImmediateUpdateAll();
		}

	}
	coreCatchExceptionsAddTraceAndThrowMacro(RenderingTreeMITK::Add)

	return node;
}

void Core::RenderingTreeMITK::RemoveAll()
{
	m_DataStorage->Remove( m_DataStorage->GetAll() );
}

/**
Removes the given rendering node. 
This function will kill the rendering node and all its subtree, and then notify its observers

\param the rendering node to be removed.
*/
bool RenderingTreeMITK::Remove(mitk::DataTreeNode::Pointer node, bool bInitializeViews /*= true*/)
{
	if ( node.IsNull() )
	{
		throw Core::Exceptions::Exception( "RenderingTreeMITK::Remove", 
			"The requested rendering node cannot be NULL, so as to removing it from the RenderingTreeMITK" );
	}

	Core::DataEntity* nodeDataEntity = NULL;

	bool wasRemoved = false;
	try
	{
		nodeDataEntity = GetDataEntity( node );

		if(nodeDataEntity != NULL)
		{
			this->DisconnectOnDestroyedSignal(nodeDataEntity->GetId());
			this->DisconnectOnModifiedSignal(nodeDataEntity);
			this->DisconnectToOnModifiedAnyNodeSignal(nodeDataEntity);
		}
		
		m_DataStorage->Remove( node );

		this->Modified();
			
	}
	coreCatchExceptionsAddTraceAndThrowMacro(RenderingTreeMITK::Remove)
	return wasRemoved;
}

/**
Removes all the rendering nodes mapped to the data entity, given just its id. If there were removed successfully,
it also casts a signal so observers could notice the event. This function will
kill the implied rendering nodes and all its subtrees.

\param dataEntityId is the id of the DataEntity to be removed
\sa DataEntity
*/
bool RenderingTreeMITK::Remove(unsigned int dataEntityId, bool bInitializeViews /*= true*/)
{			
	bool removedAny = false;

	try
	{
		mitk::NodePredicateProperty::Pointer predicate;
		predicate = mitk::NodePredicateProperty::New(
			"dataEntityID",
			mitk::IntProperty::New(dataEntityId));

		mitk::DataTreeNode::Pointer node;
		node = m_DataStorage->GetNode( predicate );
		if ( node.IsNotNull() )
		{
			m_DataStorage->Remove( node );

			this->DisconnectOnDestroyedSignal(dataEntityId);
			this->DisconnectOnModifiedSignal(GetDataEntity( node ));
			removedAny = true;

			this->Modified();
		}
	}
	coreCatchExceptionsAddTraceAndThrowMacro(RenderingTreeMITK::Remove)

	return removedAny;
}

/**
Returns true if the given node is being held by this RenderingTreeMITK
*/
bool RenderingTreeMITK::HasNode(mitk::DataTreeNode::Pointer node)
{
	return m_DataStorage->Exists( node );
}

/**
Returns the root rendering node of this RenderingTreeMITK
*/
mitk::DataTreeNode::Pointer RenderingTreeMITK::GetRootNode(void)
{
	mitk::NodePredicateProperty::Pointer predicate;
	predicate = mitk::NodePredicateProperty::New(
		"name",
		mitk::StringProperty::New("<root>"));

	mitk::DataTreeNode::Pointer node;
	node = m_DataStorage->GetNode( predicate );

	return node;
}

/**
 */
mitk::DataStorage::Pointer Core::RenderingTreeMITK::GetDataStorage() const
{
	return m_DataStorage;
}

long Core::RenderingTreeMITK::GetNumberOfDataEntities()
{
	mitk::DataStorage::SetOfObjects::ConstPointer set;
	set = m_DataStorage->GetSubset( mitk::NodePredicateProperty::New("dataEntity") );
	return set->size( );
}


/**
Creates and returns a rendering tree node with the default rendering properties and links to the given data entity.
The properties are assigned differently depending on the data entity type.
*/
mitk::DataTreeNode::Pointer RenderingTreeMITK::CreateRenderingNodeWithDefaultProperties(
	Core::DataEntity::Pointer dataEntity)
{
	mitk::DataTreeNode::Pointer newNode;
	try
	{
		// This is the tree to work with, so attach here the DataEntity
		newNode = mitk::DataTreeNode::New();

		// We need to set the window level of the ROI mask image to a default fixed
		// value. Otherwise, it is set to Auto and then, when we call 
		// SetData( ), the default properties of mitk::ImageMapper2D::SetDefaultProperties()
		// will set it to Auto and reallocate the buffer to be continuous
		// using the Channel buffer data and the Volume pointint to this buffer
		if ( dataEntity->IsROI() )
		{
			newNode->SetProperty( "levelwindow", mitk::LevelWindowProperty::New( mitk::LevelWindow(8.0, 16) ) );
			newNode->AddProperty( "dontBinarize", mitk::BoolProperty::New( true), NULL, true ) ;
		}

		// Build rendering data
		Core::RenDataFactory::Pointer factory = Core::RenDataFactory::New( );
		factory->SetInputDataEntity( 0, dataEntity );
		factory->Update();

		mitk::BaseData::Pointer mitkData;
		boost::any anyData = dataEntity->GetRenderingData( typeid( mitkData ) );
		Core::CastAnyProcessingData( anyData, mitkData );
		if ( mitkData.IsNull() )
		{
			// no need to add a node
			return NULL;
		}
		newNode->SetData( mitkData );

		// link the node with the DataEntity
		std::ostringstream name;
		name << Core::DataEntityInfoHelper::GetShortDescription(dataEntity);
		newNode->SetProperty("name", mitk::StringProperty::New(name.str()));
		newNode->SetVisibility( true );
		newNode->SetProperty("dataType", mitk::IntProperty::New( dataEntity->GetType() ) );
		newNode->SetProperty("dataEntityID", mitk::IntProperty::New( dataEntity->GetId() ) );
		
		// assign the dataEntity as a property, so as to to help find it later
		newNode->SetProperty("dataEntity", 
			Core::DataEntityProperty::New(dataEntity.GetPointer()));

		// Volume image
		if ( dataEntity.IsNotNull() )
		{ 
			// First the child types
			if ( dataEntity->IsROI() )
			{
				SetROINodeDefaultProperties( newNode );
			}
			else if ( dataEntity->IsSkeletonMesh() )
			{
				SetSurfaceMeshNodeDefaultProperties( newNode );
			}
			else if ( dataEntity->IsMeasurement() )
			{
				SetMeasurementNodeDefaultProperties( newNode );
			}
			else if( dataEntity->IsImage() )
			{
				SetVolumeImageNodeDefaultProperties( newNode );
			}
			else if ( dataEntity->IsSurfaceMesh() )
			{
				SetSurfaceMeshNodeDefaultProperties( newNode );
			}
			else if ( dataEntity->IsVolumeMesh() )
			{
				SetVolumeMeshNodeDefaultProperties( newNode );
			}
			else if ( dataEntity->IsSignal() )
			{
				// Don't include in the bounding box
				newNode->SetProperty("includeInBoundingBox", mitk::BoolProperty::New(false));
			}
			else if ( dataEntity->IsTransform() )
			{
				// Don't include in the bounding box
				newNode->SetProperty("includeInBoundingBox", mitk::BoolProperty::New(false));
			}
			else if ( dataEntity->IsPointSet( ) )
			{
				SetPointSetNodeDefaultProperties( newNode );
			}
			else if ( dataEntity->IsContour( ) )
			{
				SetContourNodeDefaultProperties( newNode );
			}
			else if ( dataEntity->IsTensor( ) )
			{
				SetVolumeImageNodeDefaultProperties( newNode );
			}
		}
	}
	coreCatchExceptionsAddTraceAndThrowMacro(RenderingTreeMITK::CreateRenderingNodeWithDefaultProperties);
	return newNode;
}


void Core::RenderingTreeMITK::SetContourNodeDefaultProperties( 
	mitk::DataTreeNode::Pointer newNode )
{
	newNode->SetColor( 0, 1, 0 );
	newNode->SetOpacity( 1 );
	newNode->SetProperty( "Width", mitk::FloatProperty::New(3) );
}


void Core::RenderingTreeMITK::SetVolumeImageNodeDefaultProperties( mitk::DataTreeNode::Pointer newNode )
{
	newNode->SetProperty("volumerendering", mitk::BoolProperty::New(false));
	newNode->SetProperty( "opacity", mitk::FloatProperty::New(1.0f));
	newNode->SetProperty( "color", mitk::ColorProperty::New(1.0,1.0,1.0));
	newNode->SetProperty("use color", mitk::BoolProperty::New(false));
	newNode->SetProperty("texture interpolation", mitk::BoolProperty::New(true));
	newNode->SetProperty("reslice interpolation", 
		mitk::VtkResliceInterpolationProperty::New( VTK_RESLICE_NEAREST ) );
	newNode->SetProperty("layer", mitk::IntProperty::New(0));
	newNode->SetProperty("in plane resample extent by geometry", mitk::BoolProperty::New(false));

	// Default LUT
	blMITKUtils::ApplyLookupTable( 
		true, 
		newNode, 
		blLookupTables::LUT_TYPE_MITK_GRAY,
		blShapeUtils::ShapeUtils::VTKScalarType( ) );

	Core::DataTreeMITKHelper::ApplyLookupTableToNMImage(newNode);	
}

void Core::RenderingTreeMITK::SetPointSetNodeDefaultProperties( 
	mitk::DataTreeNode::Pointer newNode )
{
	mitk::StringVectorProperty::Pointer labelVector;
	labelVector = mitk::StringVectorProperty::New( );
	newNode->SetProperty( "label", labelVector );
	newNode->SetProperty( "show label", mitk::BoolProperty::New( true ) );

	double size;
	size = ComputeSizeOfPoint(newNode);
	//resize points in the special case of a pointset child of a surface
	if (size > 0)
		newNode->SetProperty("pointsize", mitk::FloatProperty::New(size));	

	Core::DataTreeMITKHelper::UpdateRenderingNodeLabels( newNode );

}

void Core::RenderingTreeMITK::SetMeasurementNodeDefaultProperties( 
	mitk::DataTreeNode::Pointer newNode )
{
	newNode->SetProperty("show contour", mitk::BoolProperty::New(true));
	newNode->SetProperty("name", mitk::StringProperty::New("distance"));
	newNode->SetProperty("show distances",mitk::BoolProperty::New(true));

	Core::DataEntity::Pointer dataEntity = GetDataEntity( newNode );
	if ( dataEntity.IsNull( ) )
	{
		return;
	}

	//Additional properties for father images
	Core::DataEntity::Pointer father = dataEntity->GetFather();
	if ( father.IsNull() ||
		 father->GetType() != Core::ImageTypeId)
	{
		return;
	}

	vtkImageDataPtr inputImage;
	father->GetProcessingData( inputImage );
	if ( inputImage == NULL )
	{
		return;
	}

	//spacing in mm!
	double pointSize;
	double s1 = inputImage->GetSpacing()[0];
	double s2 = inputImage->GetSpacing()[1];
	double s3 = inputImage->GetSpacing()[2];
	s1>s2 ? pointSize  = s1 : pointSize = s2;
	pointSize>s3 ? pointSize  : pointSize = s3;
	newNode->ReplaceProperty("pointsize", mitk::FloatProperty::New( pointSize ) );
	newNode->ReplaceProperty("contoursize", mitk::FloatProperty::New(pointSize/2));
}

void Core::RenderingTreeMITK::SetSurfaceMeshNodeDefaultProperties( 
	mitk::DataTreeNode::Pointer newNode )
{
	// Set the material properties if it was a mesh
	mitk::MaterialProperty::Pointer materialProperty;
	materialProperty = mitk::MaterialProperty::New( newNode );
	// Default properties of ParaView:
	materialProperty->SetAmbientColor( 1, 1, 1 );
	materialProperty->SetAmbientCoefficient( 0 );
	materialProperty->SetDiffuseColor( 1, 1, 1 );
	materialProperty->SetDiffuseCoefficient( 1 );
	materialProperty->SetSpecularColor( 1, 1, 1 );
	materialProperty->SetSpecularCoefficient( 0.10 );
	materialProperty->SetSpecularPower( 100 );
	materialProperty->SetOpacity( 1 );
	materialProperty->SetInterpolation( mitk::MaterialProperty::Gouraud );
	newNode->SetProperty("material", materialProperty );

	// Get processing data as vtkPolyData
	ApplyLUTToFirstScalarsVector( newNode );	
}

void Core::RenderingTreeMITK::SetVolumeMeshNodeDefaultProperties( mitk::DataTreeNode::Pointer newNode )
{
	// Set the material properties if it was a mesh
		// add a default transfer function
		mitk::TransferFunction::Pointer tf = mitk::TransferFunction::New();
		float m_Min =1; //image->GetScalarValueMin();
		float m_Max =2; //image->GetScalarValueMax();
		tf->GetScalarOpacityFunction()->Initialize();
		tf->GetScalarOpacityFunction()->AddPoint ( m_Min, 0 );
		tf->GetScalarOpacityFunction()->AddPoint ( m_Max, 1 );
		tf->GetColorTransferFunction()->AddRGBPoint(m_Min,1,0,0);
		tf->GetColorTransferFunction()->AddRGBPoint(m_Max,1,1,0);
		tf->GetGradientOpacityFunction()->Initialize();
		tf->GetGradientOpacityFunction()->AddPoint(m_Min,1.0);
		tf->GetGradientOpacityFunction()->AddPoint(0.0,1.0);
		tf->GetGradientOpacityFunction()->AddPoint((m_Max*0.25),1.0);
		tf->GetGradientOpacityFunction()->AddPoint(m_Max,1.0);  
		newNode->SetProperty ( "TransferFunction", mitk::TransferFunctionProperty::New( tf.GetPointer() ) );
		
		//code sent by mitk users list
		//mitk::UnstructuredGridVtkMapper3D::SetProperties( newNode );
		newNode->SetProperty ("outline polygons", mitk::BoolProperty::New(true));
		mitk::MaterialProperty::Pointer materialProperty;
		materialProperty = mitk::MaterialProperty::New(1.0f, 1.0f, 1.0f, 1.0f, newNode);
		newNode->SetProperty("material", materialProperty );
		// Get processing data as vtkPolyData
		ApplyLUTToFirstScalarsVector( newNode );
}

void Core::RenderingTreeMITK::SetROINodeDefaultProperties( 
	mitk::DataTreeNode::Pointer newNode )
{
	blMITKUtils::ApplyMaskImageProperties(newNode);
}

void Core::RenderingTreeMITK::ApplyLUTToFirstScalarsVector( mitk::DataTreeNode::Pointer node,
													   blMITKUtils::LUT_SCALAR_MODE scalarMode/*=blMITKUtils::LUT_SCALAR_MODE_NONE*/)
{
	Core::DataEntity::Pointer dataEntity;
	dataEntity = Core::RenderingTreeMITK::GetDataEntity( node );
	
	if( scalarMode == blMITKUtils::LUT_SCALAR_MODE_NONE)
	{
		// Set active scalars to the first one for each time step if none is set
		//blMITKUtils::LUT_SCALAR_MODE scalarMode;
		scalarMode = Core::DataTreeMITKHelper::SetDefaultActiveScalars( dataEntity );
	}

	// Apply LUT
	Core::DataTreeMITKHelper::ApplyLookupTableToMesh( node, scalarMode );
}

void RenderingTreeMITK::UpdateNode(mitk::DataTreeNode::Pointer node,
							   blMITKUtils::LUT_SCALAR_MODE scalarMode/*=blMITKUtils::LUT_SCALAR_MODE_NONE*/)
{
	if ( node.IsNull() )
	{
		throw Core::Exceptions::Exception( "RenderingTreeMITK::Remove", 
			"The requested rendering node cannot be NULL, so as to removing it from the RenderingTreeMITK" );
	}

	try
	{
		ApplyLUTToFirstScalarsVector( node,scalarMode );
	}
	coreCatchExceptionsAddTraceAndThrowMacro(RenderingTreeMITK::UpdateNode)
}

void Core::RenderingTreeMITK::Show( 
	DataEntity::Pointer dataEntity, bool bShow, bool initializeViews /*= true*/ )
{
	try
	{
		mitk::DataTreeNode::Pointer node;
		Core::CastAnyProcessingData( GetNode( dataEntity ), node );

		// Change the property
		if ( node.IsNotNull() )
		{
			node->SetVisibility( bShow );
		}

		if ( initializeViews )
			Modified();
	}
	coreCatchExceptionsAddTraceAndThrowMacro(RenderingTreeMITK::Show)
}

boost::any Core::RenderingTreeMITK::GetNode( 
	Core::DataEntity::Pointer dataEntity )
{
	mitk::NodePredicateProperty::Pointer predicate;
	predicate = mitk::NodePredicateProperty::New(
		"dataEntity", 
		Core::DataEntityProperty::New(dataEntity.GetPointer()) );
	
	mitk::DataTreeNode::Pointer node;
	node = m_DataStorage->GetNode( predicate );

	return node;
}

boost::any Core::RenderingTreeMITK::GetNode( 
	unsigned int dataEntityId )
{
	mitk::NodePredicateProperty::Pointer predicate;
	predicate = mitk::NodePredicateProperty::New(
		"dataEntityID",
		mitk::IntProperty::New(dataEntityId));

	mitk::DataTreeNode::Pointer node;
	node = m_DataStorage->GetNode( predicate );
	return node;
}

double Core::RenderingTreeMITK::ComputeSizeOfPoint( mitk::DataTreeNode::Pointer node)
{
	double pointSize = -1;
	blMITKUtils::ComputePointSize( node->GetData(), pointSize, 0.005 );

	mitk::TimeSlicedGeometry::Pointer geometry = m_DataStorage->ComputeVisibleBoundingGeometry3D( NULL, "includeInBoundingBox" );
	if ( geometry.IsNotNull() )
	{
		pointSize = std::max( pointSize, 0.005 * geometry->GetDiagonalLength() );
	}

	return pointSize;
}

/**
*/
Core::DataEntity* 
Core::RenderingTreeMITK::GetDataEntity( mitk::DataTreeNode::Pointer node )
{
	if ( node.IsNull( ) )
	{
		return NULL;
	}

	Core::DataEntityProperty::Pointer dataEntityProp = NULL;
	dataEntityProp = static_cast<Core::DataEntityProperty*>
		( node->GetProperty("dataEntity") );

	if ( dataEntityProp.IsNull() )
	{
		return NULL;
	}

	Core::DataEntity* dataEntity = NULL;
	dataEntity = static_cast<Core::DataEntity*>
		( dataEntityProp->GetValue() );

	return dataEntity;
}

bool Core::RenderingTreeMITK::IsDataEntityRendered(Core::DataEntity* dataEntity)
{
	if( dataEntity == NULL )
		return false;

	mitk::DataTreeNode::Pointer node;
	Core::CastAnyProcessingData( GetNode( dataEntity ), node );
	if (node.IsNull())
		return false;

	return true;
}

bool Core::RenderingTreeMITK::IsDataEntityShown( Core::DataEntity* dataEntity )
{
	if( dataEntity == NULL )
		return false;

	mitk::DataTreeNode::Pointer node;
	Core::CastAnyProcessingData( GetNode( dataEntity ), node );
	if (node.IsNull())
		return false;

	bool isVisible;
	node->GetPropertyValue("visible", isVisible);
	return isVisible;
}

mitk::DataStorage::SetOfObjects::ConstPointer Core::RenderingTreeMITK::GetAllDataEntityNodes()
{
	mitk::DataStorage::SetOfObjects::ConstPointer set;
	set = m_DataStorage->GetSubset( mitk::NodePredicateProperty::New("dataEntity") );
	return set;
}

std::vector<DataEntity::Pointer> Core::RenderingTreeMITK::GetAllDataEntities()
{
	std::vector<DataEntity::Pointer> vector;

	mitk::DataStorage::SetOfObjects::ConstPointer set;
	set = GetAllDataEntityNodes();
	mitk::DataStorage::SetOfObjects::ConstIterator it;
	for ( it = set->Begin( ) ; it != set->End() ; it++ )
	{
		Core::DataEntity::Pointer currentDataEntity;
		currentDataEntity = GetDataEntity( it->Value() );
		if ( currentDataEntity.IsNotNull() )
		{
			vector.push_back( currentDataEntity );
		}
	}

	return vector;
}

void Core::RenderingTreeMITK::OnModifiedDataEntity( Core::SmartPointerObject* object )
{
	Core::DataEntity::Pointer dataEntity;
	dataEntity = dynamic_cast<Core::DataEntity*> ( object );
	if ( dataEntity.IsNull() )
	{
		return;
	}

	mitk::DataTreeNode::Pointer node;
	Core::CastAnyProcessingData( GetNode( dataEntity ), node );
	if ( node.IsNull() )
	{
		return;
	}

	// Build rendering data
	Core::RenDataFactory::Pointer builder = Core::RenDataFactory::New( );
	builder->SetInputDataEntity( 0, dataEntity );
	builder->Update();

	// Update rendering node with the new rendering data
	mitk::BaseData::Pointer mitkData;
	boost::any anyData = dataEntity->GetRenderingData( typeid( mitkData ) );
	Core::CastAnyProcessingData( anyData, mitkData );
	node->SetData( mitkData );

	Core::DataTreeMITKHelper::UpdateRenderingNodeLabels( node );
}

void Core::RenderingTreeMITK::UpdateMetadata( Core::DataEntity::Pointer dataEntity )
{
	mitk::DataTreeNode::Pointer node;
	Core::CastAnyProcessingData( GetNode( dataEntity ), node );
	if ( node.IsNull() )
	{
		return;
	}

	blTagMap::Pointer renderingTags = blTagMap::New( );
	dataEntity->GetMetadata( )->AddTag( "Rendering", renderingTags );

	mitk::PropertyList* propList = node->GetPropertyList();
	const mitk::PropertyList::PropertyMap* map = propList->GetMap( );
	mitk::PropertyList::PropertyMap::const_iterator it;
	for ( it = map->begin() ; it != map->end() ; it++ )
	{
		const char* propertyKey = it->first.c_str();
		mitk::BaseProperty* prop = it->second.first;

		if ( propertyKey == "dataEntityID" ||
			 propertyKey == "dataType" )
		{
			continue;
		}

		blTag::Pointer tag = ConvertFromMITK( propertyKey, prop );
		if ( tag.IsNotNull() )
		{
			renderingTags->AddTag( tag->GetName(), tag->GetValue() );
		}
	}
}

void Core::RenderingTreeMITK::UpdateRenderingProperties( Core::DataEntity::Pointer dataEntity )
{
	mitk::DataTreeNode::Pointer node;
	Core::CastAnyProcessingData( GetNode( dataEntity ), node );
	if ( node.IsNull() )
	{
		return;
	}

	blTagMap::Pointer renderingTags;
	renderingTags = dataEntity->GetMetadata( )->GetTagValue<blTagMap::Pointer>( "Rendering" );
	if ( renderingTags.IsNull() )
	{
		return;
	}

	blTagMap::Iterator it;
	for ( it = renderingTags->GetIteratorBegin() ; it != renderingTags->GetIteratorEnd() ; it++)
	{
		std::string propertyKey = it->second->GetName();

		if ( propertyKey == "dataEntityID" ||
			 propertyKey == "dataType" )
		{
			continue;
		}

		SetProperty( dataEntity, it->second );
	}

	// The LUT property cannot be serialized
	blMITKUtils::UpdateLookupTableProperty( node );
}

void Core::RenderingTreeMITK::ConnectToOnModifiedAnyNodeSignal( Core::DataEntity::Pointer dataEntity )
{
	mitk::DataTreeNode::Pointer node;
	Core::CastAnyProcessingData( GetNode( dataEntity ), node );
	mitk::BoolProperty::Pointer visibleProperty;
	if ( node.IsNull() || node->GetPropertyList() == NULL || !node->GetProperty( visibleProperty, "visible" ) )
	{
		return;
	}

	if ( m_MapObserversNode.find( node ) == m_MapObserversNode.end( ) )
	{
		itk::SimpleMemberCommand<RenderingTreeMITK>::Pointer command;
		command = itk::SimpleMemberCommand<RenderingTreeMITK>::New();
		command->SetCallbackFunction(
			this,
			&RenderingTreeMITK::OnModifiedRenderingTreeNode );
		m_MapObserversNode[ node ] = visibleProperty->AddObserver(
			itk::ModifiedEvent(), 
			command);
	}
}

void Core::RenderingTreeMITK::DisconnectToOnModifiedAnyNodeSignal( Core::DataEntity::Pointer dataEntity )
{
	mitk::DataTreeNode::Pointer node;
	Core::CastAnyProcessingData( GetNode( dataEntity ), node );
	m_MapObserversNode.erase( node );
}

void Core::RenderingTreeMITK::OnModifiedRenderingTreeNode()
{
	
}

void Core::RenderingTreeMITK::SetProperty( 
	Core::DataEntity::Pointer dataEntity, 
	blTag::Pointer property )
{
	if ( dataEntity.IsNull() )
	{
		return;
	}

	blTagMap::Pointer renderingTags;
	renderingTags = dataEntity->GetMetadata( )->GetTagValue<blTagMap::Pointer>( "Rendering" );
	if ( renderingTags.IsNull() )
	{
		renderingTags = blTagMap::New( );
		dataEntity->GetMetadata( )->AddTag( "Rendering", renderingTags );
	}
	renderingTags->AddTag( property );

	mitk::DataTreeNode::Pointer node;
	Core::CastAnyProcessingData( GetNode( dataEntity ), node );
	if ( node.IsNull() )
	{
		return;
	}

	mitk::BaseProperty::Pointer newProperty = ConvertToMITK( property );
	if ( newProperty.IsNull() )
	{
		return;
	}

	if ( node->GetProperty( property->GetName().c_str() ) )
		node->ReplaceProperty( property->GetName().c_str(), newProperty);
	else
		node->SetProperty( property->GetName().c_str(), newProperty);
}

mitk::BaseProperty::Pointer Core::RenderingTreeMITK::ConvertToMITK( blTag::Pointer tag )
{
	std::string propertyKey = tag->GetName();
	if ( tag->GetValue( ).type() == typeid( bool ) )
	{
		mitk::BoolProperty::Pointer prop;
		prop = mitk::BoolProperty::New( tag->GetValueCasted<bool>( ) );
		return prop.GetPointer();
	}
	else if ( tag->GetValue( ).type() == typeid( int ) )
	{
		mitk::IntProperty::Pointer prop;
		prop = mitk::IntProperty::New( tag->GetValueCasted<int>( ) );
		return prop.GetPointer();
	}
	else if ( tag->GetValue( ).type() == typeid( float ) )
	{
		mitk::FloatProperty::Pointer prop;
		prop = mitk::FloatProperty::New( tag->GetValueCasted<float>( ) );
		return prop.GetPointer();
	}
	else if ( tag->GetValue( ).type() == typeid( double ) )
	{
		mitk::FloatProperty::Pointer prop;
		prop = mitk::FloatProperty::New( tag->GetValueCasted<double>( ) );
		return prop.GetPointer();
	}
	else if ( tag->GetValue( ).type() == typeid( std::string ) )
	{
		mitk::StringProperty::Pointer prop;
		prop = mitk::StringProperty::New( tag->GetValueCasted<std::string>( ) );
		return prop.GetPointer();
	}

	return NULL;
}

blTag::Pointer Core::RenderingTreeMITK::GetProperty( 
	Core::DataEntity::Pointer dataEntity, const std::string& name )
{
	mitk::DataTreeNode::Pointer node;
	Core::CastAnyProcessingData( GetNode( dataEntity ), node );
	if ( node.IsNull() )
	{
		return NULL;
	}

	mitk::PropertyList* propList = node->GetPropertyList();
	mitk::BaseProperty* prop = propList->GetProperty( name.c_str() );
	if ( !prop )
	{
		return NULL;
	}
	
	return ConvertFromMITK( name.c_str(), prop );
}

blTag::Pointer Core::RenderingTreeMITK::ConvertFromMITK( 
	const char* propertyKey, mitk::BaseProperty* prop )
{
	blTag::Pointer tag;
	if ( mitk::GenericProperty<bool>* p = dynamic_cast<mitk::GenericProperty<bool>*>( prop ) )
	{
		tag = blTag::New( propertyKey, p->GetValue() );
	}
	else if ( mitk::GenericProperty<int>* p = dynamic_cast<mitk::GenericProperty<int>*>( prop ) )
	{
		tag = blTag::New( propertyKey, p->GetValue() );
	}
	else if ( mitk::GenericProperty<float>* p = dynamic_cast<mitk::GenericProperty<float>*>( prop ) )
	{
		tag = blTag::New( propertyKey, p->GetValue() );
	}
	else if ( mitk::StringProperty* p = dynamic_cast<mitk::StringProperty*>( prop ) )
	{
		tag = blTag::New( propertyKey, std::string( p->GetValue() ) );
	}
	return tag;
}

