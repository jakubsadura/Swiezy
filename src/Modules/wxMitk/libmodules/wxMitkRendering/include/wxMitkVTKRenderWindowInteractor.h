/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wxMitkVTKRenderWindowInteractor_h
#define _wxMitkVTKRenderWindowInteractor_h

#include "wxMitkRenderingWin32Header.h"
#include <vtkRenderWindowInteractor.h>

namespace mitk
{

/*
\ingroup wxMitkRendering
\author Juan Antonio Moya
\date 11 Sep 2007
*/
class WXMITKRENDERING_EXPORT wxMitkVTKRenderWindowInteractor 
	: public vtkRenderWindowInteractor
{

public:
	static wxMitkVTKRenderWindowInteractor* New();
	vtkTypeMacro(wxMitkVTKRenderWindowInteractor,vtkRenderWindowInteractor);
	wxMitkVTKRenderWindowInteractor(void);
	virtual ~wxMitkVTKRenderWindowInteractor(void);

protected:
	// unimplemented copy constructor or operator
	wxMitkVTKRenderWindowInteractor(const wxMitkVTKRenderWindowInteractor&);
	void operator=(const wxMitkVTKRenderWindowInteractor&);

};

} // namespace mitk

#endif //_wxMitkVTKRenderWindowInteractor_h_
