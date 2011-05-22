/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blMitkSurface.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"

blMitk::Surface::Surface() : mitk::Surface()
{
	m_ScalarTimeSteps = false;
}

blMitk::Surface::~Surface()
{
}

void blMitk::Surface::SetVtkPolyData( vtkPolyData* polydata, unsigned int t /*= 0*/ )
{
	// If number of scalar arrays is > 10
	if ( polydata->GetPointData() != NULL &&
		polydata->GetPointData()->GetNumberOfArrays() > 10 )
	{
		// Find all arrays with name "activation"
		std::string scalarName = "activation";
		vtkPointData *pointData = polydata->GetPointData();
		for ( int i = 0 ; i < pointData->GetNumberOfArrays( ) ; i++ )
		{
			vtkDataArray *dataArray = pointData->GetArray( i );
			size_t found;
			std::string arrayName( dataArray->GetName() );
			found = arrayName.find( scalarName.c_str() );
			if ( found != std::string::npos )
			{
				m_ArrayNames.push_back( arrayName );
			}
		}

		//m_ScalarTimeSteps true only if the number of activation array is != 0
		m_ScalarTimeSteps = (m_ArrayNames.size()!=0);
	}

	
	if(m_ScalarTimeSteps)
	{
		// Set number of time steps to size of arrayNames
		// Set the same polyData to all time steps
		Expand( m_ArrayNames.size() );

		for ( int i = 0 ; i < m_ArrayNames.size() ; i++ )
		{
			Superclass::SetVtkPolyData( polydata, i );
		}

	}
	else
	{
		Superclass::SetVtkPolyData( polydata, t );
	}

}

vtkPolyData* blMitk::Surface::GetVtkPolyData( unsigned int t /*= 0*/ )
{
	vtkPolyData* polyData = Superclass::GetVtkPolyData( t );

	if ( m_ScalarTimeSteps && polyData )
	{
		polyData->GetPointData()->SetActiveScalars( m_ArrayNames.at(t).c_str() );
	}

	return polyData;
}
