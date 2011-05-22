#ifndef MESHSTATISTICS_H
#define MESHSTATISTICS_H

// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "CILabItkMacros.h"
#include "blHistogram.h"
#include <itkLightObject.h>
#include "vtkCellData.h"
#include <vtkDataArray.h>
#include <vtkDataSet.h>
#include <vtkFieldData.h>
#include <vtkMeshQuality.h>
#include "vtkSmartPointer.h"
#include "blSignal.h"


/**
This class describes mesh statisctics data.

\author Jakub Lyko
\date  07/03/2008
*/


namespace me
{

class MeshStatistics: public itk::LightObject
{
public:
	CILAB_ITK_CLASS(MeshStatistics, itk::LightObject, itkIgnoreNewMacro)
	
	//!
	typedef vtkSmartPointer< vtkMeshQuality > VtkMeshQualityPtr;
	typedef vtkSmartPointer< vtkDataArray > VtkDataArrayPtr;

	//! 
	static Pointer New(vtkDataSet* _inputMesh);
	//!
	MeshStatistics(vtkDataSet* _inputMesh);
		
	//! 
	enum QualityType {Area, AspectRatio, EdgeRatio, RadiusRatio, Distortion, 
						TetraAspectRatio, TetraEdgeRatio, TetraCollapseRatio,
						TetraRadiusRatio,TetraMinAngle,TetraAspectFrobenius,TetraEdge};

	//!
	unsigned int getNumberOfCells();
	//!
	vtkDataArray* getQualityArray(QualityType _qualityType);
	//!
	vtkDataObject* computeHistogram(unsigned int _numberOfBins, QualityType _qualityType);
	//!
	double getAverageValueOfQuality(QualityType _qualityType);

	blSignal::Pointer GetSignal();
	
private:
	//!
	void setQuality(QualityType _qualityType);

	vtkDataSet* inputMesh;
	VtkMeshQualityPtr meshQuality;
	blSignal::Pointer signal;
};
}

#endif
