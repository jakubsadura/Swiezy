/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDTITensor.txx,v $
  Language:  C++
  Date:      $Date: 2006/03/19 04:36:58 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkDTITensor_txx
#define _itkDTITensor_txx

#include "itkDTITensor.h"
#include <itkSymmetricEigenAnalysis.h>
#include <itkSymmetricSecondRankTensor.h>
#include <itkMatrix.h>
#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"

namespace itk
{
/*---------------------------------------------------------------------------**/
/** Constructors...*/
template<class T>
DTITensor<T>::DTITensor(){
	this->Fill( itk::NumericTraits<ComponentType>::Zero );
	//m_Tolerance = 1e-5;
}

template<class T>
DTITensor<T>::DTITensor( const Self & r ):Superclass(r){//m_Tolerance = 1e-5;
}

template<class T>
DTITensor<T>::DTITensor( const Superclass & r ):Superclass(r){//m_Tolerance = 1e-5;
}


template<class T>
DTITensor<T>::DTITensor( const ComponentType & r )
{
	this->Fill( r );
//	m_Tolerance = 1e-5;
}

template<class T>
DTITensor<T>::DTITensor( const ComponentArrayType r ):Superclass(r){//m_Tolerance = 1e-5;
} 
/*---------------------------------------------------------------------------**/


/*---------------------------------------------------------------------------**/
/** Assignment operators...*/
template<class T>
DTITensor<T>& DTITensor<T>::operator= (const Self& r)
{
  Superclass::operator=(r);
  return *this;
}

template<class T>
DTITensor<T>& DTITensor<T>::operator= (const ComponentType & r)
{
  Superclass::operator=(r);
  return *this;
}

template<class T>
DTITensor<T>& DTITensor<T>::operator= (const ComponentArrayType r)
{
  Superclass::operator=(r);
  return *this;
}

template<class T>
DTITensor<T>& DTITensor<T>::operator= (const Superclass & r)
{
  Superclass::operator=(r);
  return *this;
}
/*---------------------------------------------------------------------------**/

/*---------------------------------------------------------------------------**/
/** Addition and substraction operators... */
template<class T>
DTITensor<T> DTITensor<T>::operator+(const DTITensor<T> &tensor) const
{
	Self result;
	result[0] = (*this)[0] + tensor[0];
	result[1] = (*this)[1] + tensor[1];
	result[2] = (*this)[2] + tensor[2];
	result[3] = (*this)[3] + tensor[3];
	result[4] = (*this)[4] + tensor[4];
	result[5] = (*this)[5] + tensor[5];
	return result;
}
template<class T>
DTITensor<T> DTITensor<T>::operator-(const DTITensor<T> &tensor) const
{
	Self result;
	result[0] = (*this)[0] - tensor[0];
	result[1] = (*this)[1] - tensor[1];
	result[2] = (*this)[2] - tensor[2];
	result[3] = (*this)[3] - tensor[3];
	result[4] = (*this)[4] - tensor[4];
	result[5] = (*this)[5] - tensor[5];
	return result;
}
template<class T>
const DTITensor<T>& DTITensor<T>::operator+=(const DTITensor<T> &tensor)
{
	(*this)[0] += tensor[0];
	(*this)[1] += tensor[1];
	(*this)[2] += tensor[2];
	(*this)[3] += tensor[3];
	(*this)[4] += tensor[4];
	(*this)[5] += tensor[5];
	return *this;
}
template<class T>
const DTITensor<T>& DTITensor<T>::operator-=(const DTITensor<T> &tensor)
{
	(*this)[0] -= tensor[0];
	(*this)[1] -= tensor[1];
	(*this)[2] -= tensor[2];
	(*this)[3] -= tensor[3];
	(*this)[4] -= tensor[4];
	(*this)[5] -= tensor[5];
	return *this;
}
/*---------------------------------------------------------------------------**/

/*---------------------------------------------------------------------------**/
/** Addition and substraction operators with scalar values... */
template<class T>
DTITensor<T> DTITensor<T>::operator+(const RealType & scalar) const
{
	Self result;
	result[0] = (*this)[0] + scalar;
	result[1] = (*this)[1] + scalar;
	result[2] = (*this)[2] + scalar;
	result[3] = (*this)[3] + scalar;
	result[4] = (*this)[4] + scalar;
	result[5] = (*this)[5] + scalar;
	return result;
}
template<class T>
DTITensor<T> DTITensor<T>::operator-(const RealType & scalar) const
{
	Self result;
	result[0] = (*this)[0] - scalar;
	result[1] = (*this)[1] - scalar;
	result[2] = (*this)[2] - scalar;
	result[3] = (*this)[3] - scalar;
	result[4] = (*this)[4] - scalar;
	result[5] = (*this)[5] - scalar;
	return result;
}
template<class T>
const DTITensor<T>& DTITensor<T>::operator+=(const RealType & scalar)
{
	(*this)[0] += scalar;
	(*this)[1] += scalar;
	(*this)[2] += scalar;
	(*this)[3] += scalar;
	(*this)[4] += scalar;
	(*this)[5] += scalar;
	return *this;
}
template<class T>
const DTITensor<T>& DTITensor<T>::operator-=(const RealType & scalar)
{
	(*this)[0] -= scalar;
	(*this)[1] -= scalar;
	(*this)[2] -= scalar;
	(*this)[3] -= scalar;
	(*this)[4] -= scalar;
	(*this)[5] -= scalar;
	return *this;
}
/*---------------------------------------------------------------------------**/

/*---------------------------------------------------------------------------**/
/** Product/division with scalars */
template<class T>
DTITensor<T> DTITensor<T>::operator*(const RealType &scalar) const
{
	Self result;
	result[0] = (*this)[0] * scalar;
	result[1] = (*this)[1] * scalar;
	result[2] = (*this)[2] * scalar;
	result[3] = (*this)[3] * scalar;
	result[4] = (*this)[4] * scalar;
	result[5] = (*this)[5] * scalar;
	return result;
}
template<class T>
DTITensor<T> DTITensor<T>::operator/(const RealType &scalar) const
{
	Self result;
	RealType scalar2 = 1.0f/scalar;
	result[0] = (*this)[0] * scalar2;
	result[1] = (*this)[1] * scalar2;
	result[2] = (*this)[2] * scalar2;
	result[3] = (*this)[3] * scalar2;
	result[4] = (*this)[4] * scalar2;
	result[5] = (*this)[5] * scalar2;
	return result;
}
template<class T>
const DTITensor<T>& DTITensor<T>::operator*=(const RealType &scalar)
{
	(*this)[0] *= scalar;
	(*this)[1] *= scalar;
	(*this)[2] *= scalar;
	(*this)[3] *= scalar;
	(*this)[4] *= scalar;
	(*this)[5] *= scalar;
	return *this;
}
template<class T>
const DTITensor<T>& DTITensor<T>::operator/=(const RealType &scalar)
{
	RealType scalar2 = 1.0f/scalar;
	(*this)[0] *= scalar2;
	(*this)[1] *= scalar2;
	(*this)[2] *= scalar2;
	(*this)[3] *= scalar2;
	(*this)[4] *= scalar2;
	(*this)[5] *= scalar2;
	return *this;
}
/*---------------------------------------------------------------------------**/
	
/*---------------------------------------------------------------------------**/
/** Matrix-based indexing. No bounds checking!!! */
template<class T>
const typename DTITensor<T>::ComponentType&
DTITensor<T>::operator()( unsigned int row, unsigned int col ) const
{
	return (  row<col ? (*this)[3*row+col-row*(row+1)/2] : (*this)[3*col+row-col*(col+1)/2] );
}
template<class T>
typename DTITensor<T>::ComponentType&
DTITensor<T>::operator()( unsigned int row, unsigned int col )
{
	return (  row<col ? (*this)[3*row+col-row*(row+1)/2] : (*this)[3*col+row-col*(col+1)/2] );
}
/*---------------------------------------------------------------------------**/

	
/*---------------------------------------------------------------------------**/
/** Tensor comparisons */
template<class T>
bool DTITensor<T>::operator>( const Self& tensor ) const
{
	return (  this->GetTrace() > tensor.GetTrace()  );
}
template<class T>
bool DTITensor<T>::operator>=( const Self& tensor ) const
{
	return (  this->GetTrace() >= tensor.GetTrace()  );
}
template<class T>
bool DTITensor<T>::operator<( const Self& tensor ) const
{
	return (  this->GetTrace() < tensor.GetTrace()  );
}
template<class T>
bool DTITensor<T>::operator<=( const Self& tensor ) const
{
	return (  this->GetTrace() <= tensor.GetTrace()  );
}
/*---------------------------------------------------------------------------**/
	

	
/*---------------------------------------------------------------------------**/
/** Scalar comparisons */
template<class T>
bool DTITensor<T>::operator>( const RealType& scalar ) const
{
	return (  this->GetTrace() > scalar  );
}
template<class T>
bool DTITensor<T>::operator>=( const RealType& scalar ) const
{
	return (  this->GetTrace() >= scalar  );
}
template<class T>
bool DTITensor<T>::operator<( const RealType& scalar ) const
{
	return (  this->GetTrace() < scalar  );
}
template<class T>
bool DTITensor<T>::operator<=( const RealType& scalar ) const
{
	return (  this->GetTrace() <= scalar  );
}
/*---------------------------------------------------------------------------**/
	
	
	
/** Inner product between tensors */
template<class T>
typename itk::NumericTraits<T>::FloatType
DTITensor<T>::operator^( const DTITensor<T> & tensor ) const
{
	RealType product = itk::NumericTraits<RealType>::Zero;
	product += (*this)[1] * tensor[1];
	product += (*this)[2] * tensor[2];
	product += (*this)[4] * tensor[4];
	product *= 2;
	product += (*this)[0] * tensor[0];
	product += (*this)[3] * tensor[3];
	product += (*this)[5] * tensor[5];
	return product;
}


/*---------------------------------------------------------------------------**/
/** Matrix product between tensors */
template<class T>
DTITensor<T> DTITensor<T>::operator*( const DTITensor<T> & tensor ) const
{
	Self result;
	result[0] = ((*this)[0])*tensor[0] + ((*this)[1])*tensor[1] + ((*this)[2])*tensor[2];
	result[1] = ((*this)[0])*tensor[1] + ((*this)[1])*tensor[3] + ((*this)[2])*tensor[4];
	result[2] = ((*this)[0])*tensor[2] + ((*this)[1])*tensor[4] + ((*this)[2])*tensor[5];
	result[3] = ((*this)[1])*tensor[1] + ((*this)[3])*tensor[3] + ((*this)[4])*tensor[4];
	result[4] = ((*this)[1])*tensor[2] + ((*this)[3])*tensor[4] + ((*this)[4])*tensor[5];
	result[5] = ((*this)[2])*tensor[2] + ((*this)[4])*tensor[4] + ((*this)[5])*tensor[5];
	return result;
}

template<class T>
const DTITensor<T>& DTITensor<T>::operator*=( const DTITensor<T> & tensor )
{
	Self result;
	result[0] = ((*this)[0])*tensor[0] + ((*this)[1])*tensor[1] + ((*this)[2])*tensor[2];
	result[1] = ((*this)[0])*tensor[1] + ((*this)[1])*tensor[3] + ((*this)[2])*tensor[4];
	result[2] = ((*this)[0])*tensor[2] + ((*this)[1])*tensor[4] + ((*this)[2])*tensor[5];
	result[3] = ((*this)[1])*tensor[1] + ((*this)[3])*tensor[3] + ((*this)[4])*tensor[4];
	result[4] = ((*this)[1])*tensor[2] + ((*this)[3])*tensor[4] + ((*this)[4])*tensor[5];
	result[5] = ((*this)[2])*tensor[2] + ((*this)[4])*tensor[4] + ((*this)[5])*tensor[5];
	(*this) = result;
	return (*this);
}
/*---------------------------------------------------------------------------**/
	
	
/** Identity matrix/tensor */
template<class T>
void DTITensor<T>::SetIdentity()
{
	this->Fill( itk::NumericTraits<ComponentType>::Zero );
	(*this)[0] = static_cast<ComponentType>( 1 );
	(*this)[3] = static_cast<ComponentType>( 1 );
	(*this)[5] = static_cast<ComponentType>( 1 );
}

/** Get the trace */
template<class T>
typename itk::NumericTraits<T>::FloatType
DTITensor<T>::GetTrace() const
{
	return (   static_cast<RealType>( (*this)[0] )   +   static_cast<RealType>( (*this)[3] )   +   static_cast<RealType>( (*this)[5] )   );
}


/**
 *  Compute the value of fractional anisotropy
 */
template<class T>
typename itk::NumericTraits<T>::FloatType
DTITensor<T>::GetFractionalAnisotropy() const
{
	// Compute the squared Frobenius norm:
	RealType isp   = (*this)^(*this);
	if( isp > TOL ){
		RealType trace      = this->GetTrace();
		RealType anisotropy = ( 3*isp - trace*trace )/( 2*isp );
		return static_cast< RealType >( vcl_sqrt(anisotropy) );
    }
    return itk::NumericTraits<RealType>::Zero;
}


/**
 *  Compute the value of relative anisotropy
 */
template<class T>
typename itk::NumericTraits<T>::FloatType
DTITensor<T>::GetRelativeAnisotropy() const
{
	// Basser and Perpaoli implementation, eq. [14]
	// Compute the trace and the Frobenius norm:
	RealType trace = this->GetTrace();
	RealType isp   = (*this)^(*this);
	
	// Avoid negative trace and traces small enough to look like a division by zero.
	if( trace < TOL )
		return itk::NumericTraits<RealType>::Zero;
	
	RealType anisotropy = 3*isp - trace*trace;
	
	if( anisotropy  < TOL )
		return itk::NumericTraits<RealType>::Zero;
	
	anisotropy = static_cast<RealType>(  vcl_sqrt(anisotropy )   );
	return (anisotropy/trace);
}
	
/**
 *  Compute the value of relative anisotropy
 */
template<class T>
typename itk::NumericTraits<T>::FloatType
DTITensor<T>::GetDeterminant() const
{
	RealType det = itk::NumericTraits<RealType>::Zero;
	
	det += ((*this)[0])*((*this)[3])*((*this)[5]);
	det += 2.0f*((*this)[1])*((*this)[2])*((*this)[4]);
	det -= ((*this)[0])*((*this)[4])*((*this)[4]);
	det -= ((*this)[1])*((*this)[1])*((*this)[5]);
	det -= ((*this)[2])*((*this)[2])*((*this)[3]);
	
	return det;
}

	
/**
 *  Compute the mode of the tensor as in Kindlmann et al.
 */
template<class T>
typename itk::NumericTraits<T>::FloatType
DTITensor<T>::GetMode() const
{
	RealType trace = this->GetTrace();
	RealType dNorm = (*this)^(*this) - (1.0f/3.0f)*trace*trace;
	if( dNorm < TOL )
		return itk::NumericTraits<RealType>::Zero;
	dNorm = sqrt( dNorm );
	dNorm = 7.3484692283495342946 / ( dNorm*dNorm*dNorm );
	return ( this->GetDeviatoric->GetDeterminant() * dNorm );
}
	
	
/**
 *  Compute the deviatoric tensor
 */
template<class T>
DTITensor<T> DTITensor<T>::GetDeviatoric() const
{
	Self identity;
	identity->SetIdentity();
	identity *= ( (1.0f/3.0f)*(this->GetTrace()) );
	return ( *this - identity );
}
	
/**
 *  Compute the mean diffusivity
*/
template<class T>
typename itk::NumericTraits<T>::FloatType
DTITensor<T>::GetMeanDiffusivity() const
{
	return ( (this->GetTrace()/3));
}
		

/** Compute the geometric parameters of the tensor that describes its shape */
template<class T>
void DTITensor<T>::ComputeShapeCoefficients( RealType& cl, RealType& cp, RealType& cs )
{
	cl = cp = cs = itk::NumericTraits<RealType>::Zero;
	// Compute the eigen-values:
	EigenValuesArrayType eig;
	this->ComputeEigenValues( eig );
	// Make sure the first eigenvalue is not null:
	if( eig[0] < TOL )
		return;
	// Inverse of the first eigen-value:
	RealType div = itk::NumericTraits<RealType>::One/eig[0];
	// Computation of shape parameters:
	cs  = eig[2] * div;
	cp  = eig[1] * div;
	cl  = 1 - cp;
	cp -= cs;
	return;
}
	
	
	
/** Compute the eigenvalues of the tensor. Direct method!!! */
template<class T>
void DTITensor<T>::ComputeEigenValues( EigenValuesArrayType& eig ) const
{
	// Definition of the coefficients of the characteristic 
	// polynomial (the coefficient in lambda^3 is simply 1):
	
	RealType d12s = ((*this)[1])*((*this)[1]);
	RealType d13s = ((*this)[2])*((*this)[2]);
	RealType d23s = ((*this)[4])*((*this)[4]);
	
	RealType b  =  -(   ((*this)[0]) + ((*this)[3]) + ((*this)[5])   );
	RealType c  =  ((*this)[0])*((*this)[3]) + ((*this)[0])*((*this)[5]) + ((*this)[3])*((*this)[5]);
	c          -=  ( d12s + d13s + d23s );
	RealType d  =  -((*this)[0])*((*this)[3])*((*this)[5]) - 2*((*this)[1])*((*this)[2])*((*this)[4]);
	d          +=  d13s*((*this)[3]) + d12s*((*this)[5]) + d23s*((*this)[0]);
	
	// Computation of auxiliar values:
	RealType disc = 12*c*c*c - 3*b*b*c*c - 54*b*c*d + 81*d*d + 12*b*b*b*d;
	RealType alpha = c - (1.0f/3.0f)*b*b;
	RealType res  = (1.0f/3.0f)*b;
	RealType xiR = 36*b*c - 108*d - 8*b*b*b;
	RealType xiI = itk::NumericTraits<RealType>::Zero;
	RealType lambda1;
	RealType lambda2;
	RealType lambda3;
	if( disc >= itk::NumericTraits<RealType>::Zero ){
		RealType xiRi;
		xiR     += 12*::sqrt(  disc );
		if( xiR > TOL ){
			xiR      = ::pow( (double)xiR, (double)(1.0f/3.0f) );
			xiRi = 1.0f/xiR;
		}
		else if( xiR < -TOL ){
			xiR      = -::pow( -(double)xiR, (double)(1.0f/3.0f) );
			xiRi = 1.0f/xiR;
		}
		else
			xiR = xiRi = itk::NumericTraits<RealType>::Zero;
		lambda1  =  (1.0f/6.0f)*xiR - 2*alpha*xiRi;
		lambda2  = -0.5f*lambda1 - res;
		lambda1 -= res;
		lambda3  = lambda2;
	}
	else{
		xiI  = 12*::sqrt( -disc );
		RealType phi = static_cast<RealType>( 0.523598775598299 );
		if(   vcl_abs(xiR) > TOL   )
			phi = (1.0f/3.0f)*::atan( xiI/xiR );
		RealType xiAbs = ::pow( (double)xiI*xiI+xiR*xiR, (double)(1.0f/6.0f) );
		RealType cphi  = ::cos( phi );
		RealType sphi  = ::sin( phi );
		xiR = xiAbs*cphi;
		xiI = xiAbs*sphi;
		RealType xiAbsi = 1.0f/xiAbs;
		RealType xiRi   =  xiAbsi*cphi;
		RealType xiIi   = -xiAbsi*sphi;
		
		lambda1 = (1.0f/6.0f)*xiR - 2*alpha*xiRi;
		lambda2 = -0.5f*lambda1;
		lambda3 = lambda2;
		RealType imPart = (0.144337567297406*xiI + 1.732050807568877*xiIi*alpha);
		lambda2 -= imPart;
		lambda3 += imPart;
		lambda1 -= res;
		lambda2 -= res;
		lambda3 -= res;
	}
	// Order the eigenvalues:
	eig[0]=lambda1; eig[1]=lambda2; eig[2]=lambda3;
	if( lambda1<lambda3 ){
		eig[0] = lambda3;
		eig[2] = lambda1;
	}
	if( lambda2<eig[2] ){
		eig[1] = eig[2];
		eig[2] = lambda2;
	}
	else if( lambda2>eig[0] ){
		eig[1] = eig[0];
		eig[0] = lambda2;
	}
	return;
}

	
/** Compute one single eigenvector associated to a given eigen-vector */
// Be careful!!! This method may only be used once you have checked that
// eigval is an eigenvalue with multiplicity 1!!!!
template<class T>
void DTITensor<T>
::ComputeEigenVector( RealType eigval, RealType* eigvec ) const
{
	// Compute an adequate tolerance:
	double e_tol = ( (*this)[0] + (*this)[3] + (*this)[5] );
	e_tol        = 0.001f * e_tol * e_tol;
	// Compute the corrected matrix:
	RealType matrix[3][3];
	matrix[0][0] = (*this)[0] - eigval;
	matrix[1][1] = (*this)[3] - eigval;
	matrix[2][2] = (*this)[5] - eigval;
	matrix[0][1] = matrix[1][0] = (*this)[1];
	matrix[0][2] = matrix[2][0] = (*this)[2];
	matrix[1][2] = matrix[2][1] = (*this)[4];
	// Find a minor with non-null determinant; there must be at least one, since
	// the eigen-value has multiplicity 1
	for( unsigned int r1=0; r1<2; ++r1 ){ // The whole four nested loops comprise at most 9 computations of det
		for( unsigned int r2=r1+1; r2<3; ++r2 ){
			for( unsigned int c1=0; c1<2; ++c1 ){
				for( unsigned int c2=c1+1; c2<3; ++c2 ){
					RealType det = matrix[r1][c1]*matrix[r2][c2] - matrix[r2][c1]*matrix[r1][c2];
					if( vcl_abs(det) > e_tol ){
						// We have found the minor, find the other column:
						unsigned int cr = 1 + (c1+c2)%2 - c1 - c1;
						// Find the independent term
						RealType b1 = -matrix[r1][cr];
						RealType b2 = -matrix[r2][cr];
						// Solve the system:
						det = 1/det;
						eigvec[cr] = 1;
						eigvec[c1] = det*( matrix[r2][c2]*b1 - matrix[r1][c2]*b2 );
						eigvec[c2] = det*( matrix[r1][c1]*b2 - matrix[r2][c1]*b1 );
						// Normalise to have unit norm:
						RealType norm = eigvec[0]*eigvec[0] + eigvec[1]*eigvec[1] + eigvec[2]*eigvec[2];
						norm = 1/vcl_sqrt( norm );
						eigvec[0] *= norm;
						eigvec[1] *= norm;
						eigvec[2] *= norm;
						// Return!
						return;
					}
				}
			}
		}
	}
}

	
/** Compute the whole eigen-system */
template<class T>
void DTITensor<T>
::ComputeEigenSystem( EigenValuesArrayType& eigval, EigenVectorsMatrixType& eigvec )
{
	// First of all, compute the eigenvalues, together with the rank of the filter:
	this->ComputeEigenValues( eigval );
	// Find the number of different eigenvalues; depending on this, we have more or less degrees of freedom:
	unsigned int indicator[3] = {0,1,2}; // Auxiliar value to check which eigen-values we have to compute
	unsigned int ndiffeig=3;             // The number of differente eigenvalues
	if(   vcl_abs(eigval[1]-eigval[0])  <  TOL  ){
		--ndiffeig;
		indicator[0] = 2; indicator[1] = 0; indicator[2] = 1;
	}
	if(   vcl_abs(eigval[2]-eigval[1])  <  TOL  )
		--ndiffeig;
	// Compute the eigenvectors depending on the number of different eigenvalues:
	RealType ev[3];
	RealType nr;
	switch( ndiffeig ){
		case 1: // The trivial case: all eigenvalues are equal
			eigvec.Fill( itk::NumericTraits<RealType>::Zero );
			eigvec[0][0] = eigvec[1][1] = eigvec[2][2] = itk::NumericTraits<RealType>::One;
			break;
		case 2: // One different eigenvalue; the others are equal
			this->ComputeEigenVector( eigval[indicator[0]], ev );
			eigvec[0][indicator[0]]=ev[0]; eigvec[1][indicator[0]]=ev[1]; eigvec[2][indicator[0]]=ev[2];
			nr = vcl_sqrt(ev[0]*ev[0]+ev[1]*ev[1]);
			if( nr>TOL ){
				nr = 1/nr;
				eigvec[0][indicator[1]]=ev[1]*nr; eigvec[1][indicator[1]]=-ev[0]*nr; eigvec[2][indicator[1]]=0;
				eigvec[0][indicator[2]] = eigvec[1][indicator[0]]*eigvec[2][indicator[1]] - eigvec[1][indicator[1]]*eigvec[2][indicator[0]];
				eigvec[1][indicator[2]] = eigvec[0][indicator[1]]*eigvec[2][indicator[0]] - eigvec[0][indicator[0]]*eigvec[2][indicator[1]];
				eigvec[2][indicator[2]] = eigvec[0][indicator[0]]*eigvec[1][indicator[1]] - eigvec[0][indicator[1]]*eigvec[1][indicator[0]];
			}
			else{
				eigvec[0][indicator[1]]=1; eigvec[1][indicator[1]]=0; eigvec[2][indicator[1]]=0;
				eigvec[0][indicator[2]]=0; eigvec[1][indicator[2]]=1; eigvec[2][indicator[2]]=0;
			}
			break;
		default: // The general case: three different eigenvalues
			this->ComputeEigenVector( eigval[indicator[0]], ev );
			eigvec[0][indicator[0]]=ev[0]; eigvec[1][indicator[0]]=ev[1]; eigvec[2][indicator[0]]=ev[2];
			this->ComputeEigenVector( eigval[indicator[1]], ev );
			eigvec[0][indicator[1]]=ev[0]; eigvec[1][indicator[1]]=ev[1]; eigvec[2][indicator[1]]=ev[2];
			eigvec[0][indicator[2]] = eigvec[1][indicator[0]]*eigvec[2][indicator[1]] - eigvec[1][indicator[1]]*eigvec[2][indicator[0]];
			eigvec[1][indicator[2]] = eigvec[0][indicator[1]]*eigvec[2][indicator[0]] - eigvec[0][indicator[0]]*eigvec[2][indicator[1]];
			eigvec[2][indicator[2]] = eigvec[0][indicator[0]]*eigvec[1][indicator[1]] - eigvec[0][indicator[1]]*eigvec[1][indicator[0]];
			break;
	}
	return;
}
	
/** Get color code: RGB value coded the major eignevector weighted by FA **/
template<class T>                             
typename DTITensor<T>::RGBPixelType           
DTITensor<T>::GetRGBColorCode(){              
    EigenValuesArrayType eigval;              
	ComputeEigenValues(eigval);                  
	                                             
	RealType eigvec[3];                          
	ComputeEigenVector( eigval[0], eigvec);      
                                              
    RealType fa;                              
	fa=GetFractionalAnisotropy();                
	                                             
	RGBPixelType colorCoding;                    
                                              
	for(unsigned i=0; i<3; i++){                 
		colorCoding[i]=fa*fabs(eigvec[i]);          
	}                                            
	return colorCoding;
}

/** Esta es otra forma, calculando los autovectores y autovalores como lo hace itk **/
/** Get color code: RGB value coded the major eignevector weighted by FA **/
/*
template<class T>
typename DTITensor<T>::RGBPixelType
DTITensor<T>::GetRGBColorCode(){    

    itk::SymmetricSecondRankTensor<float,3> symTensor;
	for(unsigned i=0; i<6; i++){
		symTensor[i]=(*this)[i];
	}
	
	itk::SymmetricSecondRankTensor<float,3>::EigenValuesArrayType eigenValues;
	itk::SymmetricSecondRankTensor<float,3>::EigenVectorsMatrixType eigenVectors;
	//ComputeEigenValues( eigenValues );
	symTensor.ComputeEigenAnalysis(eigenValues, eigenVectors);
    
	RealType fa;
	fa=GetFractionalAnisotropy();
    RGBPixelType colorCoding;   
	for(unsigned i=0; i<3; i++){
		colorCoding[i]=fa*fabs(eigenVectors[2][i]);
	}
	return colorCoding;
}
*/
	
/** Compute the tensor from its eigen-system */
template<class T>
void DTITensor<T>::ComputeTensor( EigenVectorsMatrixType eigvec, EigenValuesArrayType eigval )
{
	(*this)[0] = eigval[0]*eigvec(0,0)*eigvec(0,0) + eigval[1]*eigvec(0,1)*eigvec(0,1) + eigval[2]*eigvec(0,2)*eigvec(0,2);
	(*this)[1] = eigval[0]*eigvec(0,0)*eigvec(1,0) + eigval[1]*eigvec(0,1)*eigvec(1,1) + eigval[2]*eigvec(0,2)*eigvec(1,2);
	(*this)[2] = eigval[0]*eigvec(0,0)*eigvec(2,0) + eigval[1]*eigvec(0,1)*eigvec(2,1) + eigval[2]*eigvec(0,2)*eigvec(2,2);
	(*this)[3] = eigval[0]*eigvec(1,0)*eigvec(1,0) + eigval[1]*eigvec(1,1)*eigvec(1,1) + eigval[2]*eigvec(1,2)*eigvec(1,2);
	(*this)[4] = eigval[0]*eigvec(1,0)*eigvec(2,0) + eigval[1]*eigvec(1,1)*eigvec(2,1) + eigval[2]*eigvec(1,2)*eigvec(2,2);
	(*this)[5] = eigval[0]*eigvec(2,0)*eigvec(2,0) + eigval[1]*eigvec(2,1)*eigvec(2,1) + eigval[2]*eigvec(2,2)*eigvec(2,2);
}


/**
* Compute the vector in the Log-Euclidean space
*/
template<class T>
DTITensor<T>
DTITensor<T>
::ComputeLogVector() const
{
	EigenValuesArrayType eigval;
	EigenVectorsMatrixType eigvec;
	ComputeEigenSystem(eigval, eigvec);
	
	Matrix<T, 3, 3> logEigval;
	logEigval.Fill(0.0);
	
	for(unsigned i=0; i<3; i++){
		logEigval(i,i)=log(eigval(i));
			} 	
	Matrix<T,3,3> logMatrix;
	logMatrix=eigvec.GetTranspose()*logEigval*eigvec;
	
	DTITensor<T> result;
	result[0]=logMatrix[0][0];
	result[1]=logMatrix[1][1];	
	result[2]=logMatrix[2][2];
	result[3]=sqrt(2)*logMatrix[0][1];
	result[4]=sqrt(2)*logMatrix[0][2];
	result[5]=sqrt(2)*logMatrix[1][2];
	
	return result;
}


/**
* Compute the vector in the Log-Euclidean space with weigthing factor
*/
template<class T>
DTITensor<T>
DTITensor<T>
::ComputeLogVector(Vector<T,6> weights) const
{
	EigenValuesArrayType eigval;
	EigenVectorsMatrixType eigvec;
	ComputeEigenSystem(eigval, eigvec);
	
	Matrix<T, 3, 3> logEigenValues;
	logEigenValues.Fill(0.0);
	
	for(unsigned i=0; i<3; i++){
		logEigenValues(i,i)=log(eigval(i));
			} 	
	Matrix<T,3,3> logMatrix;
	logMatrix=eigvec.GetTranspose()*logEigenValues*eigvec;
	
	DTITensor<T> result;
	result[0]=weights[0]*logMatrix[0][0];
	result[1]=weights[1]*logMatrix[1][1];	
	result[2]=weights[2]*logMatrix[2][2];
	result[3]=weights[3]*logMatrix[0][1];
	result[4]=weights[4]*logMatrix[0][2];
	result[5]=weights[5]*logMatrix[1][2];
	
	return result;
}

/**
* Translate the tensor to the original space from the log euclidean space
*/
template<class T>
DTITensor<T>
DTITensor<T>
::ComputeTensorFromLogVector() const
{
	EigenValuesArrayType logEigenValues;
	EigenVectorsMatrixType logEigenVectors;

	this[3]/=sqrt(2);
	this[4]/=sqrt(2);
	this[5]/=sqrt(2);

	ComputeEigenSystem(logEigenValues, logEigenVectors);
	
	Matrix<T, 3, 3> eigval;
	eigval.Fill(0.0);
	
	for(unsigned i=0; i<3; i++){
		eigval(i,i)=exp(logEigenValues(i));
			} 	
			
	Matrix<T,3,3> tensorMatrix;
	tensorMatrix=logEigenVectors.GetTranspose()*eigval*logEigenVectors;
	
	DTITensor<T> result;	
	result[0]=tensorMatrix[0][0];
	result[1]=tensorMatrix[0][1];	
	result[2]=tensorMatrix[0][2];
	result[3]=tensorMatrix[1][1];
	result[4]=tensorMatrix[1][2];
	result[5]=tensorMatrix[2][2];
	
	return result;
}

/**
* Translate the tensor to the original space from the log euclidean space
*/
template<class T>
DTITensor<T>
DTITensor<T>
::ComputeTensorFromLogVector(Vector<T,6> weights) const
{
	EigenValuesArrayType logEigenValues;
	EigenVectorsMatrixType logEigenVectors;

	for(unsigned i=0; i<6; i++){
		this[i]/=weights[i];
	}
	ComputeEigenSystem(logEigenValues, logEigenVectors);
	
	Matrix<T, 3, 3> eigval;
	eigval.Fill(0.0);
	
	for(unsigned i=0; i<3; i++){
		eigval(i,i)=exp(logEigenValues(i));
			} 	
			
	Matrix<T,3,3> tensorMatrix;
	tensorMatrix=logEigenVectors.GetTranspose()*eigval*logEigenVectors;
	
	DTITensor<T> result;	
	result[0]=tensorMatrix[0][0];
	result[1]=tensorMatrix[0][1];	
	result[2]=tensorMatrix[0][2];
	result[3]=tensorMatrix[1][1];
	result[4]=tensorMatrix[1][2];
	result[5]=tensorMatrix[2][2];
	
	return result;
}



} // end namespace itk

#endif
