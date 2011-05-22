/*=========================================================================

  Module    : MeshLib
  File      : $RCSfile: $
  Copyright : (C)opyright (Universitat Pompeu Fabra) 2007++
			  See COPYRIGHT statement in top level directory.
  Authors   : Xavier Mellado
  Modified  : $Author: $
  Purpose   : meVTKPolyDataRefiner definition.
  Date      : $Date: $
  Version   : $Revision: $
  Changes   : $Locker: $
			  $Log: $

=========================================================================*/

#ifndef _meVTKPolyDataRefiner_h
#define _meVTKPolyDataRefiner_h

// STD
#include <vector>

// VTK
#include <vtkPolyData.h>
#include <vtkEdgeTable.h>

// ITK
#include <itkLightObject.h>

// MeshLib
#include <meVTKPolyDataEdgeSwapper.h>
//#include "EvoLibWin32Header.h"


using namespace itk;
using namespace std;


/// \brief VTK mesh refinement method.
///
/// Input is a VTK polydata.
/// Output is input VTK polydata refined as described in reference.
///
/// References:
/// Liepa, P. 2003. Filling Holes in Meshes. In Proceedings of the Eurographic Symposium on Geometry Processing, 200-206.
/// Section: 5. Mesh refinement.
///
/// TODO:
///       Complete check of mesh trip between functions
///       Complete memory check
///       General comments and code cleaning
///       Add exceptions instead of error messages
///
/// BUGS:
/// 

class meVTKPolyDataRefiner: public LightObject
{
	public:

		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef meVTKPolyDataRefiner Self;
		typedef SmartPointer< Self > Pointer;


		//-----------------------------------------------------
		// Methods
		//-----------------------------------------------------

		/// \brief Static constructor.
		itkFactorylessNewMacro( Self );

		/// \brief Set the input.
		void SetInput( vtkPolyData *inputMesh )
			{ this->input->DeepCopy( inputMesh ); }

		/// \brief Set the density factor for subdivision (alpha parameter in reference). Default [sqrt(2.0)].
		void SetDensityFactor( const double inputDensityFactor )
			{ this->densityFactor = inputDensityFactor; }

		/// \brief Set the maximum angle in radians between to triangles to allow edge swapping. Default [0.0] (coplanars).
		void SetAngle( const double inputAngle )
			{ this->angle = inputAngle; }

		/// \brief Set the minimum number of edges needed in a interior edges relaxation
		/// to continue relaxing in next iteration. Default [10].
		void SetMinimumEdgesSwapped( const unsigned int inputMinimumEdgesSwapped )
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

		/// \brief Get the output.
		vtkPolyData* GetOutput()
			{ return this->output; }

		/// \brief Compute mesh filter.
		void Update();

		bool GetVerbose() const;
		void SetVerbose(bool val);

	protected:
		
		/// \brief Default Constructor
		meVTKPolyDataRefiner();

		/// \brief Destructor
		virtual ~meVTKPolyDataRefiner();

		bool BoundarySigmaInitialization();
		float ComputeBoundarySigma( vtkIdType );

		bool Subdivision();
		bool ComputeSubdivisionAndSwapping( vtkIdType, vtkPolyData* );
		float ComputeSigma( vtkIdType );
		//void CleanPolyDataCopy( vtkPolyData*, vtkPolyData* );

		void PrintPolyDataInfo( vtkPolyData * );


		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------

		vtkPolyData *input;
		vtkPolyData *output;
		meVTKPolyDataEdgeSwapper::Pointer edgeSwapper;
		vector<float> boundarySigma;
		double densityFactor;
		double angle;
		double computationTime;
		double epsilonAngle;
		double epsilonDeterminant;
		unsigned int minimumEdgesSwapped;
		bool m_Verbose;

	private:

		meVTKPolyDataRefiner( const meVTKPolyDataRefiner& );  //purposely not implemented
		void operator = ( const meVTKPolyDataRefiner& );  //purposely not implemented
};

#endif // _meVTKPolyDataRefiner_h
