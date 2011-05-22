/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#include "dcmStandardImageReader.h"

// dcmapi
#include <dcmImageUtilities.h>

namespace dcmAPI
{

StandardImageReader::StandardImageReader()
{
	this->Initialise("Any", "Any", dcmAPI::DataSet::STANDARD, false);
}

void StandardImageReader::SetFilename( const std::string& filename )
{
    if( this->m_Filename.compare( filename ) != 0 )
    {
        this->m_Filename = filename;
        this->m_Preprocessed = false;
    }
}

void StandardImageReader::Update()
{
    if( m_Preprocessed ) 
	{
		return;
	}

	itk::ImageIOBase::IOComponentType pixelType;
	pixelType = dcmAPI::ImageUtilities::ParsePixelType( 
		dcmAPI::ImageUtilities::ReadPixelType(this->m_Filename) );
	vtkSmartPointer<vtkImageData> pImage;
	switch ( pixelType )
	{
		blVtkTemplateMacro( 
			( pImage = ImageUtilities::ReadVtkImageFromFile< PixelT, 3 >( this->m_Filename.c_str(), true ) ) );
	}
	if( pImage.GetPointer() != NULL )
	{
		m_Volumes.push_back(pImage);
		// mark as processed
		this->m_Preprocessed = true;
		// set image characteristics
        pImage.GetPointer()->GetOrigin( this->m_ImageOrigin );
        pImage.GetPointer()->GetSpacing( this->m_ImageSpacing );
        pImage.GetPointer()->GetDimensions( this->m_ImageDimensions );
        this->m_NumberOfVolumes = 1;
	}
}

void StandardImageReader::GetVolumes( std::vector< vtkSmartPointer<vtkImageData> >& volumes, std::vector< float >& timestamps)
{
    volumes = m_Volumes;
}

void StandardImageReader::GetEcgData( EcgData& ecg )
{
    throw ImageReaderException( "PhilipsUSImageReader: cannot read ECG." );
}

} // namespace 