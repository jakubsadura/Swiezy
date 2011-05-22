/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include <meNormalPlaneToVessel.h>
//#include <meCloseHoles.h>
//#include <meHoleFiller.h>

//vtk
#include <vtkClipPolyData.h>
#include <vtkPolyDataConnectivityFilter.h>
//#include <vtkCellLocator.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkCellArray.h>
#include <vtkCellLocator.h>
#include <vector>


// Default constructor
meNormalPlaneToVessel::meNormalPlaneToVessel()
{
	this->m_Mesh = vtkPolyData::New();
	this->m_ClosedMesh = vtkPolyData::New();
	this->m_Intersection = vtkPolyData::New();
	this->centerComputed = false;
}

// Destructor
meNormalPlaneToVessel::~meNormalPlaneToVessel()
{
	this->m_Mesh->Delete();
	this->m_ClosedMesh->Delete();
	this->m_Intersection->Delete();
}

//--------------------------------------------------
void meNormalPlaneToVessel::ImproveNormal()
//--------------------------------------------------
{
	double d = 0,
	       originalNormal[3],
	       normalFlipSign = 0;

	for(int i=0; i<3; i++) d+=pow(this->m_Normal[i],2);
	d = sqrt(d);
	for(int i=0; i<3; i++)
	{
		this->m_Normal[i] = this->m_Normal[i]/d;
		originalNormal[i] = this->m_Normal[i];
	}

	if(this->secondPointSet)
	{
		this->ImproveNormalFromTwoPoints();
	}
	else
	{
		this->ImproveNormalFromOnePoint();
	}

	d = 0;
	for(int i=0; i<3; i++)
	{
		d+=pow(this->m_Normal[i],2);
		normalFlipSign += this->m_Normal[i] * originalNormal[i];
	}
	d = sqrt(d);
	normalFlipSign = ( normalFlipSign>0 ? 1 : -1 );
	for(int i=0; i<3; i++) this->m_Normal[i] = normalFlipSign*this->m_Normal[i]/d;

	this->ComputeE1E2();
	this->GetConnectedCutPlane(this->m_Normal);
}

//--------------------------------------------------
void meNormalPlaneToVessel::ImproveNormalFromOnePoint()
//--------------------------------------------------
{
	double area1p,
	       area1m,
	       area2p,
	       area2m,
	       alpha1,
	       alpha2,
	       alpha,
	       angle = 0.005,
	       originalPoint[3];

/*	double vectors[9][3]={{1,0,0},{0,1,0},{0,0,1},{1.41421356237,1.41421356237,0},{1.41421356237,0,1.41421356237},{0,1.41421356237,1.41421356237},{1.41421356237,-1.41421356237,0},{1.41421356237,0,-1.41421356237},{0,1.41421356237,-1.41421356237}};

	double area = GetArea(this->m_Normal);
	int vectorId = -1;
	for(int k=0; k<9; k++)
	{
		area1p = GetArea(vectors[k]);
		if(area1p < area)
		{
			area = area1p;
			vectorId = k;
		}
	}

	if(vectorId > -1)
	{
		for(int i=0; i<3; i++) this->m_Normal[i] = vectors[vectorId][i];
	}
*/
	int iterations = 0;
for(int i=0; i<3; i++) originalPoint[i] = this->m_Point[i];
	do
	{
		this->ComputeE1E2();
//		area = this->GetArea(this->m_Normal);
this->GetConnectedCutPlane(this->m_Normal);
this->InitializeCenter();
this->ImproveCenter();
		area1p = this->GetNeighbourArea( this->e1, angle );
		area1m = this->GetNeighbourArea( this->e1, -angle );
		area2p = this->GetNeighbourArea( this->e2, angle );
		area2m = this->GetNeighbourArea( this->e2, -angle );
for(int i=0; i<3; i++) this->m_Point[i] = originalPoint[i];

		alpha1 = atan((area1p-area1m)/((area1p+area1m)*angle))/10;
		alpha2 = atan((area2p-area2m)/((area2p+area2m)*angle))/10;
		alpha = sqrt(pow(alpha1,2)+pow(alpha2,2));
		if(alpha>10*angle)
		{
			alpha1 = alpha1/alpha*10*angle;
			alpha2 = alpha2/alpha*10*angle;
			alpha = 10*angle;
		}

/*		alpha1 = (area1p-area1m)/(area1p+area1m-2*area)*angle;
		alpha2 = (area2p-area2m)/(area2p+area2m-2*area)*angle; */

/*		alpha1 = area1p-area1m;
		alpha2 = area2p-area2m;
		alpha = sqrt(pow(alpha1,2)+pow(alpha2,2));
*/

		for(int i=0; i<3; i++)
		{
			this->m_Normal[i] = cos(alpha)*this->m_Normal[i] - sin(alpha)/alpha*(alpha1*this->e1[i]+alpha2*this->e2[i]);
		}
		iterations++;
	}
	while(alpha>angle && iterations < 40);
cout << "Iterations for normal improving: " << iterations << endl;
}

//--------------------------------------------------
void meNormalPlaneToVessel::ImproveNormalFromTwoPoints()
//--------------------------------------------------
{
	double area1p,
	       area1m,
	       alpha1,
	       alpha,
	       angle = 0.005;
//	       originalPoint[3];

	int iterations = 0;
	do
	{
		this->e1[0] = this->segment[1]*this->m_Normal[2] - this->segment[2]*this->m_Normal[1];
		this->e1[1] = this->segment[2]*this->m_Normal[0] - this->segment[0]*this->m_Normal[2];
		this->e1[2] = this->segment[0]*this->m_Normal[1] - this->segment[1]*this->m_Normal[0];

		this->GetConnectedCutPlane(this->m_Normal);
//		for(int i=0; i<3; i++) originalPoint[i] = this->m_Point[i];
//		this->InitializeCenter();
//		this->ImproveCenter();
		area1p = this->GetNeighbourArea( this->e1, angle );
		area1m = this->GetNeighbourArea( this->e1, -angle );
//		for(int i=0; i<3; i++) this->m_Point[i] = originalPoint[i];

		alpha1 = atan((area1p-area1m)/((area1p+area1m)*angle))/10;
		alpha = fabs(alpha1);
		if(alpha>10*angle)
		{
			alpha1 = alpha1/alpha*10*angle;
		}

		for(int i=0; i<3; i++)
		{
			this->m_Normal[i] = cos(alpha1)*this->m_Normal[i] - sin(alpha1)*this->e1[i];
		}
		iterations++;
	}
	while(alpha>angle && iterations < 40);
cout << "Iterations for normal improving from 2 points: " << iterations << endl;
}

//--------------------------------------------------
double meNormalPlaneToVessel::GetNeighbourArea(double e[3], double angle)
//--------------------------------------------------
{
	double normal[3];
	for(int i=0; i<3; i++)
	{
		normal[i] = cos(angle)*this->m_Normal[i] + sin(angle)*e[i];
	}
	return this->GetArea(normal);
}

//--------------------------------------------------
double meNormalPlaneToVessel::GetArea(double normal[3])
//--------------------------------------------------
{
	this->GetConnectedCutPlane(normal);
	return this->ComputeArea();
}

double meNormalPlaneToVessel::GetArea()
{
	return this->GetArea(this->GetNormal());
}
//--------------------------------------------------
void meNormalPlaneToVessel::GetConnectedCutPlane(double normal[3])
//--------------------------------------------------
{
	vtkPlane * plane = vtkPlane::New();
	plane->SetOrigin(this->m_Point);
	plane->SetNormal(normal);
	vtkCutter * cutPlane = vtkCutter::New();
	cutPlane->CreateDefaultLocator();
	cutPlane->SetInput(this->m_ClosedMesh);
	cutPlane->SetCutFunction(plane);
	cutPlane->Update();

	vtkPolyDataConnectivityFilter *connec = vtkPolyDataConnectivityFilter::New();
	connec->SetInput(cutPlane->GetOutput());
	connec->SetExtractionModeToClosestPointRegion();
	connec->SetClosestPoint(this->m_Point);
	connec->Update();
	this->m_Intersection->DeepCopy(connec->GetOutput());

	plane->Delete();
	cutPlane->Delete();
	connec->Delete();
}

//--------------------------------------------------
double meNormalPlaneToVessel::ComputeArea()
//--------------------------------------------------
{
	if ( this->m_Intersection->GetNumberOfPoints() == 0 )
	{
		throw std::runtime_error( "Number of intersection points is 0" );
	}

	vtkIdList * NeighbourCells = vtkIdList::New();
	vtkIdList * FirstCell = vtkIdList::New();
	vtkIdList * SecondCell = vtkIdList::New();
	int numPoints,
		FirstPointId,
		SecondPointId,
		FirstCellId,
		SecondCellId;
	double origin[3],
		FirstPoint[3],
		SecondPoint[3],
		areaVector[3] = {0,0,0},
		area;
	for(int i=0; i<3; i++)
	{
		origin[i] = this->m_Intersection->GetPoint(0)[i];
	}
	this->m_Intersection->GetPointCells(0, NeighbourCells);
	if( NeighbourCells->GetNumberOfIds() != 2 ) 
	{
		cout << "ERROR: The countour is not closed or is not simple" << endl;
	}
	FirstCellId = NeighbourCells->GetId(0);
	this->m_Intersection->GetCellPoints(FirstCellId, FirstCell);
	if( FirstCell->GetId(0) == 0 )
	{
		FirstPointId = FirstCell->GetId(1);
	}
	else
	{
		FirstPointId = FirstCell->GetId(0);
	}
	numPoints = this->m_Intersection->GetNumberOfPoints();
	for(int p=2; p<numPoints; p++)  //
/*double length = 0,
       sublength;
for(int p=0; p<numPoints; p++)*/
	{
/*sublength = 0;*/
		this->m_Intersection->GetPointCells(FirstPointId, NeighbourCells);
		if( NeighbourCells->GetNumberOfIds() != 2 ) 
		{
			cout << "ERROR: The countour is not closed or is not simple" << endl;
		}
		SecondCellId = NeighbourCells->GetId(0);
		if( SecondCellId == FirstCellId )
		{
			SecondCellId = NeighbourCells->GetId(1);
		}
		this->m_Intersection->GetCellPoints(SecondCellId, SecondCell);
		if( SecondCell->GetId(0) == FirstPointId )
		{
			SecondPointId = SecondCell->GetId(1);
		}
		else
		{
			SecondPointId = SecondCell->GetId(0);
		}
		for(int i=0; i<3; i++)
		{
			FirstPoint[i] = this->m_Intersection->GetPoint(FirstPointId)[i]-origin[i];
			SecondPoint[i] = this->m_Intersection->GetPoint(SecondPointId)[i]-origin[i];
 		}
		areaVector[0] += FirstPoint[1]*SecondPoint[2] - FirstPoint[2]*SecondPoint[1];   //
		areaVector[1] += FirstPoint[2]*SecondPoint[0] - FirstPoint[0]*SecondPoint[2];   //
		areaVector[2] += FirstPoint[0]*SecondPoint[1] - FirstPoint[1]*SecondPoint[0];	//

/*for(int i=0; i<3; i++)
{
	sublength += pow(SecondPoint[i]-FirstPoint[i],2);
}
length += sqrt(sublength);*/
		FirstCellId = SecondCellId;
		FirstPointId = SecondPointId;		
	}
	area = sqrt(pow(areaVector[0],2)+pow(areaVector[1],2)+pow(areaVector[2],2));   //
	return area;   //
//return length;
}

//--------------------------------------------------
// This function compute 2 unit vectors, e1 and e2, that are perpendicular to m_Normal and 
// to each other.
void meNormalPlaneToVessel::ComputeE1E2()
//--------------------------------------------------
{
	this->e1[0]=1;
	this->e1[1]=0;
	this->e1[2]=0;
	this->e2[0]=0;
	this->e2[1]=1;
	this->e2[2]=0;

	if( this->m_Normal[2]==0 )
	{
		if( this->m_Normal[0]==0 )
		{
			this->e2[1]=0;
			this->e2[2]=1;
		}
		else
		{
			this->e1[0]=0;
			this->e1[2]=1;
		}
	}
	double a = 0,
		b = 0,
		c = 0;
	for(int i=0; i<3; i++)
	{
		a += this->e1[i]*this->m_Normal[i];
		b += this->e2[i]*this->m_Normal[i];
		c += pow(this->m_Normal[i],2);
	}
	a = a/c;
	b = b/c;
	for(int i=0; i<3; i++)
	{
		this->e1[i] -= a*this->m_Normal[i];
		this->e2[i] -= b*this->m_Normal[i];
	}
	a = 0;
	c = 0;
	for(int i=0; i<3; i++)
	{
		a += this->e2[i]*this->e1[i];
		c += pow(this->e1[i],2);
	}
	a = a/c;
	for(int i=0; i<3; i++)
	{
		this->e2[i] -= a*this->e1[i];
	}
	a = 0;
	for(int i=0; i<3; i++)
	{
		a += pow(this->e2[i],2);
	}
	a = sqrt(a);
	c = sqrt(c);
	for(int i=0; i<3; i++)
	{
		this->e1[i] = this->e1[i]/c;
		this->e2[i] = this->e2[i]/a;
	}
}

//--------------------------------------------------
void meNormalPlaneToVessel::ComputeMaxAndMinDiameters()
//--------------------------------------------------
{
	double 	d = 0,
		dAux,
		point1[3],
		point2[3],
		point3[3],
		p12Max,
		p13Max,
		p23Max,
		p13Min,
		p23Min;
	int numPoints = this->m_Intersection->GetNumberOfPoints(),
	    numCells = this->m_Intersection->GetNumberOfCells();

	for(int i=0; i<numPoints-1; i++)
	{
		this->m_Intersection->GetPoint(i,point1);
		for(int j=i+1; j<numPoints; j++)
		{
			this->m_Intersection->GetPoint(j,point2);
			dAux = 0;
			for(int k=0; k<3; k++)
			{
				dAux += pow(point1[k]-point2[k],2);
			}
			if(d<dAux)
			{
				d=dAux;
				for(int k=0; k<3; k++)
				{
					this->maxDPoint1[k]=point1[k];
					this->maxDPoint2[k]=point2[k];
					this->m_MaxDDirection[k]=(point1[k]-point2[k])/sqrt(d);
				}
			}
		}
	}
	this->maxD = sqrt(d);

	this->m_MinDDirection[0] = this->m_Normal[1]*this->m_MaxDDirection[2]
	                          - this->m_Normal[2]*this->m_MaxDDirection[1];
	this->m_MinDDirection[1] = this->m_Normal[2]*this->m_MaxDDirection[0]
	                          - this->m_Normal[0]*this->m_MaxDDirection[2];
	this->m_MinDDirection[2] = this->m_Normal[0]*this->m_MaxDDirection[1]
	                          - this->m_Normal[1]*this->m_MaxDDirection[0];

	d=0;
//	d=this->maxD;
/*	for(int i=0; i<numPoints-1; i++)
	{
		this->m_Intersection->GetPoint(i,point1);
		for(int j=i+1; j<numPoints; j++)
		{
			this->m_Intersection->GetPoint(j,point2);
			dAux = 0;
			pAux = 0;
			for(int k=0; k<3; k++)
			{
				dAux += pow(point1[k]-point2[k],2);
				pAux += (point1[k]-point2[k])*this->m_MaxDDirection[k];
			}
			dAux -= pow(pAux,2);
			if(d<dAux)
			{
				d=dAux;
			}
		}
	}*/
	
	for(int i=0; i<numPoints-1; i++)
	{
		this->m_Intersection->GetPoint(i,point1);
		for(int j=0; j<numCells; j++)
		{
			this->m_Intersection->GetCell(j)->GetPoints()->GetPoint(0,point2);
			this->m_Intersection->GetCell(j)->GetPoints()->GetPoint(1,point3);
			p12Max = 0;
			p13Max = 0;
			for(int k=0; k<3; k++)
			{
				p12Max += (point2[k]-point1[k])*this->m_MaxDDirection[k];
				p13Max += (point3[k]-point1[k])*this->m_MaxDDirection[k];
			}
			if( p12Max*p13Max<=0 )
			{
				p23Max = 0;
				p13Min = 0;
				p23Min = 0;
				dAux = 0;
				for(int k=0; k<3; k++)
				{
					dAux += pow(point2[k]-point1[k],2);
					p23Max += (point3[k]-point2[k])*this->m_MaxDDirection[k];
					p13Min += (point3[k]-point1[k])*this->m_MinDDirection[k];
					p23Min += (point3[k]-point2[k])*this->m_MinDDirection[k];
				}
				dAux = fabs( p13Max*p23Min/p23Max-p13Min );
				if(d<dAux)
				{
					d=dAux;
					for(int k=0; k<3; k++)
					{
						this->minDPoint1[k]= point1[k];
						this->minDPoint2[k]=( point2[k]*p13Max - point3[k]*p12Max )/( p13Max - p12Max );
					}
				}
			}
		}
	}
	this->minD = d;
}

//--------------------------------------------------
void meNormalPlaneToVessel::InitializeCenter()
//--------------------------------------------------
{
	int numPoints = this->m_Intersection->GetNumberOfPoints();
	double point1[3];
	
	for(int i=0; i<3; i++) this->m_Point[i]=0;
	for(int a=0; a<numPoints; a++)
	{
		this->m_Intersection->GetPoint(a,point1);
		for(int i=0; i<3; i++)
		{
			this->m_Point[i]+=point1[i];
		}
	}
	for(int i=0; i<3; i++) this->m_Point[i]=this->m_Point[i]/numPoints;
}

//--------------------------------------------------
void meNormalPlaneToVessel::ImproveCenter()
//--------------------------------------------------
{
	double 	point1p[3],
		point2p[3],
		point1m[3],
		point2m[3],
		d1p,
		d2p,
		d1m,
		d2m,
		d1,
		d2,
		step,
		a;
	this->ComputeE1E2();

	vtkCellLocator * locator = vtkCellLocator::New();
	locator->SetDataSet(this->m_Intersection);
	locator->BuildLocator();
	vtkIdType CellId;
	int SubId;
	double clossestPoint[3];
	
	int iterations=0;
	do
	{
		locator->FindClosestPoint(this->m_Point, clossestPoint, CellId, SubId, this->m_Radius);
		this->m_Radius = sqrt(this->m_Radius);
		step = this->m_Radius/10000;
		for(int i=0; i<3; i++)
		{
			point1p[i] = this->m_Point[i];
			point2p[i] = this->m_Point[i];
			point1m[i] = this->m_Point[i];
			point2m[i] = this->m_Point[i];
		}
		for(int i=0; i<3; i++)
		{
			point1p[i] += step*this->e1[i];
			point2p[i] += step*this->e2[i];
			point1m[i] -= step*this->e1[i];
			point2m[i] -= step*this->e2[i];
		}
		locator->FindClosestPoint(point1p, clossestPoint, CellId, SubId, d1p);
		locator->FindClosestPoint(point2p, clossestPoint, CellId, SubId, d2p);
		locator->FindClosestPoint(point1m, clossestPoint, CellId, SubId, d1m);
		locator->FindClosestPoint(point2m, clossestPoint, CellId, SubId, d2m);
		d1 = (sqrt(d1p)-sqrt(d1m))/2;
		d2 = (sqrt(d2p)-sqrt(d2m))/2;

		a = sqrt(pow(d1,2)+pow(d2,2));

		for(int i=0; i<3; i++)
		{
			this->m_Point[i] += d1*this->e1[i] + d2*this->e2[i];
		}
		iterations++;
	}
	while(a>0.9*step && iterations < 1000);

	locator->Delete();

}


//--------------------------------------------------
void meNormalPlaneToVessel::ComputeCircumscribingCenterAndRadius()
//--------------------------------------------------
{
	double d = 0,
		dAux,
		point[3],
		n[3];

	for(int i=0; i<3; i++) this->circumscribingCenter[i] = this->m_Point[i];
	int numPoints = this->m_Intersection->GetNumberOfPoints();
	for(int i=0; i<numPoints; i++)
	{
		this->m_Intersection->GetPoint(i,point);
		dAux = 0;
		for(int j=0; j<3; j++)
		{
			dAux += pow(point[j]-this->circumscribingCenter[j],2);
		}
		if(d<dAux)
		{
			d=dAux;
			for(int j=0; j<3; j++)
			{
				n[j]=(point[j]-this->circumscribingCenter[j])/sqrt(d);
			}
		}
	}
	this->circumscribingRadius = sqrt(d);

	double h = this->circumscribingRadius,
		l,
		lAux,
		v[3],
		vAux[3],
		mu = 0, // Value never used, in principle.
		muAux,
		s = 0, // Value never used, in principle.
		sAux,
		bRate = 0,
		a,
		b;

	while( bRate < 1 && numPoints )
	{
		l = h;
		for(int i=0; i<numPoints; i++)
		{
			this->m_Intersection->GetPoint(i,point);
			lAux = 0;
			sAux = 0;
			muAux = 0;
			for(int j=0; j<3; j++)
			{
				vAux[j] = point[j]-this->circumscribingCenter[j];
				sAux += pow(vAux[j],2);
				muAux += vAux[j]*n[j];
			}
			if(muAux<h*0.99999)
			{
				lAux = (d-sAux)/(2*(h-muAux));
				if(lAux<l)
				{
					l = lAux;
					s = sAux;
					mu = muAux;
					for(int j=0; j<3; j++)  v[j] = vAux[j];
				}
			}
		}
		if(pow(mu,2)<s*0.99999)
		{
			bRate = sqrt((d-pow(h,2))/(s-pow(mu,2)));
		}
		else
		{
			bRate = 1;
		}
		d -= (2*h-l)*l;
		a = (1-bRate)/2;
		b = (h+mu*bRate)/2-l;
		dAux = 0;
		for(int j=0; j<3; j++)
		{
			this->circumscribingCenter[j] += l*n[j];
			vAux[j] = a*v[j] + b*n[j];
			dAux += pow(vAux[j],2);
		}
		h = sqrt(dAux);
		for(int j=0; j<3; j++)
		{
			n[j] = vAux[j]/h;
		}
	}
	this->circumscribingRadius = sqrt(d);
}

//--------------------------------------------------
void meNormalPlaneToVessel::ComputeMeshTangentPlaneAtPoint()
//--------------------------------------------------
{
	unsigned short NeighboursNumber;
	vtkIdType *cellId,
	     cellId1,
	     CellPointsNum,
	    *pointIds,
	     n;

	std::vector<int> neighbourPointId;
	std::vector< std::vector<int> > neighbourCellPointIds;

	this->m_Mesh->BuildLinks();
	this->m_Mesh->GetPointCells(this->pointId,NeighboursNumber,cellId);
	neighbourPointId.resize(NeighboursNumber+1);
	neighbourCellPointIds.resize(NeighboursNumber);

	for(int a=0; a<NeighboursNumber; a++)
	{
		cellId1=cellId[a];
		this->m_Mesh->GetCellPoints(cellId1,CellPointsNum,pointIds);
		if(this->pointId == pointIds[0]) n=1;
		else if(this->pointId == pointIds[1]) n=2;
		else n=0;
		neighbourCellPointIds[a].resize(2);
		neighbourCellPointIds[a][0] = pointIds[n];
		neighbourCellPointIds[a][1] = pointIds[(n+1)%3];
	}

	neighbourPointId[0]=neighbourCellPointIds[0][0];
	neighbourPointId[1]=neighbourCellPointIds[0][1];

	n=1;
	int previusId = 0;
	for(int a=2; a<NeighboursNumber+1; a++)
	{
		previusId = n-1;
		n=1;
		bool notfound = true;
		while(notfound && n<NeighboursNumber)
		{
			if(n != previusId)
			{
				if(neighbourCellPointIds[n][0] == neighbourPointId[a-1])
				{
					neighbourPointId[a] = neighbourCellPointIds[n][1];
					notfound = false;
				}
				else if(neighbourCellPointIds[n][1] == neighbourPointId[a-1])
				{
					neighbourPointId[a] = neighbourCellPointIds[n][0];
					notfound = false;
				}
			}
			n++;
		}
		if(notfound)
		{
			cerr << "BAD POINT SELECTION: The selected point is on the surface boundary." << endl;
			return;
		}
	}

	if(neighbourPointId[NeighboursNumber] != neighbourPointId[0])
	{
		cerr << "BAD POINT SELECTION (2): The selected point is on the surface boundary." << endl;
		return;
	}

	double point1[3],
	       point2[3];

	for(int i=0; i<3; i++)
	{
		this->tangentPlane[i] = 0;
	}

	for(int a=0; a<NeighboursNumber; a++)
	{
		m_Mesh->GetPoint(neighbourPointId[a],point1);
		m_Mesh->GetPoint(neighbourPointId[a+1],point2);
		for(int i=0; i<3; i++)
		{
			point1[i] -= this->m_Point[i];
			point2[i] -= this->m_Point[i];
		}
		for(int i=0; i<3; i++)
		{
			this->tangentPlane[i] += point1[(i+1)%3]*point2[(i+2)%3]-point1[(i+2)%3]*point2[(i+1)%3];
		}
	}

	double d=0;
	for(int i=0; i<3; i++) d+=pow(this->tangentPlane[i],2);
	d=sqrt(d);
	for(int i=0; i<3; i++) this->tangentPlane[i] = this->tangentPlane[i]/d;
}

//--------------------------------------------------
void meNormalPlaneToVessel::InitializeNormal()
//--------------------------------------------------
{
	this->ComputeMeshTangentPlaneAtPoint();
	if(this->secondPointSet)
	{
		this->InitializeNormalFromTwoPoints();
	}
	else
	{
		this->InitializeNormalFromOnePoint();
	}
}

void meNormalPlaneToVessel::InitializeNormal( double normal[3] )
{
	for(int i=0; i<3; i++) this->m_Normal[i] = normal[i];
}
//--------------------------------------------------
void meNormalPlaneToVessel::InitializeNormalFromOnePoint()
//--------------------------------------------------
{
	for(int i=0; i<3; i++) this->m_Normal[i] = this->tangentPlane[i];
	this->ComputeE1E2();
	double vectors[8][2]={{1,0},{0.923879532511,0.382683432365},{0.707106781187,0.707106781187},{0.382683432365,0.923879532511},{0,1},{-0.382683432365,0.923879532511},{-0.707106781187,0.707106781187},{-0.923879532511,0.382683432365}};
	double vector[3],
	       area,
	       area1p;

	int vectorId = 0;
	for(int i=0; i<3; i++) vector[i] = vectors[0][0]*this->e1[i] + vectors[0][1]*this->e2[i];
	area = GetArea(vector);

	for(int k=1; k<8; k++)
	{
		for(int i=0; i<3; i++) vector[i] = vectors[k][0]*this->e1[i] + vectors[k][1]*this->e2[i];
		area1p = GetArea(vector);
		if(area1p < area)
		{
			area = area1p;
			vectorId = k;
		}
	}
	for(int i=0; i<3; i++)
	{
		this->m_Normal[i] = vectors[vectorId][0]*this->e1[i] + vectors[vectorId][1]*this->e2[i];
	}
}

//--------------------------------------------------
void meNormalPlaneToVessel::InitializeNormalFromTwoPoints()
//--------------------------------------------------
{
	// TOCHECK: This strategy can be unstable when the two selected points 
	//          define a diameter of the vessel.
	double norm = 0;
	for(int i=0; i<3; i++)
	{
		this->segment[i] = this->m_Point2[i] - this->m_Point[i];
		norm += pow(this->segment[i],2);
	}
	norm = sqrt(norm);
	for(int i=0; i<3; i++) this->segment[i] = this->segment[i]/norm;

	this->m_Normal[0] = this->tangentPlane[1]*this->segment[2] - this->tangentPlane[2]*this->segment[1];
	this->m_Normal[1] = this->tangentPlane[2]*this->segment[0] - this->tangentPlane[0]*this->segment[2];
	this->m_Normal[2] = this->tangentPlane[0]*this->segment[1] - this->tangentPlane[1]*this->segment[0];
}

void meNormalPlaneToVessel::SetSurfaceMesh( vtkPolyData * mesh )
{
	this->m_Mesh->DeepCopy( mesh );
	//		this->CloseMesh();
}

void meNormalPlaneToVessel::SetClosedSurfaceMesh( vtkPolyData * closedMesh )
{
	this->m_ClosedMesh->DeepCopy( closedMesh );
}

void meNormalPlaneToVessel::SetOrigin( double point[3] )
{
	for(int i=0; i<3; i++) this->m_Point[i] = point[i];
	this->secondPointSet = false;
}

void meNormalPlaneToVessel::SetOriginMeshId( int pointId )
{
	this->pointId = pointId;
	for(int i=0; i<3; i++) this->m_Point[i] = m_Mesh->GetPoint(pointId)[i];
	this->secondPointSet = false;
}

void meNormalPlaneToVessel::SetSecondOrigin( double point[3] )
{
	for(int i=0; i<3; i++) this->m_Point2[i] = point[i];
	this->secondPointSet = true;
}

void meNormalPlaneToVessel::SetSecondOriginMeshId( int pointId )
{
	this->point2Id = pointId;
	for(int i=0; i<3; i++) this->m_Point2[i] = m_Mesh->GetPoint(pointId)[i];
	this->secondPointSet = true;
}

void meNormalPlaneToVessel::Update()
{
	this->ImproveNormal();
	this->ImproveCenter();
	//		this->ComputeMaxAndMinDiameters();
}

void meNormalPlaneToVessel::GetNormal( double normal[3] )
{
	for(int i=0; i<3; i++) normal[i] = this->m_Normal[i];
}

double * meNormalPlaneToVessel::GetNormal()
{
	return this->m_Normal;
}
void meNormalPlaneToVessel::SetNormal( double normal[3] )
{
	for(int i=0; i<3; i++) this->m_Normal[i] = normal[i];
}

void meNormalPlaneToVessel::GetE1( double e1[3] )
{
	for(int i=0; i<3; i++) e1[i] = this->e1[i];
}

double * meNormalPlaneToVessel::GetE1()
{
	return this->e1;
}

void meNormalPlaneToVessel::GetE2( double e2[3] )
{
	for(int i=0; i<3; i++) e2[i] = this->e2[i];
}

double * meNormalPlaneToVessel::GetE2()
{
	return this->e2;
}
void meNormalPlaneToVessel::GetE1E2( double e1[3], double e2[3] )
{
	for(int i=0; i<3; i++)
	{
		e1[i] = this->e1[i];
		e2[i] = this->e2[i];
	}
}

void meNormalPlaneToVessel::SetE1E2( double e1[3], double e2[3] )
{
	for(int i=0; i<3; i++)
	{
		this->e1[i] = e1[i];
		this->e2[i] = e2[i];
	}
}

double * meNormalPlaneToVessel::GetMaxDiameterDirection()
{
	return this->e1;
}

double * meNormalPlaneToVessel::GetMaxDiameterPoints1() 
{
	return this->maxDPoint1;
}

double * meNormalPlaneToVessel::GetMaxDiameterPoints2() 
{
	return this->maxDPoint2;
}

double * meNormalPlaneToVessel::GetMinDiameterPoints1() 
{
	return this->minDPoint1;
}

double * meNormalPlaneToVessel::GetMinDiameterPoints2() 
{
	return this->minDPoint2;
}

void meNormalPlaneToVessel::GetMaxDiameterPoints(double point1[3], double point2[3])
{
	for(int i=0; i<3; i++) 
	{
		point1[i] = this->maxDPoint1[i];
		point2[i] = this->maxDPoint2[i];
	}
}

void meNormalPlaneToVessel::GetMinDiameterPoints(double point1[3], double point2[3])
{
	for(int i=0; i<3; i++) 
	{
		point1[i] = this->minDPoint1[i];
		point2[i] = this->minDPoint2[i];
	}
}

void meNormalPlaneToVessel::GetMaxDiameterDirection( double e[3] )
{
	for(int i=0; i<3; i++) e[i] = this->m_MaxDDirection[i];
}
void meNormalPlaneToVessel::GetMinDiameterDirection( double e[3] )
{
	for(int i=0; i<3; i++) e[i] = this->m_MinDDirection[i];
}

double * meNormalPlaneToVessel::GetMinDiameterDirection()
{
	return this->m_MinDDirection;
}

void meNormalPlaneToVessel::GetMaxAndMinDiameterDirections( double e1[3], double e2[3] )
{
	for(int i=0; i<3; i++)
	{
		e1[i] = this->m_MaxDDirection[i];
		e2[i] = this->m_MinDDirection[i];
	}
}

double meNormalPlaneToVessel::GetMaxDiameter()
{
	return this->maxD;
}

double meNormalPlaneToVessel::GetMinDiameter()
{
	return this->minD;
}

void meNormalPlaneToVessel::GetMaxAndMinDiameters( double & max, double & min )
{
	max = this->maxD;
	min = this->minD;
}

void meNormalPlaneToVessel::GetContourCrossSection( vtkPolyData * contour )
{
	contour->DeepCopy(this->m_Intersection);
	contour->Update();
}

vtkPolyData * meNormalPlaneToVessel::GetContourCrossSection()
{
	return this->m_Intersection;
}

void meNormalPlaneToVessel::ComputeInscribedCenterAndRadius()
{
	double radius = this->m_Radius,
		center[3];
	for(int i=0; i<3; i++) center[i] = this->m_Point[i];
	if(!centerComputed) this->ImproveCenter();
	this->inscribedRadius = this->m_Radius;
	this->m_Radius = radius;
	for(int i=0; i<3; i++)
	{
		this->inscribedCenter[i] = this->m_Point[i];
		this->m_Point[i] = center[i];
	}
}

double meNormalPlaneToVessel::GetInscribedRadius()
{
	return this->inscribedRadius;
}

double * meNormalPlaneToVessel::GetInscribedCenter()
{
	return this->inscribedCenter;
}

void meNormalPlaneToVessel::GetInscribedCenter( double center[3] )
{
	for(int i=0; i<3; i++) center[i] = this->inscribedCenter[i];
}

double meNormalPlaneToVessel::GetCircumscribingRadius()
{
	return this->circumscribingRadius;
}

double * meNormalPlaneToVessel::GetCircumscribingCenter()
{
	return this->circumscribingCenter;
}

void meNormalPlaneToVessel::GetCircumscribingCenter( double center[3] )
{
	for(int i=0; i<3; i++) center[i] = this->circumscribingCenter[i];
}

double * meNormalPlaneToVessel::GetCenter()
{
	return this->m_Point;
}

void meNormalPlaneToVessel::GetCenter( double center[3] )
{
	for(int i=0; i<3; i++) center[i] = this->m_Point[i];
}

void meNormalPlaneToVessel::SetCenter( double center[3] )
{
	for(int i=0; i<3; i++) this->m_Point[i] = center[i];
}

void meNormalPlaneToVessel::SetConnectedCutPlane(vtkPolyData *connec) 
{
	this->m_Intersection->DeepCopy(connec);
}

// //--------------------------------------------------
// void meNormalPlaneToVessel::CloseMesh()
// //--------------------------------------------------
// {
// //	meHoleFiller ::Pointer meshcloser = meHoleFiller::New();
// /*	meCloseHoles::Pointer meshcloser = meCloseHoles::New();
// 	meshcloser->SetInput(this->m_Mesh);
// 	meshcloser->Update();
// 	this->m_ClosedMesh->DeepCopy(meshcloser->GetOutput());*/
// 	meCloseHoles meshcloser;
// 	meshcloser.SetInput(this->m_Mesh);
// 	meshcloser.Update();
// 	this->m_ClosedMesh->DeepCopy(meshcloser->GetOutput());
// //this->m_ClosedMesh->DeepCopy(this->m_Mesh);
// }

