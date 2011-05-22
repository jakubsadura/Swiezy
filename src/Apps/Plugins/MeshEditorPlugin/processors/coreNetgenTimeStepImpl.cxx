/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreNetgenTimeStepImpl.h"

Core::NetgenImpl::NetgenImpl( ) 
{
	ResetData();
}

Core::NetgenImpl::~NetgenImpl()
{
}

boost::any Core::NetgenImpl::GetDataPtr() const
{
	return m_NetgenMesh;
}

void Core::NetgenImpl::SetAnyData( boost::any val )
{
	m_NetgenMesh = boost::any_cast<meNetgenMesh::Pointer> (val);
}

void Core::NetgenImpl::ResetData()
{
	m_NetgenMesh = meNetgenMesh::New();
}

bool Core::NetgenImpl::SetData( blTagMap::Pointer tagMap, ImportMemoryManagementType mem )
{
	std::vector<Point3D>* points;
	std::vector<SurfaceElement3D>* surfaceElements;

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

	points = tagPoints->GetValueCasted< std::vector<Point3D>* >();
	surfaceElements = tagSurfaceElements->GetValueCasted< std::vector<SurfaceElement3D>* >();

	ResetData();

	SurfaceElement3D surfaceElement;
	Point3D point[ 3 ];

	for (	unsigned i = 0; 
		i < surfaceElements->size(); 
		i++ )
	{
		surfaceElement = (*surfaceElements)[ i ];
		if ( surfaceElement.size() != 3 )
		{
			throw Core::Exceptions::Exception( 
				"Core::NetgenImpl::SetData", 
				"A Netgen cell can only contain 3 points!" );
		}
		point[ 0 ] = (*points)[ surfaceElement[ 0 ] ];
		point[ 1 ] = (*points)[ surfaceElement[ 1 ] ];
		point[ 2 ] = (*points)[ surfaceElement[ 2 ] ];
		nglib::Ng_STL_AddTriangle( m_NetgenMesh->GetGeom( ),	
			&point[ 0 ][ 0 ], 
			&point[ 1 ][ 0 ], 
			&point[ 2 ][ 0 ] );
	}

	nglib::Ng_STL_InitSTLGeometry( m_NetgenMesh->GetGeom( ) );

	return true;
}

bool Core::NetgenImpl::GetData( blTagMap::Pointer tagMap )
{

	if ( GetGenerateTemporalData() )
	{

		// Surface elements
		SurfaceElement3D surfaceElement;
		surfaceElement.resize( 3 );

		Point3D point;
		point.resize( 3 );

		// If the mesh has been generated, use it, else use the geometry
		if ( nglib::Ng_GetNP( m_NetgenMesh->GetNGMesh() ) != 0 )
		{
			// Add the points
			m_Points.resize( nglib::Ng_GetNP( m_NetgenMesh->GetNGMesh() ) );
			for (	unsigned iNumPoint = 0; 
				iNumPoint < m_Points.size(); 
				iNumPoint++ )
			{
				nglib::Ng_GetPoint( m_NetgenMesh->GetNGMesh( ), iNumPoint + 1, &point[ 0 ] );
				m_Points[ iNumPoint ] = point;
			}

			m_SurfaceElements.resize( nglib::Ng_GetNSE( m_NetgenMesh->GetNGMesh( ) ) );

			for (	unsigned i = 0; 
				i < m_SurfaceElements.size(); 
				i++ )
			{
				nglib::Ng_GetSurfaceElement ( m_NetgenMesh->GetNGMesh( ), i + 1, &surfaceElement[ 0 ] );
				surfaceElement[ 0 ]--;
				surfaceElement[ 1 ]--;
				surfaceElement[ 2 ]--;
				m_SurfaceElements[ i ] = surfaceElement;
			}

		}
		else
		{
			// Add the points
			m_Points.resize( nglib::Ng_STL_GetNumPoints( m_NetgenMesh->GetGeom() ) );
			for (	unsigned iNumPoint = 0; 
				iNumPoint < m_Points.size(); 
				iNumPoint++ )
			{
				nglib::Ng_STL_GetPoint( m_NetgenMesh->GetGeom(), iNumPoint + 1, &point[ 0 ] );
				m_Points[ iNumPoint ] = point;
			}



			m_SurfaceElements.resize( nglib::Ng_STL_GetNumTriangles(  m_NetgenMesh->GetGeom() ) );

			for (	unsigned i = 0; 
				i < m_SurfaceElements.size(); 
				i++ )
			{
				nglib::Ng_STL_GetTriangle (  m_NetgenMesh->GetGeom(), i + 1, &surfaceElement[ 0 ] );
				surfaceElement[ 0 ]--;
				surfaceElement[ 1 ]--;
				surfaceElement[ 2 ]--;
				m_SurfaceElements[ i ] = surfaceElement;
			}
		}

		tagMap->AddTag( "Points", &m_Points );
		tagMap->AddTag( "SurfaceElements", &m_SurfaceElements );
	}

	return true;
}

void Core::NetgenImpl::CleanTemporalData()
{
	m_Points.clear( );
	m_SurfaceElements.clear();
}
