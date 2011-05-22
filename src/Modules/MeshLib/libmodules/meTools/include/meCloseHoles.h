/*
* Copyright (c) 2008,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __meCloseHoles_h
#define __meCloseHoles_h

//BaseLib library
#include <blLightObject.h>
#include <blSmartPointer.h>
#include <blMacro.h>

//#include "MeshLibWin32Header.h" 

//vtk library
#include <vtkFeatureEdges.h>
#include <vtkIdList.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkTriangleFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkPolyDataCollection.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkPolyDataNormals.h>

//Standard
#include <vector>
/**
 This class (previously geoCloseHoles) implements different methods for clossing holes in a 
 vtkPolyData mesh.
				
\ingroup meshLib
\author Jose M. Pozo
\date 2008-02-13
*/

using namespace std;

class  meCloseHoles : public blLightObject
{
public:
	//-----------------------------------------------------
	// Typedefs
	//-----------------------------------------------------

	typedef meCloseHoles Self;
	typedef blSmartPointer< Self > Pointer;
	blNewMacro(Self);

	enum closeHoleAlgorithmEnum
	{
		SINGLE_CENTER_POINT,
		RADIAL_TRIANGLES,
		LINEAR_TO_CENTER_NONREDUCING_TRIANGLES,
		LINEAR_TO_CENTER,
		SMOOTH_TO_CENTER,
		SMOOTH_PATCH
	};
    		
	//-----------------------------------------------------
	// Methods
	//-----------------------------------------------------

	void SetInput(vtkPolyData* input)
	{
		this->input = input;
	}

	void SetInputHole(vtkPolyData* inputHole)
	{
		this->inputHole = inputHole;
	}

	vtkPolyData* GetOutput()
	{
		return this->output;
	}

	std::vector<vtkPolyData*> GetPatchOutput()
	{
		return this->patchOutput;
	}

	void Update();

	void Run();

	void SetAlgorithm(closeHoleAlgorithmEnum algoritm)
	{
		this->closeHoleAlgorithm = algoritm;
	}
	void SetAlgorithmToSingleCenterPoint()
	{
		this->closeHoleAlgorithm = SINGLE_CENTER_POINT;
	}
	void SetAlgorithmToRadialTriangles()
	{
		this->closeHoleAlgorithm = RADIAL_TRIANGLES;
	}
	void SetAlgorithmToLinearToCenterNonReducingTriangles()
	{
		this->closeHoleAlgorithm = LINEAR_TO_CENTER_NONREDUCING_TRIANGLES;
	}
	void SetAlgorithmToLinearToCenter()
	{
		this->closeHoleAlgorithm = LINEAR_TO_CENTER;
	}
	void SetAlgorithmToSmoothToCenter()
	{
		this->closeHoleAlgorithm = SMOOTH_TO_CENTER;
	}
	void SetAlgorithmToSmoothPatch()  // Non-operative yet.
	{
		this->closeHoleAlgorithm = SMOOTH_PATCH;
	}


	void ComputePatchForHole(vtkPolyDataNormals* normals, 
							vtkPoints* points, 
							vtkCellArray* polys);

protected:
	
	/// \brief Default Constructor
	meCloseHoles();

	/// \brief Destructor
	virtual ~meCloseHoles();

private:

	meCloseHoles(const meCloseHoles&);  // Not implemented.
	void operator=(const meCloseHoles&); // Not implemented.
    
	//-----------------------------------------------------
	// Variables
	//-----------------------------------------------------

	vtkPolyData *input,
				*output,
				*inputHole;
	std::vector<vtkPolyData*> patchOutput;

	closeHoleAlgorithmEnum closeHoleAlgorithm;

	//-----------------------------------------------------
	// Private functions
	//-----------------------------------------------------

};

#endif


