/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
//Shape alignment using projective transformation

#include "blProjectiveAlignmentFilter.h"
#include "blAlignment.h"
#include <blSimilarity2DTransform.h>
#include <iostream>
#include <vnl/algo/vnl_matrix_inverse.h>
#include <vnl/vnl_cross.h>
#include <vnl/vnl_vector_ref.h>

blProjectiveAlignmentFilter::blProjectiveAlignmentFilter():
	_numberOfPoints(0),
	_numberOfDimensions(0),
	_eps( 0.001 ),
	_maxIterations( 10 )
{
}


blProjectiveAlignmentFilter::~blProjectiveAlignmentFilter()
{
}


void blProjectiveAlignmentFilter::SetInput(const vnlMatrixDouble & theMatrix, 
					unsigned int numberOfPoints, 
					unsigned int numberOfDimensions)
{
	if( numberOfDimensions!=2 )
		throw "ProjectiveAlignment: Unsupported number of dimensions";

	_numberOfPoints = numberOfPoints;
	_numberOfDimensions = numberOfDimensions;

	_data = theMatrix;
}


int blProjectiveAlignmentFilter::Update()
{

	//compute mean shape
	//blMath::ComputeMeanRow( _data, _mean );
	_mean = _data.get_row(0);

	const int NumberOfLandmarks = _mean.size() / 2; 

	vnl_vector<double> centroid(2);
	blAlignment::Centroid( _mean.begin(), 2, NumberOfLandmarks, centroid.begin() );
	blSimilarity2DTransform::Pointer similarity = blSimilarity2DTransform::New();
	similarity->SetParameters( 1, 0, -centroid[0], -centroid[1] );
	similarity->TransformPoints( _mean );


	//rescale it to unit length 
	_mean.normalize();

	vnl_vector<double> old_mean;

	double error;

	int count = 0;

	blAllign2FacesWithHomology aligner;

	do
	{
		old_mean = _mean;


		//align all shapes to the mean shape
		for( size_t i=0; i<_data.rows(); i++ )
		{
			vnl_vector_ref<double> shape( _data.cols(), _data[i] );
//			vnl_vector<double> shapeout;

//			blAlignment::AlignShape2D_Projective( shape, _mean, shapeout, H );
//			shape.copy_in( shapeout.begin() );
			aligner.AlignFaces( shape, _mean, NumberOfLandmarks  );
			shape.copy_in( aligner.GetAlignedFace().begin() );
		}

		//compute mean shape
		blMath::ComputeMeanRow( _data, _mean );
		//rescale it to unit length 
		_mean.normalize();

		error = (old_mean-_mean).magnitude();
		
		//std::cout<<"blProjectiveAlignment: error "<<error<<std::endl;

		count++;
		
	} while ( error >= _eps && count<_maxIterations );


	return 0;
}


blAllign2FacesWithHomology::blAllign2FacesWithHomology()
{
	_imm_transform_dependencies.nose_tip.push_back(58);

	for( int i=0; i<=5; i++ ) _imm_transform_dependencies.left_plane.push_back(i);
	for( int i=7; i<=12; i++ ) _imm_transform_dependencies.right_plane.push_back(i);
	for( int i=13; i<=57; i++ ) _imm_transform_dependencies.front_plane.push_back(i);
	_imm_transform_dependencies.front_plane.push_back(6); //add chin

	////////////////////
	_imm_transform_dependencies.left_plane.push_back(39);
	_imm_transform_dependencies.right_plane.push_back(43);
	/////////////////////

	_imm_transform_dependencies.left_line.push_back(25);
	_imm_transform_dependencies.left_line.push_back(52);

	_imm_transform_dependencies.right_line.push_back(17);
	_imm_transform_dependencies.right_line.push_back(52);

	_imm_transform_dependencies.nose_line.push_back(17);
	_imm_transform_dependencies.nose_line.push_back(25);

}





void blAllign2FacesWithHomology::AlignFaces(const vnlVectorDouble& shape, const vnlVectorDouble& refShape, int NumberOfLandmarks)
{
	vnlMatrixDouble H; //homography

	vnlVectorDouble frontal_part_shape = ExtractSubshape( shape, _imm_transform_dependencies.front_plane );

	vnlVectorDouble frontal_part_ref_shape = ExtractSubshape( refShape, _imm_transform_dependencies.front_plane );

	vnlVectorDouble frontal_aligned_shape( frontal_part_shape.size() );

	blAlignment::AlignShape2D_Projective( frontal_part_shape, frontal_part_ref_shape, frontal_aligned_shape, H );

	_alignedShape.set_size( shape.size() );
	_alignedShape.copy_in( shape.begin() );
	blAlignment::TransformPointsHomog( _alignedShape, H );

	//ToMatlab( shape.begin(), shape.size(), "c:\\1\\shape.m", "s" );
	//ToMatlab( refShape.begin(), refShape.size(), "c:\\1\\rshape.m", "r" );
	//ToMatlab( _alignedShape.begin(), _alignedShape.size(), "c:\\1\\ashape.m", "a" );

	//
	//
	//% MathType!MTEF!2!1!+-
	//% feaafiart1ev1aaatCvAUfeBSjuyZL2yd9gzLbvyNv2CaerbuLwBLn
	//% hiov2DGi1BTfMBaeXatLxBI9gBaerbd9wDYLwzYbItLDharqqtubsr
	//% 4rNCHbGeaGqiVu0Je9sqqrpepC0xbbL8F4rqqrFfpeea0xe9Lq-Jc9
	//% vqaqpepm0xbba9pwe9Q8fs0-yqaqpepae9pg0FirpepeKkFr0xfr-x
	//% fr-xb9adbaqaaeGaciGaaiaabeqaamaabaabaaGceaqabeaacaWHbb
	//% Gaeyypa0ZaaSaaaeaacaWHLbGaaiOlaiaahYgadaahaaWcbeqaaiaa
	//% dsfaaaaakeaacaWHLbWaaWbaaSqabeaacaWGubaaaOGaaiOlaiaahY
	//% gaaaGaai4oaaqaaaqaaiaahIhacaGGNaGaeyypa0JaaCisamaaDaaa
	//% leaacaWGmbaabaGaeyOeI0IaaGymaaaakiaahIhacqGHsislcqGH+a
	//% GpdaqadaqaaiaahMeacqGHRaWkdaqadaqaaiabeg7aHjabgkHiTiaa
	//% igdaaiaawIcacaGLPaaacaWHbbaacaGLOaGaayzkaaGaaCiEaiaacE
	//% cacqGH9aqpcaWH4baabaGaaGOmamaabmaabaGaeqySdeMaeyOeI0Ia
	//% aGymaaGaayjkaiaawMcaaiaacYhacaWHbbGaaCiEaiaacEcacaGG8b
	//% WaaWbaaSqabeaacaaIYaaaaOGaey4kaSYaaeWaaeaacaWHbbGaaCiE
	//% aiaacEcaaiaawIcacaGLPaaadaahaaWcbeqaaiaadsfaaaGcdaqada
	//% qaaiaahIhacaGGNaGaeyOeI0IaaCiEaaGaayjkaiaawMcaaiabgUca
	//% RmaabmaabaGaaCiEaiaacEcacqGHsislcaWH4baacaGLOaGaayzkaa
	//% WaaWbaaSqabeaacaWGubaaaOWaaeWaaeaacaWHbbGaaCiEaiaacEca
	//% aiaawIcacaGLPaaacqGH9aqpcaaIWaaabaWaaeWaaeaacqaHXoqycq
	//% GHsislcaaIXaaacaGLOaGaayzkaaGaaiiFaiaahgeacaWH4bGaai4j
	//% aiaacYhadaahaaWcbeqaaiaaikdaaaGccqGHRaWkdaaadaqaaiaahg
	//% eacaWH4bGaai4jaiaacYcacaWH4bGaai4jaiabgkHiTiaahIhaaiaa
	//% wMYicaGLQmcacqGH9aqpcaaIWaaabaGaeqySdeMaeyypa0ZaaSaaae
	//% aacaGG8bGaaCyqaiaahIhacaGGNaGaaiiFamaaCaaaleqabaGaaGOm
	//% aaaakiabgkHiTmaaamaabaGaaCyqaiaahIhacaGGNaGaaiilaiaahI
	//% hacaGGNaGaeyOeI0IaaCiEaaGaayzkJiaawQYiaaqaaiaacYhacaWH
	//% bbGaaCiEaiaacEcacaGG8bWaaWbaaSqabeaacaaIYaaaaaaakiabg2
	//% da9iaaigdacqGHsisldaWcaaqaamaaamaabaGaaCyqaiaahIhacaGG
	//% NaGaaiilaiaahIhacaGGNaGaeyOeI0IaaCiEaaGaayzkJiaawQYiaa
	//% qaaiaacYhacaWHbbGaaCiEaiaacEcacaGG8bWaaWbaaSqabeaacaaI
	//% Yaaaaaaaaaaa!B58D!
	//\[
	//\begin{array}{l}
	// {\bf{A}} = \frac{{{\bf{e}}.{\bf{l}}^T }}{{{\bf{e}}^T .{\bf{l}}}}; \\ 
	//  \\ 
	// {\bf{x}}' = {\bf{H}}_L^{ - 1} {\bf{x}} -  > \left( {{\bf{I}} + \left( {\alpha  - 1} \right){\bf{A}}} \right){\bf{x}}' = {\bf{x}} \\ 
	// 2\left( {\alpha  - 1} \right)|{\bf{Ax}}'|^2  + \left( {{\bf{Ax}}'} \right)^T \left( {{\bf{x}}' - {\bf{x}}} \right) + \left( {{\bf{x}}' - {\bf{x}}} \right)^T \left( {{\bf{Ax}}'} \right) = 0 \\ 
	// \left( {\alpha  - 1} \right)|{\bf{Ax}}'|^2  + \left\langle {{\bf{Ax}}',{\bf{x}}' - {\bf{x}}} \right\rangle  = 0 \\ 
	// \alpha  = \frac{{|{\bf{Ax}}'|^2  - \left\langle {{\bf{Ax}}',{\bf{x}}' - {\bf{x}}} \right\rangle }}{{|{\bf{Ax}}'|^2 }} = 1 - \frac{{\left\langle {{\bf{Ax}}',{\bf{x}}' - {\bf{x}}} \right\rangle }}{{|{\bf{Ax}}'|^2 }} \\ 
	// \end{array}
	//\]
	vnlMatrixDouble epiLo(3,1,0);
	vnlMatrixDouble epiRo(3,1,0);
	
	epiLo[0][0]=5000.00;
	epiLo[1][0]=0;
	epiLo[2][0]=1.00;
	epiRo = epiLo;
	epiRo[0][0]*=-1.00;

	//rotate epipoles
	vnlMatrixDouble epiL = H*epiLo;
	vnlMatrixDouble epiR = H*epiRo;


	//compute lines as 3x matrices
	vnlMatrixDouble left_line = ComputeLine( _alignedShape, _imm_transform_dependencies.left_line );
	vnlMatrixDouble right_line = ComputeLine( _alignedShape, _imm_transform_dependencies.right_line );
//	vnlMatrixDouble nose_line = ComputeLine( _alignedShape, _imm_transform_dependencies.nose_line );

//	if( nose_line[0][0] == 0 ) nose_line[0][0] = 0.0000001; //to make it not lie on the same line with epipole

	vnlMatrixDouble A_left = epiL * left_line.transpose() / (epiL.transpose() * left_line)[0][0];
	vnlMatrixDouble A_right = epiR * right_line.transpose() / (epiR.transpose() * right_line)[0][0];
//	vnlMatrixDouble A_nose = epipole * nose_line.transpose() / (epipole.transpose() * nose_line)[0][0];


//	double cr_nose = ComputeCR( refShape, _alignedShape, A_nose, _imm_transform_dependencies.nose_tip );
	double cr_left = ComputeCR( refShape, _alignedShape, A_left, _imm_transform_dependencies.left_plane );
	double cr_right = ComputeCR( refShape, _alignedShape, A_right, _imm_transform_dependencies.right_plane );


	//compute homologies
	vnlMatrixDouble I(3,3,0); //identity matrix
	for( int i=0; i<3; i++ ) I[i][i] = 1;

	vnlMatrixDouble H_left = vnl_matrix_inverse<double>( I + (cr_left - 1)*A_left );
	vnlMatrixDouble H_right = vnl_matrix_inverse<double>( I + (cr_right - 1)*A_right );
//	vnlMatrixDouble H_nose = vnl_matrix_inverse<double>( I + (cr_nose - 1)*A_nose );

	//update the points
	TransformSelectedPoints( _alignedShape, H_left, _imm_transform_dependencies.left_plane );
	TransformSelectedPoints( _alignedShape, H_right, _imm_transform_dependencies.right_plane );
//	TransformSelectedPoints( _alignedShape, H_nose, _imm_transform_dependencies.nose_tip );

//	ToMatlab( _alignedShape.begin(), _alignedShape.size(), "c:\\1\\ashape1.m", "a1" );
}






vnlVectorDouble blAllign2FacesWithHomology::ExtractSubshape( const vnlVectorDouble& shape, const IndexArray& indices ) const
{
	const int NumberOfDimensions = 2;

	vnlVectorDouble result( indices.size() * NumberOfDimensions );


	int j = 0;
	for( size_t i=0; i<indices.size(); i++)
	{
		const int ind = indices[i] * NumberOfDimensions;

		for(int k=0; k<NumberOfDimensions; k++ )
			result[j++] = shape[ ind + k ];
	}

	return result;
}



vnlMatrixDouble blAllign2FacesWithHomology::ComputeLine( const vnlVectorDouble& shape, const IndexArray& line_indices ) const
{ //=cross product of two points
	const int NumberOfDimensions = 2;

	vnlVectorDouble point1(3,1);
	int ind = line_indices[0];
	point1[0] = shape[NumberOfDimensions * ind];
	point1[1] = shape[NumberOfDimensions * ind + 1];

	vnlVectorDouble point2(3,1);
	ind = line_indices[1];
	point2[0] = shape[NumberOfDimensions * ind];
	point2[1] = shape[NumberOfDimensions * ind + 1];

//TODO::COMMENTED OUT BY VDIALINOS
// 	vnlVectorDouble line_as_vect = cross_3d(point1, point2);
///////////////
//TODO::ADDED BY VDIALINOS
	vnlVectorDouble line_as_vect = vnl_cross_3d(point1, point2);
/////////////////////

	vnlMatrixDouble result(3,1);
	result.copy_in( line_as_vect.begin() );

	return result;
}



double blAllign2FacesWithHomology::Ax( const double* pt_tr, const vnlMatrixDouble& A ) const
{
	vnlVectorDouble v(3,1);
	v[0] = pt_tr[0];
	v[1] = pt_tr[1];

	return ( A * v ).squared_magnitude();
}


double blAllign2FacesWithHomology::MinimizationSumElement( const double* pt_ref, const double* pt_tr, const vnlMatrixDouble& A ) const
{
	vnlVectorDouble p_ref(3,1);
	p_ref[0] = pt_ref[0];
	p_ref[1] = pt_ref[1];

	vnlVectorDouble p_tr(3,1);
	p_tr[0] = pt_tr[0];
	p_tr[1] = pt_tr[1];

	return dot_product( p_ref - p_tr, A*p_ref ) ;
}


double blAllign2FacesWithHomology::ComputeCR(  const vnlVectorDouble& shape_ref, const vnlVectorDouble& shape_tr, const vnlMatrixDouble& A, const IndexArray& indices ) const
{
	const int NumberOfDimensions = 2;

	double cr = 0;
	double norm = 0;
	for( size_t i=0; i<indices.size(); i++ )
	{
		const int ind = indices[i] * NumberOfDimensions ;
		cr += MinimizationSumElement( &(shape_ref[ind]), &(_alignedShape[ind]), A );
		norm += Ax( &(shape_ref[ind]), A );
	}

	return 1 - cr / norm;
}


void blAllign2FacesWithHomology::TransformSelectedPoints( vnlVectorDouble& shape, const vnlMatrixDouble& T, const IndexArray& indices ) const
{
	const int NumberOfDimensions = 2;

	for( size_t i=0; i<indices.size(); i++ )
	{
		const int ind = indices[i] * NumberOfDimensions;

		vnlVectorDouble v(3,1);
		v[0] = shape[ind];
		v[1] = shape[ind+1];

		vnlVectorDouble v_t = T*v;
		const double k = v_t[2];

		shape[ind] = v_t[0]/k;
		shape[ind+1] = v_t[1]/k;
	}
}


vnlVectorDouble blAllign2FacesWithHomology::TransformFace( const vnlVectorDouble& face, const vnlVectorDouble& transform, int NumberOfLandmarks ) const
{
	vnlVectorDouble transformed_face = face;

	vnlMatrixDouble H = blAlignment::SynthesizeNonSkewHomography( transform );
	blAlignment::TransformPointsHomog( transformed_face, H );

//	ToMatlab( transformed_face.begin(), transformed_face.size(), "c:\\1\\tf.m", "v" );

	vnlMatrixDouble epiLo(3,1,0);
	vnlMatrixDouble epiRo(3,1,0);
	
	epiLo[0][0]=5000.00;
	epiLo[1][0]=0;
	epiLo[2][0]=1.00;
	epiRo = epiLo;
	epiRo[0][0]*=-1.00;

	//rotate epipoles
	vnlMatrixDouble epiL = H*epiLo;
	vnlMatrixDouble epiR = H*epiRo;


	//compute lines as 3x1 matrices
	vnlMatrixDouble left_line = ComputeLine( transformed_face, _imm_transform_dependencies.left_line );
	vnlMatrixDouble right_line = ComputeLine( transformed_face, _imm_transform_dependencies.right_line );
//	vnlMatrixDouble nose_line = ComputeLine( transformed_face, _imm_transform_dependencies.nose_line );

//	if( nose_line[0][0] == 0 ) nose_line[0][0] = 0.0000001; //to make it not lie on the same line with epipole

	vnlMatrixDouble A_left = epiL * left_line.transpose() / (epiL.transpose() * left_line)[0][0];
	vnlMatrixDouble A_right = epiR * right_line.transpose() / (epiR.transpose() * right_line)[0][0];
//	vnlMatrixDouble A_nose = epipole * nose_line.transpose() / (epipole.transpose() * nose_line)[0][0];


//	double cr_nose = transform[9];
	double cr_left = transform[7];
	double cr_right = transform[8];


	//compute homologies
	vnlMatrixDouble I(3,3,0); //identity matrix
	for( int i=0; i<3; i++ ) I[i][i] = 1;

	vnlMatrixDouble H_left = vnl_matrix_inverse<double>( I + (cr_left - 1)*A_left );
	vnlMatrixDouble H_right = vnl_matrix_inverse<double>( I + (cr_right - 1)*A_right );
//	vnlMatrixDouble H_nose = vnl_matrix_inverse<double>( I + (cr_nose - 1)*A_nose );

	//update the points
	TransformSelectedPoints( transformed_face, H_left, _imm_transform_dependencies.left_plane );
	TransformSelectedPoints( transformed_face, H_right, _imm_transform_dependencies.right_plane );
//	TransformSelectedPoints( transformed_face, H_nose, _imm_transform_dependencies.nose_tip );

	return transformed_face;
}

