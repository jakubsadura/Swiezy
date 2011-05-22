/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blVTKMarshaller.h"
#include <vtkPolyDataWriter.h>
#include <vtkPolyDataReader.h>
#include <vtkStructuredPointsWriter.h>
#include <vtkStructuredPointsReader.h>
#include <vtkCharArray.h>
#include "vtkSmartPointer.h"

using namespace blVTKHelperTools;

blVTKHelperTools::VTKMarshaller::VTKMarshaller()
{
}

VTKPackage blVTKHelperTools::VTKMarshaller::ConvertIntoPackageAndDeleteImpl( vtkPolyData*& pd, bool deletePd )
{
	typedef vtkSmartPointer<vtkPolyDataWriter> WriterPtr;
	WriterPtr writer = WriterPtr::New();
	writer->SetFileTypeToBinary();
	writer->WriteToOutputStringOn();
	writer->SetInput(pd);
	writer->Write();

	if( deletePd )
	{
		pd->Delete();
		pd = NULL;
	}

	return VTKPackage(writer->RegisterAndGetOutputString(), writer->GetOutputStringLength());
}

VTKPackage blVTKHelperTools::VTKMarshaller::ConvertIntoPackageAndDeleteImpl( vtkStructuredPoints*& sp, bool deleteSp )
{
	typedef vtkSmartPointer<vtkStructuredPointsWriter> WriterPtr;
	WriterPtr writer = WriterPtr::New();
	writer->SetFileTypeToBinary();
	writer->WriteToOutputStringOn();
	writer->SetInput(sp);
	writer->Write();

	if( deleteSp )
	{
		sp->Delete();
		sp = NULL;
	}

	return VTKPackage(writer->RegisterAndGetOutputString(), writer->GetOutputStringLength());
}

blVTKHelperTools::VTKPackage blVTKHelperTools::VTKMarshaller::ConvertIntoPackageAndDelete( vtkPolyData*& pd )
{
	return ConvertIntoPackageAndDeleteImpl(pd, true);
}

blVTKHelperTools::VTKPackage blVTKHelperTools::VTKMarshaller::ConvertIntoPackage( vtkPolyData* pd )
{
	return ConvertIntoPackageAndDeleteImpl(pd, false);
}

blVTKHelperTools::VTKPackage blVTKHelperTools::VTKMarshaller::ConvertIntoPackageAndDelete( vtkStructuredPoints*& pd )
{
	return ConvertIntoPackageAndDeleteImpl(pd, true);
}

blVTKHelperTools::VTKPackage blVTKHelperTools::VTKMarshaller::ConvertIntoPackage( vtkStructuredPoints* pd )
{
	return ConvertIntoPackageAndDeleteImpl(pd, false);
}

void blVTKHelperTools::VTKMarshaller::Discard( const VTKPackage& package )
{
	if( this-m_RawStrings.count(package.objectAsString) )
	{
		this->m_RawStrings.erase(this->m_RawStrings.find(package.objectAsString));
	}
	delete [] package.objectAsString;
}

blVTKHelperTools::VTKMarshaller::~VTKMarshaller()
{
	for( RawStringSet::iterator it = this->m_RawStrings.begin(); it != this->m_RawStrings.end(); ++it )
	{
		delete *it;
	}
}

blVTKHelperTools::VTKUnMarshaller::VTKUnMarshaller()
{
}

vtkPolyData* blVTKHelperTools::VTKUnMarshaller::ConvertToPolyData( const VTKPackage& package)
{
	typedef vtkSmartPointer<vtkPolyDataReader> ReaderPtr;
	ReaderPtr reader = ReaderPtr::New();
	reader->ReadFromInputStringOn();
	typedef vtkSmartPointer<vtkCharArray> CharArrayPtr;
	CharArrayPtr objectAsArray = CharArrayPtr::New();
	objectAsArray->SetArray(package.objectAsString, package.length, 1);
	reader->SetInputArray( objectAsArray );
	reader->Update();
	vtkPolyData* result = reader->GetOutput();
	result->Register(NULL);
	return result;
}

vtkStructuredPoints* blVTKHelperTools::VTKUnMarshaller::ConvertToStructuredPoints( const VTKPackage& package)
{
	typedef vtkSmartPointer<vtkStructuredPointsReader> ReaderPtr;
	ReaderPtr reader = ReaderPtr::New();
	reader->ReadFromInputStringOn();
	typedef vtkSmartPointer<vtkCharArray> CharArrayPtr;
	CharArrayPtr objectAsArray = CharArrayPtr::New();
	objectAsArray->SetArray(package.objectAsString, package.length, 1);
	reader->SetInputArray(objectAsArray);
	reader->Update();
	vtkStructuredPoints* result = reader->GetOutput();
	result->Register(NULL);
	return result;
}
