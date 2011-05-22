// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef _blPolyDataDistanceTransformFilter_h
#define _blPolyDataDistanceTransformFilter_h

#include "BaseLibVTKWin32Header.h"

//VTK
#include <vtkImageAlgorithm.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkVoxelModeller.h>
#include <vtkCellLocator.h>
#include <vtkDataArray.h>
#include <vtkCell.h>
#include <vtkPointData.h>
#include <vtkPointLocator.h>
#include <vtkCellData.h>
#include <vtkPoints.h>
#include <vtkMath.h>

#include <time.h>
#include <sys/timeb.h>

#include <blLightObject.h>
#include <blSmartPointer.h>
#include <blMacro.h>
#include "BaseLibVTKWin32Header.h"


/// \brief Distance transform to a vtkPolyData surface.
///
/// This class stores the exact Euclidean distance to the input surface in an image of user defined
/// resolution. Typically, the surface is stored as a vtkPolyData although other VTK representations
/// are also allowed. Once the image grid is defined, the distance from each voxel to the surface is
/// computed looking for the nearest cell (triangle) resulting on an exact Euclidean distance transform.
/// This class also allows for computing the signed distance transform and the option for building
/// an offset from the surface's bounding box.
/// \ingroup blToolsVTK
class BASELIBVTK_EXPORT blPolyDataDistanceTransformFilter: public blLightObject
{
public:

	//-----------------------------------------------------
	// Typedefs
	//-----------------------------------------------------

	typedef blPolyDataDistanceTransformFilter Self;
	typedef blSmartPointer< Self > Pointer;


	//-----------------------------------------------------
	// Methods
	//-----------------------------------------------------

	/// \brief Static constructor
	blNewMacro( Self );

	/// \brief Set the input polydata.
	void SetInput( vtkPolyData *inputPolydata )
	{ this->input->DeepCopy( inputPolydata ); };

	/// \brief Set the voxel dimensions.
	///
	/// Set inc x, inc y, and inc z (voxel dimensions) in millimeters.
	void SetSpacing( double inputSpacing[3] )
	{
		this->spacing[0] = inputSpacing[0];
		this->spacing[1] = inputSpacing[1];
		this->spacing[2] = inputSpacing[2];
	};

	/// \brief Get the voxel dimensions.
	///
	/// Get inc x, inc y, and inc z (voxel dimensions) in millimeters.
	void GetSpacing( double outputSpacing[3] )
	{
		outputSpacing[0] = this->spacing[0];
		outputSpacing[1] = this->spacing[1];
		outputSpacing[2] = this->spacing[2];
	};

	/// \brief Get the voxel dimensions.
	///
	/// Get inc x, inc y, and inc z (voxel dimensions) in millimeters.
	double* GetSpacing()
	{ return this->spacing; };

	/// \brief Set the bounding box offset.
	///
	/// Set the off x, off y, and off z to enlarge the polydata bounding box.
	/// This offset is used to avoid the object boundary meeting with the image size.
	/// Some applications need the object totally embedded inside the image boundaries.
	void SetOffset( double inputOffset[3] )
	{
		this->offset[0] = inputOffset[0];
		this->offset[1] = inputOffset[1];
		this->offset[2] = inputOffset[2];
	};

	/// \brief Get the bounding box offset.
	///
	/// Get the off x, off y, and off z to enlarge the polydata bounding box.
	/// This offset is used to avoid the object boundary meeting with the image size.
	/// Some applications need the object totally embedded inside the image boundaries.
	void GetOffset( double outputOffset[3] )
	{
		outputOffset[0] = this->offset[0];
		outputOffset[1] = this->offset[1];
		outputOffset[2] = this->offset[2];
	};

	/// \brief Get the bounding box offset.
	///
	/// Get the off x, off y, and off z to enlarge the polydata bounding box.
	/// This offset is used to avoid the object boundary meeting with the image size.
	/// Some applications need the object totally embedded inside the image boundaries.
	double* GetOffset()
	{ return this->offset; };

	/// \brief Get the dimensions of the resulting image data.
	void GetDimensions( unsigned int outputDimension[3] )
	{
		outputDimension[0] = this->dimension[0];
		outputDimension[1] = this->dimension[1];
		outputDimension[2] = this->dimension[2];
	};

	/// \brief Get the dimensions of the resulting image data.
	unsigned int* GetDimensions()
	{ return this->dimension; };

	/// \brief Set if signed distance transform computation.
	///
	/// If switched off, all distance values will be positive; otherwise, voxels inside
	/// the object will be negative.
	void SetSigned( bool inputSigned )
	{ this->sign = inputSigned; };

	/// \brief Get if signed distance transform computation.
	///
	/// If switched off, all distance values will be positive; otherwise, voxels inside
	/// the object will be negative.
	bool GetSigned()
	{ return this->sign; };

	/// \brief Set the input polydata.
	vtkImageData* GetOutput()
	{ return this->output; }

	/// \brief Returns computation time (in milliseconds).
	double GetComputationTime()
	{ return this->computationTime; };

	/// \brief Reads a polydata and create a 3D distance transform image.
	///
	void Update();

private:
	/**
	This function finds connected regions of pixels with negative distances on the outside of 
	the zero-contour in the distance image. This information is used to correct distance image values with the wrong sign.
	*/
	bool CorrectDistances();

protected:

	blPolyDataDistanceTransformFilter();
	virtual ~blPolyDataDistanceTransformFilter();

	void NormalAverage();
	void CellNormalAverage();
	void CellNormalComputation();


	//-----------------------------------------------------
	// Variables
	//-----------------------------------------------------

	vtkPolyData *input;
	vtkImageData *output;
	double spacing[3];			// Output image resolution.
	double offset[3];			// Offset from the actual surface's bounding box.
	unsigned int dimension[3];	// Integer dimensions of the output image.
	bool sign;					// If signed distance transform.
	vtkDataArray *pointNormals;
	vtkDataArray *cellNormals;
	double computationTime;


private:

	blPolyDataDistanceTransformFilter(const blPolyDataDistanceTransformFilter&);  // Not implemented.
	void operator=(const blPolyDataDistanceTransformFilter&);  // Not implemented.
};

#endif // _blPolyDataDistanceTransformFilter_h
