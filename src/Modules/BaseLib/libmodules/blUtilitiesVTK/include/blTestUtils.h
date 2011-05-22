/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLTESTUTILS_H
#define BLTESTUTILS_H

#include <blITKImagesDefines.h>
#include <itkImageFileReader.h>
#include <itkImageToImageFilter.h>
#include "BaseLibVTKWin32Header.h"
#include "CILabNamespaceMacros.h"
#include <string>

#include <limits>

CILAB_BEGIN_NAMESPACE(base)

/**
The aim of this class is to set up a regression test using a minimal amount of code.
The information needed for a regression test is:

1. The filename of the test input (image)
2. The test output (image)
3. The filename to be used for write the test output
4. The filename of the gold standard

To retrieve this information, the TestHelper class requires the user to supply the following:

1. The relative path for writing the test output. TestHelper will automatically prepend this path with 
<build>/TestOutput, where <build> is the location of the CISTIB toolkit build folder).

2. Optionally: the name of the test. By default, the name of the test is the name of the last folder in
the test output path (see point 1 above)

3. The relative path of each of the test input files. TestHelper will automatically prepend this path with 
<toolkit>/Data/Tests, where <toolkit> is the location of the CISTIB toolkit folder.

Example:

1 base::TestHelper helper("evoLib/evoLeclercEdgeDetectorImageFilter");
2 evoLeclercEdgeDetectorImageFilter::Pointer filter = evoLeclercEdgeDetectorImageFilter::New();
3 filter->SetInput(helper.GetImage<evoAPI::RealImage>("EvoLib/testImage-3dra.vtk"));
4 TS_ASSERT( helper.CompareResultToReference(filter->GetOutput()) )

Discussion:

Line 1 instantiates the helper, supplying a relative path to the location where to write the test outputs. The
test name equals the name of the last subfolder: evoLeclercEdgeDetectorImageFilter. The user can override this
default by supplying a second argument to the constructor.
In this case, the data are written to 

In line 3, the helper is used to read the test input image from the test input folder: 
<toolkit>/Data/Tests/EvoLib/testImage-3dra.vtk
If this path does not exist, then an exception will result, unless TestHelper is able to locate the input
file by prepending the input filename with the relative path supplied in the constructor (see 1.):
<toolkit>/Data/Tests/evoLib/evoLeclercEdgeDetectorImageFilter/testImage-3dra.vtk

In line 4, the helper is used to compare the test output to the gold standard. The helper will write the test output
to: <build>/TestOutputs/evoLib/evoLeclercEdgeDetectorImageFilter/evoLeclercEdgeDetectorImageFilter.result.vtk
and compare this result to the gold standard in 
<toolkit>/Data/evoLib/evoLeclercEdgeDetectorImageFilter/evoLeclercEdgeDetectorImageFilter.reference.vtk

\author Maarten Nieber
\date 24 dic 2008
*/

class BASELIBVTK_EXPORT TestHelper
{
public:
	//! Reads image from the toolkit folder "Data/Tests/"
	//! \param filename - relative path to the test image within the Data/Tests folder of the toolkit
	TestHelper(
		const std::string& sourceFolder, 
		const std::string& buildFolder, 
		const std::string& relativeFolder, 
		std::string testName = "");

	template< class ItkImageType >
	typename ItkImageType::Pointer GetImage(const std::string &relativePath) const 
	{ 
		typedef itk::ImageFileReader< ItkImageType > ReaderType;
		typename ReaderType::Pointer reader = ReaderType::New();
		reader->SetFileName(this->GetInputFilenameFullPath(relativePath).c_str());
		reader->Update();
		return reader->GetOutput();
	}

	bool CompareResultToReference(
		base::RealVolumeType::Pointer result, 
		float tolerance = std::numeric_limits<float>::min()
		);

	bool CompareResultToReference(
		base::UnsignedCharVolumeType::Pointer result, 
		float tolerance = std::numeric_limits<float>::min()
	);

	//! Returns the folder where outputs for this test are written
	std::string GetOutputFolder() const;

	//! Returns full path to \a inputFilename by prefixing it with the path to the test data of the CISTIB toolkit
	std::string GetInputFilenameFullPath(const std::string& inputFilename) const;

	//! Get default output filename for storing the result of the current test
	std::string GetOutputFilename() const;

	//! Get default filename of the reference data for the current test
	std::string GetReferenceOutputFilename() const;

	//! Get default folder for reading inputs for the current test
	std::string GetInputFolder() const;

private:
	std::string m_SourceFolder;
	std::string m_BuildFolder;
	std::string m_RelativeTestFolder;
	std::string m_TestName;
};

CILAB_END_NAMESPACE(base)

#endif //BLTESTUTILS_H
