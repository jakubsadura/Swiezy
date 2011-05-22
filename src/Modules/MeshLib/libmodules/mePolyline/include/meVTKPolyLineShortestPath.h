#ifndef MEVTKPOLYLINESHORTESTPATH_H
#define MEVTKPOLYLINESHORTESTPATH_H

// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <itkLightObject.h>
#include <vtkPolyData.h>
// #include "CILabExceptionMacros.h"

/**

\author: Martin Bianculli	
\date	30/01/2008
*/

class meVTKPolyLineShortestPath: public itk::LightObject
{
public:
	//-----------------------------------------------------
	// Typedefs
	//-----------------------------------------------------

	typedef meVTKPolyLineShortestPath Self;
	typedef itk::SmartPointer<Self> Pointer;

	//-----------------------------------------------------
	// Methods
	//-----------------------------------------------------

	/// Static constructor
	itkFactorylessNewMacro(Self);

	/// Set input VTK PolyLine to be converted into ITK PolyLine.
	void SetInput( vtkPolyData *input )
	{ 
		this->input = input; 
	}

	/// computes the centerline. A path between them must exist 
	void SetCellsIds(vtkIdType cellId1, vtkIdType cellId2)
	{
		this->cellId1=cellId1;
		this->cellId2=cellId2;
	}

	/// Get output VTK PolyLine, containing the shortest path
	vtkPolyData *GetOutput()
	{ 
		return this->output; 
	}

	///
	void Update();

private:
	///
	meVTKPolyLineShortestPath();
	///
	virtual ~meVTKPolyLineShortestPath();
	
	//-----------------------------------------------------
	// Variables
	//-----------------------------------------------------

	///
	vtkPolyData *input;
	///
	vtkPolyData *output;

	/// Start centerline here
	vtkIdType cellId1;
	/// End centerline here
	vtkIdType cellId2;

private:
	/// purposely not implemented
	meVTKPolyLineShortestPath( const meVTKPolyLineShortestPath& );  
	/// purposely not implemented
	void operator = ( const meVTKPolyLineShortestPath& );
};

#endif // MEVTKPOLYLINESHORTESTPATH_H
