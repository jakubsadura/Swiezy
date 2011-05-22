/*=========================================================================

  Module    : MeshLib
  File      : $RCSfile: $
  Copyright : (C)opyright (Universitat Pompeu Fabra) 2007++
			  See COPYRIGHT statement in top level directory.
  Authors   : Xavier Mellado
  Modified  : $Author: $
  Purpose   : meVTKPolyDataEdgeSwapper definition.
  Date      : $Date: $
  Version   : $Revision: $
  Changes   : $Locker: $
			  $Log: $

=========================================================================*/

#ifndef _meVTKPolyDataEdgeSwapper_h
#define _meVTKPolyDataEdgeSwapper_h

// STD
#include <vector>

// VTK
#include <vtkPolyData.h>
#include <vtkEdgeTable.h>

// ITK
#include <itkLightObject.h>

//#include "EvoLibWin32Header.h"


using namespace itk;
using namespace std;


/// \brief VTK polydata edge swapper method. Used mainly in meVTKPolyDataRefining.
///
/// Input is a VTK polydata.
/// IMPORTANT: IN PLACE filter, input polydata is modified directly without doing a copy.
///
/// References:
/// Liepa, P. 2003. Filling Holes in Meshes. In Proceedings of the Eurographic Symposium on Geometry Processing, 200-206.
///
/// TODO:
///       Check criterions with Jose Maria
///       Add exceptions instead of error messages
///
/// BUGS:
///       (Solved) Problems with normals in edge swapping curvature criterion?
///       (Solved) Coplanar condition is wrong. Problem with small vectors and determinant.
/// 

class meVTKPolyDataEdgeSwapper: public LightObject
{
	public:

		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef meVTKPolyDataEdgeSwapper Self;
		typedef SmartPointer< Self > Pointer;


		//-----------------------------------------------------
		// Methods
		//-----------------------------------------------------

		/// \brief Static constructor.
		itkFactorylessNewMacro( Self );

		/// \brief Set the input polydata to be modified IN PLACE. NO copy done.
		void SetInput( vtkPolyData *inputMesh )
			{ this->input = inputMesh; }

		/// \brief Set the maximum angle in RADIANS between to triangles to allow edge swapping. Default [0.0] (coplanars).
		void SetAngle( const double inputAngle )
			{ this->angle = inputAngle; }

		/// \brief Set the minimum number of edges needed on a interior edges relaxation
		/// to continue relaxing in next iteration. Default [10].
		void SetMinimumEdgesSwapped( const vtkIdType inputMinimumEdgesSwapped )
			{ this->minimumEdgesSwapped = inputMinimumEdgesSwapped; }

		/// \brief Set the epsilon angle in RADIANS between to triangles to consider them coplanars. Default [1.0e-03].
		void SetEpsilonAngle( const double inputEpsilonAngle )
			{ this->epsilonAngle = inputEpsilonAngle; }

		/// \brief Set the epsilon absolut determinant value to consider two triangles coplanars. Default [1.0e-03].
		void SetEpsilonDeterminant( const double inputEpsilonDeterminant )
			{ this->epsilonDeterminant = inputEpsilonDeterminant; }

		/// \brief Get the computation time (in seconds).
		double GetComputationTime()
			{ return this->computationTime; };

		bool RelaxEdge( vtkIdType, vtkIdType, vtkIdType, vtkEdgeTable * = NULL );

		/// \brief Compute mesh filter.
		void Update();


	protected:
		
		/// \brief Default Constructor
		meVTKPolyDataEdgeSwapper();

		/// \brief Destructor
		virtual ~meVTKPolyDataEdgeSwapper();

		bool FindOppositePoint( vtkIdType, vtkIdType, vtkIdType, vtkIdType & );
		void RelaxAllInteriorEdges();
		bool CoplanarTriangles( double *, double *, double *, double *, double *, double * );
		double AngleBetweenTriangles( double *, double *, double *, double *, double *, double *, double *, double * );
		bool SwapEdgeCoplanarCriterion( double *, double *, double *, double *, double * );
		bool SwapEdgeCurvatureCriterion( double *, double *, double *, double * );


		void PrintPolyDataInfo( vtkPolyData * );


		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------

		vtkPolyData *input;
		double densityFactor;
		double angle;
		double computationTime;
		double epsilonAngle;
		double epsilonDeterminant;
		vtkIdType minimumEdgesSwapped;


	private:

		meVTKPolyDataEdgeSwapper( const meVTKPolyDataEdgeSwapper& );  //purposely not implemented
		void operator = ( const meVTKPolyDataEdgeSwapper& );  //purposely not implemented
};

#endif // _meVTKPolyDataEdgeSwapper_h
