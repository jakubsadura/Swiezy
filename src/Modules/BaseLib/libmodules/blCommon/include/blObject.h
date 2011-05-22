/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blObject_h
#define __blObject_h

#include "blMacro.h"
#include "blSmartPointer.h"
#include "BaseLibWin32Header.h"
#include "blLightObject.h"
#include "blTimeStamp.h"

/**
* \brief blLightObject with modification time tracking
*
* \author Xavi Planes
* \date 5 Feb 2010
* \ingroup blCommon
*/
class blObject : public blLightObject
{

public:

	typedef blObject Self;
	typedef blSmartPointer<Self> Pointer;

	blNewMacro(Self);

	/** Return this objects modified time.  */
	virtual unsigned long GetMTime() const;

	/** Update the modification time for this object. Many filters rely on the
	* modification time to determine if they need to recompute their data.  */
	virtual void Modified() const;

protected:
	blObject();
	virtual ~blObject();

private:
	blObject(const blObject&); //purposely not implemented
	void operator=(const blObject&); //purposely not implemented

private:
	/** Keep track of modification time. */
	mutable blTimeStamp m_MTime;
};

#endif
