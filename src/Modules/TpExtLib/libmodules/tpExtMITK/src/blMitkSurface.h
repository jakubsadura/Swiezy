/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blMitkSurface_H
#define _blMitkSurface_H

#include "TpExtLibMITKWin32Header.h"
#include "mitkCommon.h"
#include "mitkBaseData.h"
#include "mitkTimeSlicedGeometry.h"
#include "mitkSurface.h"
#include "itkImageRegion.h"
#include <vector>

class vtkPolyData;

namespace blMitk {

/**
 \brief Redefinition of mitk::Surface, allowing one vtkPolyData repeated
 along several time steps and changing the active scalar depending on the
 time step.

 \ingroup blUtilitiesMITK
 \date 27 01 10
 \author Xavi Planes
 */
  class TPEXTLIBMITK_EXPORT Surface : public mitk::Surface
  {
  protected:

  public:
    mitkClassMacro(Surface, mitk::Surface);

    itkNewMacro(Self);

	/** If the number of PointData scalar arrays is > 10 then 
	search all scalar arrays with the name "activation" and
	Resize this object to the size of this array
	*/
	void SetVtkPolyData(vtkPolyData* polydata, unsigned int t = 0);

	//! Call SetActiveScalars( ) to the output vtkPolyData
	vtkPolyData* GetVtkPolyData(unsigned int t = 0);

  protected:

    Surface();

    virtual ~Surface();

  protected:
    //!
	bool m_ScalarTimeSteps;

	//! Scalar array names. One for each time step
	std::vector<std::string> m_ArrayNames;
  };

} // namespace mitk

#endif // _blMitkSurface_H
