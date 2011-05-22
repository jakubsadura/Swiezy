/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
/**
* @file itkDicomRead.h 
* @brief DICOM reading methods using ITK.
*/
#ifndef ITKDICOMREAD_H
#define ITKDICOMREAD_H

// boost
#include <boost/filesystem.hpp>

// VTK
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

// ITK
#include <itkImage.h>
#include <itkImageSeriesReader.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkGDCMImageIO.h>
#include <itkImageToVTKImageFilter.h>
#include <itkIndent.h>
#include <itkCommand.h>

/**
* \defgroup itkread ITK Readers
* \ingroup viewer
*/

//@{ 
namespace itk
{

/**
* Command Observer.
* Derives from itk::Command and reimplements
* the Execute() method.
*/
class CommandObserver : public itk::Command 
{

public:
    //! Standard class typedefs.
    typedef CommandObserver Self;
    typedef itk::Command Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::ProcessObject ProcessType;
    typedef const ProcessType* ProcessPointer;
    //! Method for creation through the object factory.
    itkNewMacro( Self );
    //! Execute with non const input.
    void Execute(itk::Object *caller, const itk::EventObject & event)
    {
        Execute( (const itk::Object *)caller, event);
    }
    //! Execute with const input.
    void Execute(const itk::Object * object, const itk::EventObject & event)
    {
        ProcessPointer filter = dynamic_cast< ProcessPointer >( object );
        if( typeid( event ) == typeid( itk::ProgressEvent ) )
        {
            const int nProgress = (int)(filter->GetProgress() * 100);
            std::cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"; // X+3+1 chars + 1 for fun...
            std::cout << "[itk] Loading: "; // X chars
            // assure line is always of the same size
            if( nProgress < 10 ) std::cout << "  "; // 2 chars + 1 for number
            else if( nProgress < 100 ) std::cout << " "; // 1 char + 2 for number
            std::cout << nProgress << "%";
        }
        else if( typeid( event ) == typeid( itk::StartEvent ) )
        {
            std::cout << "[itk] Loading:   0%";
        }
        else if( typeid( event ) == typeid( itk::EndEvent ) )
        {
            std::cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"; // X+3+1 chars + 1 for fun...
            std::cout << "[itk] Loading: 100%" << std::endl;
        }
    }
protected:
    //! Constructor: protect from instantiation.
    CommandObserver() {}
    //! Destructor: protect from instantiation.
    ~CommandObserver() {}
private:
    //! Copy constructor: purposely not implemented.
    CommandObserver( const Self& );
    //! Assignement operator: purposely not implemented.
    void operator=( const Self& );

}; // class CommandObserver

/**
* Get the list of files to read from a DICOM folder.
* @param path The path to the DICOM folder.
* @return The file list.
*/
std::vector<std::string> GetDicomFolderFileList(const boost::filesystem::path& inputPath)
{
    itk::GDCMSeriesFileNames::Pointer sorter = itk::GDCMSeriesFileNames::New();
    sorter->SetInputDirectory( inputPath.file_string() );
    return sorter->GetInputFileNames();
}

/**
* Get the list of files to read from an input path.
* @param path The path to input path.
* @return The file list.
*/
std::vector<std::string> GetDicomFilesList(const boost::filesystem::path& inputPath)
{
    // file list to load
    std::vector<std::string> fileList;
    // single file case
    if( boost::filesystem::is_regular_file(inputPath) )
    {
        std::cout << "[itk] Reading DICOM file." << std::endl;
        fileList.push_back(inputPath.file_string());
    }
    // folder case
    else if( boost::filesystem::is_directory(inputPath))
    {
        std::cout << "[itk] Reading DICOM folder." << std::endl;
        fileList = itk::GetDicomFolderFileList(inputPath.file_string());
        std::cout << "[itk] " << fileList.size() << " file(s)." << std::endl;
    }
    else
    {
        std::cerr << "[itk][ERROR] Neither file, nor folder, this is a problem..." << std::endl;
    }
    // return
    return fileList;
}

/**
* Read a DICOM file/folder with a itk::ImageSeriesReader and itk::GDCMImageIO.
* @param paths The list of DICOM files paths.
* @return The read data.
*/
template< class T, unsigned int N >
vtkSmartPointer<vtkImageData> ReadDicomFiles(const std::vector<std::string>& paths)
{
	// tyepdef
    typedef itk::Image< T, N > ImageType;
    // reader
    typename itk::ImageSeriesReader< ImageType >::Pointer reader = 
        itk::ImageSeriesReader< ImageType >::New();
    // gdcm io
    itk::GDCMImageIO::Pointer gdcmImageIO = itk::GDCMImageIO::New();
    reader->SetImageIO( gdcmImageIO );
    // observers
    CommandObserver::Pointer observer = CommandObserver::New();
    reader->AddObserver( itk::StartEvent(), observer );
    reader->AddObserver( itk::ProgressEvent(), observer );
    reader->AddObserver( itk::EndEvent(), observer );
    // file or directory?
    if( paths.size() == 1 )
    {
        reader->SetFileName( paths[0] );
    }
    else
    {
        reader->SetFileNames( paths );
    }
    // image adaptor (itk -> vtk)
	typename itk::ImageToVTKImageFilter< ImageType >::Pointer imageAdaptor 
	    = itk::ImageToVTKImageFilter< ImageType >::New();
    imageAdaptor->SetInput(reader->GetOutput());
    imageAdaptor->Update();
    // final image
    vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
    imageData->DeepCopy( imageAdaptor->GetOutput() );
    return imageData;
}

/**
* Read a DICOM file/folder with a itk::ImageSeriesReader and itk::GDCMImageIO.
* @param paths The list of DICOM files paths.
* @param pixelType The pixel type.
* @return The read data.
*/
std::vector< vtkSmartPointer<vtkImageData> > ReadDicomFiles(
    const std::vector<std::string>& paths,
    const itk::ImageIOBase::IOComponentType& pixelType)
{
    vtkSmartPointer<vtkImageData> imageData;
    // print out file lsit
    std::cout << "[itk] files:";
    for( unsigned int i=0; i < paths.size(); ++i )
    {
        std::string path = paths[i];
        std::cout << " " << path.substr( path.find_last_of('/')+1, path.size() );
    }
    std::cout << std::endl;
    // load the files
    switch(pixelType)
    {
        case itk::ImageIOBase::DOUBLE:
            std::cout << "[itk] Data type: ImageIOBase::DOUBLE" << std::endl;
            imageData = itk::ReadDicomFiles< double, 3 >(paths);
            break;
        case itk::ImageIOBase::FLOAT:
            std::cout << "[itk] Data type: ImageIOBase::FLOAT" << std::endl;
            imageData = itk::ReadDicomFiles< float, 3 >(paths);
            break;
        case itk::ImageIOBase::LONG:
            std::cout << "[itk] Data type: ImageIOBase::LONG" << std::endl;
            imageData = itk::ReadDicomFiles< long, 3 >(paths);
            break;
        case itk::ImageIOBase::ULONG:
            std::cout << "[itk] Data type: ImageIOBase::ULONG" << std::endl;
            imageData = itk::ReadDicomFiles< unsigned long, 3 >(paths);
            break;
        case itk::ImageIOBase::INT:
            std::cout << "[itk] Data type: ImageIOBase::INT" << std::endl;
            imageData = itk::ReadDicomFiles< int, 3 >(paths);
            break;
        case itk::ImageIOBase::UINT:
            std::cout << "[itk] Data type: ImageIOBase::UINT" << std::endl;
            imageData = itk::ReadDicomFiles< unsigned int, 3 >(paths);
            break;
        case itk::ImageIOBase::SHORT:
            std::cout << "[itk] Data type: ImageIOBase::SHORT" << std::endl;
            imageData = itk::ReadDicomFiles< short, 3 >(paths);
            break;
        case itk::ImageIOBase::USHORT:
            std::cout << "[itk] Data type: ImageIOBase::USHORT" << std::endl;
            imageData = itk::ReadDicomFiles< unsigned short, 3 >(paths);
            break;
        case itk::ImageIOBase::CHAR:
            std::cout << "[itk] Data type: ImageIOBase::CHAR" << std::endl;
            imageData = itk::ReadDicomFiles< char, 3 >(paths);
            break;
        case itk::ImageIOBase::UCHAR:
            std::cout << "[itk] Data type: ImageIOBase::UCHAR" << std::endl;
            imageData = itk::ReadDicomFiles< unsigned char, 3 >(paths);
            break;
        default:
            throw std::runtime_error("[itk] Unkown pixel type.");
    }
    // return
    std::vector< vtkSmartPointer<vtkImageData> > data;
    data.push_back( imageData );
    return data;
}

} // namepsace itk
//@}

#endif // ITKDICOMREAD_H

