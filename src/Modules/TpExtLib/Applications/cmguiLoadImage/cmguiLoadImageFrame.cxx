/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

#include "cmguiLoadImageFrame.h"

#define WX_USER_INTERFACE

extern "C" {
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
#include "user_interface/event_dispatcher.h"
#include "finite_element/finite_element_helper.h"
#include "finite_element/export_finite_element.h"
}

#include "vtkStructuredPoints.h"
#include "vtkStructuredPointsReader.h"
#include "vtkSmartPointer.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkImageCast.h"

#include <sstream>

BEGIN_EVENT_TABLE(cmguiLoadImageFrame, wxFrame)
END_EVENT_TABLE()

cmguiLoadImageFrame::cmguiLoadImageFrame( wxApp *app_in, Cmiss_command_data *command_data_in ) 
: wxFrame(NULL, -1,
		  "CMGUI_API_DEMO",
		  wxDefaultPosition,
		  wxSize(800, 600),
		  wxDEFAULT_FRAME_STYLE)
{
	m_commandData = command_data_in;
	m_app = app_in;

	m_sceneViewer = Cmiss_scene_viewer_create_wx(
		Cmiss_command_data_get_scene_viewer_package(m_commandData),
		(void *)this, CMISS_SCENE_VIEWER_BUFFERING_DOUBLE,
		CMISS_SCENE_VIEWER_STEREO_ANY_MODE, 24, 24, 24);


	// Load VTK image
	vtkSmartPointer<vtkStructuredPointsReader> reader = vtkSmartPointer<vtkStructuredPointsReader>::New();
	reader->SetFileName( "I:\\Data\\TEST\\MRI\\OutputVolume.vtk" );
	reader->Update();
	vtkSmartPointer<vtkImageData> pImage = reader->GetOutput();

	CreateImageRegion( "Image", pImage );

	Cmiss_scene_viewer_view_all(m_sceneViewer);
	Cmiss_scene_viewer_set_antialias_mode(m_sceneViewer, 4);
	Cmiss_scene_viewer_set_projection_mode(m_sceneViewer, CMISS_SCENE_VIEWER_PROJECTION_PERSPECTIVE);
}

void cmguiLoadImageFrame::CreateImageRegion(
	const std::string &name, vtkImageData* vtkImage )
{

	vtkImage = ConvertImageToUChar( vtkImage );

	Cmiss_region *rootRegion;
	rootRegion = Cmiss_command_data_get_root_region(m_commandData);

	m_Region = Cmiss_region_create();
	Cmiss_region_add_child_region( rootRegion, m_Region, name.c_str(), 0 );

	int dimensions[3];
	vtkImage->GetDimensions( dimensions );
	for ( int z = 0 ; z < dimensions[ 2 ] ; z++ )
	{
		std::stringstream str;
		str << name << z;
		std::string regionName = str.str();

		// Create region
		Cmiss_region_id childRegion = Cmiss_region_create();
		Cmiss_region_add_child_region( m_Region, childRegion, regionName.c_str(), 0 );

		struct Cmgui_image * cmguiImage = CreateCmguiImage( vtkImage, z );

		CreatePlane( childRegion, vtkImage, z );

		CreateTexture( vtkImage, cmguiImage, str.str() );

		struct Scene *scene = Scene_viewer_get_scene( m_sceneViewer );
		Scene_add_graphical_element_group(scene, childRegion, 0, (char*) regionName.c_str());

		std::string command;
		command = "gfx modify g_element " + name + "/" + regionName + " surface material " + regionName + "_material texture_coordinates xi";
		Cmiss_command_data_execute_command( m_commandData, command.c_str() );
	}

	write_exregion_file_of_name( "C:\\cmgui", m_Region, rootRegion, 1, 1, 0, 
		FE_WRITE_ALL_FIELDS, 0, NULL, 0, FE_WRITE_COMPLETE_GROUP, FE_WRITE_RECURSIVE );

	vtkImage->UnRegister( NULL );
}

struct Cmgui_image * cmguiLoadImageFrame::CreateCmguiImage( 
	vtkImageData* vtkImage, int z )
{
	void *scalarPtr = vtkImage->GetScalarPointer( 0, 0, z );
	vtkImage->GetPointData()->GetScalars()->GetNumberOfComponents();
	vtkImage->GetPointData()->GetScalars()->GetNumberOfTuples();
	int typeSize = vtkImage->GetPointData()->GetScalars()->GetDataTypeSize();

	int dimensions[3];
	vtkImage->GetDimensions( dimensions );

	// Create Cmgui_image
	struct Cmgui_image *cmgui_image;
	cmgui_image = Cmgui_image_constitute( 
		dimensions[ 0 ], 
		dimensions[ 1 ], 
		1, 
		typeSize, 
		dimensions[ 0 ]*1*typeSize, 
		(unsigned char* ) scalarPtr );

	return cmgui_image;
}


void cmguiLoadImageFrame::CreateTexture( 
	vtkImageData* vtkImage,
	struct Cmgui_image *cmgui_image,
	const std::string &name )
{

	// Create texture "image_texture"
	std::string texture_name = name + "_texture";
	Cmiss_texture_id texture = CREATE(Texture)(texture_name.c_str());
	Texture_set_image(texture, cmgui_image, "", "", 0, 0, 1, 0, 0, 0, 0);
	Cmiss_texture_set_filter_mode(texture, CMISS_TEXTURE_FILTER_LINEAR);
	Cmiss_texture_manager_id texture_manager = Cmiss_command_data_get_texture_manager( m_commandData );
	ADD_OBJECT_TO_MANAGER(Texture)(texture, texture_manager);

	// Create material "image_material"
	Cmiss_graphics_module_id gModule = Cmiss_command_data_get_graphics_module( m_commandData );
	Cmiss_material_id material;
	material = Cmiss_graphics_module_create_material(gModule);

	std::string material_name = name + "_material";
	Cmiss_material_set_name(material, material_name.c_str() );
	Graphical_material_set_texture( material, texture );
}

void cmguiLoadImageFrame::CreatePlane( Cmiss_region_id region, vtkImageData* vtkImage, int z )
{

	// Create nodes
	Cmiss_node_id mainNode;
	mainNode = Cmiss_node_create( z, region );

	// Get FE_region
	struct FE_region *fe_region = FE_node_get_FE_region(mainNode);
	FE_region_begin_change(fe_region);

	// create a 3-D coordinate field
	FE_field *coordinate_field = FE_field_create_coordinate_3d(fe_region, "coordinates");

	// create and fill nodes
	struct FE_node *template_node = CREATE(FE_node)(/*cm_node_identifier*/1, fe_region, /*template_node*/NULL);
	define_FE_field_at_node_simple(template_node, coordinate_field, /*number_of_derivatives*/0, /*derivative_value_types*/NULL);

	double origin[3];
	vtkImage->GetOrigin( origin );
	double spacing[3];
	vtkImage->GetSpacing( spacing );
	int dimensions[3];
	vtkImage->GetDimensions( dimensions );

	FE_value coordinates[3];
	coordinates[ 0 ] = origin[ 0 ];
	coordinates[ 1 ] = origin[ 1 ];
	coordinates[ 2 ] = origin[ 2 ] + z * spacing [ 2 ];
	int identifier1 = AddCoordinate( template_node, coordinate_field, fe_region, coordinates );
	coordinates[ 0 ] = origin[ 0 ] + dimensions[ 0 ] * spacing [ 0 ];
	coordinates[ 1 ] = origin[ 1 ];
	coordinates[ 2 ] = origin[ 2 ] + z * spacing [ 2 ];
	int identifier2 = AddCoordinate( template_node, coordinate_field, fe_region, coordinates );
	coordinates[ 0 ] = origin[ 0 ];
	coordinates[ 1 ] = origin[ 1 ] + dimensions[ 1 ] * spacing [ 1 ];
	coordinates[ 2 ] = origin[ 2 ] + z * spacing [ 2 ];
	int identifier3 = AddCoordinate( template_node, coordinate_field, fe_region, coordinates );
	coordinates[ 0 ] = origin[ 0 ] + dimensions[ 0 ] * spacing [ 0 ];
	coordinates[ 1 ] = origin[ 1 ] + dimensions[ 1 ] * spacing [ 1 ];
	coordinates[ 2 ] = origin[ 2 ] + z * spacing [ 2 ];
	int identifier4 = AddCoordinate( template_node, coordinate_field, fe_region, coordinates );

	DESTROY(FE_node)(&template_node);

	/* establish mode which automates creation of shared faces*/
	FE_region_begin_define_faces(fe_region);

	/* create a tetrahedron with linear simplex field*/
	FE_element *template_element;
	//template_element = FE_element_create_with_simplex_shape(fe_region, /*dimension*/2);
	const int shape_type[] = { POLYGON_SHAPE, 4, POLYGON_SHAPE };
	FE_element_shape *element_shape = CREATE(FE_element_shape)(2, shape_type, fe_region); 
	ACCESS(FE_element_shape)(element_shape);

	CM_element_information element_identifier;
	element_identifier.type = CM_ELEMENT;
	element_identifier.number = 1;
	template_element = CREATE(FE_element)(&element_identifier, element_shape, fe_region,
	/*template_element*/(FE_element *)NULL);
	ACCESS(FE_element)(template_element);

	DEACCESS(FE_element_shape)(&element_shape);

	set_FE_element_number_of_nodes(template_element, 4);
	FE_element_define_field_simple(template_element, coordinate_field, LINEAR_SIMPLEX);

	element_identifier.number = FE_region_get_next_FE_element_identifier(fe_region, CM_ELEMENT, 1);
	FE_element *element = CREATE(FE_element)(&element_identifier, (struct FE_element_shape *)NULL,
		(struct FE_region *)NULL, template_element);
	ACCESS(FE_element)(element);
	set_FE_element_node(element, 0, FE_region_get_FE_node_from_identifier(fe_region, identifier1 ) );
	set_FE_element_node(element, 1, FE_region_get_FE_node_from_identifier(fe_region, identifier2 ) );
	set_FE_element_node(element, 2, FE_region_get_FE_node_from_identifier(fe_region, identifier3 ) );
	set_FE_element_node(element, 3, FE_region_get_FE_node_from_identifier(fe_region, identifier4 ) );
	FE_region_merge_FE_element_and_faces_and_nodes(fe_region, element);
	DEACCESS(FE_element)(&element);

	DEACCESS(FE_element)(&template_element);

	FE_region_end_define_faces(fe_region);

	DEACCESS(FE_field)(&coordinate_field);

	FE_region_end_change(fe_region);
}

int main(int argc,char *argv[])
{
	int cmgui_argc = 2;
	const char **cmgui_argv = (const char**) (new char*[3]);
	*cmgui_argv = strdup(argv[0]);
	*(cmgui_argv + 1) = strdup("-server");
	*(cmgui_argv + 2) = 0;
	struct Cmiss_command_data *command_data = Cmiss_command_data_create(cmgui_argc, cmgui_argv, const_cast<char *>("0.0"));

	if (wxEntryStart(argc, argv))
	{
		//wxXmlResource::Get()->InitAllHandlers();
		// Should do this as soon after wxEntry as possible

		struct User_interface *user_interface = Cmiss_command_data_get_user_interface(command_data);

		struct Event_dispatcher *event_dispatcher;
		event_dispatcher = User_interface_get_event_dispatcher( user_interface );

		Event_dispatcher_initialise_wx_app(event_dispatcher);
	}

	Cmiss_command_data_execute_command(command_data, "gfx create texture temp");

	cmguiLoadImageFrame *frame = new cmguiLoadImageFrame(wxTheApp,command_data);
	frame->Show();
	wxTheApp->SetTopWindow(frame);
	wxTheApp->OnRun();

	Cmiss_command_data_destroy(&command_data);
}

int cmguiLoadImageFrame::AddCoordinate( 
	struct FE_node *template_node, 
	FE_field *coordinate_field, 
	struct FE_region *fe_region,
	FE_value coordinates[3] )
{
	int identifier = FE_region_get_last_FE_nodes_idenifier(fe_region)+1;
	int number_of_values_confirmed;
	Cmiss_node_id node = CREATE(FE_node)(identifier, /*fe_region*/NULL, template_node);
	ACCESS(FE_node)(node);
	set_FE_nodal_field_FE_value_values(coordinate_field, node, coordinates, &number_of_values_confirmed);
	FE_region_merge_FE_node(fe_region, node);
	DEACCESS(FE_node)(&node);

	return identifier;
}

vtkImageData* cmguiLoadImageFrame::ConvertImageToUChar( vtkImageData* vtkImage )
{
	vtkSmartPointer<vtkImageCast> cast = vtkSmartPointer<vtkImageCast>::New();
	cast->SetInput( vtkImage );
	cast->SetOutputScalarTypeToUnsignedChar();
	cast->Update();
	
	vtkImageData* output;
	output = cast->GetOutput();
	output->Register( NULL );
	return output;
}
