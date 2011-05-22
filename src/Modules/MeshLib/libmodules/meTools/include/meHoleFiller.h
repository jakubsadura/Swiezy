/*=========================================================================

  Module    : MeshLib
  File      : $RCSfile: $
  Copyright : (C)opyright (Universitat Pompeu Fabra) 2007++
			  See COPYRIGHT statement in top level directory.
  Authors   : Xavier Mellado
  Modified  : $Author: $
  Purpose   : meHoleFiller definition.
  Date      : $Date: $
  Version   : $Revision: $
  Changes   : $Locker: $
			  $Log: $

=========================================================================*/

#ifndef _meHoleFiller_h
#define _meHoleFiller_h

// VTK
#include <vtkPolyData.h>


// ITK
#include <itkLightObject.h>

//#include "EvoLibWin32Header.h"


/// \brief Hole filling method.
///
/// Input is a VTK polydata.
/// Output is input VTK polydata but with all holes filled.
/// This method is based on:
/// Liepa, P. 2003. Filling Holes in Meshes. In Proceedings of the Eurographic Symposium on Geometry Processing, 200-206.
/// But instead of using hole triangulation described on this paper, vtkTriangleFilter is used.

class meHoleFiller: public itk::LightObject
{
	public:

		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef meHoleFiller Self;
		typedef itk::SmartPointer< Self > Pointer;


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

		/// \brief Set the maximum angle in degrees between to triangles to allow edge swapping. Default [0.0] (coplanars).
		void SetAngle( const double inputAngle )
			{ this->angle = inputAngle; }

		/// \brief Set the minimum number of edges needed in a interior edges relaxation
		/// to continue relaxing in next iteration. Default [10].
		void SetMinimumEdgesSwapped( const unsigned int inputMinimumEdgesSwapped )
			{ this->minimumEdgesSwapped = inputMinimumEdgesSwapped; }

		/// \brief Set percentage of initial average movement to stop iterating.
		/// Note default settings make this criterion default one. Default [0.0001] (0.01%).
		void SetPercentage( const double inputPercentage )
			{ this->percentage = inputPercentage; }

		/// \brief Set umbrella operator order. Default[2].
		///
		/// Order can be 0, 1 or 2. Order 0 is NO fairing. Order 1 homogenize triangle coordinates.
		/// It is good for adding flat caps and optimize triangle size. Order 2 homogenize triangle curvatures,
		/// so boundary curvature is propagated towards the inside of the patch.
		/// NOTE: order is applied to ALL holes, do not mix cap closing and hole curvature preservation closing.
		void SetOrder( const unsigned int inputOrder )
			{ this->order = inputOrder; }

		/// \brief Get the output.
		vtkPolyData* GetOutput()
			{ return this->output; }

		/// \brief Compute the image filter.
		void Update();

		bool GetVerbose() const;
		void SetVerbose(bool val);

	protected:
		
		/// \brief Default Constructor
		meHoleFiller();

		/// \brief Destructor
		virtual ~meHoleFiller();

		void PrintPolyDataInfo( vtkPolyData * );


		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------

		vtkPolyData *input;
		vtkPolyData *output;
		double densityFactor, angle, percentage;
		unsigned int order;
		unsigned int minimumEdgesSwapped;

		bool m_Verbose;

	private:

		meHoleFiller( const meHoleFiller& );  //purposely not implemented
		void operator = ( const meHoleFiller& );  //purposely not implemented
};

#endif // _meHoleFiller_h
