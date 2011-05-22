/*=========================================================================
This file was copied from EvoLib/EvoToolsVTK module in order to
remove dependencies between the threshold segmentation and EvoLib.
Authors: jlyko and sordas. Date: 2009-04-22$
=========================================================================*/

#ifndef _meRemoveInnerSurfaceFilter_h
#define _meRemoveInnerSurfaceFilter_h

#include "itkImage.h"
#include "vtkPolyDataAlgorithm.h"
#include "vtkSmartPointer.h"


/// \brief This class remove inner surface from original mesh.
/// \ingroup evoToolsVTK

template <class ItkImageType>
class RemoveInnerSurfaceFilter : public vtkPolyDataAlgorithm
{
public:
	//typedef itk::Image< unsigned char, 3 > RealImageType;

public:
	// Description:
	// Construct with default extraction mode to extract largest regions.
	static RemoveInnerSurfaceFilter *New();

	void SetInputDistanceMap(typename ItkImageType::Pointer distanceMap) { m_distanceMap = distanceMap; }
	typename ItkImageType::Pointer GetInputDistanceMap() { return m_distanceMap; }

protected:
	RemoveInnerSurfaceFilter();
	~RemoveInnerSurfaceFilter();

	// Usual data generation method
	int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

	double GetClosedSurfaceVolume(vtkSmartPointer< vtkPolyData > );

private:
	typename ItkImageType::Pointer m_distanceMap;
};

#include "meRemoveInnerSurfaceFilter.txx"

#endif //_meRemoveInnerSurfaceFilter_h
