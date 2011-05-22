#ifndef pacsTest_h
#define pacsTest_h

// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "cxxtest/TestSuite.h"
#include "pacsAPIFindSCU.h"
#include "pacsAPIDimse.h"
#include "pacsAPIMoveSCU.h"

class pacsTest : public CxxTest::TestSuite
{
public:
	//! Tests the association to a given PACS server
	void TestAssociation();

	//! Queries a PACS server and checks if the retrieved patient names is the one given
	void TestQuery();

	//! Move something from the PACS
	void TestMove();

private:
	//
	PACS::FindSCU m_Find;
	//
	PACS::Association m_Association;
	//
	PACS::MoveSCU m_Move;


//********MEMBERS
private:
	static const char *m_serverAddress;
	static unsigned long m_serverPort;
	static unsigned long m_reciveOnrPort;
};

#endif //pacsTest_h
