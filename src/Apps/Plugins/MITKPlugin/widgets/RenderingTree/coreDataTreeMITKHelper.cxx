/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataTreeMITKHelper.h"
#include "coreMitkProperties.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHelper.txx"
#include "coreDataContainer.h"
#include "coreKernel.h"
#include "coreVTKPolyDataHolder.h"
#include "coreReportExceptionMacros.h"
#include "coreVTKUnstructuredGridHolder.h"

#include "mitkVtkScalarModeProperty.h"
#include "mitkLookupTable.h"
#include "mitkLookupTableProperty.h"
#include "mitkProperties.h"
#include "mitkRenderingManager.h"
#include "mitkSurface.h"
#include "mitkImage.h"
#include "mitkStringVectorProperty.h"

#include "blLookupTables.h"
#include "blMITKUtils.h"

#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkStringArray.h"
#include "coreRenderingTreeMITK.h"

/**
When the dataEntity is a NM image a HotMetal LUT is applied
*/
void Core::DataTreeMITKHelper::ApplyLookupTableToNMImage(
	mitk::DataTreeNode::Pointer imageNode,
	blLookupTables::LUT_TYPE lutType )
{
	Core::DataEntity::Pointer dataEntity;
	dataEntity = Core::RenderingTreeMITK::GetDataEntity( imageNode );

	if ( dataEntity.IsNull() || (dataEntity->GetMetadata( )->GetModality()!= Core::NM))
		
	{	
		return;
	}

	blMITKUtils::ApplyLookupTableToImage(true,imageNode,lutType); 

}
/**
 */
void Core::DataTreeMITKHelper::ApplyLookupTableToMesh(
						mitk::DataTreeNode::Pointer meshNode, 
						blMITKUtils::LUT_SCALAR_MODE mode )
{
	Core::DataEntity::Pointer dataEntity;
	dataEntity = Core::RenderingTreeMITK::GetDataEntity( meshNode );

	Core::vtkPolyDataPtr surfaceMesh;
	Core::vtkUnstructuredGridPtr volumeMesh;
	if ( dataEntity.IsNull() || ! (dataEntity->IsSurfaceMesh() || dataEntity->IsVolumeMesh())
		|| mode == blMITKUtils::LUT_SCALAR_MODE_NONE )
	{	
		return;
	}

	// Use the rendering representation because it is more generic. We
	// don't need to implement it for each processing type
	mitk::BaseData::Pointer mitkData;
	boost::any anyData = dataEntity->GetRenderingData( typeid( mitkData ) );
	Core::CastAnyProcessingData( anyData, mitkData );
	mitk::Surface* surfacedata = 
		dynamic_cast<mitk::Surface*>(mitkData.GetPointer()); 
	int dataType = VTK_VOID;
	double maxScalar = DBL_MIN;
	double minScalar = DBL_MAX;
	for ( size_t i = 0 ; i < dataEntity->GetNumberOfTimeSteps( ) ; i++ )
	{
		surfaceMesh = surfacedata->GetVtkPolyData( i );

		double range[2];

		vtkDataArray *ptScalars, *cellScalars;
		ptScalars = surfaceMesh->GetPointData( )->GetScalars();
		cellScalars = surfaceMesh->GetCellData( )->GetScalars();

		if ( surfaceMesh && ( ptScalars || cellScalars ) )
		{
			surfaceMesh->GetScalarRange( range );
			maxScalar = std::max( range[ 1 ], maxScalar );
			minScalar = std::min( range[ 0 ], minScalar );
		}

		if ( ptScalars != NULL )
		{
			dataType = ptScalars->GetDataType( );
		}

		if ( cellScalars != NULL )
		{
			dataType = cellScalars->GetDataType( );
		}
	}

	// Choose the LUT
	blLookupTables::LUT_TYPE lutType;
	// If the data is short type and range is from 0 to 14 or less
	// use a fixed LUT of 16 colors (cardiac mesh)
	if ( maxScalar - minScalar < 16 && dataType == VTK_SHORT )
	{
		maxScalar = minScalar + 16;
		lutType = blLookupTables::LUT_TYPE_16_COLORS;
	}
	else
	{
		lutType = blLookupTables::LUT_TYPE_RAINBOW_HUE_INVERTED;
	}


	// Build the node properties
	mitk::VtkScalarModeProperty::Pointer scalarMode = mitk::VtkScalarModeProperty::New();
	switch (mode )
	{
	case blMITKUtils::LUT_SCALAR_MODE_POINT_DATA:scalarMode->SetScalarModeToPointData();break;
	case blMITKUtils::LUT_SCALAR_MODE_CELL_DATA:scalarMode->SetScalarModeToCellData();break;
	}
	meshNode->SetProperty("scalar mode", scalarMode );
	meshNode->SetProperty("use color", mitk::BoolProperty::New(true));
	meshNode->SetProperty("color mode", mitk::BoolProperty::New(true));
	// This two properties will call the vtkLookupTable::SetScalarRange( )
	meshNode->SetProperty("ScalarsRangeMaximum", mitk::FloatProperty::New(float(maxScalar)));
	meshNode->SetProperty("ScalarsRangeMinimum", mitk::FloatProperty::New(float(minScalar)));
	meshNode->SetProperty("scalar visibility", mitk::BoolProperty::New(true));

	// Create the LUT
	blMITKUtils::CreateLut(meshNode, lutType);
}

void Core::DataTreeMITKHelper::SetActiveScalars( 
	Core::DataEntityHolder::Pointer dataEntityHolder, 
	std::string strName,
	int iTimeStep )
{
	try
	{
		Core::vtkPolyDataPtr polyData;
		Core::DataEntityHelper::GetProcessingData( 
			dataEntityHolder, 
			polyData,
			iTimeStep );
		// 	mitk::Surface* surfacedata = 
		// 		dynamic_cast<mitk::Surface*>(dataEntityHolder->GetSubject()->GetRenderingData().GetPointer()); 
		// 	polyData = surfacedata->GetVtkPolyData();
		if ( polyData && polyData->GetPointData( ) )
		{
			polyData->GetPointData( )->SetActiveScalars( strName.c_str() );
		}
		if ( polyData && polyData->GetCellData( ) )
		{
			polyData->GetCellData( )->SetActiveScalars( strName.c_str() );
		}
	}
	coreCatchExceptionsLogAndNoThrowMacro( Core::DataTreeMITKHelper::SetActiveScalars )
}

blMITKUtils::LUT_SCALAR_MODE Core::DataTreeMITKHelper::SetDefaultActiveScalars( 
	Core::DataEntity::Pointer dataEntity )
{
	blMITKUtils::LUT_SCALAR_MODE scalarMode = blMITKUtils::LUT_SCALAR_MODE_NONE;

	// Set active scalars to the first one for each time step
	for	(size_t iTimeStep = 0 ; iTimeStep < dataEntity->GetNumberOfTimeSteps( ) ;iTimeStep++)
	{
		Core::vtkPolyDataPtr surfaceMesh;
		// Use the rendering representation because it is more generic. We
		// don't need to implement it for each processing type
		mitk::BaseData::Pointer mitkData;
		boost::any anyData = dataEntity->GetRenderingData( typeid( mitkData ) );
		Core::CastAnyProcessingData( anyData, mitkData );
		mitk::Surface* surfacedata;
		surfacedata = dynamic_cast<mitk::Surface*>(mitkData.GetPointer()); 
		if ( surfacedata != NULL )
		{
			surfaceMesh = surfacedata->GetVtkPolyData( iTimeStep );
		}

		// Colorize the mesh using the first scalars vector
		if ( surfaceMesh )
		{	
			bool bDataAvailable = SetDefaultActiveScalars( surfaceMesh->GetPointData( ) );
			if ( bDataAvailable )
			{
				scalarMode = blMITKUtils::LUT_SCALAR_MODE_POINT_DATA;
			}
		}

		if (surfaceMesh && scalarMode == blMITKUtils::LUT_SCALAR_MODE_NONE )
		{
			bool bDataAvailable = SetDefaultActiveScalars( surfaceMesh->GetCellData( ) );
			if ( bDataAvailable )
			{
				scalarMode = blMITKUtils::LUT_SCALAR_MODE_CELL_DATA;
			}
		}
	}

	return scalarMode;
}

bool Core::DataTreeMITKHelper::SetDefaultActiveScalars( vtkDataSetAttributes* data )
{
	bool bDataAvilable = false;

	if ( data )
	{
		if ( data->GetNumberOfArrays() )
		{
			// If there's no active array
			if ( !data->GetAttribute( vtkDataSetAttributes::SCALARS )  )
			{
 				const char* arrayName = data->GetArrayName( 0 );
 				if (arrayName ==  NULL ||
					!strcmp(arrayName, "Normals"))
 					return bDataAvilable;
				data->SetActiveScalars( arrayName );
			}

			bDataAvilable = true;
		}
	}

	return bDataAvilable;
}



/**
*/
void Core::DataTreeMITKHelper::ChangeShowLabelsProperty(  
	Core::DataEntity::Pointer dataEntity,
	Core::RenderingTree::Pointer tree,  
	bool show )
{
	if ( tree.IsNull() || dataEntity.IsNull() )
	{
		return ;
	}

	mitk::DataTreeNode::Pointer node;
	Core::CastAnyProcessingData( tree->GetNode( dataEntity ), node );

	if ( node.IsNotNull() )
	{
		mitk::BoolProperty *showLabelProperty;
		showLabelProperty = dynamic_cast<mitk::BoolProperty *>(node->GetProperty("show label") );
		if ( showLabelProperty )
		{
			// Update all the mappers
			node->SetProperty("show label", mitk::BoolProperty::New(show) );
			node->GetData( )->Modified();
		}
	}

	Core::DataEntity::ChildrenListType childrenList = dataEntity->GetChildrenList();
	for ( int i = 0 ; i < childrenList.size() ;i++ )
	{
		ChangeShowLabelsProperty( childrenList[ i ], tree, show );
	}

}

void Core::DataTreeMITKHelper::GetShowLabelsProperty(  
	Core::DataEntity::Pointer dataEntity,
	Core::RenderingTree::Pointer tree,
	bool &showLabels )
{
	if ( tree.IsNull() || dataEntity.IsNull() )
	{
		return ;
	}

	mitk::DataTreeNode::Pointer node;
	Core::CastAnyProcessingData( tree->GetNode( dataEntity ), node );

	if ( node.IsNotNull() )
	{
		node->GetBoolProperty( "show label", showLabels );
	}

	Core::DataEntity::ChildrenListType childrenList = dataEntity->GetChildrenList();
	for ( int i = 0 ; i < childrenList.size() ;i++ )
	{
		GetShowLabelsProperty( childrenList[ i ], tree, showLabels );
	}

}

void Core::DataTreeMITKHelper::UpdateRenderingNodeLabels( mitk::DataTreeNode::Pointer node )
{
	try{

		if ( node.IsNull() )
		{
			return;
		}

		Core::DataEntity::Pointer dataEntity;
		dataEntity = Core::RenderingTreeMITK::GetDataEntity( node );

		// Resize the MITK point set if necessary
		unsigned iNumTimeSteps = dataEntity->GetNumberOfTimeSteps( );

		// Get the property for showing labels
		mitk::StringVectorProperty *stringVectorProperty = NULL;
		stringVectorProperty = dynamic_cast<mitk::StringVectorProperty *>
			(node->GetProperty("label"));
		if ( stringVectorProperty == NULL )
		{
			return;
		}

		stringVectorProperty->SetNumberOfTimeSteps( iNumTimeSteps );

		// We should use iNumTimeSteps
		for ( unsigned iTimeStep = 0 ; iTimeStep < iNumTimeSteps ; iTimeStep++ )
		{
			vtkPolyDataPtr	processingData;
			dataEntity->GetProcessingData( 
				processingData, 
				iTimeStep );

			// Reset to 0
			stringVectorProperty->GetStringVector( iTimeStep ).resize( 0 );

			// Get landmarks name
			vtkStringArray *landmarksName = NULL;
			vtkPointData *pd = processingData->GetPointData();
			if ( pd && pd->GetNumberOfArrays() > 0 )
			{
				vtkAbstractArray *abstractData = NULL;

				// Resize the columns
				int arrayNum = 0;
				landmarksName = NULL;
				while ( landmarksName == NULL && arrayNum < pd->GetNumberOfArrays() )
				{
					abstractData = pd->GetAbstractArray(arrayNum);
					landmarksName = vtkStringArray::SafeDownCast(abstractData);
					arrayNum++;
				}
			}

			if ( landmarksName != NULL )
			{
				for( int i = 0; i < landmarksName->GetNumberOfValues(); i++ )
				{
					// Add the label
					if ( stringVectorProperty != NULL && i < landmarksName->GetNumberOfValues() )
					{
						stringVectorProperty->GetStringVector( iTimeStep ).push_back( 
							landmarksName->GetValue( i ) );
					}
				}
			}
		}


		// Avoid exceptions when calling GetProcessingData
	}coreCatchExceptionsLogAndNoThrowMacro(
		"Core::DataTreeMITKHelper::UpdateRenderingNodeLabels");

}
