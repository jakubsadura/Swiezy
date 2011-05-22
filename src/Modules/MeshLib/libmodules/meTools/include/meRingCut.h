/*
* Copyright (c) 2008,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef _meRingCut_h
#define _meRingCut_h

//VTK
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkSmartPointer.h>

//BASELIB
#include <blLightObject.h>
#include <blSmartPointer.h>
#include <blMacro.h>

//#include "MeshLibWin32Header.h"

using namespace std;
/**
\brief class to compute shape descriptors
		geoRingCut implementation
\ingroup meshLib
\author Jose M. Pozo
\date 2008/04/17
*/

	
class meRingCut: public blLightObject
{
		public:
	
		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef meRingCut Self;
		typedef blSmartPointer< Self > Pointer;
		
		//-----------------------------------------------------
		// Methods
		//-----------------------------------------------------

		/// \brief Static constructor
		blNewMacro( Self );

		/// \brief Set surface input
		void SetInput( vtkPolyData *Input );

		/// \brief Set skeleton
		void SetSkeleton( vtkPolyData *Skeleton );

		/// \brief Get VTK poly data of the wanted part of the surface
		vtkPolyData *GetFirstOutput();

		/// \brief Get VTK poly data of the unwanted part of the surface
		vtkPolyData *GetSecondOutput();

		/// \brief Cut and split the surface
		void Update();

		void SetSkeletonPoint( int PointId );

		void SetPoint(double point[3]);

		void SetNormal(double normal[3]);

		void SetSkeletonPoint( double skeletonPoint[ 3 ] );

		void SetOriginMeshId(int pointId);

		void SetSecondOriginMeshId(int pointId);

		vtkPolyData *GetContourCrossSection();

		void GetContourCrossSection(vtkPolyData * contour);


		double GetRadius();

		double* GetCenter();

		void GetCenter(double center[3]);

		double* GetNormal();

		void GetNormal(double normal[3]);

		protected:
			
		/// \brief Default Constructor
		meRingCut();

		/// \brief Destructor
		virtual ~meRingCut();


		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------
				/// \brief Input mesh
		vtkSmartPointer<vtkPolyData>	m_Input,
				m_ClosedMesh,
				/// \brief Skeleton
				m_Skeleton,
				/// \brief Wanted surface after the cut
				m_FirstOutput,
				/// \brief Unwanted surface after the cut
				m_SecondOutput,
				m_Intersection;

			/// \brief Identifier of the Skeleton point
		int 	m_PointId,
			meshPointId,
			meshSecondPointId;

			/// \brief Coordinates of the point for defining the cutting ring
		double	m_PointCoordinates[3],
			/// \brief Components of the vector perpendicular to the cutting ring
			m_Normal[3],
			/// \brief Radius defining the cutting ring
			m_Radius;

			/// \brief True if point and tangent have been computed from pointId.
		bool 	m_PointAndTangentComputed,
			meshPointUsed,
			meshSecondPointUsed;

		private:

		meRingCut(const meRingCut&);  //purposely not implemented
		void operator = (const meRingCut&);  //purposely not implemented

		void GetPointCoordinates();
		void GetTangentVector();
		void CloseMesh();
	};

#endif // _meRingCut_h
