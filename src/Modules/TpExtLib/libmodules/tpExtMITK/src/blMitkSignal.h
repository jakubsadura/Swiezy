/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blMitkSignal_H
#define _blMitkSignal_H

#include "TpExtLibMITKWin32Header.h"
#include "mitkCommon.h"
#include "mitkBaseData.h"
#include "blSignalCollective.h"

namespace blMitk {

/**
 \brief Signal rendering data

 \ingroup blUtilitiesMITK
 \date 18 02 10
 \author Xavi Planes
 */
  class TPEXTLIBMITK_EXPORT Signal : public mitk::BaseData
  {
  protected:

  public:
    mitkClassMacro(Signal, mitk::BaseData);

    itkNewMacro(Self);

	virtual void SetSignal(blSignalCollective::Pointer signal);

	//!
	void SetRequestedRegionToLargestPossibleRegion();

	//!
	bool RequestedRegionIsOutsideOfTheBufferedRegion();

	//!
	virtual bool VerifyRequestedRegion();

	//!
	void SetRequestedRegion(itk::DataObject *data);

  protected:

    Signal();

    virtual ~Signal();

  protected:

    //!
	blSignalCollective::Pointer m_Signal;
  };

} // namespace mitk

#endif // _blMitkSignal_H
