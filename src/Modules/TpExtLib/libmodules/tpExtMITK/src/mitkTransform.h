/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _mitkTransform_H
#define _mitkTransform_H

#include "TpExtLibMITKWin32Header.h"
#include "mitkCommon.h"
#include "mitkBaseData.h"
#include "itkTransformBase.h"

namespace mitk
{

/**
\brief Transform points and vector from an input space to an output space

\ingroup blUtilitiesMITK
\date 18 02 10
\author Xavi Planes
*/
class TPEXTLIBMITK_EXPORT Transform : public mitk::BaseData
{
protected:

public:
	typedef itk::TransformBase				TransformType;
	typedef TransformType::Pointer			TransformPointer;
	typedef std::vector<itk::TransformBase::Pointer> TransformVectorType;

	mitkClassMacro(Transform, mitk::BaseData);

	itkNewMacro(Self);

	//!
	virtual void Initialize( int numTimeSteps );

	//!
	virtual void SetTransform(int pos, TransformPointer transform);

	//!
	virtual TransformPointer GetTransform( int pos );

	//!
	void SetRequestedRegionToLargestPossibleRegion();

	//!
	bool RequestedRegionIsOutsideOfTheBufferedRegion();

	//!
	virtual bool VerifyRequestedRegion();

	//!
	void SetRequestedRegion(itk::DataObject *data);

protected:

	Transform();

	virtual ~Transform();

protected:

	//!
	TransformVectorType m_Transform;
};

}

#endif // _mitkTransform_H
