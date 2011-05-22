#ifndef __blOrth_txx 
#define __blOrth_txx

/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>
#include <vnl/vnl_vector_ref.h>
#include <vnl/algo/vnl_svd.h>

#include <vector>
#include <float.h>
#include <blMath.h>
#include <blVector.h>

namespace blOrth
{

template <class TElement> 
void RemoveZeroRows( vnl_matrix<TElement>* matrix, bool transposeResult, std::vector<unsigned int>* nonzero_elements = NULL )
{
	typedef  vnl_vector_ref<TElement> VectorRefType;

	const unsigned int rows = matrix->rows();
	const unsigned int cols = matrix->cols();
	const unsigned int vector_length = cols;

	std::vector<unsigned int> nonzero_rows_indexes;
	nonzero_rows_indexes.reserve( rows );
	
	for( size_t i=0; i<rows; i++ )
	{
		VectorRefType the_row( vector_length, (*matrix)[i] );
		const double one_norm = the_row.one_norm(); //sum of absolute values
		if( one_norm != 0 ) //nonzero row
		{
			nonzero_rows_indexes.push_back(i);
		}
	}

	//fill the new matrix with 
	const unsigned int nonzero_rows = nonzero_rows_indexes.size();
	
	vnl_matrix<TElement> small_matrix;

	if( !transposeResult )
	{
		small_matrix.set_size( nonzero_rows, vector_length );
	
		for( size_t i=0; i<nonzero_rows; i++ )
		{
			small_matrix.set_row( i, (*matrix)[ nonzero_rows_indexes[i] ] );
		}
	}
	else 
	{
		small_matrix.set_size( vector_length, nonzero_rows );
	
		for( size_t i=0; i<nonzero_rows; i++ )
		{
			VectorRefType the_row( vector_length, (*matrix)[ nonzero_rows_indexes[i] ] );
			small_matrix.set_column( i, the_row );
		}
	}
	
	(*matrix) = small_matrix;
	
	if (nonzero_elements != NULL) (*nonzero_elements) = nonzero_rows_indexes;
}




	
//if transposed = false -> vectors are in columns
template <class TElement>
void GramSchmidt( vnl_matrix<TElement>* matrix, bool transposed = false  )
{
	//we will work with rows, it is more convenient for or purpose
	if(!transposed) matrix->inplace_transpose();
	
	const int vector_length = matrix->cols();

	typedef  vnl_vector_ref<TElement> VectorRefType;
	typedef  vnl_vector<TElement> VectorType;
	

	//first vector is already in place, we just normalize it
	{
		VectorRefType u ( vector_length, (*matrix)[0] );
		u /= sqrt( dot_product( u, u ) );
	}

	VectorType v( vector_length );


	for( size_t UInd = 1; UInd < matrix->rows(); UInd++ )
	{ //for each vector
		VectorRefType u ( vector_length, (*matrix)[UInd] );
		v = u;
		
		for( size_t VInd = 0; VInd<UInd; VInd++ )
		{
			VectorRefType u_prev( vector_length, (*matrix)[VInd] );
	
			u -= u_prev * dot_product( v, u_prev );
		}
		
		const double dp = dot_product<double>( u, u );
		if (dp>DBL_EPSILON)
			u /= sqrt( dp );
		else u.fill(0);
		
		
	}

	//if the supplied matrix was not ransposed, i.e. has data in columns, the result must be transposed 
	//so the output will have data in columns
	RemoveZeroRows( matrix, !transposed );
	
}





//if transposed = false -> vectors are in columns
template <class TElement>
void GramSchmidtModified( vnl_matrix<TElement>* matrix, bool transposed = false, vnl_vector<TElement>* lengths=NULL  )
{
	//we will work with rows, it is more convenient for or purpose
	if(!transposed) matrix->inplace_transpose();
	
	const int vector_length = matrix->cols();
	const int number_of_vectors = matrix->rows();

	typedef  vnl_vector_ref<TElement> VectorRefType;
	typedef  vnl_vector<TElement> VectorType;
	
	const bool storeLengths = (lengths!=NULL);
	
	double* vector_lengths = new double[number_of_vectors];
	
	//first vector is already in place, we just normalize it
	{
		VectorRefType u ( vector_length, (*matrix)[0] );
		const double length = sqrt( dot_product( u, u ) ); 
		u /= length;
		vector_lengths[0] = length;
	}

	for( int j=1; j<number_of_vectors; j++)
	{
		VectorRefType u_j ( vector_length, (*matrix)[j] );

		for( int i=0; i<j; i++ )
		{
			VectorRefType u_i( vector_length, (*matrix)[i] );
			
			u_j -= u_i * dot_product( u_j, u_i );
		}

		const double length = sqrt( dot_product( u_j, u_j ) );
		if (length>DBL_EPSILON)
		{
			u_j /= length;
			vector_lengths[j] = length;
		}
		else 
		{
			u_j.fill(0);	
			vector_lengths[j] = 0;
		}
	}



	//if the supplied matrix was not ransposed, i.e. has data in columns, the result must be transposed 
	//so the output will have data in columns
	if( storeLengths )
	{
		std::vector<unsigned int> nonzero_rows;
		RemoveZeroRows( matrix, !transposed, &nonzero_rows );
		
		lengths->set_size( nonzero_rows.size() );
		for(size_t i=0; i<nonzero_rows.size(); i++ ) (*lengths)[i] = vector_lengths[ nonzero_rows[i] ];
	}
	else
		RemoveZeroRows( matrix, !transposed );
	
	delete[] vector_lengths;
}







//if transposed = false -> vectors are in columns
template <class TElement>
void OrthonormalizeViaSVD( vnl_matrix<TElement>* matrix  )
{
	
	vnl_svd<TElement> svd( *matrix /*, DBL_EPSILON*/ );

	const unsigned int m = matrix->rows();
	const unsigned int n = matrix->cols();

	vnl_vector<double> s(1);

	if (m > 1)
	{
		s.set_size( svd.W().rows() );
		s.copy_in( &svd.W()[0] ); //svd::W_ is a vnl_vector
	}
	else if ( m == 1 )
	{
		s[0] = svd.W()[0];
	}
	else 
	{
		s[0] = 0;
	}

	double eps = DBL_EPSILON;
	if ( typeid(TElement) == typeid(float) )
	{
		eps = FLT_EPSILON;
	}

	const double tol = __max(m,n) * s.max_value() * eps;

	unsigned int r = 0;
	for(size_t i=0; i<s.size(); i++)
		if( s[i] > tol ) r++;
	
	(*matrix) = svd.U().get_n_columns(0,r);
}

}

#endif
