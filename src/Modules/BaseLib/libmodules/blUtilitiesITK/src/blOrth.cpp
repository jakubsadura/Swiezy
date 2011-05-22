/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blOrth.h"

void TestIfCompiles()
{
	vnl_matrix<double> m;
	vnl_vector<double> v;
	blOrth::GramSchmidt(&m, true);
	blOrth::GramSchmidtModified(&m, true, &v);
	blOrth::OrthonormalizeViaSVD(&m);
	blOrth::RemoveZeroRows(&m, true, NULL);
}
