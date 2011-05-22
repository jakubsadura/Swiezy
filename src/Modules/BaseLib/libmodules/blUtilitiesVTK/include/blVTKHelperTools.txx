/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blVTKHelperTools_txx
#define __blVTKHelperTools_txx

#include <stdexcept>
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkAbstractArray.h"

/**
\brief Return the short array with name arrayName in the PointData
\note If no array is found, throw an exception
*/
template <class ArrayType>
ArrayType *blVTKHelperTools::GetPointDataArray( 
				vtkPolyData* polyData, 
				const char* arrayName )
{
	ArrayType *concreteArray;
	vtkAbstractArray	*abstractArray;
	abstractArray = polyData->GetPointData()->GetAbstractArray( arrayName );
	concreteArray = ArrayType::SafeDownCast( abstractArray );
	if ( concreteArray == NULL )
	{
		std::ostringstream strError;
		strError <<"Array "<< arrayName << " not found" << std::endl;
		throw std::runtime_error( strError.str( ) );
	}

	return concreteArray;
}


#endif
