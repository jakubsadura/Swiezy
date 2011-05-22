/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef DCMGDCMDATAREADER_H
#define DCMGDCMDATAREADER_H

// DCMAPI_EXPORT
#include "dcmAPIIOWin32Header.h"

// dcmapi
#include "dcmAbstractImageReader.h"

// vtk
#include <vtkImageData.h>
#include <vtkSmartPointer.h>

namespace dcmAPI
{

/** 
* Reader for standard images.
* 
* To use this data you need to call:
* - SetFilename()
* - Update()
* - Any of the Get*() methods
* 
* \ingroup img_readers
*/
class DCMAPIIO_EXPORT StandardImageReader : public AbstractImageReader
{
public:

	//!
	StandardImageReader();

    //@defgroup groupi Interface methods.
    //@{
    
    /**
    * Set the name of the file to read.
    * Resets the processed flag, a call to Update will read the new file.
    * @input filename The file name.
    */
	void SetFilename( const std::string& filename );

    /**
    * Main 'filter' method, reads the files and stores the data.
    */
    void Update();

    /**
    * Get the read volumes and timestamps.
    * @param volumes A reference to an array of image pointers, will be over written.
    * @param timestamps A reference to an array of floats, will be over written.
    */
    void GetVolumes( std::vector< vtkSmartPointer<vtkImageData> >& volumes, std::vector< float >& timestamps);

    /**
    * Get the ECG data.
    * @param ecg A reference to the ECG data, will be over written.
    */
	void GetEcgData( EcgData& ecg );
	
	//@}

private:

	//!
	std::vector< vtkSmartPointer<vtkImageData> > m_Volumes;


}; // class StandardImageReader

} // namespace dcmAPI

#endif // DCMGDCMDATAREADER_H
