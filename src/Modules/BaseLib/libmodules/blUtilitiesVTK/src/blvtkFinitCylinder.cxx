/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: blvtkFinitCylinder.cxx,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "blvtkFinitCylinder.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(blvtkFinitCylinder, "$Revision: 1.1 $");
vtkStandardNewMacro(blvtkFinitCylinder);

// Construct plane passing through origin and normal to z-axis.
blvtkFinitCylinder::blvtkFinitCylinder()
{
  this->Normal[0] = 0.0;
  this->Normal[1] = 0.0;
  this->Normal[2] = 1.0;

  this->Origin[0] = 0.0;
  this->Origin[1] = 0.0;
  this->Origin[2] = 0.0;

  this->radius = 1;
  this->width = 0.1;
}

void blvtkFinitCylinder::ProjectPoint(double x[3], double origin[3], 
                            double normal[3], double xproj[3])
{
  double t, xo[3];

  xo[0] = x[0] - origin[0];
  xo[1] = x[1] - origin[1];
  xo[2] = x[2] - origin[2];

  t = vtkMath::Dot(normal,xo);

  xproj[0] = x[0] - t * normal[0];
  xproj[1] = x[1] - t * normal[1];
  xproj[2] = x[2] - t * normal[2];
}

void blvtkFinitCylinder::Push(double distance)
{
  int i;

  if ( distance == 0.0 )
    {
    return;
    }
  for (i=0; i < 3; i++ )
    {
    this->Origin[i] += distance * this->Normal[i];
    }
  this->Modified();
}

// Project a point x onto plane defined by origin and normal. The 
// projected point is returned in xproj. NOTE : normal NOT required to
// have magnitude 1.
void blvtkFinitCylinder::GeneralizedProjectPoint(double x[3], double origin[3],
                                       double normal[3], double xproj[3])
{
  double t, xo[3], n2;

  xo[0] = x[0] - origin[0];
  xo[1] = x[1] - origin[1];
  xo[2] = x[2] - origin[2];

  t = vtkMath::Dot(normal,xo);
  n2 = vtkMath::Dot(normal, normal);

  if (n2 != 0)
    {
    xproj[0] = x[0] - t * normal[0]/n2;
    xproj[1] = x[1] - t * normal[1]/n2;
    xproj[2] = x[2] - t * normal[2]/n2;
    }
  else
    {
    xproj[0] = x[0];
    xproj[1] = x[1];
    xproj[2] = x[2];
    }
}


// Evaluate plane equation for point x[3].
double blvtkFinitCylinder::EvaluateFunction(double x[3])
{
  double NormalNormSquare,
         NormalProjection,
         PerpendicularProjection,
         xo[3],
         DistanceToCylinder,
         DistanceToPlanes,
         Distance;

  xo[0] = x[0]-this->Origin[0];
  xo[1] = x[1]-this->Origin[1];
  xo[2] = x[2]-this->Origin[2];

  NormalNormSquare = vtkMath::Dot(this->Normal, this->Normal); 
  NormalProjection = vtkMath::Dot(this->Normal,xo)/sqrt(NormalNormSquare);
  PerpendicularProjection = sqrt(vtkMath::Dot(xo,xo)-pow(NormalProjection,2));

  DistanceToCylinder = PerpendicularProjection - this->radius;
  DistanceToPlanes = fabs(NormalProjection - this->width/2) - this->width/2;

  if(DistanceToCylinder>0 & DistanceToPlanes>0)
  {
	Distance = sqrt(pow(DistanceToCylinder,2)+pow(DistanceToPlanes,2));
  }
  else
  {
	Distance = ( DistanceToCylinder > DistanceToPlanes ? DistanceToCylinder : DistanceToPlanes );
  }

  return Distance;
}

// Evaluate function gradient at point x[3].
void blvtkFinitCylinder::EvaluateGradient(double x[3], double n[3])
{
  double NormalNormSquare,
         NormalProjection,
         PerpendicularProjection,
         xo[3],
         DistanceToCylinder,
         DistanceToPlanes,
         Distance,
         PerpendicularVector[3],
         nNorm;
  int s;

  xo[0] = x[0]-this->Origin[0];
  xo[1] = x[1]-this->Origin[1];
  xo[2] = x[2]-this->Origin[2];

  NormalNormSquare = vtkMath::Dot(this->Normal, this->Normal); 
  NormalProjection = vtkMath::Dot(this->Normal,xo)/sqrt(NormalNormSquare);
  PerpendicularProjection = sqrt(vtkMath::Dot(xo,xo)-pow(NormalProjection,2));

  for(int i=0; i<3; i++)
  {
	PerpendicularVector[i] = ( x[0] 
                               - NormalProjection*this->Normal[i]/sqrt(NormalNormSquare) )
                               /PerpendicularProjection;
  }


  DistanceToCylinder = PerpendicularProjection - this->radius;
  DistanceToPlanes = fabs(NormalProjection - this->width/2) - this->width/2;

  s = ( NormalProjection - this->width/2 >0 ? 1 : -1);

  if(DistanceToCylinder>0 & DistanceToPlanes>0)
  {
	for(int i=0; i<3; i++)
	{
	  n[i] = xo[i]
	       - (s+1)/2 * this->width * this->Normal[i] / sqrt(NormalNormSquare) 
               - this->radius * PerpendicularVector[i];
	}
	nNorm = sqrt( vtkMath::Dot(n,n) );
	for(int i=0; i<3; i++)
	{
	  n[i] = n[i]/nNorm;
	}
  }
  else if(DistanceToCylinder>DistanceToPlanes)
  {
	for(int i=0; i<3; i++)
	{
	  n[i] = PerpendicularVector[i];
	}
  }
  else
  {
	for (int i=0; i<3; i++)
	{
	  n[i] = s*this->Normal[i];
	}
  }

}

#define VTK_FINIT_CYLINDER_TOL 1.0e-06

// Given a line defined by the two points p1,p2; and a plane defined by the
// normal n and point p0, compute an intersection. The parametric
// coordinate along the line is returned in t, and the coordinates of 
// intersection are returned in x. A zero is returned if the plane and line
// do not intersect between (0<=t<=1). If the plane and line are parallel,
// zero is returned and t is set to VTK_LARGE_DOUBLE.
int blvtkFinitCylinder::IntersectWithLine(double p1[3], double p2[3], double n[3], 
                               double p0[3], double& t, double x[3])
{
  double num, den, p21[3];
  double fabsden, fabstolerance;

  // Compute line vector
  // 
  p21[0] = p2[0] - p1[0];
  p21[1] = p2[1] - p1[1];
  p21[2] = p2[2] - p1[2];

  // Compute denominator.  If ~0, line and plane are parallel.
  // 
  num = vtkMath::Dot(n,p0) - ( n[0]*p1[0] + n[1]*p1[1] + n[2]*p1[2] ) ;
  den = n[0]*p21[0] + n[1]*p21[1] + n[2]*p21[2];
  //
  // If denominator with respect to numerator is "zero", then the line and
  // plane are considered parallel. 
  //

  // trying to avoid an expensive call to fabs()
  if (den < 0.0)
    {
    fabsden = -den;
    }
  else
    {
    fabsden = den;
    }
  if (num < 0.0)
    {
    fabstolerance = -num*VTK_FINIT_CYLINDER_TOL;
    }
  else
    {
    fabstolerance = num*VTK_FINIT_CYLINDER_TOL;
    }
  if ( fabsden <= fabstolerance )
    {
    t = VTK_DOUBLE_MAX;
    return 0;
    }

  // valid intersection
  t = num / den;

  x[0] = p1[0] + t*p21[0];
  x[1] = p1[1] + t*p21[1];
  x[2] = p1[2] + t*p21[2];

  if ( t >= 0.0 && t <= 1.0 )
    {
    return 1;
    }
  else
    {
    return 0;
    }
}

void blvtkFinitCylinder::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Normal: (" << this->Normal[0] << ", " 
    << this->Normal[1] << ", " << this->Normal[2] << ")\n";

  os << indent << "Origin: (" << this->Origin[0] << ", " 
    << this->Origin[1] << ", " << this->Origin[2] << ")\n";
}
