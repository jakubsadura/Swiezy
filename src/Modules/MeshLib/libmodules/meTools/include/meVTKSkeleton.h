/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meVTKSkeleton_h
#define _meVTKSkeleton_h

#include <vtkSmartPointer.h>
#include <vtkPolyDataAlgorithm.h>
#include "vtkUnstructuredGrid.h"
#include "vtkPolyData.h"
#include "meSkeletonTypes.h"

#include <map>
#include <vector>

/**
\brief Extract Skeleton from a surface
       from the SkeletonAPI of Nacho Larrabide 
\author Chiara Riccobene 
\date 3 Nov 2009
*/

class  meVTKSkeleton: public vtkPolyDataAlgorithm
{
	public:
		static meVTKSkeleton *New();
		
	/**
	* \brief Sets input as a vtk polydata surface
	* \param [in] inputData Input surface in the form of a vtkPolyData structure. 
	* From this surface, the distrance transform is internally computed.
	*/
	void SetInputAsSurface(vtkPolyData *inputDataSurf);

	//! Sets/Gets for the distance transform filter parameters. Only used if the surface is set and not the distance map.
	void SetDistanceTransformFilterDimensions(unsigned int *dimensions);
	void GetDistanceTransformFilterDimensions(unsigned int *dimensions);
	void SetDistanceTransformFilterMaxDimension(unsigned int uiMaxDimension);
	void GetDistanceTransformFilterMaxDimension(unsigned int &uiMaxDimension);
	void SetDistanceTransformFilterSpacing(double *spacing);
	void GetDistanceTransformFilterSpacing(double *spacing);
	void SetDistanceTransformFilterOffset(double  *offset);
	void GetDistanceTransformFilterOffset(double  *offset);
	void SetDistanceTransformFilterSing(bool sign);
	void GetDistanceTransformFilterSing(bool sign);

	//! Sets/Gets for the distance transform smoothing filter
	void SetDistanceTransformSmoothingSigma(double sigma);
	double GetDistanceTransformSmoothingSigma();

	//! Sets/Gets for the distance transform smoothing filter
	void SetMedialCurveThreshold(double threshold);
	double GetMedialCurveThreshold();

	//! Sets/Gets decimation reduction used on the input surface before the distance map is computed
	void SetInputSurfaceDecimationReduction(double decimateReduction);
	double GetInputSurfaceDecimationReduction();

	/**
	* \brief Sets input as a itk image containing the distance map representation of the surface
	* \param [in] inputData Input surface in the form of a distance map. 
	*/
	void SetInputAsITKImage(SkeletonAPITypes::InputImagePointer inputData);

	/**
	* \brief Gets the output as a itk image containing zeros everywhere but where 
	* the centerline is.
	*/
	SkeletonAPITypes::OutputImagePointer GetOutputAsITKImage(void);

	/**
	* \brief Gets the output as a vtk polydata.
	* \param removeSmallBranches - If true, branches in the skeleton below a threshold are removed (at the moment,
	* the threshold value is hard-coded).
	*/
	vtkPolyData* GetOutputAsVTKPolyData(bool removeSmallBranches = true);

	//!Smooths the output polyline using a spline (IMPORTANT: only serves if the output is  recovered polydata).
	void SetSplineSmoothOutput(bool);
	
	SkeletonAPITypes::InputImagePointer GetInputAsITKImage( );

	protected:
		
		//! brief Default Constructor.
		meVTKSkeleton();

		//! Destructor.
		virtual ~meVTKSkeleton();

		int RequestData(vtkInformation *, 
			vtkInformationVector **, vtkInformationVector *);
			
		//! Compute the distance map
		void ComputeDistanceMap( vtkPolyData* input );
		//! Compute offset and spacing
		void ComputeSpacingAndOffset( vtkPolyData* input);
		//! Creation of distance image
		void CreateDistanceImage( vtkPolyData* input);
		//! Compute Associated Average Outward Flux and skeleton
		void ComputeAssociatedAverageOutwardFlux();
		//! Internal auxiliary method used for the computation of the centerline as a polydata.
		void ConvertVtkToPolyline(vtkUnstructuredGrid *vtkUG, vtkPolyData *vtkPD);
	

	private:

		//! Purposely not implemented
		meVTKSkeleton( const meVTKSkeleton& );	

		//! Purposely not implemented
		void operator = ( const meVTKSkeleton& );	
		
		//!Pointer to input surface as distance map.
		SkeletonAPITypes::InputImagePointer inputData;
		
		//! Pointer to output image containing the centerline.

		SkeletonAPITypes::OutputImagePointer outputData;

		//! variables used by the distance trasnform filter
		unsigned int m_uiMaxDimension;
		unsigned int dimensions[3];
		bool sign;

		//! Smoothing parameters
		double sigma;

		//! Medial curve filter parameters
		double threshold;

		//! Decimation reduction percentage
		double decimateReduction;

		SkeletonAPITypes::InputImageType::SpacingType spacing;
		SkeletonAPITypes::InputImageType::PointType offset;
		SkeletonAPITypes::InputImageType::RegionType region;
		SkeletonAPITypes::InputImageType::IndexType startR;
		SkeletonAPITypes::InputImageType::SizeType sizeR;

		double spac[3];
		double offs[3];

		unsigned long totalPoints;

		bool splineSmoothOutputOn;

};


#endif // _blIDSImageFileReader_h
