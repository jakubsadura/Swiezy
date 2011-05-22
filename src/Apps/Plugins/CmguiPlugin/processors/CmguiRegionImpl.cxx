/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiRegionImpl.h"

extern "C" {
#include "finite_element/finite_element_helper.h"
}


CmguiPlugin::CmguiRegionImpl::CmguiRegionImpl( Cmiss_region *rootRegion, const std::string &name )
{
	m_Name = name;
	m_RootRegion = rootRegion;
	m_Data = NULL;
	ResetData();
}

CmguiPlugin::CmguiRegionImpl::~CmguiRegionImpl()
{
	if ( m_Data != NULL )
	{
		DestroyRegion( m_Data );
		DEACCESS(Cmiss_region)(&m_Data);
	}
}

void CmguiPlugin::CmguiRegionImpl::DestroyRegion( DataType &region )
{
	int numChildRegions;
	Cmiss_region_get_number_of_child_regions(region, &numChildRegions);
	while( numChildRegions > 0 ) {
		Cmiss_region_id childRegionID = Cmiss_region_get_child_region(region, 0);
		DestroyRegion( childRegionID );
		Cmiss_region_get_number_of_child_regions(region, &numChildRegions);
	}

	DataType parent = Cmiss_region_get_parent( region );
	Cmiss_region_remove_child_region( parent, region );

	DEACCESS(Cmiss_region)(&parent);
}

boost::any CmguiPlugin::CmguiRegionImpl::GetDataPtr() const
{
	return m_Data;
}

void CmguiPlugin::CmguiRegionImpl::ResetData()
{
	if ( m_Data != NULL )
	{
		DestroyRegion( m_Data );
		DEACCESS(Cmiss_region)(&m_Data);
	}

	m_Data = Cmiss_region_create();
	if ( !Cmiss_region_add_child_region(m_RootRegion, m_Data, m_Name.c_str(), 0) )
	{
		DestroyRegion( m_Data );
		DEACCESS(Cmiss_region)(&m_Data);
	}
}

void CmguiPlugin::CmguiRegionImpl::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<DataType> ( val );
}

bool CmguiPlugin::CmguiRegionImpl::SetData( 
	blTagMap::Pointer tagMap, Core::ImportMemoryManagementType mem/* = gmCopyMemory*/ )
{
	blTag::Pointer tagDataPtr = tagMap->FindTagByName( "DataPtr" );
	if ( tagDataPtr.IsNotNull() )
	{
		DataType srcData;
		if ( tagDataPtr->GetValue<DataType>( srcData ) )
		{
			m_Data = srcData;
			return true;
		}
	}

	ResetData();
	if ( m_Data == NULL )
	{
		return false;
	}

	blTag::Pointer tagPoints = tagMap->FindTagByName( "Points" );
	if ( tagPoints.IsNull() ) return false;
	std::vector<Point3D>* points;
	points = tagPoints->GetValueCasted< std::vector<Point3D>* >();

	blTag::Pointer tagSurfaceElements = tagMap->FindTagByName( "SurfaceElements" );
	if ( tagSurfaceElements.IsNull() ) return false;
	std::vector<SurfaceElement3D>* surfaceElements;
	surfaceElements = tagSurfaceElements->GetValueCasted< std::vector<SurfaceElement3D>* >();

	int return_code;

	struct FE_region *fe_region = Cmiss_region_get_FE_region(m_Data);
	FE_region_begin_change(fe_region);

	/* create a 3-D coordinate field*/
	FE_field *coordinate_field = FE_field_create_coordinate_3d(fe_region,(char*)"coordinate");

	/* create and fill nodes*/
	struct FE_node *template_node = CREATE(FE_node)(
		/*cm_node_identifier*/1, fe_region, /*template_node*/NULL);
	return_code = define_FE_field_at_node_simple(
		template_node, coordinate_field, /*number_of_derivatives*/0, /*derivative_value_types*/NULL);

	
	const int number_of_nodes = points->size();
	FE_value coordinates[3];
	int initial_identifier = FE_region_get_last_FE_nodes_idenifier(fe_region)+1;
	int i;
	for (i = 0; i < number_of_nodes; i++)
	{
		 coordinates[0] = (*points)[ i ][ 0 ];
		 coordinates[1] = (*points)[ i ][ 1 ];
		 coordinates[2] = (*points)[ i ][ 2 ];
		 int identifier = i + initial_identifier;
		 struct FE_node *node = CREATE(FE_node)(identifier, /*fe_region*/NULL, template_node);
		 FE_region_merge_FE_node(fe_region, node);
		 ACCESS(FE_node)(node);
		 int number_of_values_confirmed;
		 return_code=set_FE_nodal_field_FE_value_values(
			 coordinate_field, node, coordinates, &number_of_values_confirmed);
		 DEACCESS(FE_node)(&node);
	}
	DESTROY(FE_node)(&template_node);


	/* establish mode which automates creation of shared faces*/
	FE_region_begin_define_faces(fe_region);

	struct CM_element_information element_identifier;
	FE_element *element;
	FE_element *template_element;
	/* create a tetrahedron with linear simplex field*/
	template_element = FE_element_create_with_simplex_shape(fe_region, /*dimension*/2);
	set_FE_element_number_of_nodes(template_element, 3);
	FE_element_define_field_simple(template_element, coordinate_field, LINEAR_SIMPLEX);

	const int number_of_elements = surfaceElements->size();
	for (i = 0 ; i < number_of_elements; i++)
	{
		SurfaceElement3D	surfaceElement = (*surfaceElements)[ i ];
		element_identifier.type = CM_ELEMENT;
		element_identifier.number = FE_region_get_next_FE_element_identifier(fe_region, CM_ELEMENT, i);
		element = CREATE(FE_element)(&element_identifier, (struct FE_element_shape *)NULL,
			(struct FE_region *)NULL, template_element);
		ACCESS(FE_element)(element);
		struct FE_node *node;
		node = FE_region_get_FE_node_from_identifier(fe_region,surfaceElement[0] + initial_identifier );
		return_code=set_FE_element_node(element, 0, node);
		node = FE_region_get_FE_node_from_identifier(fe_region,surfaceElement[1] + initial_identifier );
		return_code=set_FE_element_node(element, 1, node);
		node = FE_region_get_FE_node_from_identifier(fe_region,surfaceElement[2] + initial_identifier );
		return_code=set_FE_element_node(element, 2, node);
		FE_region_merge_FE_element(fe_region, element);
		DEACCESS(FE_element)(&element);
	}
	DEACCESS(FE_element)(&template_element);

	/* must remember to end define faces mode*/
	FE_region_end_define_faces(fe_region);

	DEACCESS(FE_field)(&coordinate_field);

	FE_region_end_change(fe_region);

	return true;
}

bool CmguiPlugin::CmguiRegionImpl::GetData( blTagMap::Pointer tagMap )
{
	if ( m_Data == NULL )
	{
		return false;
	}

	if ( GetGenerateTemporalData() )
	{

		struct FE_region *fe_region = Cmiss_region_get_FE_region(m_Data);

		FE_field *coordinate_field;
		FE_region_get_default_coordinate_FE_field(fe_region, &coordinate_field );

		int num_nodes = FE_region_get_number_of_FE_nodes( fe_region );

		Point3D point;
		point.resize( 3 );
		m_Points.resize( num_nodes );
		for ( int i = 0 ; i < num_nodes ; i++)
		{
			int return_code;

			struct FE_node *node;
			node = FE_region_get_FE_node_from_identifier( fe_region, i + 1 );

			int number_of_values;
			FE_value time = 0;
			FE_value *values;
			return_code = get_FE_nodal_field_FE_value_values(
				coordinate_field, node, &number_of_values, time, &values);

			if ( return_code )
			{
				for ( int j = 0 ; j < number_of_values && j < 3 ; j++ )
				{
					point[ j ] = values[ j ];
				}

				m_Points[ i ] = point;

				free((char *) values );
			}
		}



		int num_elem = FE_region_get_number_of_FE_elements( fe_region );
		m_SurfaceElements.resize( num_elem );
		for ( int i = 0 ; i < num_elem ; i++ )
		{
			struct FE_element *elem;
			CM_element_information cm;
			cm.number = i + 1;
			cm.type = CM_ELEMENT;
			elem = FE_region_get_FE_element_from_identifier( fe_region, &cm );
			
			int dimension = get_FE_element_dimension( elem );
			if ( dimension != 2 )
			{
				continue;
			}

			int num_nodes;
			get_FE_element_number_of_nodes( elem, &num_nodes );
			if ( num_nodes == 0 )
			{
				continue;
			}

			SurfaceElement3D	surfaceElement;
			surfaceElement.resize( num_nodes );
			for ( int j = 0 ; j < num_nodes ; j++ )
			{
				struct FE_node *node;
				get_FE_element_node( elem, j, &node );
				surfaceElement[ j ] = get_FE_node_identifier( node ) - 1;
			}

			m_SurfaceElements[ i ] = surfaceElement;
		}

		tagMap->AddTag( "Points", &m_Points );
		tagMap->AddTag( "SurfaceElements", &m_SurfaceElements );

		return true;
	}

	tagMap->AddTag( "DataPtr", m_Data ); 

	return true;
}

bool CmguiPlugin::CmguiRegionImpl::IsSurfaceMesh( Cmiss_region *region )
{
	struct FE_region *fe_region = Cmiss_region_get_FE_region(region);

	int num_elem = FE_region_get_number_of_FE_elements( fe_region );
	for ( int i = 0 ; i < num_elem ; i++ )
	{
		struct FE_element *elem;
		CM_element_information cm;
		cm.number = i + 1;
		cm.type = CM_ELEMENT;
		elem = FE_region_get_FE_element_from_identifier( fe_region, &cm );

		int dimension = get_FE_element_dimension( elem );
		if ( dimension != 2 )
		{
			return false;
		}
	}

	return true;
}
