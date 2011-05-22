/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

namespace CmguiPlugin
{

class PlaneFactory : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(PlaneFactory,Core::SmartPointerObject);

	//!
	Cmiss_region_id GetRegion() const;
	void SetRegion(Cmiss_region_id val);

	double *GetPoint( int i ) const;
	void SetPoint( int i, double x, double y, double z );

	//!
	static void CreatePointFrom3DImage( 
		int i, 
		std::vector<double> &origin, 
		std::vector<double> &spacing,
		std::vector<int> &dimensions,
		int z,
		double point[3] );

	//!
	static void CreatePointFrom2DImage( 
		int i, 
		std::vector<float> &position, 
		std::vector<double> &spacing,
		std::vector<int> &dimensions,
		std::vector<float> &orientation,
		double point[3] );

	//!
	void Update( );

private:
	//!
	PlaneFactory( );

	//! Create a plane region using x, y directions and z as slice number
	void CreatePlane(  );

	//! Add a coordinate node to the template_node
	static int AddCoordinate( 
		struct FE_node *template_node, 
		FE_field *coordinate_field, 
		struct FE_region *fe_region, 
		FE_value coordinates[3] );

private:
	//!
	Cmiss_region_id m_Region;
	//!
	double m_Point[4][3];
};

}




