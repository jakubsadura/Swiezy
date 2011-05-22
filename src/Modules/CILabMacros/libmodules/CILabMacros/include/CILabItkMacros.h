/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef CILABITKMACROS_H
#define CILABITKMACROS_H

/**
This file contains macro's for adding standard itk typedefs, declarations and implementations to code.
*/

/**
This is a helper macro for CILAB_ITK_CLASS. It allows you to write:
CILAB_ITK_CLASS(MyClass, itk::LightObject, itkIgnoreNewMacro)
This means that the macro does NOT insert any New() function in MyClass.
*/

#define itkIgnoreNewMacro(X)

/**
This macro's defines the standard way to put itk typedefs and the itkFactorylessNewMacro into the header file
when your class inherits from an itk object.

\param THISCLASS - Name of this class
\param SUPERCLASS - Name of the superclass of this class
\param SUPERCLASS - Name of the superclass of this class
\param NEWMACRO - Type of new macro to be used. Should be either itkNewMacro or itkFactorylessNewMacro

\note The THISCLASS and SUPERCLASS argument may not contain a comma. If you need to supply a classname with
a comma, then first create a typedef, for example:

\code
class UnaryOperationAPI : public itk::ImageToImageFilter<itk::Image<UnaryOperationAPIPixelType, 3>, itk::Image<UnaryOperationAPIPixelType, 3> >
{
public:
	typedef itk::ImageToImageFilter<itk::Image<UnaryOperationAPIPixelType, 3>, itk::Image<UnaryOperationAPIPixelType, 3> > SuperClass_;
CILAB_ITK_CLASS(UnaryOperationAPI, SuperClass_, itkNewMacro)
};

\endcode
*/
#define CILAB_ITK_CLASS(THISCLASS, SUPERCLASS, NEWMACRO) \
typedef THISCLASS Self; \
typedef SUPERCLASS Superclass; \
typedef itk::SmartPointer<Self> Pointer; \
typedef itk::SmartPointer<const Self> ConstPointer; \
NEWMACRO(Self); \
itkTypeMacro(THISCLASS, SUPERCLASS);

#endif //CILABITKMACROS_H
