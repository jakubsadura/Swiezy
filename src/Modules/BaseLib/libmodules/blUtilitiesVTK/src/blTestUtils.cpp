/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blTestUtils.h"
#include "blImageUtils.h"
#include <CILabBoostMacros.h>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

namespace Private
{

template< class ItkImageType >
bool Compare( 
	typename ItkImageType::Pointer result, 
	const std::string& outputFilename, 
	const std::string& referenceFilename,
	float tolerance )
{
	typedef typename itk::ImageFileWriter < ItkImageType > WriterType;
	typename WriterType::Pointer writer = WriterType::New();

	path p(outputFilename);
	if(!exists(p.branch_path()))
	{
		create_directories(p.branch_path());
	}
	writer->SetFileName(outputFilename.c_str());
	writer->SetInput(result);
	writer->Write();
	return blImageUtils::CompareImages(
		referenceFilename.c_str(), 
		outputFilename.c_str(), 
		tolerance
	);
}

}
base::TestHelper::TestHelper( 
	const std::string& sourceFolder, 
	const std::string& buildFolder, 
	const std::string& relativeTestFolder, 
	std::string testName )
{
	m_SourceFolder = sourceFolder;
	m_BuildFolder = buildFolder;
	this->m_RelativeTestFolder = relativeTestFolder;
	this->m_TestName = testName;
	if( testName == "" )
	{
		this->m_TestName = path(relativeTestFolder).leaf();
	}
}

bool base::TestHelper::CompareResultToReference( base::RealVolumeType::Pointer result, float tolerance )
{
	//boost::filesystem::create_hard_link(this->GetReferenceOutputFilename(), this->GetOutputFilename() + ".linkToReference.lnk");
	return Private::Compare<base::RealVolumeType>(
		result, 
		this->GetOutputFilename(), 
		this->GetReferenceOutputFilename(), 
		tolerance
	);
}

bool base::TestHelper::CompareResultToReference( base::UnsignedCharVolumeType::Pointer result, float tolerance )
{
	//boost::filesystem::create_hard_link(this->GetReferenceOutputFilename(), this->GetOutputFilename() + ".linkToReference.lnk");
	return Private::Compare<base::UnsignedCharVolumeType>(
		result, 
		this->GetOutputFilename(), 
		this->GetReferenceOutputFilename(), 
		tolerance
	);
}

std::string base::TestHelper::GetInputFilenameFullPath(const std::string& inputFilename) const
{
	cilabBoostFormatMacro(
		"%1%/%2%/%3%", 
		m_SourceFolder % "Data/Tests" % inputFilename,
		result
	);
	if( !exists(result) )
	{
		cilabBoostFormatMacro(
			"%1%/%2%/%3%/%4%", 
			m_SourceFolder % "Data/Tests" % this->m_RelativeTestFolder % inputFilename,
			inputFilenameFullPath2
		);
		if( exists(inputFilenameFullPath2) )
		{
			result = inputFilenameFullPath2;
		}
	}

	return result;
}

std::string base::TestHelper::GetOutputFolder() const
{
	cilabBoostFormatMacro(
		"%1%/bin/TestOutput/%2%", 
		m_BuildFolder % this->m_RelativeTestFolder, 
		outputFolder
	);
	if( !exists(path(outputFolder) ) )
	{
		create_directories(outputFolder);
	}
	return outputFolder;
}

std::string base::TestHelper::GetOutputFilename() const
{
	cilabBoostFormatMacro(
		"%1%/%2%.result.vtk", 
		this->GetOutputFolder() % this->m_TestName, 
		outputFilename
	);
	return outputFilename;
}

std::string base::TestHelper::GetReferenceOutputFilename() const
{
	cilabBoostFormatMacro(
		"%1%/%2%/%3%.reference.vtk", 
		this->GetInputFolder() % this->m_RelativeTestFolder % this->m_TestName, 
		outputFilename
	);
	return outputFilename;
}

std::string base::TestHelper::GetInputFolder() const
{
	cilabBoostFormatMacro("%1%/%2%", m_SourceFolder % "Data/Tests", inputFolder);
	return inputFolder;
}
