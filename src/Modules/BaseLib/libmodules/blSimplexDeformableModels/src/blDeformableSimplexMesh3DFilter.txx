/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef BLDEFORMABLESIMPLEXMESH3DFILTER_TXX
#define BLDEFORMABLESIMPLEXMESH3DFILTER_TXX

#include "blDeformableSimplexMesh3DFilter.h"

#include "itkNumericTraits.h"

#include <set>
#include <vtkDecimatePro.h>

#include <vxl_version.h>
#if VXL_VERSION_DATE_FULL > 20040406
# include <vnl/vnl_cross.h>
# define itk_cross_3d vnl_cross_3d
#else
# define itk_cross_3d cross_3d
#endif

/* Constructor. */
template <typename TInputMesh, typename TOutputMesh>
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh> 
::blDeformableSimplexMesh3DFilter()
{
	this->m_Step = 0;
	this->m_Iterations = 20;
	this->m_Alpha = 0.2;
	this->m_Beta  = 0.01;
	this->m_Gamma = 0.05;
	this->m_Damping = 0.65;
	this->m_Rigidity = 0;

	this->m_Data = NULL;

	this->locator = vtkCellLocator::New();
	this->centerlineLocator = vtkPointLocator::New();
	this->locator->SetNumberOfCellsPerBucket(4);
}

template <typename TInputMesh, typename TOutputMesh>
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::~blDeformableSimplexMesh3DFilter()
{
	
}


template <typename TInputMesh, typename TOutputMesh>
void 
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::SetCenterline(vtkPolyData *vtkPDCenterline)
{
	this->centerline = vtkPDCenterline;
	this->centerlineLocator->SetDataSet(this->centerline);
	this->centerlineLocator->BuildLocator();
}

template <typename TInputMesh, typename TOutputMesh>
void 
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::SetCheckPoints(bool checkPoints)
{
	this->checkPoints = checkPoints;
}

template <typename TInputMesh, typename TOutputMesh>
void 
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::SetSurface(vtkPolyData *surf)
{
	vtkDecimatePro *decimateFilter = vtkDecimatePro::New();
	decimateFilter->SetInput( surf );
	decimateFilter->SetTargetReduction( 0.85 );
	decimateFilter->PreserveTopologyOn();
	decimateFilter->BoundaryVertexDeletionOff();
	decimateFilter->SplittingOff ();
	decimateFilter->Update();

	this->surf  = vtkPolyData::New();
	this->surf ->DeepCopy( decimateFilter->GetOutput() );

	decimateFilter->Delete();

	//this->surf = surf;
	this->locator->SetDataSet(this->surf);
	this->locator->BuildLocator();
}

/* PrintSelf. */
template <typename TInputMesh, typename TOutputMesh>
void
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::PrintSelf(std::ostream& os, Indent indent) const
{
	Superclass::PrintSelf(os,indent);
	os << indent << "alpha = " << this->m_Alpha << std::endl;
	os << indent << "beta = " << this->m_Beta << std::endl;
	os << indent << "gamma = " << this->m_Gamma << std::endl;
	os << indent << "rigidity = " << this->m_Rigidity << std::endl;
	os << indent << "No. of Iterations = " << this->m_Iterations << std::endl;

}/* End PrintSelf. */


/* Generate Data */
template <typename TInputMesh, typename TOutputMesh>
void
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::GenerateData()
{
	this->Initialize();

	this->m_Step = 0;

	while ( this->m_Step < this->m_Iterations )
	{
		const float progress = static_cast<float>( this->m_Step ) / 
			static_cast<float>( this->m_Iterations );

		this->UpdateProgress( progress );

		this->ComputeGeometry();

		if ( this->m_Step % 10 == 0 && this->m_Step > 0 )
		{
			this->UpdateReferenceMetrics();
		}

		this->ComputeDisplacement();
		m_Step++;
	}
	InputPointsContainerConstPointer      myPoints = this->GetInput(0)->GetPoints();
	InputPointsContainerConstIterator     points = myPoints->Begin();
	while( points != myPoints->End() )
	{
		blSimplexMeshGeometry* data;
		unsigned long idx = points.Index();
		data = this->m_Data->GetElement(idx);
		points++;
	}
	this->ComputeOutput();
}


/* Set default value of parameters and initialize local data container 
*  such as forces, displacements and displacement derivatives. */
template <typename TInputMesh, typename TOutputMesh>
void 
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::Initialize()
{
	InputPointsContainerConstPointer      myPoints = this->GetInput(0)->GetPoints();
	InputPointsContainerConstIterator     points = myPoints->Begin();

	if (this->m_Data.IsNull() )
	{
		this->m_Data = this->GetInput(0)->GetGeometryData();
	}

	while( points != myPoints->End() ) 
	{
		blSimplexMeshGeometry* data;
		unsigned long idx = points.Index();

		data = this->m_Data->GetElement(idx);
		data->pos = points.Value();

		data->neighbors[0] = myPoints->GetElement(data->neighborIndices[0]);
		data->neighbors[1] = myPoints->GetElement(data->neighborIndices[1]);
		data->neighbors[2] = myPoints->GetElement(data->neighborIndices[2]);

		// Store neighbor set with a specific radius
		InputNeighbors* neighborsList = this->GetInput(0)->GetNeighbors( points.Index() , m_Rigidity);
		InputNeighborsIterator neighborIt = neighborsList->begin();

		while ( neighborIt != neighborsList->end() )
		{
			data->neighborSet.insert( *neighborIt++ );
		}
		points++;
		delete neighborsList;
	}
}

/* Compute normals. */
template <typename TInputMesh, typename TOutputMesh>
void
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::ComputeGeometry() 
{
	PointType Foot;
	CovariantVectorType normal;
	CovariantVectorType z;
	double x[3], closest[3], distCent;
	vtkIdType pointId;
	VectorType tmp;

	FixedPointsMapType::iterator fixedIt;

	typename GeometryMapType::Iterator  dataIt = this->m_Data->Begin();
	blSimplexMeshGeometry* data, *neighborData;

	this->m_AverageSurfaceCurvature = 0.0;
	this->m_AverageDistanceToCenterline = 0.0;
	unsigned long points = 0;
	unsigned long fixPoints = 0;

	while ( dataIt != this->m_Data->End() )
	{
		data = dataIt.Value();
		neighborData = this->m_Data->GetElement(data->neighborIndices[0]);
		data->neighbors[0] = neighborData->pos;
		neighborData = this->m_Data->GetElement(data->neighborIndices[1]);
		data->neighbors[1] = neighborData->pos;
		neighborData = this->m_Data->GetElement(data->neighborIndices[2]);
		data->neighbors[2] = neighborData->pos;

		// Compute normal
		normal.Fill(0.0);
		z.Set_vnl_vector( itk_cross_3d( (data->neighbors[1] - data->neighbors[0]).Get_vnl_vector() ,
			(data->neighbors[2] - data->neighbors[0]).Get_vnl_vector()) );
		z.Normalize();
		normal += z;

		// Save the normal to the point data
		data->normal = normal;

		// Compute phi
		data->ComputeGeometry();

		tmp = data->neighbors[0] - data->pos;
		double D = 1.0/(2*data->sphereRadius); 
		double tmpNormalProd = dot_product(tmp.Get_vnl_vector(),data->normal.Get_vnl_vector());
		double sinphi =  2 * data->circleRadius * D * vnl_math_sgn( tmpNormalProd );
		double phi = vcl_asin(sinphi);
		data->phi = phi;
		data->meanCurvature = vcl_abs(sinphi/data->circleRadius);
		//std::cout << "curvature : " << data->meanCurvature << std::endl;

		// Accumulate the surface curvature to compute the mean.
		this->m_AverageSurfaceCurvature += data->meanCurvature;
		tmp = data->pos - data->neighbors[0];

		// Compute the foot of p projection of p onto the triangle spanned by its neighbors
		double distance = -tmpNormalProd;
		tmp.Set_vnl_vector((data->pos).Get_vnl_vector() - distance * normal.Get_vnl_vector() );
		Foot.Fill(0.0);
		Foot += tmp;

		data->distance = ((data->circleCenter)-Foot).GetNorm();

		if (this->m_Step < 1) {
			this->m_FixedPoints.insert(FixedPointsPairType(dataIt.Index(),false));
		}
		fixedIt = this->m_FixedPoints.find(dataIt.Index());
		x[0] = data->pos[0];
		x[1] = data->pos[1];
		x[2] = data->pos[2];
		pointId = this->centerlineLocator->FindClosestPoint(x);
		centerline->GetPoints()->GetPoint(pointId, closest);
		distCent =	((x[0]-closest[0])*(x[0]-closest[0]) +
			(x[1]-closest[1])*(x[1]-closest[1]) +
			(x[2]-closest[2])*(x[2]-closest[2]));
		distCent = sqrt(distCent);
		m_AverageDistanceToCenterline += distCent;
		if (fixedIt->second == true){
			this->m_AverageFixedDistanceToCenterline +=distCent;
			fixPoints++;
		}

		data->eps = ComputeBarycentricCoordinates( Foot, data);
		dataIt.Value() = data;
		dataIt++;
		points++;
	}
	this->m_AverageSurfaceCurvature/=points;
	this->m_AverageDistanceToCenterline/=points;
	// If 1/3 the points are fixed
	if (fixPoints>0){
		this->m_AverageFixedDistanceToCenterline/=fixPoints;
	}
	//std::cout << "Average curvature : " << m_AverageSurfaceCurvature << std::endl;
	//std::cout << "Average distance to centerline : " << m_AverageDistanceToCenterline << ", fixPoints : " << fixPoints << std::endl;

}

template <typename TInputMesh, typename TOutputMesh>
void
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::ComputeDisplacement()
{
	InputMeshPointer inputMesh = const_cast<TInputMesh*>(this->GetInput(0));
	typename GeometryMapType::Iterator dataIt = m_Data->Begin();
	blSimplexMeshGeometry* data;
	VectorType displacement;
	unsigned long pointId;

	while( dataIt != m_Data->End() ) 
	{
		data = dataIt.Value();
		pointId = dataIt.Index();

		this->ComputeSmoothingForce( pointId, data );

		this->ComputeExternalForce( data );      

		displacement.Set_vnl_vector( m_Alpha * (data->smoothingForce).Get_vnl_vector() +
			(data->externalForce).Get_vnl_vector() );

		//std::cout << "displacement : " << displacement << std::endl;

		data->pos += displacement;
		inputMesh->GetPoints()->InsertElement( dataIt.Index(), data->pos );

		dataIt++;
	}
}


template <typename TInputMesh, typename TOutputMesh>
void
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::ComputeNewtonianLawOfMotion(unsigned long pointId, blSimplexMeshGeometry* data)
{
	VectorType diffScheme;

	diffScheme.Fill(0.0);
	if( this->m_Step >0 ){
		diffScheme = (data->pos - data->oldPos);
		diffScheme = diffScheme * (1.0-this->m_Damping);
	}
	data->newtonianLawOfMotion = diffScheme;
}

/* */
template <typename TInputMesh, typename TOutputMesh>
void
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::ComputeSmoothingForce(unsigned long pointId, blSimplexMeshGeometry* data)
{
	/////////////////////////////////////////////////////////////////////////////
	VectorType tangentForce, normalForce, diffScheme, curv_for, neigh_for;
	double eps1Diff, eps2Diff, eps3Diff, refCurv;
	PointType eps, epsRef;
	blSimplexMeshGeometry::NeighborSetType neighborSet;
	PointType f_int;
	double r, d, phi, phiRef;

	//Tangential normal component

	//baricentric coordinates force
	eps = data->eps;
	epsRef = data->referenceMetrics;
	refCurv = this->GetInput(0)->GetReferenceCurvatureRadius();

	eps1Diff = epsRef[0]-eps[0];
	eps2Diff = epsRef[1]-eps[1];
	eps3Diff = epsRef[2]-eps[2];

	tangentForce.Set_vnl_vector( eps1Diff * (data->neighbors[0]).Get_vnl_vector() +
		eps2Diff * (data->neighbors[1]).Get_vnl_vector() +
		eps3Diff * (data->neighbors[2]).Get_vnl_vector()
		);

	//Normal component calculation
	normalForce.Fill(0.0);
	FixedPointsMapType::iterator fixedIt;
	fixedIt = this->m_FixedPoints.find(pointId);
	double L = (double) NumericTraits<unsigned long>::max();
	double L_Ref = (double) NumericTraits<unsigned long>::max();

	if (!(fixedIt->second)) {
		// use the curvature radius data.
		r = data->circleRadius;
		d = data->distance;
		phi = data->phi;

		neighborSet = data->neighborSet;
		// the reference phi angle is the average of the neighbors.
		NeighborSetIterator neighborIt = neighborSet.begin();
		phiRef = 0.0;

		while ( neighborIt != neighborSet.end() )
		{
			phiRef += this->m_Data->GetElement(*neighborIt++)->phi;
		}
		phiRef /= (double) neighborSet.size();

		L = L_Func(r,d,phi);
		L_Ref = L_Func(r,d,phiRef);

		normalForce.Set_vnl_vector(( L - L_Ref) * (data->normal).Get_vnl_vector());
	}

	data->smoothingForce.Fill(0.0);

	double percT = 1.0;

	data->smoothingForce += percT * (tangentForce);
}

/** Compute model Displacement according to image gradient forces */
template <typename TInputMesh, typename TOutputMesh>
void
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::ComputeExternalForce( blSimplexMeshGeometry* data)
{
	//PointType vec_for, tmp_vec_1, tmp_vec_2, tmp_vec_3;
	//GradientIndexType coord, coord2, tmp_co_1, tmp_co_2, tmp_co_3;

	//coord[0] = static_cast<GradientIndexValueType>(data->pos[0]);
	//coord[1] = static_cast<GradientIndexValueType>(data->pos[1]);
	//coord[2] = static_cast<GradientIndexValueType>(data->pos[2]);

	//coord2[0] = static_cast<GradientIndexValueType>( vcl_ceil(data->pos[0]) );
	//coord2[1] = static_cast<GradientIndexValueType>( vcl_ceil(data->pos[1]) );
	//coord2[2] = static_cast<GradientIndexValueType>( vcl_ceil(data->pos[2]) );

	//tmp_co_1[0] = coord2[0];
	//tmp_co_1[1] = coord[1];
	//tmp_co_1[2] = coord[2];

	//tmp_co_2[0] = coord[0];
	//tmp_co_2[1] = coord2[1];
	//tmp_co_2[2] = coord[2];

	//tmp_co_3[0] = coord[0];
	//tmp_co_3[1] = coord[1];
	//tmp_co_3[2] = coord2[2];

	//if ( (coord[0] >= 0) && (coord[1] >= 0) && (coord[2] >= 0) && 
	//     (coord2[0] < m_ImageWidth) && (coord2[1] < m_ImageHeight) && (coord2[2] < m_ImageDepth) )
	//  {      
	//  vec_for[0] = m_Gradient->GetPixel(coord)[0];
	//  vec_for[1] = m_Gradient->GetPixel(coord)[1];
	//  vec_for[2] = m_Gradient->GetPixel(coord)[2];

	//  tmp_vec_1[0] = m_Gradient->GetPixel(tmp_co_1)[0] - m_Gradient->GetPixel(coord)[0];
	//  tmp_vec_1[1] = m_Gradient->GetPixel(tmp_co_1)[1] - m_Gradient->GetPixel(coord)[1];
	//  tmp_vec_1[2] = m_Gradient->GetPixel(tmp_co_1)[2] - m_Gradient->GetPixel(coord)[2];
	//  tmp_vec_2[0] = m_Gradient->GetPixel(tmp_co_2)[0] - m_Gradient->GetPixel(coord)[0];
	//  tmp_vec_2[1] = m_Gradient->GetPixel(tmp_co_2)[1] - m_Gradient->GetPixel(coord)[1];
	//  tmp_vec_2[2] = m_Gradient->GetPixel(tmp_co_2)[2] - m_Gradient->GetPixel(coord)[2];
	//  tmp_vec_3[0] = m_Gradient->GetPixel(tmp_co_3)[0] - m_Gradient->GetPixel(coord)[0];
	//  tmp_vec_3[1] = m_Gradient->GetPixel(tmp_co_3)[1] - m_Gradient->GetPixel(coord)[1];
	//  tmp_vec_3[2] = m_Gradient->GetPixel(tmp_co_3)[2] - m_Gradient->GetPixel(coord)[2];

	//  vec_for[0] = vec_for[0] + ((data->pos)[0]-coord[0])*tmp_vec_1[0] 
	//    + ((data->pos)[1]-coord[1])*tmp_vec_2[0] + ((data->pos)[2]-coord[2])*tmp_vec_3[0];
	//  vec_for[1] = vec_for[1] + ((data->pos)[1]-coord[1])*tmp_vec_2[1]
	//    + ((data->pos)[0]-coord[0])*tmp_vec_1[1] + ((data->pos)[2]-coord[2])*tmp_vec_3[1];
	//  vec_for[2] = vec_for[2] + ((data->pos)[2]-coord[2])*tmp_vec_3[2]
	//    + ((data->pos)[1]-coord[1])*tmp_vec_2[2] + ((data->pos)[0]-coord[0])*tmp_vec_1[2];
	//  }
	//else
	//  {
	//  vec_for.Fill(0);
	//  }

	//double mag = dot_product(data->normal.Get_vnl_vector(),vec_for.Get_vnl_vector());
	//
	//vec_for[0] = mag*(data->normal)[0]/*num_for*/;
	//vec_for[1] = mag*(data->normal)[1]/*num_for*/; 
	//vec_for[2] = mag*(data->normal)[2]/*num_for*/; 

	//mag = vec_for.GetVectorFromOrigin().GetNorm();

	//if (mag > 0.5) 
	//  {
	//  for (int i=0; i<3; i++) 
	//    vec_for[i] = (0.5 * vec_for[i])/mag;
	//  }

	//data->externalForce[0] = m_Beta * vec_for[0];
	//data->externalForce[1] = m_Beta * vec_for[1];
	//data->externalForce[2] = m_Beta * vec_for[2];
}



/* Copy the content of m_Location into the Output. */
template <typename TInputMesh, typename TOutputMesh>
void
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::ComputeOutput() 
{
	OutputMeshType * output = this->GetOutput();
	output->SetPoints( const_cast<TInputMesh*>(this->GetInput(0))->GetPoints() );
	output->SetPointData( const_cast<TInputMesh*>(this->GetInput(0))->GetPointData() );
	output->SetCells( const_cast<TInputMesh*>(this->GetInput(0))->GetCells() );
	output->SetGeometryData(this->m_Data);
	output->SetLastCellId( this->GetInput(0)->GetLastCellId() );
}


/*  */
template <typename TInputMesh, typename TOutputMesh>
void
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::UpdateReferenceMetrics()
{
	//InputMeshConstPointer inputMesh = this->GetInput(0);
	//InputPointsContainerConstPointer   points = inputMesh->GetPoints();
	OutputMeshPointer outputMesh = this->GetOutput(0);
	OutputPointsContainerPointer   points = outputMesh->GetPoints();

	double H;
	double H_N1;
	double H_N2;
	double H_N3;
	double H_Mean;

	GeometryMapIterator dataIt = m_Data->Begin();

	blSimplexMeshGeometry* data;

	while ( dataIt != m_Data->End() )
	{
		data = dataIt->Value();
		H_N1 =((blSimplexMeshGeometry*)(m_Data->GetElement(data->neighborIndices[0])))->meanCurvature;
		H_N2 =((blSimplexMeshGeometry*)(m_Data->GetElement(data->neighborIndices[1])))->meanCurvature;
		H_N3 =((blSimplexMeshGeometry*)(m_Data->GetElement(data->neighborIndices[2])))->meanCurvature;
		H = data->meanCurvature;

		H_Mean = (H_N1 + H_N2 + H_N3)/3.0;

		PointType deltaH;

		deltaH[0] = (H_N1 - H_Mean)/H_Mean;
		deltaH[1] = (H_N2 - H_Mean)/H_Mean;
		deltaH[2] = (H_N3 - H_Mean)/H_Mean;

		PointType eps, eps_opt;
		// Compute optimal reference metrics
		eps_opt[0] = (1.0/3.0) + m_Gamma * deltaH[0];
		eps_opt[1] = (1.0/3.0) + m_Gamma * deltaH[1];
		eps_opt[2] = (1.0/3.0) + m_Gamma * deltaH[2];

		eps = data->referenceMetrics;

		eps[0] = eps[0] + 0.5 * (eps_opt[0] - eps[0]) ;
		eps[1] = eps[1] + 0.5 * (eps_opt[1] - eps[1]) ;
		eps[2] = eps[2] + 0.5 * (eps_opt[2] - eps[2]) ;

		// Set current reference metrics
		data->referenceMetrics = eps;
		outputMesh->SetPointData( dataIt->Index() , H );
		dataIt.Value() = data;
		dataIt++;
	}
}


template <typename TInputMesh, typename TOutputMesh>
double blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::L_Func(double r,double d, double phi)
{
	double r2 = r*r;
	double d2 = d*d;
	double r2Minusd2 = r2-d2;
	double tanPhi = vcl_tan(phi);

	double eps = 1.0;
	if (phi*vnl_math_sgn(phi) > vnl_math::pi_over_2)
	{
		eps = -1.0;
	}
	double L;
	double tmpSqr = r2 + r2Minusd2 * tanPhi*tanPhi;
	if (tmpSqr > 0)
	{
		double denom = eps*sqrt(tmpSqr) + r;
		if ( denom != 0 ) 
		{
			L = (r2Minusd2 * tanPhi) / denom;
		}
		else
		{
			L = (double) NumericTraits<unsigned long>::max();
		}
	}
	else 
	{
		L = (double) NumericTraits<unsigned long>::max();      
	}
	return L;
}


template <typename TInputMesh, typename TOutputMesh>
typename blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>::PointType
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::ComputeBarycentricCoordinates( PointType p, blSimplexMeshGeometry* data)
{
	PointType a,b,c;
	a = data->neighbors[0];
	b = data->neighbors[1];
	c = data->neighbors[2];

	VectorType n,na,nb,nc;
	n.Set_vnl_vector( itk_cross_3d((b-a).Get_vnl_vector(), (c-a).Get_vnl_vector()) );
	na.Set_vnl_vector( itk_cross_3d((c-b).Get_vnl_vector(), (p-b).Get_vnl_vector()) );
	nb.Set_vnl_vector( itk_cross_3d((a-c).Get_vnl_vector(), (p-c).Get_vnl_vector()) );
	nc.Set_vnl_vector( itk_cross_3d((b-a).Get_vnl_vector(), (p-a).Get_vnl_vector()) );

	PointType eps;
	eps[0] = dot_product(n.Get_vnl_vector(),na.Get_vnl_vector()) / n.GetSquaredNorm();
	eps[1] = dot_product(n.Get_vnl_vector(),nb.Get_vnl_vector()) / n.GetSquaredNorm();
	eps[2] = dot_product(n.Get_vnl_vector(),nc.Get_vnl_vector()) / n.GetSquaredNorm();

	return eps;
}

template <typename TInputMesh, typename TOutputMesh>
void 
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::UpdateMeanMinimumDistance()
{
	double minDistAvg;
	blSimplexMeshGeometry *data;
	double x[3], closestPoint[3];
	double distVessel;
	vtkIdType cellId;
	int subId;

	CrossSectionMatrixType *m_CrossSections;
	OutputMeshPointer outputMesh = this->GetOutput(0);
	m_CrossSections = outputMesh->GetCrossSectionMatrix();

	unsigned int nCenters = 0;
	if (m_CrossSections !=NULL) {
		nCenters = m_CrossSections->cols();
		if (!this->m_CrossSectionMeanMinDist.empty()){
			this->m_CrossSectionMeanMinDist.clear();
		}

		for (unsigned long center = 0; center<nCenters; center++){

			minDistAvg = 0.0;

			for (unsigned long ptI = 0; ptI<m_CrossSections->rows();ptI++)
			{
				data = this->m_Data->GetElement(m_CrossSections->get(ptI, center));

				x[0] = data->pos[0];
				x[1] = data->pos[1];
				x[2] = data->pos[2];

				this->locator->FindClosestPoint (x, closestPoint, cellId, subId, distVessel);
				minDistAvg += sqrt(distVessel);			
			}
			minDistAvg /= m_CrossSections->rows();
			this->m_CrossSectionMeanMinDist.push_back(minDistAvg);
		}
	}
}

template <typename TInputMesh, typename TOutputMesh>
double
blDeformableSimplexMesh3DFilter<TInputMesh, TOutputMesh>
::GetCorrespondingCrossSectionMeanDist(unsigned long idx){

	OutputMeshPointer outputMesh = this->GetOutput(0); 

	unsigned long center;
	if (!outputMesh->m_PointToCenterMap)
	{
		outputMesh->ComputePointToCenterInfo();
	}
	center = outputMesh->m_PointToCenterMap->GetElement(idx);
	return this->m_CrossSectionMeanMinDist[center];
}

#endif // BLDEFORMABLESIMPLEXMESH3DFILTER_TXX

