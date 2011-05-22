/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _tpExtLibCmguiPCH_H
#define _tpExtLibCmguiPCH_H

#define WX_USER_INTERFACE

extern "C" {
#include "api/cmiss_command_data.h"
#include "api/cmiss_element.h"
#include "api/cmiss_field.h"
#include "api/cmiss_field_composite.h"
#include "api/cmiss_field_image.h"
#include "api/cmiss_region.h"
#include "api/cmiss_scene_viewer.h"
#include "api/cmiss_texture.h"

#include "command/cmiss.h"
#include "graphics/scene.h"
#include "graphics/scene_viewer.h"
#include "user_interface/message.h"

#include "api/cmiss_command_data.h"
#include "api/cmiss_field_image.h"
#include "api/cmiss_field.h"
#include "api/cmiss_field_composite.h"
#include "api/cmiss_region.h"
#include "api/cmiss_field.h"
#include "api/cmiss_scene_viewer.h"
#include "api/cmiss_material.h"
#include "api/cmiss_graphics_module.h"
#include "api/cmiss_texture.h"
#include "general/image_utilities.h"
#include "graphics/texture.h"
#include "graphics/material.h"
#include "finite_element/finite_element_helper.h"
#include "finite_element/export_finite_element.h"
}

#include "vtkStructuredPoints.h"
#include "vtkStructuredPointsReader.h"
#include "vtkSmartPointer.h"
#include "vtkImageData.h"

#include "wx/app.h"

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#endif // _tpExtLibCmguiPCH_H