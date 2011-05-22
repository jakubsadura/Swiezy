/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _cmguiLoadImage_H
#define _cmguiLoadImage_H

#define WX_USER_INTERFACE

#include "wx/app.h"

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
}

class vtkImageData;

/**
\brief 
\ingroup CMGUIPlugin
\author Xavi Planes
\date Nov 2010
*/
class cmguiLoadImageFrame : public wxFrame
{
public:
	cmguiLoadImageFrame(wxApp *app_in, Cmiss_command_data *command_data_in);

	~cmguiLoadImageFrame()
	{
		m_app->ExitMainLoop();
	}


private:
	//!
	void InitCmgui( );

	//!
	void CreateImageRegion( const std::string &name, vtkImageData* pImage );

	//!
	int AddCoordinate( struct FE_node *template_node, FE_field *coordinate_field, struct FE_region *fe_region, FE_value coordinates[3] );

	//!
	void CreateTexture( vtkImageData* pImage, struct Cmgui_image *cmgui_image, const std::string &name );

	//!
	void CreatePlane( Cmiss_region_id region, vtkImageData* pImage, int z );

	//!
	struct Cmgui_image * CreateCmguiImage( vtkImageData* vtkImage, int z );

	//!
	vtkImageData* ConvertImageToUChar( vtkImageData* vtkImage );

private:
	//!
	wxApp *m_app;
	
	//!
	Cmiss_command_data *m_commandData;

	//!
	Cmiss_scene_viewer_id m_sceneViewer;

	//!
	Cmiss_region_id m_Region;

	DECLARE_EVENT_TABLE();
};


#endif // _cmguiLoadImage_H
