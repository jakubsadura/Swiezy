/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blVTKMarshallerTest.h"
#include "blImageUtils.h"
#include "blTestUtils.h"
#include "blVTKMarshaller.h"
#include <CISTIBToolkit.h>
#include <vtkPolyData.h>
#include <blShapeUtils.h>

namespace Private
{

}

void blVTKMarshallerTest::TestWithPolyData()
{
	// Read a poly-data
	base::TestHelper helper(
		CISTIB_TOOLKIT_FOLDER,
		CISTIB_TOOLKIT_BUILD_FOLDER,
		"BaseLib/blVTKToString", "TestWithPolyData");
	std::string filename = helper.GetInputFolder() + "/BaseLib/input-CT.vtk";
	vtkPolyData* polyData = blShapeUtils::ShapeUtils::LoadShapeFromFile("E:/Code/CISTIBToolkitClean/Data/SampleData/Angio/Aneurisms/aneu_1.vtk");

	// Convert the poly-data into a package that can be sent across a dll boundary
	blVTKHelperTools::VTKMarshaller converterAtClientSide;
	blVTKHelperTools::VTKPackage package = converterAtClientSide.ConvertIntoPackageAndDelete(polyData);

	// Convert the package into a poly data polyData
	blVTKHelperTools::VTKUnMarshaller converterAtServerSide;
	vtkPolyData* receivedObject = converterAtServerSide.ConvertToPolyData(package);
	
	// Client can now discard the package (if not, package is discarded automatically when converterAtClientSide is destructed)
	converterAtClientSide.Discard(package);

	// Write the received poly data
	blShapeUtils::ShapeUtils::SaveShapeToFile(receivedObject, helper.GetOutputFilename().c_str());
}

void blVTKMarshallerTest::TestWithImage()
{
	// Read an image
	base::TestHelper helper(
		CISTIB_TOOLKIT_FOLDER,
		CISTIB_TOOLKIT_BUILD_FOLDER,
		"BaseLib/blVTKToString", "TestWithImage");
	std::string filename = helper.GetInputFolder() + "/BaseLib/input-CT.vtk";
	vtkStructuredPoints* image = blImageUtils::LoadImageFromFileAsVTK("E:/Code/CISTIBToolkitClean/Data/SampleData/Angio/Images/input-ct.vtk");

	// Convert the image into a package that can be sent across a dll boundary
	blVTKHelperTools::VTKMarshaller converterAtClientSide;
	blVTKHelperTools::VTKPackage package = converterAtClientSide.ConvertIntoPackageAndDelete(image);

	// Convert the package into an image
	blVTKHelperTools::VTKUnMarshaller converterAtServerSide;
	vtkStructuredPoints* receivedObject = converterAtServerSide.ConvertToStructuredPoints(package);

	// Client can now discard the package (if not, package is discarded automatically when converterAtClientSide is destructed)
	converterAtClientSide.Discard(package);

	// Write the received image
	blImageUtils::SaveImageToFile(receivedObject, helper.GetOutputFilename().c_str(), NULL);
}
