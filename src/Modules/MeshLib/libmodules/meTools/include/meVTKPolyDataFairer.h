/*=========================================================================

  Module    : MeshLib
  File      : $RCSfile: $
  Copyright : (C)opyright (Universitat Pompeu Fabra) 2007++
			  See COPYRIGHT statement in top level directory.
  Authors   : Xavier Mellado
  Modified  : $Author: $
  Purpose   : meVTKPolyDataFairer definition.
  Date      : $Date: $
  Version   : $Revision: $
  Changes   : $Locker: $
			  $Log: $

=========================================================================*/

#ifndef _meVTKPolyDataFairer_h
#define _meVTKPolyDataFairer_h

// STD
#include <vector>

// VTK
#include <vtkType.h>
#include <vtkPolyData.h>

// ITK
#include <itkLightObject.h>


using namespace itk;
using namespace std;


/// \brief VTK mesh fairing method.
///
/// Input are VTK polydata. First input is the surrounding mesh, second input is mesh to be faired (patching mesh).
/// Output is second input VTK polydata faired as described in reference.
///
/// IMPORTANT: Umbrella operator implemented is described at reference [1], "umbrella vector".
/// The one mentioned in [6] is not implemented, "umbrella vector" norm.
/// It seems that [1] give better results than [2].
///
/// References:
/// (1) Leif Kobbelt, Swen Campagna, Jens Vorsatz, and Hans-Peter Seidel.
/// Interactive Multiresolution Modeling on Arbitrary Meshes.
/// SIGGRAPH 98 Conference Proceedings.
/// Section: 3.1 The umbrella algorithm.
///
/// (2)Liepa, P. 2003. Filling Holes in Meshes.
/// In Proceedings of the Eurographic Symposium on Geometry Processing, 200-206.
/// Section: 6. Fairing.
///
/// TODO:
///
/// BUGS:
/// 

class meVTKPolyDataFairer: public LightObject
{
	public:

		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef meVTKPolyDataFairer Self;
		typedef SmartPointer< Self > Pointer;


		//-----------------------------------------------------
		// Methods
		//-----------------------------------------------------

		/// \brief Static constructor.
		itkFactorylessNewMacro( Self );

		/// \brief Set surrounding and patch meshes.
		void SetInput( vtkPolyData *inputMesh, vtkPolyData *inputPatch )
			{
				this->surrounding->DeepCopy( inputMesh );
				this->patch->DeepCopy( inputPatch );
			}

		/// \brief Set umbrella operator order. Default[2].
		///
		/// Order can be 0, 1 or 2. Order 0 is NO fairing. Order 1 homogenize triangle coordinates.
		/// It is good for adding flat caps and optimize triangle size. Order 2 homogenize triangle curvatures,
		/// so boundary curvature is propagated towards the inside of the patch.
		void SetOrder( const unsigned int inputOrder )
			{ this->order = inputOrder; }

		/// \brief Set number of fairing iterations. If it is set different from 0,
		/// it will used instead of average percentage movement. Default [0].
		void SetNumberOfIterations( const unsigned int inputNumberOfIterations )
			{ this->numberOfIterations = inputNumberOfIterations; }

		/// \brief Set percentage of initial average movement to stop iterating.
		/// Note default settings make this criterion default one. Default [0.0001] (0.01%).
		void SetPercentage( const double inputPercentage )
			{ this->percentage = inputPercentage; }

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

		typedef vector<double> point3D;

		/// \brief Default Constructor
		meVTKPolyDataFairer();

		/// \brief Destructor
		virtual ~meVTKPolyDataFairer();


		bool ComputePatchBoundaryPoints();
		void ComputeFirstOrderUmbrellaFunction();
		void FirstOrderUmbrellaOperator( vtkIdType, vtkPolyData *, bool, vector<double>&, unsigned int & );
		double ApplyUmbrellaFunction();
		void FindPointNeighbours( vtkIdType, vtkPolyData *, bool, vtkIdList * );
		void ComputeSecondOrderUmbrellaFunction();
		void SecondOrderUmbrellaOperator( vtkIdType, vtkPolyData *, vector<double>& );
		double ComputeDiagonalElement( vtkIdType, vtkPolyData * );

		void PrintPolyDataInfo( vtkPolyData * );


		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------

		vtkPolyData *surrounding, *patch;
		vtkPolyData *output;
		vector< vtkIdType > patchPoint;
		vector< vtkIdType > neighbourPoints;
		vector< point3D > firstOrderUmbrella;
		vector< point3D > secondOrderUmbrella;
		unsigned int order;
		unsigned int numberOfIterations;
		double percentage;
		double computationTime;
		bool m_Verbose;

	private:

		meVTKPolyDataFairer( const meVTKPolyDataFairer& );  //purposely not implemented
		void operator = ( const meVTKPolyDataFairer& );  //purposely not implemented
};

#endif // _meVTKPolyDataFairer_h
