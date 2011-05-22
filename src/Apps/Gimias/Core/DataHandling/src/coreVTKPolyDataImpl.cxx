/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVTKPolyDataImpl.h"

Core::VtkPolyDataImpl::VtkPolyDataImpl( ) 
{
	ResetData();
}

Core::VtkPolyDataImpl::~VtkPolyDataImpl()
{

}

boost::any Core::VtkPolyDataImpl::GetDataPtr() const
{
	return m_Data;
}

bool Core::VtkPolyDataImpl::SetDataPtr( boost::any val )
{
	bool result = false;

	if ( val.type() == typeid( vtkPolyDataPtr ) )
	{
		m_Data = boost::any_cast<vtkPolyDataPtr> ( val );
		result = true;
	}
	else if ( val.type() == typeid( vtkPolyData* ) )
	{
		vtkPolyData* processingDataCasted;
		processingDataCasted = boost::any_cast<vtkPolyData*> ( val );
		m_Data = vtkPolyDataPtr( processingDataCasted );
		result = true;
	}

	return result;
}

bool Core::VtkPolyDataImpl::IsValidType( const std::type_info &type )
{
	return ( type == typeid( vtkPolyDataPtr ) ||
			 type == typeid( vtkPolyData* ) );
}

void Core::VtkPolyDataImpl::ResetData()
{
	m_Data = vtkSmartPointer<vtkPolyData>::New();
}

bool Core::VtkPolyDataImpl::SetData( 
	blTagMap::Pointer tagMap, ImportMemoryManagementType mem/* = gmCopyMemory*/ )
{
	blTag::Pointer tagDataPtr = tagMap->FindTagByName( "DataPtr" );
	if ( tagDataPtr.IsNotNull() )
	{
		vtkPolyDataPtr srcData;
		if ( tagDataPtr->GetValue<vtkPolyDataPtr>( srcData ) )
		{
			m_Data->DeepCopy( srcData );
			return true;
		}
	}

	blTag::Pointer tagPoints = tagMap->FindTagByName( "Points" );
	if ( tagPoints.IsNull() )
	{
		return false;
	}
	blTag::Pointer tagSurfaceElements = tagMap->FindTagByName( "SurfaceElements" );
	if ( tagSurfaceElements.IsNull() )
	{
		return false;
	}

	std::vector<Point3D>* points;
	points = tagPoints->GetValueCasted< std::vector<Point3D>* >();
	std::vector<SurfaceElement3D>* surfaceElements;
	surfaceElements = tagSurfaceElements->GetValueCasted< std::vector<SurfaceElement3D>* >();

	vtkSmartPointer<vtkPoints>	pointsVtk = vtkSmartPointer<vtkPoints>::New( );
	pointsVtk->SetDataTypeToDouble( );
	pointsVtk->SetNumberOfPoints( points->size() );
	for ( unsigned iNumPoint = 0 ; iNumPoint < points->size() ;iNumPoint++ )
	{
		pointsVtk->SetPoint( iNumPoint, (*points)[ iNumPoint ][ 0 ], (*points)[ iNumPoint ][ 1 ], (*points)[ iNumPoint ][ 2 ] );
	}
	m_Data->SetPoints( pointsVtk );

	// Surface elements
	vtkSmartPointer<vtkCellArray>	cellArray = vtkSmartPointer<vtkCellArray>::New( );
	SurfaceElement3D	surfaceElement;
	for ( unsigned i = 0 ; i < surfaceElements->size() ; i++ )
	{
		surfaceElement = (*surfaceElements)[ i ];
		if ( !surfaceElement.empty() )
		{
			std::vector<vtkIdType> pointsId;
			pointsId.resize( surfaceElement.size() );
			for ( int j = 0 ; j < pointsId.size() ; j++ )
			{
				pointsId[ j ] = surfaceElement[ j ];
			}
			cellArray->InsertNextCell( pointsId.size(), &pointsId[ 0 ] );
		}
	}
	m_Data->SetPolys( cellArray );

	return true;
}

bool Core::VtkPolyDataImpl::GetData( blTagMap::Pointer tagMap )
{
	if ( GetGenerateTemporalData() )
	{
		Point3D point;
		point.resize( 3 );
		m_Points.resize( m_Data->GetNumberOfPoints() );

		for (	vtkIdType pointId = 0; 
			pointId < m_Data->GetNumberOfPoints(); 
			pointId++ )
		{
			m_Data->GetPoint( pointId, &point[ 0 ] );
			m_Points[ pointId ] = point;
		}


		// Surface elements
		vtkCell			*pCell;
		SurfaceElement3D	surfaceElement;


		m_SurfaceElements.resize( m_Data->GetNumberOfCells() );


		for (	vtkIdType cellId = 0; 
			cellId < m_Data->GetNumberOfCells(); 
			cellId++ )
		{
			pCell = m_Data->GetCell( cellId );
			surfaceElement.resize( pCell->GetNumberOfPoints() );
			for (	int iPoint = 0; 
				iPoint < pCell->GetNumberOfPoints(); 
				iPoint++ )
			{
				vtkIdType pointId = pCell->GetPointId( iPoint );
				surfaceElement[ iPoint ] = pointId;
			}

			m_SurfaceElements[ cellId ] = surfaceElement;
		}
		tagMap->AddTag( "Points", &m_Points );
		tagMap->AddTag( "SurfaceElements", &m_SurfaceElements );
	}

	tagMap->AddTag( "DataPtr", m_Data );

	return true;
}

void* Core::VtkPolyDataImpl::GetVoidPtr() const
{
	void* ptr; ptr = m_Data; return ptr;
}

void Core::VtkPolyDataImpl::SetVoidPtr( void* ptr )
{
	m_Data = reinterpret_cast<vtkPolyData*> ( ptr );
}

void Core::VtkPolyDataImpl::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<vtkPolyDataPtr> ( val );
}

void Core::VtkPolyDataImpl::CleanTemporalData()
{
	m_Points.clear();
	m_SurfaceElements.clear();
}

