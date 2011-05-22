/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meVTKManualNeckCuttingFilter_h
#define _meVTKManualNeckCuttingFilter_h

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataAlgorithm.h>

// BaseLib
#include "BaseLibWin32Header.h"
#include "meMeshTypes.h"

/**
\brief Manual Neck Cutting for an input mesh

\author Chiara Riccobene 
\date 10 Nov 2009
*/

class  meVTKManualNeckCuttingFilter: public vtkPolyDataAlgorithm
{
	public:
		static meVTKManualNeckCuttingFilter *New();


	protected:
		
		//! brief Default Constructor.
		meVTKManualNeckCuttingFilter();

		//! Destructor.
		virtual ~meVTKManualNeckCuttingFilter();

		virtual int RequestData(vtkInformation *, 
			vtkInformationVector **, vtkInformationVector *);


	private:

		//! Purposely not implemented
		meVTKManualNeckCuttingFilter( const meVTKManualNeckCuttingFilter& );	

		//! Purposely not implemented
		void operator = ( const meVTKManualNeckCuttingFilter& );	

		/**
		Mark the cells of inputMeshOriented with 1 if they are in the border 
		with landmarksLine
		*/
		void LabelBorderCells( 
			vtkSmartPointer<vtkPolyData> landmarksLine, 
			vtkSmartPointer<vtkPolyData> inputMeshOriented );

		/**
		Mark the cells of inputMeshOriented with 2 if they are between two
		selected cells with (1)
		*/
		void LabelNeighborCells( 
			vtkSmartPointer<vtkPolyData> landmarksLine, 
			vtkSmartPointer<vtkPolyData> inputMeshOriented );

		//! Get the Point ID of the input points for the currentLineId
		void GetInputMeshIDs( 
			vtkSmartPointer<vtkPolyData> landmarksLine, 
			vtkSmartPointer<vtkPolyData> inputMeshOriented, 
			vtkIdType currentLineId, 
			vtkIdType &pointID1InputMesh, 
			vtkIdType &pointID2InputMesh );

		//! Get the selected cell (1) using two points
		vtkIdType GetSelectedCellID( 
			vtkSmartPointer<vtkPolyData>  inputMeshOriented, 
			vtkIdType pointID1InputMesh, 
			vtkIdType pointID2InputMesh );

	
		//! Get the third point
		vtkIdType GetThirdPoint( 
			vtkSmartPointer<vtkPolyData> inputMeshOriented,
			vtkIdType pointID1, 
			vtkIdType pointID2, 
			vtkIdType cellID );

		//! Threshold and create two surfaces, and get the first region
		vtkSmartPointer<vtkPolyData> GetFirstComponent( 
			vtkSmartPointer<vtkPolyData> landmarksLine, 
			vtkSmartPointer<vtkPolyData> inputMeshOriented );

		//! Threshold and create two surfaces, and get the second region
		vtkSmartPointer<vtkPolyData> GetSecondComponent( 
			vtkSmartPointer<vtkPolyData> landmarksLine, 
			vtkSmartPointer<vtkPolyData> inputMeshOriented );

};


#endif // _meVTKManualNeckCuttingFilter_h
