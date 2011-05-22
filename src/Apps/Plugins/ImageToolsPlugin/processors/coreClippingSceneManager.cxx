/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreClippingSceneManager.h"
#include "coreDataContainer.h"
#include "coreKernel.h"
#include "coreVTKPolyDataHolder.h"
//VTK
#include "vtkCubeSource.h"
#include "vtkLineSource.h"
#include "vtkConeSource.h"
#include "vtkPlaneSource.h"
#include "vtkSmartPointer.h"
#include "vtkAppendPolyData.h"
#include "vtkClipPolyData.h"
#include "vtkBox.h"
//MITK
#include "mitkVtkRepresentationProperty.h"
#include <mitkRenderingManager.h>
#include <mitkProperties.h>

Core::ClippingSceneManager::ClippingSceneManager(
	Core::ClippingProcessor::ClippingProcessorDataHolder::Pointer dataHolder,
	Core::RenderingTreeMITK::Pointer renderingTree
)
{
	this->ConstructDataHolder(dataHolder);
	this->ConstructBoundingBoxDataEntity();			
	this->ConstructClippingPlaneDataEntity();
	this->ConstructMarkerSetDataEntity();
	this->ConstructRenderingTree(renderingTree);
}

void Core::ClippingSceneManager::OnClippingDataChanged()
{
	Core::ClippingProcessorData::Pointer clippingData = this->m_DataHolder->GetSubject();
	if( clippingData.IsNull() )
		return;

	this->UpdateBoundingBox(clippingData);
	this->CalculateClippingPlane(clippingData);
	this->UpdateClippingPlaneActor();

	float markerSize = float(
		clippingData->m_box[1] - clippingData->m_box[0] + 
		clippingData->m_box[3] - clippingData->m_box[2] + 
		clippingData->m_box[5] - clippingData->m_box[4]);
	markerSize = float( 0.04 * markerSize / 3.0 );
	this->SetMarkerSize( markerSize );
	this->UpdateMarkerLocations();
	mitk::RenderingManager::GetInstance()->ForceImmediateUpdateAll();
}

Core::ClippingSceneManager::~ClippingSceneManager()
{
	this->m_RenderingTree->Remove(this->m_BoundingBoxDataEntity->GetId( ) );
	this->m_RenderingTree->Remove(this->m_PlaneDataEntity->GetId( ) );
	this->m_RenderingTree->Remove(this->m_MarkerSetDataEntity->GetId( ) );
}

void Core::ClippingSceneManager::ConstructBoundingBoxDataEntity()
{

	this->m_BoundingBox = mitk::Cuboid::New();
	this->m_BoundingBoxDataEntity = Core::DataEntity::New(Core::MITKDataTypeId);
	this->m_BoundingBoxDataEntity->AddTimeStep(this->m_BoundingBox);
	this->m_BoundingBoxDataEntity->SetRenderingData(mitk::BaseData::Pointer(this->m_BoundingBox.GetPointer()));
}

void Core::ClippingSceneManager::ConstructClippingPlaneDataEntity()
{
	this->m_Plane = mitk::Surface::New();
	this->m_PlaneDataEntity = Core::DataEntity::New(Core::MITKDataTypeId);
	this->m_PlaneDataEntity->AddTimeStep(this->m_Plane);
	this->m_PlaneDataEntity->SetRenderingData(mitk::BaseData::Pointer(this->m_Plane.GetPointer()));
}

void Core::ClippingSceneManager::ConstructDataHolder( Core::ClippingProcessor::ClippingProcessorDataHolder::Pointer dataHolder )
{
	this->m_DataHolder = dataHolder;
	if( this->m_DataHolder.IsNull() )
	{
		this->m_DataHolder = Core::ClippingProcessor::ClippingProcessorDataHolder::New();
	}
	this->m_DataHolder->AddObserver(this, &Core::ClippingSceneManager::OnClippingDataChanged);
}

void Core::ClippingSceneManager::ConstructRenderingTree( 
	Core::RenderingTreeMITK::Pointer &renderingTree )
{
	//float BLUE_COLOR[ 3 ] = {0.25, 0.25, 1.0};
	float BLUE_COLOR[ 3 ] = {1.0, 1.0, 1.0};
	this->m_RenderingTree = renderingTree;
	if( renderingTree.IsNull() )
	{
		this->m_RenderingTree = Core::RenderingTreeMITK::New();
	}
	this->m_RenderingTree->Add(this->m_MarkerSetDataEntity);
	mitk::DataTreeNode::Pointer node;
	boost::any anyData = this->m_RenderingTree->GetNode(this->m_MarkerSetDataEntity);
	Core::CastAnyProcessingData( anyData, node );
	node->SetName("Markers");

	this->m_RenderingTree->Add(this->m_BoundingBoxDataEntity);
	anyData = this->m_RenderingTree->GetNode(this->m_BoundingBoxDataEntity);
	Core::CastAnyProcessingData( anyData, node );
	node->SetName("BoundingBox");
	node->SetProperty( 
									"representation", 
									mitk::VtkRepresentationProperty::New("Wireframe") );

	this->m_RenderingTree->Add(this->m_PlaneDataEntity);
	anyData = this->m_RenderingTree->GetNode(this->m_PlaneDataEntity);
	Core::CastAnyProcessingData( anyData, node );
	node->SetName( "ClippingPlane");
	node->SetOpacity( 0.5);
	node->SetColor( BLUE_COLOR);
	
	//! Interactors
// 	mitk::Point3D offset, direction;
// 	direction[0] = 1;
// 	direction[1] = 0;
// 	direction[2] = 0;
// 
//	this->m_LineBoundInteractor = blMitkLineBoundPointInteractor::New(
// 		this->m_RenderingTree->GetNode(this->m_PlaneDataEntity),
// 		offset,
// 		direction);
	
//	Core::vtkPolyDataPtr polyData;
//	this->m_MarkerSetDataEntity->GetProcessingData( polyData );
//	this->m_ClipperInteractor = blMitkClipperInteractor::New( 
// 		"ClipperInteractor", 
// 		this->m_RenderingTree->GetNode(this->m_MarkerSetDataEntity),
// 		polyData 
}

void Core::ClippingSceneManager::SetMarkerSize( float markerSize )
{
	mitk::DataTreeNode::Pointer node;
	boost::any anyData = this->m_RenderingTree->GetNode(this->m_MarkerSetDataEntity);
	Core::CastAnyProcessingData( anyData, node );
	node->SetProperty(
									"pointsize", 
									mitk::FloatProperty::New(markerSize)	);
}

void Core::ClippingSceneManager::CalculateClippingPlane( Core::ClippingProcessorData::Pointer clippingData )
{
	//TODO  set center and set normal using markers
	vtkSmartPointer<vtkPlaneSource> planeSource= vtkSmartPointer<vtkPlaneSource>::New();
	vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
	//vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
	vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
	vtkSmartPointer<vtkClipPolyData> clip = vtkSmartPointer<vtkClipPolyData>::New();
	vtkSmartPointer<vtkBox> box = vtkSmartPointer<vtkBox>::New();

	double d = 5;
	planeSource->SetOrigin(0,0,0);
	planeSource->SetPoint1(2*d,0,0 );
	planeSource->SetPoint2(0,2*d, 0 );
	planeSource->SetCenter(clippingData->m_center);
	planeSource->SetNormal(clippingData->m_normal );
	planeSource->SetXResolution(100);
	planeSource->SetYResolution(100);
	
	double p2[3];
	p2[0] =clippingData->m_box[1];
	p2[1] =(clippingData->m_box[3]+ clippingData->m_box[2])*0.5;
	p2[2] =(clippingData->m_box[5]+ clippingData->m_box[4])*0.5;
 
	lineSource->SetPoint1(clippingData->m_center);
	lineSource->SetPoint2(p2);
// 	coneSource->SetCenter(p2);
// 	coneSource->SetDirection( clippingData->m_normal);
// 	coneSource->SetHeight( 0.8 );
// 	coneSource->SetAngle( 10 );
// 	coneSource->SetRadius( 0.2 );


	box->SetBounds(clippingData->m_box);
	clip->SetInput(planeSource->GetOutput());
	clip->SetClipFunction(box);
	clip->InsideOutOn();
	clip->Update();
	//blShapeUtils::ShapeUtils::SaveShapeToFile(clip->GetOutput(),"clipfilter.vtk");
	//blShapeUtils::ShapeUtils::SaveShapeToFile(planeSource->GetOutput(),"planesource.vtk");

	appendFilter->AddInput( clip->GetOutput( ) );
	appendFilter->AddInput( lineSource->GetOutput( ) );
//	appendFilter->AddInput( coneSource->GetOutput( ) );
	appendFilter->Update();

	this->m_Plane->SetVtkPolyData( appendFilter->GetOutput() );
//	this->m_LineBoundInteractor->SetOffset(clippingData->m_center);
}

void Core::ClippingSceneManager::UpdateClippingPlaneActor()
{
	
}

void Core::ClippingSceneManager::UpdateMarkerLocations()
{
	mitk::PointSet::PointType point;
	
	//TODO take center and second point
	point[0] = this->m_Center[0];
	point[1] = this->m_Center[1];
	point[2] = this->m_Center[2];
	this->m_MarkerSet->SetPoint(0, point);
	point[0] = this->m_Max[0];
	point[1] = this->m_Max[1];
	point[2] = this->m_Max[2];
	this->m_MarkerSet->SetPoint(1, point);
}

void Core::ClippingSceneManager::UpdateBoundingBox(Core::ClippingProcessorData::Pointer clippingData )
{
	vtkCubeSource* cube = vtkCubeSource::New();
	cube->SetBounds(clippingData->m_box);
	cube->Update();
	this->m_BoundingBox->SetVtkPolyData(cube->GetOutput());
	cube->GetCenter(clippingData->m_center);
	this->m_Center[0] = mitk::PointSet::CoordinateType( clippingData->m_center[0] );
	this->m_Center[1] = mitk::PointSet::CoordinateType( clippingData->m_center[1] );
	this->m_Center[2] = mitk::PointSet::CoordinateType( clippingData->m_center[2] );
	this->m_Max[0] = mitk::PointSet::CoordinateType( clippingData->m_box[1] );
	this->m_Max[1] = mitk::PointSet::CoordinateType( ( clippingData->m_box[3] + clippingData->m_box[2])*0.5 ); 
	this->m_Max[2] =mitk::PointSet::CoordinateType( (clippingData->m_box[5] + clippingData->m_box[4])*0.5 );
	cube->Delete();
}

void Core::ClippingSceneManager::ConstructMarkerSetDataEntity()
{
	this->m_MarkerSet = mitk::PointSet::New();
	while( this->m_MarkerSet->GetSize() < 2 )
	{
		this->m_MarkerSet->InsertPoint(this->m_MarkerSet->GetSize(), mitk::PointSet::PointType());
	}
	this->m_MarkerSetDataEntity = Core::DataEntity::New(Core::MITKDataTypeId);
	this->m_MarkerSetDataEntity->AddTimeStep(this->m_MarkerSet);
	this->m_MarkerSetDataEntity->SetRenderingData(mitk::BaseData::Pointer(this->m_MarkerSet));
}

bool Core::ClippingSceneManager::GetIsInClippingMode() const
{
	return m_IsInClippingMode;
}

void Core::ClippingSceneManager::SetIsInClippingMode( bool flag )
{
	this->m_IsInClippingMode = flag;
	this->m_RenderingTree->Show(this->m_BoundingBoxDataEntity, this->m_IsInClippingMode);
	this->m_RenderingTree->Show(this->m_PlaneDataEntity, this->m_IsInClippingMode);
	this->m_RenderingTree->Show(this->m_MarkerSetDataEntity, this->m_IsInClippingMode);
// 	if (this->m_IsInClippingMode)
// 	{
// 		mitk::GlobalInteraction::GetInstance()->AddInteractor(this->m_LineBoundInteractor);
// 		mitk::GlobalInteraction::GetInstance()->AddInteractor(this->m_ClipperInteractor);
// 	}
// 	else
// 	{
// 		mitk::GlobalInteraction::GetInstance()->RemoveInteractor(this->m_LineBoundInteractor);
//		mitk::GlobalInteraction::GetInstance()->RemoveInteractor(this->m_ClipperInteractor);
//	}
}

void Core::ClippingSceneManager::UpdateModifiedOutput(Core::ClippingProcessor::Pointer processor)
{
	// Hide input
	Core::DataEntity::Pointer inputShape = processor->GetInputDataEntityHolder()->GetSubject( );
	this->m_RenderingTree->Show( inputShape, false );

	Core::DataEntity::Pointer dataEntity = processor->GetOutputDataEntityHolder()->GetSubject( );
	if (dataEntity.IsNotNull())
	{
		// Add to the data entity list 
		Core::DataContainer::Pointer data = Core::Runtime::Kernel::GetDataContainer();
		Core::DataEntityList::Pointer list = data->GetDataEntityList();
		list->Add( dataEntity );

		// Connect the subject to notify when is removed from the data list
		list->ConnectOutputHolder( processor->GetOutputDataEntityHolder() );

		// Render the generated mesh
		this->m_RenderingTree->Add( dataEntity, true,true );

	}	
}
