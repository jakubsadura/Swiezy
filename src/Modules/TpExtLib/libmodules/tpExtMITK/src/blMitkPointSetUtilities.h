/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLMITKPOINTSETUTILITIES_H
#define BLMITKPOINTSETUTILITIES_H

#include "TpExtLibMITKWin32Header.h"
#include "CILabNamespaceMacros.h"
#include "boost/shared_ptr.hpp"
#include "mitkPointOperation.h"
#include "itkPoint.h"
#include "mitkPointSet.h"
#include "mitkInteractionConst.h"

CILAB_BEGIN_NAMESPACE(blMitk)

TPEXTLIBMITK_EXPORT inline void RemovePoint(mitk::PointSet& _pointSet, int _iPoint)
{
	boost::shared_ptr<mitk::PointOperation> doOp(  new mitk::PointOperation(mitk::OpREMOVE, 0, _pointSet.GetPoint(_iPoint), _iPoint) );
	_pointSet.ExecuteOperation(doOp.get());
}

template< class T>
void AddPoint(mitk::PointSet& _pointSet, const itk::Point<T, 3>& _p)
{
	mitk::PointSet::PointType mitkPoint;
	for( int i = 0; i < _p.GetPointDimension(); ++i )
		mitkPoint[i] = _p[i];
	_pointSet.InsertPoint(_pointSet.GetSize(), mitkPoint);

}

template< class T>
void CopyPoint(const  mitk::PointSet::PointType& _lhs, itk::Point<T, 3>& _rhs )
{
	for( int i = 0; i < _lhs.GetPointDimension(); ++i )
		(_rhs)[i] = _lhs[i];
}

CILAB_END_NAMESPACE(blMitk)

#endif //BLMITKPOINTSETUTILITIES_H
