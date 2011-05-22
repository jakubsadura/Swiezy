/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#include "blSimplexMeshGeometry.h"

#include "itkNumericTraits.h"

#include <vxl_version.h>
#if VXL_VERSION_DATE_FULL > 20040406
# include <vnl/vnl_cross.h>
# define cross_3d vnl_cross_3d
#endif

blSimplexMeshGeometry
::blSimplexMeshGeometry()
{
	double c = 1.0/3.0;
	PointType p;
	p.Fill(0.0);

	this->pos.Fill(0);
	this->oldPos.Fill(0);
	this->referenceMetrics.Fill(c);
	this->eps.Fill(c);
	this->normal.Fill(0); 
	this->externalForce.Fill(0);
	this->smoothingForce.Fill(0);
	this->circleRadius = 0;
	this->circleCenter.Fill(0);
	this->sphereRadius = 0;
	this->distance = 0;
	this->phi = 0;
	this->isFixed = false;

	this->neighborIndices.Fill((unsigned long) itk::NumericTraits<unsigned long>::max());
	this->neighbors.Fill(p);
	this->forceIndex = 0;
	this->multiplier = 0;
	this->neighborSet.empty();
	this->trajectory.empty();
	this->neighborCells.empty();
	this->meanCurvature = c;
}


blSimplexMeshGeometry
::~blSimplexMeshGeometry()
{
}

void 
blSimplexMeshGeometry
::ComputeGeometry()
{
  VectorType b,c,cXb, tmp;

  //Compute the circum circle (center and radius)
  b = this->neighbors[2] - this->neighbors[0];
  c = this->neighbors[1] - this->neighbors[0];
        
  cXb.SetVnlVector( cross_3d<double>(c.GetVnlVector(),b.GetVnlVector()) );
 
  tmp.SetVnlVector( b.GetSquaredNorm() * 
                        cross_3d<double>( cXb.GetVnlVector(), c.GetVnlVector() ) +
                      c.GetSquaredNorm() * 
                        cross_3d<double>( b.GetVnlVector() , cXb.GetVnlVector() ) );

  double cXbSquaredNorm = 2 * cXb.GetSquaredNorm();
  
  circleRadius = tmp.GetNorm()/(cXbSquaredNorm);
  tmp[0] /= (cXbSquaredNorm);
  tmp[1] /= (cXbSquaredNorm);
  tmp[2] /= (cXbSquaredNorm);
  circleCenter = this->neighbors[0] + tmp;

  // Compute the circum sphere (center and radius) of a point
  VectorType d,dXc,bXd,sphereTmp, denom;

  d = pos - this->neighbors[0];
  dXc.SetVnlVector( cross_3d<double>(d.GetVnlVector(),c.GetVnlVector()) );
  bXd.SetVnlVector( cross_3d<double>(b.GetVnlVector(),d.GetVnlVector()) );

  sphereTmp.SetVnlVector( d.GetSquaredNorm()* cXb.GetVnlVector() +
                            b.GetSquaredNorm()* dXc.GetVnlVector() +
                            c.GetSquaredNorm()* bXd.GetVnlVector()
                          );
  
  double val = 2 * (c[0]*(b[1]*d[2]-b[2]*d[1]) - 
                    c[1]*( b[0]*d[2]-b[2]*d[0] ) + 
                    c[2]*( b[0]*d[1]-b[1]*d[0] ));

  // Fix for points which lay on their neighbors plane
  // necessary ??
  if (val == 0) 
    {
    val = 1;
    }

  sphereRadius = sphereTmp.GetNorm()/val;

  if (sphereRadius < 0) {
    sphereRadius = -1 * sphereRadius;
  }
}
