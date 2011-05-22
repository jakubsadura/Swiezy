/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreDataEntityTypes_H
#define _coreDataEntityTypes_H

namespace Core{

/** 
Enumeration field that holds the different data formats a 
DataEntity can represent 
The types can used using the operator |
\author Xavi Planes
\date 04 Nov 2009
\ingroup gmDataHandling
*/
enum DataEntityType { 
	//! ProcessingData is Unknown
	UnknownTypeId = 1, 
	//! ProcessingData is itk::Image or vtkImageData
	ImageTypeId = 2, 
	//! ProcessingData is vtkPolyData
	SurfaceMeshTypeId = 4, 
	//! ProcessingData is VTKUnstructuredGrid
	VolumeMeshTypeId = 8, 
	//! ProcessingData is blSignal
	SignalTypeId = 16,
	//! Only defined the reader for ".mhd" files
	VectorFieldTypeId = 32,
	//! ProcessingData is ITKTransformFile
	TransformId = 64,
	//! ProcessingData is Skeleton vtkPolyData only Lines
	SkeletonTypeId = 128,
	//! ProcessingData is vtkPolyData only points and vertex
	PointSetTypeId = 256,
	//! ProcessingData is a MITK::BaseData
	MITKDataTypeId = 512,
	//! 2D Contour
	ContourTypeId = 1024,
	//! Measurement
	MeasurementTypeId = 2048,
	//! Roi image data (mask Image)
	ROITypeId = 4096,
	//! image data defined by slices and orientations
	SliceImageTypeId = 8192,
	//! ProcessingData is blTagMap. Doesn't have rendering data
	NumericDataTypeId = 16384,
	//! ProcessingData is itk::DTITensor
	TensorTypeId = 32768

};

//! Enumeration field that holds the different scalar types a DataEntity can represent 
enum ScalarPixelType { Int, UInt, Float, Short, UShort };


} // end namespace Core

#endif // _coreDataEntityTypes_H

