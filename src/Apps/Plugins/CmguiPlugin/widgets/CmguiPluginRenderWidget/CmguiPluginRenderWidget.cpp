/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiPluginRenderWidget.h"

#include "CmguiPluginRenderWidgetCanvas.h"
#include "CmguiPluginCmgui.h"

// STD
#include <limits>

// Cmgui
extern "C"
{
#include "api/cmiss_command_data.h"
#include "api/cmiss_field_image.h"
#include "api/cmiss_field.h"
#include "api/cmiss_field_composite.h"
#include "api/cmiss_region.h"
#include "api/cmiss_field.h"
#include "api/cmiss_scene_viewer.h"
#include "graphics/scene_viewer.h"
}

#define max3(x, y, z) (std::max(std::max(x, y), z))

using namespace CmguiPlugin;


// Event the widget

BEGIN_EVENT_TABLE(RenderWidget, wxPanel)
    EVT_PAINT(RenderWidget::OnPaint)
END_EVENT_TABLE()

RenderWidget::RenderWidget(Cmgui *cmgui, wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: wxPanel(parent, id, pos, size, style)
{
    m_cmgui = cmgui;

	m_RenderingTree = NULL;
    
    m_plot = new RenderWidgetCanvas(this, wxID_PLOT);
    
    wxString name = wxT("CMGUI Rendering");
    SetLabel(name);
    SetName(name);
    
    do_layout();
    
    Init();
}

RenderWidget::~RenderWidget()
{
	// Disconnect observer on modified
	SetPrivateRenderingTree( NULL );
}

void RenderWidget::do_layout()
{
    m_sizer = new wxBoxSizer(wxVERTICAL);
    m_sizer->Add(m_plot, 1, wxEXPAND|wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 1);

	SetSizer(m_sizer);
    m_sizer->Fit(this);
}

void RenderWidget::OnPaint(wxPaintEvent& event) {
    m_plot->exists();
    m_plot->Redraw();
    event.Skip();
}


void RenderWidget::Init()
{
	RenderingTree::Pointer tree = RenderingTree::New( m_cmgui, m_plot );
	SetPrivateRenderingTree( tree.GetPointer() );
    m_plot->SetSceneViewer( m_RenderingTree->GetSceneViewer( ) );
    
    int width = 0;
    int height = 0;
    this->GetSize(&width, &height);
    this->SetSize(width - 1, height);
    this->SetSize(width, height);
    
	Scene_viewer_add_repaint_required_callback( 
		m_RenderingTree->GetSceneViewer( ), RenderWidget::repaint_callback, this );

    Enable(true);
}

void CmguiPlugin::RenderWidget::SetRenderingTreeInstance( Core::RenderingTree::Pointer val )
{
	m_RenderingTree = dynamic_cast<RenderingTree*> (val.GetPointer());
}

void CmguiPlugin::RenderWidget::RenderScene( void )
{
	// 
	ComputeTimeRange();

	// calculate the smallest axiparallel box that contains all nodes in the region
	double lower[3], upper[3];

	Cmiss_region *rootRegion = m_cmgui->GetRootRegion( );
	bool boundingsCalculated = getBoundings(rootRegion, lower, upper);

	if (boundingsCalculated) {
		// calculate center of the box
		double centerX = (upper[0] + lower[0]) / 2;
		double centerY = (upper[1] + lower[1]) / 2;
		double centerZ = (upper[2] + lower[2]) / 2;

		// calculate the radius of a sphere around the calculated center (of the box) that
		// contains most of the box area
		double radius = max3(upper[0] - centerX, upper[1] - centerY, upper[2] - centerZ);

		// TODO: instead calculate here a sphere that contains all the box (it's really not too complicated)

		//std::cout << "center = (" << centerX << " / " << centerY << " / " << centerZ << ")" << std::endl;
		//std::cout << "radius = " << radius << std::endl;

		// set new view parameters
		//Scene_viewer_set_view_simple(sceneViewer, centerX, centerY, centerZ, 20*radius, 40, 20*radius);
		//Scene_viewer_set_view_simple(sceneViewer, centerX, centerY, centerZ, 20*radius, 10, 20*radius);

		Scene_viewer_view_all( m_RenderingTree->GetSceneViewer( ) );

	} else {
		Scene_viewer_view_all( m_RenderingTree->GetSceneViewer( ) );
		//std::cout << "No nodes found to calculate boundings." << std::endl;
	}
}

Core::RenderingTree::Pointer CmguiPlugin::RenderWidget::GetPrivateRenderingTree() const
{
	return m_RenderingTree.GetPointer();
}

bool CmguiPlugin::RenderWidget::Enable( bool enable /*= true */ )
{
	m_plot->Enable( enable );
	return wxPanel::Enable ( enable );
}

std::string CmguiPlugin::RenderWidget::GetName()
{
	return wxPanel::GetName( ).c_str();
}

bool CmguiPlugin::RenderWidget::getBoundings(
	Cmiss_region_id region, double *lower, double *upper) 
{
	class : public CmguiNodeVisitor {
	public:
		double *lower, *upper;
		bool initialized;
		Cmiss_field_id coordField;

		int visit(struct Cmiss_node *node) {
			double coordinates[3];
			Cmiss_field_evaluate_at_node(coordField, node, 0, 3, coordinates);
			//std::cout << node << std::endl;
			//std::cout << coordinates[0] << " / " << coordinates[1] << " / " << coordinates[2] << std::endl;
			if (!initialized) {
				for (int currentDimension = 0; currentDimension < 3; currentDimension++) {
					lower[currentDimension] = coordinates[currentDimension];
					upper[currentDimension] = coordinates[currentDimension];
				}
				initialized = true;
			} else {
				for (int currentDimension = 0; currentDimension < 3; currentDimension++) {
					lower[currentDimension] = std::min(lower[currentDimension], coordinates[currentDimension]);
					upper[currentDimension] = std::max(upper[currentDimension], coordinates[currentDimension]);
				}
			}
			return 1;
		}
	} boundingsNodeVisitor;
	boundingsNodeVisitor.lower = lower;
	boundingsNodeVisitor.upper = upper;
	boundingsNodeVisitor.coordField = Cmiss_region_find_field_by_name(region, "coordinates");
	boundingsNodeVisitor.initialized = false;
	if ( boundingsNodeVisitor.coordField )
	{
		Cmiss_region_for_each_node_in_region(region, nodeVisitorWrapper, &boundingsNodeVisitor);
	}

	int numChildRegions;
	Cmiss_region_get_number_of_child_regions(region, &numChildRegions);
	for (int childRegionNr = 0; childRegionNr < numChildRegions; childRegionNr++) 
	{
		Cmiss_region_id childRegionID = Cmiss_region_get_child_region(region, childRegionNr);
		boundingsNodeVisitor.initialized = getBoundings( childRegionID, lower, upper );
	}

	return boundingsNodeVisitor.initialized;
}

int CmguiPlugin::RenderWidget::GetTime()
{
	Cmiss_scene_viewer_id scene_viewer = m_RenderingTree->GetSceneViewer( );
	struct Scene *scene = Scene_viewer_get_scene( scene_viewer );
	Cmiss_time_keeper_id time_keeper;
	time_keeper = Scene_get_default_time_keeper(scene);
	return Cmiss_time_keeper_get_time( time_keeper );
}

void CmguiPlugin::RenderWidget::SetTime( int time )
{
	Cmiss_scene_viewer_id scene_viewer = m_RenderingTree->GetSceneViewer( );
	struct Scene *scene = Scene_viewer_get_scene( scene_viewer );
	Cmiss_time_keeper_id time_keeper;
	time_keeper = Scene_get_default_time_keeper(scene);
	Cmiss_time_keeper_set_time( time_keeper, time * 1.0 );
}

void CmguiPlugin::RenderWidget::GetTimeRange( int &min, int &max )
{
	Cmiss_scene_viewer_id scene_viewer = m_RenderingTree->GetSceneViewer( );
	struct Scene *scene = Scene_viewer_get_scene( scene_viewer );
	Cmiss_time_keeper_id time_keeper;
	time_keeper = Scene_get_default_time_keeper(scene);

	if ( time_keeper )
	{
		min = Cmiss_time_keeper_get_minimum( time_keeper );
		max = Cmiss_time_keeper_get_maximum( time_keeper );
	}
}

void CmguiPlugin::RenderWidget::ComputeTimeRange()
{

	std::vector<Core::DataEntity::Pointer> allDataEntities;
	allDataEntities = GetPrivateRenderingTree()->GetAllDataEntities();
	int min = 0;
	int max = 0;
	std::vector<Core::DataEntity::Pointer>::iterator it;
	for ( it = allDataEntities.begin() ; it != allDataEntities.end() ; it++ )
	{
		min = std::min( (*it)->GetMetadata()->GetTagValue<int>( "TimeMin" ), min );
		max = std::max( (*it)->GetMetadata()->GetTagValue<int>( "TimeMax" ), max );
	}

	Cmiss_scene_viewer_id scene_viewer = m_RenderingTree->GetSceneViewer( );
	struct Scene *scene = Scene_viewer_get_scene( scene_viewer );
	Cmiss_time_keeper_id time_keeper;
	time_keeper = Scene_get_default_time_keeper(scene);
	Cmiss_time_keeper_set_minimum( time_keeper, min );
	Cmiss_time_keeper_set_maximum( time_keeper, max );
}

void CmguiPlugin::RenderWidget::SetTimeStep( int time )
{
	BaseWindow::SetTimeStep( time );

	if ( GetEnableTimeUpdates() )
	{
		SetTime( time );
	}
}

void CmguiPlugin::RenderWidget::repaint_callback( 
	Cmiss_scene_viewer_id scene_viewer, 
	void *callback_data,
	void *renderWidget )
{
	CmguiPlugin::RenderWidget* render = reinterpret_cast<CmguiPlugin::RenderWidget*> ( renderWidget );
	if ( render->GetCamera3DHolder( ).IsNull() )
	{
		return ;
	}

	render->Lock3DCamera( true );

	Core::Camera3D::Pointer camera = render->GetCamera3DHolder( )->GetSubject();
	Scene_viewer_get_lookat_parameters(scene_viewer,
		&camera->m_eyex,&camera->m_eyey,&camera->m_eyez,
		&camera->m_lookatx,&camera->m_lookaty,&camera->m_lookatz,
		&camera->m_upx,&camera->m_upy,&camera->m_upz);
	Scene_viewer_get_view_angle(scene_viewer, &camera->m_view_angle);
	// Adjust zoom factor to be compatible with MITK
	camera->m_view_angle = camera->m_view_angle * 50;
	Cmiss_scene_viewer_get_near_and_far_plane(scene_viewer, 
		&camera->m_near_clip_plane, &camera->m_far_clip_plane );
	camera->m_initialized = true;

	render->GetCamera3DHolder( )->NotifyObservers();

	render->Lock3DCamera( false );

	return ;
}

void CmguiPlugin::RenderWidget::UpdateCamera3D( Core::Camera3D::Pointer cam )
{
	Core::Camera3D::Pointer camera = GetCamera3DHolder( )->GetSubject();
	if ( camera->m_initialized == false || GetLock3DCamera() )
	{
		return;
	}

	Scene_viewer_set_lookat_parameters( m_RenderingTree->GetSceneViewer( ),
		camera->m_eyex, camera->m_eyey, camera->m_eyez,
		camera->m_lookatx, camera->m_lookaty, camera->m_lookatz,
		camera->m_upx, camera->m_upy, camera->m_upz);
	// Adjust zoom factor to be compatible with MITK
	Scene_viewer_set_view_angle( m_RenderingTree->GetSceneViewer( ), camera->m_view_angle / 50 );
	Cmiss_scene_viewer_set_near_and_far_plane(m_RenderingTree->GetSceneViewer( ), 
		camera->m_near_clip_plane, camera->m_far_clip_plane );
}

void CmguiPlugin::RenderWidget::RequestUpdateAll()
{
	Scene_viewer_view_all( m_RenderingTree->GetSceneViewer( ) );
}

