#ifndef BLSTORAGEPOLICYTEST_H
#define BLSTORAGEPOLICYTEST_H

// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "CILabNamespaceMacros.h"
#include "cxxtest/TestSuite.h"
#include "blStoragePolicy.h"
#include "blUblasMatrix.h"
#include <time.h>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/blas.hpp>
#include <boost/numeric/ublas/operation.hpp>

class blStoragePolicyTest : public CxxTest::TestSuite 
{
public:
	//! 
	void TestUblasMatrixWithStoragePolicy()
	{
		time_t start,end;
		time (&start);
				
		ublas::SetDiskFileStorageFolder("C:\\StoragePolicyWorkingFolder",true);

		using namespace boost::numeric::ublas;
		matrix< double,row_major, blDiskStoragePolicy<double> > m (3, 3);
		for (unsigned i = 0; i < m.size1 (); i++ )
			for (unsigned j = 0; j < m.size2 (); j++ )
				m (i, j) = i + j ;

		//! read the data
	
		//m.begin1(); //doesn't work
		//m.erase_element(1,1);
		
		matrix< double,row_major, blDiskStoragePolicy<double> > n (3, 3);
		for (unsigned i = 0; i < n.size1 (); i++ )
			for (unsigned j = 0; j < n.size2 (); j++ )
				n (i, j) = 3*i;

		//! testing some basic operations 
		//vector<double> v (2);
		//for (i = 0; i < v.size(); ++ i)
		//	v (i) = i;
		//vector<double> nv = prod(n,v);       // matrix/vector product

		//! Level 3 BLAS testing operations
		boost::numeric::ublas::blas_3::gmm(m,3,2,m,n);
	
		TS_ASSERT_EQUALS( m(0,0) , 30);
		TS_ASSERT_EQUALS( m(0,1) , 33);
		TS_ASSERT_EQUALS( m(0,2) , 36);
		TS_ASSERT_EQUALS( m(1,0) , 51);
		TS_ASSERT_EQUALS( m(1,1) , 54);
		TS_ASSERT_EQUALS( m(1,2) , 57);
		TS_ASSERT_EQUALS( m(2,0) , 72);
		TS_ASSERT_EQUALS( m(2,1) , 75);
		TS_ASSERT_EQUALS( m(2,2) , 78);

		//boost::numeric::ublas::blas_3::tmm(m,8,m,n);

		//boost::numeric::ublas::blas_3::tsm(m,8,m,n); //! it doesn't work

		//boost::numeric::ublas::blas_3::srk(m,8,54,n);
		//boost::numeric::ublas::blas_3::hrk(m,8,54,n);
		//boost::numeric::ublas::blas_3::sr2k(m,8,54,n,n);
		//boost::numeric::ublas::blas_3::hr2k(m,8,54,n,n);

		time (&end);
		double dif = difftime (end,start);
		printf (" It takes %.2lf seconds to run the gmm algorithm.\n", dif );

	}

	//!
	void TestBracketsOperator()
	{
		ublas::SetDiskFileStorageFolder("C:\\StoragePolicyWorkingFolder",true);
		blDiskStoragePolicy<double> samplePolicy(44);
		blDiskStoragePolicy<double>::reference r = samplePolicy[0];
		r = 123.456;
		double d = r;
		TS_ASSERT_EQUALS(d, 123.456);
	}


	//!
	void TestStoragePolicyResize( void )
	{
		//! the disk storage threhsold is 10
		//! Create a file
		ublas::SetDiskFileStorageFolder("C:\\StoragePolicyWorkingFolder",true);
		blDiskStoragePolicy<double> samplePolicy(5);
		//! Put some data on the file
		blDiskStoragePolicy<double>::reference r = samplePolicy[3];
		r = 20.23;
		
		//! Resize the file to go above the threshold and check the extension
		samplePolicy.resize(20);
		// check the extension of the file after the resize
		TS_ASSERT_EQUALS(samplePolicy.size(), 20);

		//! Check the data after resize
		double d = r;
		TS_ASSERT_EQUALS(d, 20.23);		

		//! Put some data on the file
		blDiskStoragePolicy<double>::reference s = samplePolicy[7];
		s = 199.323;
		//! Resize the file and check the extension
		samplePolicy.resize(30);
		// check the extension of the file after the resize
		TS_ASSERT_EQUALS(samplePolicy.size(), 30);

		//! Check the data after resize
		d = s;
		TS_ASSERT_EQUALS(d, 199.323);	

		//! check if the additional elements are copies of x
		samplePolicy.resize(45, 34.56);
		for (int i = 30; i< 45; i++ )
		{
			blDiskStoragePolicy<double>::reference r = samplePolicy[i];
			double d = r;
			TS_ASSERT_EQUALS(d , 34.56);
		}

		blDiskStoragePolicy<double>::reference t = samplePolicy[4];
		t = 45.98;
		//! resize to go below the threshold
		samplePolicy.resize(5);
		TS_ASSERT_EQUALS(samplePolicy.size(), 5);
		double e = t;
		TS_ASSERT_EQUALS(e, 45.98);		
	}

	//!
	void TestStoragePolicyConstructors( void )
	{
		//! Default Constructor
		ublas::SetDiskFileStorageFolder("C:\\StoragePolicyWorkingFolder",true);
		blDiskStoragePolicy<double> samplePolicy;
		TS_ASSERT_EQUALS(samplePolicy.size(), 0);

		//! Fill-Constructor
		ublas::SetDiskFileStorageFolder("C:\\StoragePolicyWorkingFolder",true);
		blDiskStoragePolicy<double> anotherSamplePolicy( 20, 123.456 );
		for (int i = 0; i< 20; i++ )
		{
			blDiskStoragePolicy<double>::reference r = anotherSamplePolicy[i];
			double d = r;
			TS_ASSERT_EQUALS(d , 123.456);
		}

		//! Range constructor
		ublas::SetDiskFileStorageFolder("C:\\StoragePolicyWorkingFolder",true);
		boost::numeric::ublas::vector<double> v (3);
		for (int i = 0; i < v.size(); i++)
			v (i) = i;
	}

	//!
	void TestStoragePolicySwap( void )
	{
		ublas::SetDiskFileStorageFolder("C:\\StoragePolicyWorkingFolder",true);
		blDiskStoragePolicy<double> m(23);
		//! Put some data on the file
		blDiskStoragePolicy<double>::reference r = m[0];
		r = 20.23;
		r = m[1];
		r = 21.44;
		r = m[2];
		r = 32.72;

		blDiskStoragePolicy<double> n(18);
		//! Put some data on the file
		r = n[0];
		r = 50.78;
		r = n[1];
		r = 74.36;
		r = n[2];
		r = 85.62;

		n.swap(m);
		
		r = m[0];
		double d = r;
		TS_ASSERT_EQUALS(d , 50.78);
		r = m[1];
		d = r;
		TS_ASSERT_EQUALS(d , 74.36);
		r = m[2];
		d = r;
		TS_ASSERT_EQUALS(d , 85.62);


		r = n[0];
		d= r;
		TS_ASSERT_EQUALS(d , 20.23);
		r = n[1];
		d = r;
		TS_ASSERT_EQUALS(d , 21.44);
		r = n[2];
		d = r;
		TS_ASSERT_EQUALS(d , 32.72);
	}
};

#endif //BLSTORAGEPOLICYTEST_H
