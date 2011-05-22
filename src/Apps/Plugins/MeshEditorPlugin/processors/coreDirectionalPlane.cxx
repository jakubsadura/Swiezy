/*=========================================================================

  Program   : GIMIAS
  Module    : $RCSfile: DirectionalPlane.cpp,v $
  Language  : C++
  Date      : $Date: 2008-07-14 14:39:25 $
  Version   : $Revision: 1.1.2.6 $

  Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), 
  Barcelona, Spain. http:://www.cilab.upf.edu

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

	//---------------------------------------------------
	// HEADERS
	//---------------------------------------------------

	#include "coreDirectionalPlane.h"

	// CoreLib
	#include "coreAssert.h"
	#include "coreReportExceptionMacros.h"
	#include "coreDataEntityHelper.h"

	// VTK
	#include "vtkPlaneSource.h"
	#include "vtkLineSource.h"
	#include "vtkConeSource.h"
	#include "vtkMath.h"
	#include "vtkAppendPolyData.h"

	// BaseLib
	#include "blShapeUtils.h"

	//---------------------------------------------------
	// OPERATIONS
	//---------------------------------------------------

using namespace Core;

DirectionalPlane::DirectionalPlane( )
{
	try{

		//----------------------------------------------------
		m_iDirection = 0;
		m_NormalVector[ 0 ] = 0;
		m_NormalVector[ 1 ] = 0;
		m_NormalVector[ 2 ] = 0;
		m_bIsValidPlane = false;
		m_dCenter[ 0 ] = 0;
		m_dCenter[ 1 ] = 0;
		m_dCenter[ 2 ] = 0;
		m_dRadius = 0;


		//----------------------------------------------------
		// Create VTK objects

		m_planeSource = vtkPlaneSource::New();
		m_lineSource = vtkLineSource::New();
		m_coneSource = vtkConeSource::New();
		
		m_appendFilter = vtkAppendPolyData::New();
		m_appendFilter->AddInput( m_planeSource->GetOutput( ) );
		m_appendFilter->AddInput( m_lineSource->GetOutput( ) );
		m_appendFilter->AddInput( m_coneSource->GetOutput( ) );



	}coreCatchExceptionsAddTraceAndThrowMacro(
		"DirectionalPlane::DirectionalPlane");
}



/**
 */
DirectionalPlane::~DirectionalPlane( )
{
	this->m_planeSource->Delete( );
	this->m_lineSource->Delete( );
	this->m_coneSource->Delete( );
	this->m_appendFilter->Delete( );
}



/**
 */
void DirectionalPlane::UpdateNormalVector( 
						double selectedPoint[3],
						Core::vtkPolyDataPtr skeleton )
{
	try{

		//-------------------------------------------------------
		// Compute normal vector
		m_NormalVector[ 0 ] = 0;
		m_NormalVector[ 1 ] = 0;
		m_NormalVector[ 2 ] = 0;
		vtkIdType	idPoint;
		idPoint = skeleton->FindPoint( 
						selectedPoint[ 0 ], selectedPoint[ 1 ], selectedPoint[ 2 ] );
		vtkIdList	*neighborPoints = vtkIdList::New( );
		blShapeUtils::ShapeUtils::GetNeighborPoints( 
						skeleton,
						idPoint,
						neighborPoints );
		
		if ( neighborPoints->GetNumberOfIds() == 2 )
		{
			blShapeUtils::ShapeUtils::GetTangentVector( 
				skeleton,
				neighborPoints->GetId( ( 0 + m_iDirection ) % 2 ),
				neighborPoints->GetId( ( 1 + m_iDirection ) % 2 ),
				m_NormalVector );
			m_bIsValidPlane = true;
		}
		else
		{
			m_bIsValidPlane = false;
		}
		neighborPoints->Delete();

	}coreCatchExceptionsReportAndNoThrowMacro(
		"DirectionalPlane::UpdateNormalVector()");
}



/**
 */
void DirectionalPlane::UpdatePlane( )
{

	try{

		if ( m_bIsValidPlane 
			 && m_dRadius > 0 
			 && ( m_NormalVector[ 0 ] != 0 || m_NormalVector[ 1 ] != 0 || m_NormalVector[ 2 ] != 0 ) )
		{

			//-------------------------------------------------------
			m_planeSource->SetOrigin( 0, 0, 0 );
			m_planeSource->SetPoint1( 2*m_dRadius, 0, 0 );
			m_planeSource->SetPoint2( 0, 2*m_dRadius, 0 );
			m_planeSource->SetCenter( m_dCenter[ 0 ], m_dCenter[ 1 ], m_dCenter[ 2 ] );
			m_planeSource->SetNormal( 
							m_NormalVector[ 0 ], 
							m_NormalVector[ 1 ], 
							m_NormalVector[ 2 ] );


			//-------------------------------------------------------
			// Draw line and cone
			double center[3];
			m_planeSource->GetCenter(center);
			m_lineSource->SetPoint1(center);

			// Create the normal vector
			double p2[3];
			m_planeSource->GetNormal( m_NormalVector );

			vtkMath::Normalize( m_NormalVector );
			double d = sqrt( vtkMath::Distance2BetweenPoints(
					m_planeSource->GetPoint1(),
					m_planeSource->GetPoint2() ) );

			p2[0] = center[0] + 0.35 * d * m_NormalVector[0];
			p2[1] = center[1] + 0.35 * d * m_NormalVector[1];
			p2[2] = center[2] + 0.35 * d * m_NormalVector[2];
			m_lineSource->SetPoint2(p2);
			m_coneSource->SetCenter(p2);
			m_coneSource->SetDirection( m_NormalVector );
			m_coneSource->SetHeight( m_dRadius );
			m_coneSource->SetAngle( 30 );
			m_coneSource->SetRadius( m_dRadius / 4 );
		}


	}coreCatchExceptionsReportAndNoThrowMacro(
		"DirectionalPlane::UpdatePlane()");
}



/**
 */
void DirectionalPlane::SetDirection( int iDirection )
{
	m_iDirection = iDirection;
	UpdatePlane();
}



/**
 */
Core::vtkPolyDataPtr DirectionalPlane::GetVtkPolyData()
{
	Core::vtkPolyDataPtr	polyData = NULL;

	try{

		m_appendFilter->Update();
		polyData = m_appendFilter->GetOutput();

	}coreCatchExceptionsReportAndNoThrowMacro(
		"DirectionalPlane::OnPointerChanged()");

	return polyData;
}



/**
 */
void DirectionalPlane::GetNormalVector( double normal[3] )
{
	normal[ 0 ] = m_NormalVector[ 0 ];
	normal[ 1 ] = m_NormalVector[ 1 ];
	normal[ 2 ] = m_NormalVector[ 2 ];
}



/**
 */
void DirectionalPlane::SetNormalVector( double normal[3] )
{
	m_NormalVector[ 0 ] = normal[ 0 ];
	m_NormalVector[ 1 ] = normal[ 1 ];
	m_NormalVector[ 2 ] = normal[ 2 ];
}


/**
 */
bool DirectionalPlane::IsValidPlane( )
{
	return m_bIsValidPlane;
}



/**
 */
void DirectionalPlane::SetValidPlane( bool bVal )
{
	m_bIsValidPlane = bVal;
	UpdatePlane();
}



/**
 */
void DirectionalPlane::SetCenter( double dCenter[3] )
{
	m_dCenter[0] = dCenter[0];
	m_dCenter[1] = dCenter[1];
	m_dCenter[2] = dCenter[2];
	UpdatePlane();
}



/**
 */
void DirectionalPlane::GetCenter( double dCenter[3] )
{
	dCenter[0] = m_dCenter[0];
	dCenter[1] = m_dCenter[1];
	dCenter[2] = m_dCenter[2];
}



/**
 */
void DirectionalPlane::SetRadius( double dRadius )
{
	m_dRadius = dRadius;
	if ( m_dRadius == 0 )
	{
		m_bIsValidPlane = false;
	}
	UpdatePlane();
}



/**
 */
double DirectionalPlane::GetRadius( )
{
	return m_dRadius;
}



/**
 */
void DirectionalPlane::PrintSelf(std::ostream& os) const
{
	os << "m_iDirection" << m_iDirection << std::endl;
	os << "m_dCenter" 
		<< m_dCenter[ 0 ] << " "
		<< m_dCenter[ 1 ] << " "
		<< m_dCenter[ 2 ] << " "
		<< std::endl;
	os << "m_dRadius" << m_dRadius << std::endl;
	os << "m_dCenter" 
		<< m_NormalVector[ 0 ] << " "
		<< m_NormalVector[ 1 ] << " "
		<< m_NormalVector[ 2 ] << " "
		<< std::endl;
	os << "m_bIsValidPlane" << m_bIsValidPlane << std::endl;

}
