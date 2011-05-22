/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blLightObject_h
#define __blLightObject_h

#include "blMacro.h"
#include "blSmartPointer.h"
#include "BaseLibWin32Header.h"


/**
* \brief Base Class for everything
* \ingroup blCommon
*/
class blLightObject
{

public:

	typedef blLightObject Self;
	typedef blSmartPointer<Self> Pointer;

	blNewMacro(Self);

	virtual void Register() const { _counter++; };

	virtual void UnRegister() const { if( (--_counter) <= 0 ) delete this; };

	//virtual void Delete() const { this->UnRegister(); };

protected:
	blLightObject();
	virtual ~blLightObject();

private:
	blLightObject(const blLightObject&); //purposely not implemented
	void operator=(const blLightObject&); //purposely not implemented

private:
	mutable int _counter;
};

#endif
