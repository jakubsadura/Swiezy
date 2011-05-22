/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meVTKPolyDataSplitter_h
#define _meVTKPolyDataSplitter_h

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


/// \brief VTK polydata splitter class. It splits a mesh into a patching and a surrounding mesh given a cell id list containing
/// cell ids to be splitted.
///
/// Input is VTK polydata.
/// Output is the VTK polydata splitted. First output is the surrounding mesh, second output is patching mesh resulting
/// from split from input cells given a cell id list.
///
/// TODO:
///
/// BUGS:
/// 

class BASELIBVTK_EXPORT meVTKPolyDataSplitter: public LightObject
{
	public:

		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef meVTKPolyDataSplitter Self;
		typedef SmartPointer< Self > Pointer;


		//-----------------------------------------------------
		// Methods
		//-----------------------------------------------------

		/// \brief Static constructor.
		itkFactorylessNewMacro( Self );

		/// \brief Set surrounding and patch meshes.
		void SetInput( vtkPolyData *inputMesh )
			{ this->input->DeepCopy( inputMesh ); }

		/// \brief Set cell ids list to extract from input mesh.
		void SetCellIdList( vtkIdList *inputList )
			{ this->cellIdList->DeepCopy( inputList ); }

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

		/// \brief Get output meshes.
		void GetOutput( vtkPolyData *& outputSurrounding, vtkPolyData *&outputPatch )
			{
				outputSurrounding = this->surrounding;
				outputPatch = this->patch;
			}

		/// \brief Compute mesh filter.
		void Update();


	protected:

		/// \brief Default Constructor
		meVTKPolyDataSplitter();

		/// \brief Destructor
		virtual ~meVTKPolyDataSplitter();


		bool CheckCellIdList();
		//void ComputePatchMesh();
		void ComputeMeshes();
		void InsertCell( vtkIdType, vtkPoints *, vtkCellArray *, vtkIdList * );


		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------

		vtkPolyData *input;
		vtkPolyData *surrounding, *patch;
		vtkIdList *cellIdList;
		bool tag;
		double computationTime;


	private:

		meVTKPolyDataSplitter( const meVTKPolyDataSplitter& );  //purposely not implemented
		void operator = ( const meVTKPolyDataSplitter& );  //purposely not implemented
};

#endif // _meVTKPolyDataSplitter_h
