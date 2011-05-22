/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __itkDataEntityIO_h
#define __itkDataEntityIO_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "gmItkDataEntityIOWin32Header.h"

#include "itkImageIOBase.h"
#include "coreDataEntity.h"

class vtkImageData;

namespace itk
{
/** \class DataEntityIO
 * \brief ImageIO object for reading and writing imaegs from a DataEntity
 *
 * DataEntityIO is an ImageIO object that allows you to
 * retrieve/store an image in a DataEntity using a standard ITK
 * ImageFileReader or ImageFileWriter.  THis allows a plugin to be
 * written once and compiled into a shared object module that GIMIAS
 * can communicate with directly or compiled into a command line
 * program that can be executed outside of GIMIAS.  In the former, the
 * plugin will be provided with a DataEntity pointer for the "file" to read/write
 * and the ImageFileReader/Writer will use the DataEntityIO object to
 * perform "IO" operations directly.  In the latter,
 * the plugin will be provided with a filename for its inputs/outputs
 * and other ITK ImageIO objects will be employed by the
 * ImageFileReader/ImageFileWriter to read and write the data.
 *
 * The "filename" specified will look like a URI:
 *     DataEntity:<DataEntity*>                  - local slicer
 *
 */
class GMITKDATAENTITYIO_EXPORT DataEntityIO : public ImageIOBase
{
public:
  /** Standard class typedefs. */
  typedef DataEntityIO            Self;
  typedef ImageIOBase  Superclass;
  typedef SmartPointer<Self>  Pointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DataEntityIO, ImageIOBase);

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*);
  
  /** Set the spacing and dimension information for the set filename. */
  virtual void ReadImageInformation();
  
  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanWriteFile(const char*);

  /** Writes the header of the image.
   * Assumes SetFileName has been called with a valid file name. */
  virtual void WriteImageInformation();

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegion has been set properly. */
  virtual void Write(const void* buffer);

protected:
  DataEntityIO();
  ~DataEntityIO();
  void PrintSelf(std::ostream& os, Indent indent) const;


private:
  DataEntityIO(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool IsADataEntity(const char*);
  Core::DataEntity* FileNameToDataEntity(const char*);

  /** Write the image information to the node and specified image */
  virtual void WriteImageInformation(Core::DataEntity::Pointer dataEntity, vtkImageData*);
  
};


} /// end namespace itk
#endif /// __itkDataEntityIO_h

