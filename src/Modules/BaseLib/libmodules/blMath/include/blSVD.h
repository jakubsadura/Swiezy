/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blSVD_h
#define _blSVD_h

#include <vnl/algo/vnl_netlib.h>
#include <vnl/vnl_fortran_copy.h>
#include <vcl_algorithm.h>
#include <v3p_netlib_mangle.h>

#define macro(p, T) \
inline void vnl_linpack_svdc(vnl_netlib_svd_proto(T)) \
{ p##svdc_(vnl_netlib_svd_params); }
macro(s, float);
macro(d, double);
macro(c, vcl_complex<float>);
macro(z, vcl_complex<double>);
#undef macro


class blSVD
{
public: 


/**
 * \brief Returns inverse of a transposed matrix. I.e. if you need to 
 * calculate inverse of a matrix of which you have transpose, give this 
 * transpose to this function.
 * \ingroup blMath
 *
 * VNL_SVD calculates transpose of supplied matrix, so this function 
 * avoids this useless transpose and I tweaked some things so it uses 
 * less memory.
 *
 * I had to put body here because of VC6++ bugs with templates
 * 
 * not sure but it seems to damage the parameter matrix M, that is why 
 * vnl created a copy.
 */
template <class T>
static int PInverseOfTransposedMatrix(
						vnl_matrix<T> const& M, 
						vnl_matrix<T>& inverse, 
						double zero_out_tol)
{
	typedef typename vnl_numeric_traits<T>::abs_t singval_t;

	int m_ = M.cols();
	int n_ = M.rows();

	  
	assert(m_ > 0);
	assert(n_ > 0);

	int result = 0;

	vnl_diag_matrix<singval_t> W_(n_);// Singular values, sorted in decreasing order
	vnl_diag_matrix<singval_t> Winverse_(n_);
	vnl_matrix<T> V_(n_, n_);       // Columns Vi are basis for nullspace of M for Wi = 0
//	vnl_matrix<T> U_(m_, n_);        // Columns Ui are basis for range of M for Wi != 0
	vnl_matrix<T> U_(n_, m_);        // Columns Ui are basis for range of M for Wi != 0

	{
		long int n = m_;
		long int p = n_;
		int mm = vcl_min(n+1,p);

		// Copy source matrix into fortran storage
		// SVD is slow, don't worry about the cost of this transpose.
		//vnl_fortran_copy<T> X(M);

		// Make workspace vectors.
		vnl_vector<T> work(n, T(0));
		//vnl_vector<T> uspace(n*p, T(0));
		//vnl_vector<T> vspace(p*p, T(0));
		vnl_vector<T> wspace(mm, T(0)); // complex fortran routine actually _wants_ complex W!
		vnl_vector<T> espace(p, T(0));


		// Call Linpack SVD
		long int info = 0;
		const long int job = 21; // min(n,p) svs in U, n svs in V (i.e. economy size)
		vnl_linpack_svdc((T*)M.data_block(), &n, &n, &p,
                     wspace.data_block(),
                     espace.data_block(),
                     U_.data_block(), &n,
                     V_.data_block(), &p,
                     work.data_block(),
                     &job, &info);

		// Error return?
		if (info != 0)
		{
			// If info is non-zero, it contains the number of singular values
			// for this the SVD algorithm failed to converge. The condition is
			// not bogus. Even if the returned singular values are sensible,
			// the singular vectors can be utterly wrong.

			// It is possible the failure was due to NaNs or infinities in the
			// matrix. Check for that now.
			M.assert_finite();

			// If we get here it might be because
			// 1. The scalar type has such
			// extreme precision that too few iterations were performed to
			// converge to within machine precision (that is the svdc criterion).
			// One solution to that is to increase the maximum number of
			// iterations in the netlib code.
			//
			// 2. The LINPACK dsvdc_ code expects correct IEEE rounding behaviour,
			// which some platforms (notably x86 processors)
			// have trouble doing. For example, gcc can output
			// code in -O2 and static-linked code that causes this problem.
			// One solution to this is to persuade gcc to output slightly different code
			// by adding and -fPIC option to the command line for v3p\netlib\dsvdc.c. If
			// that doesn't work try adding -fbloat-store, which should fix the problem
			// at the expense of being significantly slower for big problems. Note that
			// if this is the cause, vxl/vnl/tests/test_svd should have failed.
			//
			// You may be able to diagnose the problem here by printing a warning message.
			vcl_cerr << __FILE__ ": suspicious return value (" << info << ") from SVDC\n"
		         << __FILE__ ": M is " << M.rows() << 'x' << M.cols() << vcl_endl;

			//vnl_matlab_print(vcl_cerr, M, "M", vnl_matlab_print_format_long);
			return -1;
		}


		for (int j = 0; j < mm; ++j)
			W_(j, j) = vcl_abs(wspace(j)); // we get rid of complexness here.

		for (int j = mm; j < n_; ++j)
			W_(j, j) = 0;

	}


	int rank_ = W_.rows();

	if (zero_out_tol >= 0) // Zero out small sv's and update rank count.
	{
	  for (unsigned k = 0; k < W_.rows(); k++)
	  {
		singval_t& weight = W_(k, k);
		if (vcl_abs(weight) <= zero_out_tol)
		{
		  Winverse_(k,k) = 0;
		  weight = 0;
		  --rank_;
		} else
		{
		  Winverse_(k,k) = singval_t(1.0)/weight;
		}
	  }
	}

	//calc inverse
	vnl_matrix<T> Winverse(Winverse_.rows(),Winverse_.columns());
	Winverse.fill(T(0));
	for (unsigned i=0;i<rank_;i++)
		Winverse(i,i)=Winverse_(i,i);


	//inverse = V_.transpose() * Winverse * U_;

	inverse.set_size(V_.rows(), V_.cols());
	vnl_fastops::AtB( inverse, V_, Winverse );

	inverse *= U_;

	return result;
}


};




#endif
