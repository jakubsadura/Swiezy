/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreMitkProperties.h"

/**
Create the implementation of the template functions for DataEntityProperty
*/
class DataEntityPropertyTest
{
	DataEntityPropertyTest( )
	{
		Core::DataEntityProperty::Pointer de1;
		Core::DataEntityProperty::Pointer de2;
		de1->GetValue( );
		de1->GetValueAsString( );
		de1->Assignable( *de2 );
		de1 = de2;
		de1 == de2;
	}
};


