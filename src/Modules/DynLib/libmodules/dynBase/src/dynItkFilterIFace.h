/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynItkFilterIFace_H
#define _dynItkFilterIFace_H

#include "dynFilterIFace.h"
#include "itkLightObject.h"

/**
Internal interface for ITK Filter

\author Xavi Planes
\date 18 Sept 2010
\ingroup DynLib
*/
class dynItkFilterIFace : public itk::LightObject, public dynFilterIFace
{
public:
	static Pointer New( );
	typedef dynItkFilterIFace        Self;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self>  ConstPointer;
};

typedef dynItkFilterIFace::Pointer (*exNewSmartPointer)();

#endif // _dynItkFilterIFace_H

