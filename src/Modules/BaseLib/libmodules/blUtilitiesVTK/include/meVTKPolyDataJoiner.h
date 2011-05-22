/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meVTKPolyDataJoiner_h
#define _meVTKPolyDataJoiner_h

#include "BaseLibVTKWin32Header.h"

// STD
#include <vector>

// VTK
#include <vtkType.h>
#include <vtkPolyData.h>
#include <vtkIdList.h>

// ITK
#include <itkLightObject.h>


using namespace itk;
using namespace std;


/// \brief VTK polydata joiner class. It joins a patch to a surrounding mesh returning cell id list containing
/// new cell ids created.
///
/// Input are VTK polydata. First input is the surrounding mesh, second input is mesh to be added (patching mesh).
/// Output is the VTK polydata joined.
///
/// IMPORTANT: This class assumes:
/// 1) surrounding and patching meshes properly match. If not, output will be whatever thing.
/// 2) patching mesh do NOT have holes, that is, there is only one set of adjacent points at boundaries (lika a semisphere).
///
/// TODO:
/// Assumption 1: For the time being, I haven’t found a better solution to map patching mesh boundary points
/// in surrounding mesh, and viceversa, than use the FindPoint() function.
///
/// But this function doesn’t look for an exact match of input point at point set. It looks for nearest neighbour.
/// If the point already exists at mesh, function returns it, otherwise it returns the nearest neighbour
/// (in my opinion, it should seek for an exact match, and another function should seek for nearest neighbours).
/// This is the reason why, if patching and surrounding mesh boundaries don’t match, the algorithm still will work
/// but output could be whatever thing.
///
/// A solution for this will be creation of a function for exact search. But, depending on mesh size,
/// it could be slow for real time interaction. FindPoint() uses octrees and other helping structures
/// to speed-up search process of the nearest neighbour. I am not sure about the real cost of this.
///
/// Assumption 2: could be modified easily at ComputeTwinBoundaryPoints(). But the effect in other filters
/// should be taken into account. If patch have holes perhaps some local filters could fail, check this.
///
/// Modify ComputeTwinBoundaryPoints() is easy instead of treat only a single hole, place everything
/// inside numHoles == 1 condition (except last loop for interior points) in a loop until there are
/// no more holes to add to translation point id vectors.
///
/// BUGS:
/// 

class BASELIBVTK_EXPORT meVTKPolyDataJoiner: public LightObject
{
	public:

		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef meVTKPolyDataJoiner Self;
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

		/// \brief Set tag added cells. Default[false].
		void SetTagging( bool inputTag )
			{ this->tag = inputTag; };

		/// \brief Set tag added cells.
		void SetTaggingOn( )
			{ this->tag = true; };

		/// \brief Set not tag added cells.
		void SetTaggingOff( )
			{ this->tag = false; };

		/// \brief Get the computation time (in seconds).
		double GetComputationTime()
			{ return this->computationTime; };

		/// \brief Get the list of cell ids added to output mesh.
		vtkIdList* GetCellList();

		/// \brief Get output mesh.
		vtkPolyData* GetOutput()
			{ return this->output; }

		/// \brief Compute mesh filter.
		void Update();


	protected:

		/// \brief Default Constructor
		meVTKPolyDataJoiner();

		/// \brief Destructor
		virtual ~meVTKPolyDataJoiner();

		bool ComputeTwinBoundaryPoints();
		bool AddCells();


		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------

		vtkPolyData *surrounding, *patch;
		vtkPolyData *output;
		vtkIdList *cellIdList;
		vector< vtkIdType > twinPoint;
		bool tag;
		double computationTime;


	private:

		meVTKPolyDataJoiner( const meVTKPolyDataJoiner& );  //purposely not implemented
		void operator = ( const meVTKPolyDataJoiner& );  //purposely not implemented
};

#endif // _meVTKPolyDataJoiner_h
