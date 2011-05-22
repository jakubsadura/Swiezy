/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiPlaneFactory.h"
extern "C" {
#include "finite_element/finite_element_helper.h"
}

CmguiPlugin::PlaneFactory::PlaneFactory()
{

}

Cmiss_region_id CmguiPlugin::PlaneFactory::GetRegion() const
{
	return m_Region;
}

void CmguiPlugin::PlaneFactory::SetRegion( Cmiss_region_id val )
{
	m_Region = val;
}

double * CmguiPlugin::PlaneFactory::GetPoint( int i ) const
{
	return (double*) &m_Point[i][0];
}

void CmguiPlugin::PlaneFactory::SetPoint( int i, double x, double y, double z )
{
	m_Point[i][0] = x; m_Point[i][1] = y; m_Point[i][2] = z;
}

void CmguiPlugin::PlaneFactory::CreatePointFrom3DImage( 
	int i, 
	std::vector<double> &origin, 
	std::vector<double> &spacing,
	std::vector<int> &dimensions,
	int z,
	double point[3] )
{
	switch ( i )
	{
	case 0: 
		point[ 0 ] = origin[ 0 ];
		point[ 1 ] = origin[ 1 ];
		point[ 2 ] = origin[ 2 ] + z * spacing [ 2 ];
		break;
	case 1: 
		point[ 0 ] = origin[ 0 ] + dimensions[ 0 ] * spacing [ 0 ];
		point[ 1 ] = origin[ 1 ];
		point[ 2 ] = origin[ 2 ] + z * spacing [ 2 ];
		break;
	case 2: 
		point[ 0 ] = origin[ 0 ];
		point[ 1 ] = origin[ 1 ] + dimensions[ 1 ] * spacing [ 1 ];
		point[ 2 ] = origin[ 2 ] + z * spacing [ 2 ];
		break;
	case 3: 
		point[ 0 ] = origin[ 0 ] + dimensions[ 0 ] * spacing [ 0 ];
		point[ 1 ] = origin[ 1 ] + dimensions[ 1 ] * spacing [ 1 ];
		point[ 2 ] = origin[ 2 ] + z * spacing [ 2 ];
		break;
	}
}

void CmguiPlugin::PlaneFactory::CreatePointFrom2DImage( 
	int i, 
	std::vector<float> &position, 
	std::vector<double> &spacing,
	std::vector<int> &dimensions,
	std::vector<float> &orientation,
	double point[3] )
{
	float dx0 = dimensions[ 0 ] * spacing [ 0 ] * orientation[ 0 ];
	float dy0 = dimensions[ 0 ] * spacing [ 0 ] * orientation[ 1 ];
	float dz0 = dimensions[ 0 ] * spacing [ 0 ] * orientation[ 2 ];
	float dx1 = dimensions[ 1 ] * spacing [ 1 ] * orientation[ 3 ];
	float dy1 = dimensions[ 1 ] * spacing [ 1 ] * orientation[ 4 ];
	float dz1 = dimensions[ 1 ] * spacing [ 1 ] * orientation[ 5 ];
	switch ( i )
	{
	case 0: 
		point[ 0 ] = position[ 0 ];
		point[ 1 ] = position[ 1 ];
		point[ 2 ] = position[ 2 ];
		break;
	case 1: 
		point[ 0 ] = position[ 0 ] + dx0;
		point[ 1 ] = position[ 1 ] + dy0;
		point[ 2 ] = position[ 2 ] + dz0;
		break;
	case 2: 
		point[ 0 ] = position[ 0 ] + dx1;
		point[ 1 ] = position[ 1 ] + dy1;
		point[ 2 ] = position[ 2 ] + dz1;
		break;
	case 3: 
		point[ 0 ] = position[ 0 ] + dx0 + dx1;
		point[ 1 ] = position[ 1 ] + dy0 + dy1;
		point[ 2 ] = position[ 2 ] + dz0 + dz1;
		break;
	}
}

void CmguiPlugin::PlaneFactory::CreatePlane( )
{
	// Create nodes
	Cmiss_node_id mainNode;
	int id = Cmiss_region_get_number_of_nodes_in_region( m_Region );
	mainNode = Cmiss_node_create( id, m_Region );

	// Get FE_region
	struct FE_region *fe_region = FE_node_get_FE_region(mainNode);
	FE_region_begin_change(fe_region);

	// create a 3-D coordinate field
	FE_field *coordinate_field = FE_field_create_coordinate_3d(fe_region, "coordinates");

	// create and fill nodes
	struct FE_node *template_node = CREATE(FE_node)(/*cm_node_identifier*/1, fe_region, /*template_node*/NULL);
	define_FE_field_at_node_simple(template_node, coordinate_field, /*number_of_derivatives*/0, /*derivative_value_types*/NULL);

	FE_value coordinates[3];
	coordinates[ 0 ] = m_Point[ 0 ][ 0 ];
	coordinates[ 1 ] = m_Point[ 0 ][ 1 ];
	coordinates[ 2 ] = m_Point[ 0 ][ 2 ];
	int identifier1 = AddCoordinate( template_node, coordinate_field, fe_region, coordinates );
	coordinates[ 0 ] = m_Point[ 1 ][ 0 ];
	coordinates[ 1 ] = m_Point[ 1 ][ 1 ];
	coordinates[ 2 ] = m_Point[ 1 ][ 2 ];
	int identifier2 = AddCoordinate( template_node, coordinate_field, fe_region, coordinates );
	coordinates[ 0 ] = m_Point[ 2 ][ 0 ];
	coordinates[ 1 ] = m_Point[ 2 ][ 1 ];
	coordinates[ 2 ] = m_Point[ 2 ][ 2 ];
	int identifier3 = AddCoordinate( template_node, coordinate_field, fe_region, coordinates );
	coordinates[ 0 ] = m_Point[ 3 ][ 0 ];
	coordinates[ 1 ] = m_Point[ 3 ][ 1 ];
	coordinates[ 2 ] = m_Point[ 3 ][ 2 ];
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

int CmguiPlugin::PlaneFactory::AddCoordinate( 
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

void CmguiPlugin::PlaneFactory::Update()
{
	CreatePlane();
}
