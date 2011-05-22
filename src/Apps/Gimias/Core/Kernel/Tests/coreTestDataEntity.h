#ifndef CORETESTDATAENTITY_H
#define CORETESTDATAENTITY_H

// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "cxxtest/TestSuite.h"
//#include "blCxxTest.h"

#include "coreDataEntity.h"
#include "coreRenderingTree.h"
#include "coreVTKPolyDataHolder.h"

/**
\brief Test data entity
\ingroup CoreTests
\author Xavi Planes
\date 21-07-08
*/
class CoreTestDataEntity : public CxxTest::TestSuite//, public blCxxTest
{
public:
	void setUp( );

	void tearDown( );

	/**
	 CreateDataEntity(), ChangeProcessingData(), AddToRenderingTree() and
	 RemoveFromRenderingTree( )
	 */
	void test0();

	void TestGetProcessingData();

protected:
	//! Create a data entity using a vtkPolyData loaded from disk
	void CreateDataEntity();

	//! Load a vtkPolyData from disk and call DataEntity::SetTimeStep( )
	void ChangeProcessingData();

	//! Create m_tree and add m_DataEntity to the tree
	void AddToRenderingTree();

	//! Remove m_DataEntity from the tree
	void RemoveFromRenderingTree();

	//! Load data entity from disk
	void LoadDataEntity( );

private:
	Core::DataEntityHolder::Pointer m_dataEntityHolderOutput;
	Core::RenderingTree::Pointer m_tree;
	Core::vtkPolyDataPtr m_PolyData;
};

#endif //CORETESTDATAENTITY_H
