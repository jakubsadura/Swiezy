// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreDataEntityListTest.h"
#include "coreDataEntityList.h"

void CoreDataEntityListTest::TestUnknownDataEntityId()
{
	Core::DataEntityList::Pointer list = Core::DataEntityList::New();
	TS_ASSERT_THROWS( list->GetDataEntity(123) , Core::Exceptions::DataEntityIdNotFound );
	TS_ASSERT_THROWS( list->Remove(123), Core::Exceptions::DataEntityIdNotFound ); 
}

void CoreDataEntityListTest::TestAdd()
{
	Core::DataEntityList::Pointer list = Core::DataEntityList::New();
	Core::DataEntity::Pointer de = Core::DataEntity::New(Core::ImageTypeId);
	TS_ASSERT_EQUALS( list->GetCount(), 0);	
	list->Add(de);
	TS_ASSERT_EQUALS( list->GetCount(), 1);	
	list->Add(de);
	TS_ASSERT_EQUALS( list->GetCount(), 1);
}
