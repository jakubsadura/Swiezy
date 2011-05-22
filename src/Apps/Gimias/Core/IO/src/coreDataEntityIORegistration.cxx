/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataEntityIORegistration.h"

#include "coreDataEntityReader.h"
#include "coreDataEntityWriter.h"

#include "coreVTKImageDataReader.h"
#include "coreVTKImageDataWriter.h"

#include "coreVTKPolyDataReader.h"
#include "coreVTKPolyDataWriter.h"

#include "coreVTKUnstructuredGridReader.h"
#include "coreVTKUnstructuredGridWriter.h"

#include "coreMHDVectorFieldReader.h"

#include "coreCgnsFileReader.h"
#include "coreCgnsFileWriter.h"

#include "coreNumericDataReader.h"
#include "coreNumericDataWriter.h"

#include "coreDICOMFileReader.h"

#include "coreITKTransformReader.h"
#include "coreITKTransformWriter.h"

Core::IO::DataEntityIORegistration::DataEntityIORegistration()
{
}

Core::IO::DataEntityIORegistration::~DataEntityIORegistration()
{
}

void Core::IO::DataEntityIORegistration::RegisterDefaultFormats()
{
	DataEntityReader::RegisterFormatReader( BaseDataEntityReader::Pointer(DICOMFileReader::New()) );
	DataEntityReader::RegisterFormatReader( BaseDataEntityReader::Pointer(VTKImageDataReader::New()) );
	DataEntityReader::RegisterFormatReader( BaseDataEntityReader::Pointer(VTKPolyDataReader::New()) );
	DataEntityReader::RegisterFormatReader( BaseDataEntityReader::Pointer(VTKUnstructuredGridReader::New()) );
	DataEntityReader::RegisterFormatReader( BaseDataEntityReader::Pointer(MHDVectorFieldReader::New()) );
	DataEntityReader::RegisterFormatReader( BaseDataEntityReader::Pointer(CGNSFileReader::New()) );
	DataEntityReader::RegisterFormatReader( BaseDataEntityReader::Pointer(NumericDataReader::New()) );
	DataEntityReader::RegisterFormatReader( BaseDataEntityReader::Pointer(ITKTransformReader::New()) );

	DataEntityWriter::RegisterFormatWriter( BaseDataEntityWriter::Pointer(VTKImageDataWriter::New()) );
	DataEntityWriter::RegisterFormatWriter( BaseDataEntityWriter::Pointer(VTKPolyDataWriter::New()) );
	DataEntityWriter::RegisterFormatWriter( BaseDataEntityWriter::Pointer(VTKUnstructuredGridWriter::New()) );
	DataEntityWriter::RegisterFormatWriter( BaseDataEntityWriter::Pointer(CGNSFileWriter::New()) );
	DataEntityWriter::RegisterFormatWriter( BaseDataEntityWriter::Pointer(NumericDataWriter::New()) );
	DataEntityWriter::RegisterFormatWriter( BaseDataEntityWriter::Pointer(ITKTransformWriter::New()) );
}
