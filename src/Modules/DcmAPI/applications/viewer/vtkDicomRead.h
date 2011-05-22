/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
/**
* @file vtkDicomRead.h
* @brief DICOM reading methods using VTK.
*/
#ifndef VTKDICOMREAD_H
#define VTKDICOMREAD_H

// boost
#include <boost/filesystem.hpp>

// VTK
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkCommand.h>
#include <vtkDICOMImageReader.h>

/**
* \defgroup vtkread VTK Readers
* \ingroup viewer
*/

//@{ 
namespace vtk
{

/**
* Observer to know if an event was thrown and display its data. Used for errors and warnings.
*/
class ObserverEvent : public vtkCommand
{
public:
    // Constructor
    ObserverEvent() : m_gotMessage(false) {}
    // New
    static ObserverEvent* New() { return new ObserverEvent(); }
    // Did get message?
    bool DidGetMessage() const { return m_gotMessage; }
protected:
    // Desctructor
    ~ObserverEvent() {}
private:
    // Execute
    void Execute(vtkObject* caller, unsigned long ul, void* pData)
    {
        std::cout << (char*)pData;
        m_gotMessage = true;
    }
    // flag to know if a message was sent
    bool m_gotMessage;
    
}; // class ObserverEvent

/**
* Observer on the loading of a file. Displays progress on stdout.
*/
class ObserverFileIO : public vtkCommand
{
public:
    // Constructor
    ObserverFileIO() : m_isLoading(true) {}
    // New
    static ObserverFileIO* New()
    { return new ObserverFileIO(); }
    // set the loading flag
    void SetIsLoading(bool isLoading) { m_isLoading = isLoading; }
    // get the loading flag
    bool IsLoading() const { return m_isLoading; }
protected:
    // Desctructor
    ~ObserverFileIO() {}
private:
    // flag to know if loading or writing
    bool m_isLoading;
    // Execute
    void Execute(vtkObject* caller, unsigned long eventId, void* pData)
    {
    	switch(eventId) 
    	{
	        case vtkCommand::ProgressEvent:
	        {
                // Format the progress string.
                const int nProgress = (int)((*(double*)pData) * 100.0);
                // go back to start of line
                std::cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"; // X+3+1 chars + 1 for fun...
                if( m_isLoading ) std::cout << "\b[vtk] Loading: "; // X chars
                else std::cout << "[vtk] Saving: "; // X chars
                // assure line is always of the same size
                if( nProgress < 10 ) std::cout << "  "; // 2 chars + 1 for number
                else if( nProgress < 100 ) std::cout << " "; // 1 char + 2 for number
                std::cout << nProgress << "%";
	        }
	        break;
	        case vtkCommand::StartEvent:
	        {
		        if( m_isLoading ) std::cout << "[vtk] Loading:   0%";
		        else std::cout << "[vtk] Saving:   0%";
	        }
	        break;
	        case vtkCommand::EndEvent:
	        {
                std::cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"; // X+3+1 chars + 1 for fun...
		        if( m_isLoading ) std::cout << "\b[vtk] Loading: 100%" << std::endl;
		        else std::cout << "[vtk] Saving: 100%" << std::endl;
	        }
	        break;
            default:
	            std::cerr << "[vtk] Unknown event." << std::endl;
		        break;
        }
    }
    
}; // class ObserverFileIO

/**
* Read a DICOM file/folder with a vtkDICOMImageReader.
* @param path The path to the DICOM file/folder.
* @return The read data.
*/
std::vector< vtkSmartPointer<vtkImageData> > ReadDicom(const boost::filesystem::path& inputPath)
{
    // Reader
    vtkSmartPointer<vtkDICOMImageReader> reader = 
        vtkSmartPointer<vtkDICOMImageReader>::New();
    // Observe file loading progress
    ObserverFileIO* observeProgress = ObserverFileIO::New();
    reader->AddObserver(vtkCommand::StartEvent, observeProgress);
    reader->AddObserver(vtkCommand::ProgressEvent, observeProgress);
    reader->AddObserver(vtkCommand::EndEvent, observeProgress);
    // Observe errors and warnings while loading the images
    ObserverEvent* observeError = ObserverEvent::New();
    reader->AddObserver(vtkCommand::ErrorEvent, observeError);
    ObserverEvent* observeWarning = ObserverEvent::New();
    reader->AddObserver(vtkCommand::WarningEvent, observeWarning);
    // file or directory?
    if( boost::filesystem::is_directory(inputPath))
    {
        std::cout << "[vtk] Reading DICOM folder." << std::endl;
        reader->SetDirectoryName(inputPath.file_string().c_str());
    }
    else
    {
        std::cout << "[vtk] Reading DICOM file." << std::endl;
        reader->SetFileName(inputPath.file_string().c_str());
    }
    // Read all the DICOM files in the specified directory
    reader->Update();
    // handle error
    if (observeError->DidGetMessage() || observeWarning->DidGetMessage())
    {
        throw std::runtime_error("[vtk] Error/Warning reading DICOM images.");
    }
    // clean
    observeProgress->Delete();
    observeError->Delete();
    observeWarning->Delete();
    
    // return
    std::vector< vtkSmartPointer<vtkImageData> > data;
    data.push_back( reader->GetOutput() );
    std::cout << "[vtk] Data type: " << data[0]->GetScalarTypeAsString() << std::endl;
    return data;
}

} // namepsace vtk
//@}

#endif // VTKDICOMREAD_H

