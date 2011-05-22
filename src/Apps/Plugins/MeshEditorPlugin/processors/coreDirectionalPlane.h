/*=========================================================================

  Program   : GIMIAS
  Module    : $RCSfile: DirectionalPlane.h,v $
  Language  : C++
  Date      : $Date: 2008-05-07 16:04:13 $
  Version   : $Revision: 1.1.2.4 $

  Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), 
  Barcelona, Spain. http:://www.cilab.upf.edu

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef DirectionalPlane_H
#define DirectionalPlane_H


// MITK
#include "mitkDataTreeNode.h"
#include "mitkDataTree.h"
#include "coreDataEntityHelper.h"
#include "gmProcessorsWin32Header.h"

class vtkPlaneSource;
class vtkLineSource;
class vtkConeSource;
class vtkAppendPolyData;

namespace Core{


/**
 * \brief vtkPlaneSource with a normal arrow
 * \ingroup MeshPlugin
 *
 * The normal vector is computed using only two neighbors. 
 *
 * \note If there's more than two or less neighbors then normal = {0,0,0}
 *
 */
	class PLUGIN_EXPORT DirectionalPlane
	{
	
	// TYPE DEFINITIONS
	
	public:
		typedef DirectionalPlane Self;


	
	// OPERATIONS
	
	public:
		//! 
		DirectionalPlane( );

		~DirectionalPlane( );

		//! Set member variable and update plane
		void SetCenter( double dCenter[3] );

		void GetCenter( double dCenter[3] );

		//! Set member variable and update plane
		void SetRadius( double dRadius );

		//! Get member variable 
		double GetRadius( );

		//! Set member variable and update plane
		void SetDirection( int iDirection );

		//! Get the vtkPolyData of the plane
		Core::vtkPolyDataPtr GetVtkPolyData();

		void GetNormalVector( double normal[3] );

		void SetNormalVector( double normal[3] );

		bool IsValidPlane( );

		//! Set member variable and update plane
		void SetValidPlane( bool bVal );

		//! Update normal vector position using a point and the skeleton
		void UpdateNormalVector( 
						double selectedPoint[3],
						Core::vtkPolyDataPtr skeleton );

		void PrintSelf(std::ostream& os) const;




	
	// PRIVATE OPERATIONS
	
	private:

		/// Update plane position according to the center
		void UpdatePlane( );



	
	// ATTRIBUTES
	
	private:
		
		//@{ 
		/// \name Input data

		/// Direction of the normal: 0 first neighbor to second, 1 second to first ...
		int m_iDirection;

		/// Center of the plane
		double m_dCenter[3];

		/// Radius of the plane
		double m_dRadius;
		//@}


		/// Normalized normal vector
		double m_NormalVector[ 3 ];

		/// If normal vector or center cannot be computed, it is false
		bool m_bIsValidPlane;


		
		//@{ 
		/// \name VTK objects

		vtkPlaneSource  *m_planeSource;
		vtkLineSource	*m_lineSource;
		vtkConeSource	*m_coneSource;

		vtkAppendPolyData*	m_appendFilter;

		//@}

	};
} // namespace Core
#endif //DirectionalPlane_H

