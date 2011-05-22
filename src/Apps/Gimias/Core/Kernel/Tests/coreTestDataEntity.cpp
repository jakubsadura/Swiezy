// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreTestDataEntity.h"
#include "coreDataEntityFactory.h"
#include "coreDataEntityHelper.h"
#include "coreKernel.h"
#include "CISTIBToolkit.h"
#include "blShapeUtils.h"

#define TOOLKIT_DATA_ROOT CISTIB_TOOLKIT_FOLDER "/Data"
const char* STR_INPUT_MESH = TOOLKIT_DATA_ROOT "/Misc/Meshes/Cube.vtk";
const char* STR_INPUT_IMAGE_FILENAME = TOOLKIT_DATA_ROOT "/Cardio/CT/Input/pa73_phs_r_%02d.vtk";
const int INPUT_NUM_FILES = 15;

void CoreTestDataEntity::test0()
{
	CreateDataEntity();
	ChangeProcessingData();
	AddToRenderingTree();
	RemoveFromRenderingTree( );
	LoadDataEntity( );
}

void CoreTestDataEntity::setUp()
{
	// We don't create anything here
	//blCxxTest::setUp( );

	Core::Runtime::Kernel::Initialize( NULL, Core::Runtime::Console);
}

void CoreTestDataEntity::tearDown()
{
	// Destroy internal referneces
	m_dataEntityHolderOutput->SetSubject ( NULL );
	m_tree = NULL;

	// Cleanup allocation of all the members of static objects
	Core::Runtime::Kernel::Terminate(); 

	// We cannot detect memory leaks of MITK because there are some
	// classes that use static member variables like mitk::EnumerationProperty
	// And we should implement the functinos to clear all this memory
	// Unregister the created factories. We need to assure that all
	// static memory is deleted
	//blCxxTest::tearDown( );
}

void CoreTestDataEntity::CreateDataEntity()
{
	m_dataEntityHolderOutput = Core::DataEntityHolder::New();

	// Load shape from file
	this->m_PolyData = Core::vtkPolyDataPtr::New();
	m_PolyData.TakeReference( blShapeUtils::ShapeUtils::LoadShapeFromFile( STR_INPUT_MESH ) );

	m_dataEntityHolderOutput->SetSubject(
		Core::DataEntityFactory::Build(
			this->m_PolyData, 
			Core::DataEntityFactory::VtkPoly)
		);
}

void CoreTestDataEntity::ChangeProcessingData()
{
	// Load shape from file again
	this->m_PolyData = Core::vtkPolyDataPtr::New();
	m_PolyData.TakeReference( blShapeUtils::ShapeUtils::LoadShapeFromFile( STR_INPUT_MESH ) );

	m_dataEntityHolderOutput->SetSubject(
		Core::DataEntityFactory::Build(
			this->m_PolyData, 
			Core::DataEntityFactory::VtkPoly)
		);
}

void CoreTestDataEntity::AddToRenderingTree()
{
	m_tree = Core::RenderingTree::New( );
	mitk::DataTreeNode::Pointer node = m_tree->Add( 
		m_dataEntityHolderOutput->GetSubject( ) );
}

void CoreTestDataEntity::RemoveFromRenderingTree()
{
	m_tree->Remove( m_dataEntityHolderOutput->GetSubject( )->GetId( ) );
}

void CoreTestDataEntity::TestGetProcessingData()
{
	this->CreateDataEntity();
	TS_ASSERT(this->m_PolyData);
	Core::vtkPolyDataPtr polyData;
	TS_ASSERT( m_dataEntityHolderOutput->GetSubject()->GetProcessingData(polyData) );
	TS_ASSERT( polyData == this->m_PolyData );

	Core::vtkPolyDataPtr polyData2;
	Core::CastAnyProcessingData( m_dataEntityHolderOutput->GetSubject()->GetProcessingData(), polyData2 );
	TS_ASSERT(polyData == polyData2 );
}

void CoreTestDataEntity::LoadDataEntity( )
{
	std::vector<std::string> filenames;
	for ( int i = 0 ; i < INPUT_NUM_FILES ; i++ )
	{
		char	inputName[256];
		sprintf( inputName, STR_INPUT_IMAGE_FILENAME, i + 1);
		
		filenames.push_back( inputName );
	}

	Core::DataEntity::Pointer dataEntity;
	dataEntity = Core::DataEntityHelper::LoadDataEntity( filenames );
	TS_ASSERT( dataEntity.IsNotNull() );
}


