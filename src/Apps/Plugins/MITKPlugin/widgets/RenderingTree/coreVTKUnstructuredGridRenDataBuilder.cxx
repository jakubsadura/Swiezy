/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVTKUnstructuredGridRenDataBuilder.h"
#include "blVTKHelperTools.h"
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkSmartPointer.h>
#include <mitkUnstructuredGrid.h>
#include <mitkSurface.h>
#include "coreVTKUnstructuredGridHolder.h"
#include "coreVTKPolyDataHolder.h"
#include "blMitkSurface.h"
#include "vtkArrayCalculator.h"

using namespace Core;

//!
VTKUnstructuredGridRenDataBuilder::VTKUnstructuredGridRenDataBuilder(void)
{
}

//!
VTKUnstructuredGridRenDataBuilder::~VTKUnstructuredGridRenDataBuilder(void)
{
}

//!
void VTKUnstructuredGridRenDataBuilder::Update( )
{
	// Get input
	Core::DataEntity::Pointer dataEntity = GetInputDataEntity( 0 );

	if ( !CheckInput( ) )
	{
		return;
	}

	//! Try casting to the expected format. The first image is used to initialization
	vtkUnstructuredGridPtr volumeMesh;
	vtkSmartPointer<vtkDataSetSurfaceFilter> filter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	bool worked = dataEntity->GetProcessingData( volumeMesh, 0 );
	if(!worked || volumeMesh == NULL)
	{
		return;
	}

	mitk::BaseData::Pointer mitkData;
	boost::any anyData = dataEntity->GetRenderingData( typeid( mitkData ) );
	Core::CastAnyProcessingData( anyData, mitkData );
	blMitk::Surface::Pointer mitkSurface = dynamic_cast<blMitk::Surface*> ( mitkData.GetPointer() );

	// Add a reference to vtkPolyData inside mitk::Surface
	if ( mitkSurface.IsNull() )
	{
		mitkSurface = blMitk::Surface::New();
	}
	unsigned int timeSteps = dataEntity->GetNumberOfTimeSteps();


	vtkPolyDataPtr renPolydata;
	mitkSurface->Expand( timeSteps );
	for ( int iCount = 0 ; iCount < timeSteps ; iCount++ )
	{
		dataEntity->GetProcessingData( volumeMesh, iCount );
		
		filter->SetInput( volumeMesh );
		filter->Update();
		renPolydata = vtkPolyDataPtr::New();
		renPolydata->DeepCopy(filter->GetOutput());

		// Compute magnitude for PointData arrays with multiple components
		vtkSmartPointer<vtkArrayCalculator> calc = vtkSmartPointer<vtkArrayCalculator>::New();
		calc->SetInput( renPolydata );
		calc->SetAttributeModeToUsePointData();

		int nA = renPolydata->GetPointData( )->GetNumberOfArrays( );
		for ( int i = 0; i < nA; i++ ) 
		{
			std::string arrayName( renPolydata->GetPointData( )->GetArrayName( i ) );
			vtkDataArray * dataArray;
			dataArray = renPolydata->GetPointData( )->GetArray( arrayName.c_str( ) );
			if ( dataArray == NULL ) 
			{
				continue;
			}

			int nComponents = dataArray->GetNumberOfComponents( ); 
			if ( nComponents > 1 )
			{
				std::stringstream strFunction;
				strFunction << "sqrt(";
				for ( int c = 0 ; c < nComponents ; c++ )
				{
					std::stringstream strComp;
					strComp << "s_" << c;
					calc->AddScalarVariable( strComp.str().c_str(), arrayName.c_str(), c);
					if ( c!= 0 )
					{
						strFunction << " + ";
					}
					strFunction << strComp.str() << "*" << strComp.str();
				}
				strFunction << ")";
				calc->SetFunction( strFunction.str().c_str() );
				calc->SetResultArrayName( arrayName.c_str() );
				calc->SetResultArrayType( dataArray->GetDataType() );
				calc->Update();

				// Copy array
				vtkDataArray * outDataArray = calc->GetOutput()->GetPointData()->GetArray( arrayName.c_str() );
				renPolydata->GetPointData()->GetArray( arrayName.c_str() )->DeepCopy( outDataArray );
			}
		}

// 		if (abs(filter->GetOutput()->GetBounds()[1]-filter->GetOutput()->GetBounds()[0 ]) < 0.10 &&
// 			abs(filter->GetOutput()->GetBounds()[3]-filter->GetOutput()->GetBounds()[2 ])< 0.10 &&
// 			abs(filter->GetOutput()->GetBounds()[5]-filter->GetOutput()->GetBounds()[4 ]) < 0.10 )
// 			blVTKHelperTools::ScaleShape( 
// 			filter->GetOutput(), 
// 			renPolydata, 
// 			100 );
// 		else
		

		mitkSurface->SetVtkPolyData( renPolydata, iCount );
	}

	dataEntity->SetRenderingData( mitk::BaseData::Pointer(mitkSurface) );
//
//	// Add a reference to vtkPolyData inside mitk::Surface
//	vtkSmartPointer<vtkDataSetSurfaceFilter> filter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
//	vtkPolyDataPtr renPolydata;
//	mitk::Surface::Pointer mitkSurface = mitk::Surface::New();
//
//	for ( int iCount = 0 ; iCount < dataEntity->GetNumberOfTimeSteps() ; iCount++ )
//	{
//		renPolydata = vtkPolyDataPtr::New();
//		dataEntity->GetProcessingData( meshPtr, iCount );
//		filter->SetInput( meshPtr );
//		filter->Update();
//
//// 		if (abs(filter->GetOutput()->GetBounds()[1]-filter->GetOutput()->GetBounds()[0 ]) < 0.10 &&
//// 			abs(filter->GetOutput()->GetBounds()[3]-filter->GetOutput()->GetBounds()[2 ])< 0.10 &&
//// 			abs(filter->GetOutput()->GetBounds()[5]-filter->GetOutput()->GetBounds()[4 ]) < 0.10 )
//// 			blVTKHelperTools::ScaleShape( 
//// 			filter->GetOutput(), 
//// 			renPolydata, 
//// 			100 );
//// 		else
//		renPolydata->DeepCopy(filter->GetOutput());
//		mitkSurface->SetVtkPolyData( renPolydata, iCount );
//	}
//
//	dataEntity->SetRenderingData( mitk::BaseData::Pointer(mitkSurface) );
}
