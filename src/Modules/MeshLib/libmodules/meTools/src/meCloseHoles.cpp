/*
* Copyright (c) 2008,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <meCloseHoles.h>
#include <vtkPolyDataNormals.h>
#include <vtkPointLocator.h>
#include <vtkSmartPointer.h>
#include <cmath>
//-------------------------------------------------------//
meCloseHoles::meCloseHoles()
{
	this->output = vtkPolyData::New();
	this->patchOutput.clear();
	this->closeHoleAlgorithm = SMOOTH_TO_CENTER;
}


//Destructor
meCloseHoles::~meCloseHoles()
{
	this->output->Delete();
	this->patchOutput.clear();
}

void meCloseHoles::Update()
{
	vtkPolyDataNormals * normals = vtkPolyDataNormals::New();
	normals->SetInput(this->input);
	normals->SplittingOff();
	normals->SetFeatureAngle(180.0);
	normals->ComputePointNormalsOff();
	normals->ComputeCellNormalsOn();
	normals->AutoOrientNormalsOn();
	normals->ConsistencyOn();
	normals->Update();
	normals->GetOutput()->BuildLinks();

	vtkPointLocator * locator = vtkPointLocator::New();
	locator->SetDataSet(this->input);
	locator->BuildLocator();

	vtkFeatureEdges * edges = vtkFeatureEdges::New();
	edges->SetInput(normals->GetOutput());
	edges->BoundaryEdgesOn();
	edges->FeatureEdgesOff();
	edges->ManifoldEdgesOff();
	edges->NonManifoldEdgesOff();
	edges->Update();          
	
	//PolyDataConnectivityFilter: each hole is identified as a region
	vtkPolyDataConnectivityFilter *connec = vtkPolyDataConnectivityFilter::New();
	connec->SetInput(edges->GetOutput());
	connec->SetExtractionModeToSpecifiedRegions();
	connec->Update();
	edges->Delete();

	int numEdgesInRegion = 0,
	    numRegions = connec->GetNumberOfExtractedRegions(),
	    nUsedIds,
	    pointId,
	    pointId1,
	    pointId2;

	this->patchOutput.clear();
	patchOutput.resize(numRegions);

	vtkCleanPolyData * clean = vtkCleanPolyData::New();
	vector<vtkPolyData*> poly(numRegions);
	vtkPolyData * patch;
	vtkPoints * points;
	vtkCellArray * polys;
	vtkIdList * ptIds = vtkIdList::New(),
	          * ptIds1 = vtkIdList::New(),
	          * ptIds2 = vtkIdList::New(),
	          * ptIdsOrdered = vtkIdList::New(),
	          * cellIds = vtkIdList::New(),
	          * cellIdsOrdered = vtkIdList::New();

	vector<int> nptsInCorona,
	            nptsUntilCorona;

	vector< vector<bool> > onlyOneCorrespondingPoint;
	vector< vector<double> > normalsAtPoint;

	//For each hole (region) the points are stored and sorted
	for(int idRegion = 0; idRegion < numRegions;	idRegion++ )
	{
		connec->AddSpecifiedRegion(idRegion);
		connec->Update();   
		numEdgesInRegion = connec->GetOutput()->GetNumberOfLines();
		clean->SetInput(connec->GetOutput());
		clean->Update();    

		//For each hole, point and connectivity lists (not sorted) are created.
		int npts = clean->GetOutput()->GetNumberOfPoints();
		clean->GetOutput()->BuildLinks(); //in order to sort
 		clean->GetOutput()->BuildCells();
		clean->GetOutput()->Update();

		double center[3]={0,0,0},
		       point[3],
		       point1[3],
		       point2[3];

		points = vtkPoints::New();
		ptIdsOrdered->Reset();
		cellIdsOrdered->Reset();

		int idUsed = 0;
		for(int id=0; id< numEdgesInRegion; id++)
		{
			cellIdsOrdered->InsertNextId(idUsed);
			clean->GetOutput()->GetCellPoints(idUsed,ptIds);
			ptIdsOrdered->InsertNextId(ptIds->GetId(0));
			clean->GetOutput()->GetPointCells(ptIds->GetId(1),cellIds);
			idUsed = (cellIds->GetId(0) == idUsed ? cellIds->GetId(1) : idUsed=cellIds->GetId(0));
			clean->GetOutput()->GetPoint(ptIds->GetId(0),point);
			points->InsertNextPoint(point);
		}
		for(int id=0; id<npts; id++)
		{
			clean->GetOutput()->GetPoint(id,point);
			for(int i=0; i<3; i++) center[i]+=point[i];
		}
		for(int i=0; i<3; i++) center[i]/=npts;

		double normal[3]={0,0,0};
		double area=0,
		       theta,
		       R,
		       a,
		       l,
		       b,
		       mn;
		if(this->closeHoleAlgorithm == SMOOTH_TO_CENTER || this->closeHoleAlgorithm == SMOOTH_PATCH)
		{
			points->GetPoint(npts-1,point1);
			for(int i=0; i<3; i++) point1[i]-=center[i];
			for(int id=0; id< npts; id++)
			{
				points->GetPoint(id,point);
				for(int i=0; i<3; i++)
				{
					point[i]-=center[i];
					point2[i]=point[i]-point1[i];
				}
				for(int i=0; i<3; i++) normal[i]+=point1[(i+1)%3]*point2[(i+2)%3]-point1[(i+2)%3]*point2[(i+1)%3];
				for(int i=0; i<3; i++) point1[i]=point2[i];
			}
			for(int i=0; i<3; i++) area+=pow(normal[i],2);
			area=sqrt(area);
			for(int i=0; i<3; i++) normal[i]/=area;
		}
		int numberOfCoronas=( this->closeHoleAlgorithm==SINGLE_CENTER_POINT ? 1 : npts/6+1 );
		if(this->closeHoleAlgorithm == SMOOTH_PATCH) numberOfCoronas*=5;
		nptsInCorona.clear();
		nptsInCorona.resize(numberOfCoronas);
		nptsUntilCorona.clear();
		nptsUntilCorona.resize(numberOfCoronas+1);
		nptsUntilCorona[0]=0;
		nptsInCorona[0]=npts;
		onlyOneCorrespondingPoint.clear();
		onlyOneCorrespondingPoint.resize(numberOfCoronas);
		onlyOneCorrespondingPoint[0].resize(npts);
		if(this->closeHoleAlgorithm == SMOOTH_PATCH)
		{
			normalsAtPoint.resize(npts);
			for(int id=0; id<npts; id++)
			{
				normalsAtPoint[id].clear();
				normalsAtPoint[id].resize(3);
				points->GetPoint(id,point);
				pointId=locator->FindClosestPoint(point);
				normals->GetOutput()->GetPointCells(pointId,cellIds);
//				for(int i=0; i<3; i++) normalsAtPoint[id][i]=0;
				a=cellIds->GetNumberOfIds();
				for(int j=0; j<a; j++)
				{
					normals->GetOutput()->GetCellPoints(cellIds->GetId(j),ptIds);
					int sameId;
					if(ptIds->GetId(0)==pointId) sameId=0;
					else if(ptIds->GetId(1)==pointId) sameId=1;
					else sameId=2;
					this->input->GetPoint(ptIds->GetId((sameId+1)%3),point1);
					this->input->GetPoint(ptIds->GetId((sameId+2)%3),point2);
					for(int i=0; i<3; i++)
					{
						point1[i]-=point[i];
						point2[i]-=point[i];
					}
					for(int i=0; i<3; i++)
					{
						normalsAtPoint[id][i]+=point1[(i+1)%3]*point2[(i+2)%3]-point1[(i+2)%3]*point2[(i+1)%3];
					}
					area=0;
					for(int i=0; i<3; i++) area+=pow(normalsAtPoint[id][i],2);
					area=sqrt(area);
					for(int i=0; i<3; i++) normalsAtPoint[id][i]/=area;
				}
			}
		}
		
		double typicalLengthSq=0,
		       lengthSq;
		clean->GetOutput()->GetPoint(0,point);
		for(int i=0; i<3; i++) typicalLengthSq+=pow(point[i]-center[i],2);
		typicalLengthSq/=numberOfCoronas*numberOfCoronas;
		bool signChange = true;

		for(int c=1; c<numberOfCoronas; c++)
		{
			nptsUntilCorona[c]=nptsUntilCorona[c-1]+nptsInCorona[c-1];
			nUsedIds=0;
			if(this->closeHoleAlgorithm == SMOOTH_PATCH)
			{
/*				double displacement=1e10;
				for(int id=0; id<nptsInCorona[c-1]; id++)
				{
					points->GetPoint(nptsUntilCorona[c-1]+id,point);
					a=0;
					b=0;
					mn=0;
					for(int i=0; i<3; i++)
					{
						point[i]-=center[i];
						a+=point[i]*normalsAtPoint[id][i];
						b+=point[i]*normal[i];
						mn+=normalsAtPoint[id][i]*normal[i];
					}
					l=(mn*a-b)/(1-pow(mn,2));
					if(firstDisplacement)
					{
						if(displacement>l) displacement=l;
						firstDisplacement = false;
					}
					else
					{
						if(displacement>l) displacement=l;
					} 
				}
				for(int i=0; i<3; i++)
				{
					center[i]+=displacement*normal[i];
				}*/
				for(int i=0; i<3; i++)
				{
					center[i]=0;
					for(int id=0; id<nptsInCorona[c-1]; id++) center[i]+=points->GetPoint(nptsUntilCorona[c-1]+id)[i];
					center[i]/=nptsInCorona[c-1];
				}
			}
			for(int id=0; id<nptsInCorona[c-1]; id++)
			{
				points->GetPoint(id+nptsUntilCorona[c-1],point1);
				points->GetPoint((id+1)%nptsInCorona[c-1]+nptsUntilCorona[c-1],point2);
				lengthSq=0;
				for(int i=0; i<3; i++) lengthSq+=pow(point1[i]-point2[i],2);
				onlyOneCorrespondingPoint[c-1][id]=
					((this->closeHoleAlgorithm==LINEAR_TO_CENTER || this->closeHoleAlgorithm==SMOOTH_TO_CENTER ) 
					&& lengthSq<typicalLengthSq && (id==0 || !onlyOneCorrespondingPoint[c-1][id-1]) 
					&& nptsInCorona[c-1]>3	// Added to correct bad behaviour in extrange contours.
					);
				if(!onlyOneCorrespondingPoint[c-1][id])
				{
					switch(this->closeHoleAlgorithm)
					{
						case RADIAL_TRIANGLES :
							for(int i=0; i<3; i++)
							{
								point[i]=(point1[i]*(numberOfCoronas-c)+center[i])/(numberOfCoronas-c+1);
							}
						break;
						case LINEAR_TO_CENTER :
						case LINEAR_TO_CENTER_NONREDUCING_TRIANGLES :
							for(int i=0; i<3; i++)
							{
								point[i]=((point1[i]+point2[i])/2*(numberOfCoronas-c)+center[i])/(numberOfCoronas-c+1);
							}
						break;
						case SMOOTH_TO_CENTER :
							a=0;
							l=0;
							for(int i=0; i<3; i++)
							{
								point[i]=(point1[i]+point2[i])/2-center[i];
								a+=point[i]*normal[i];
								l+=pow(point[i],2);
							}
							R=l/a/2;
							theta=asin(a/sqrt(l));
							theta=2*theta*(1.-1./(numberOfCoronas-c+1));
							if(fabs(theta)<0.001)
							{
								l=pow(a,2)/l;
								double fractionAngle=1.-1./(numberOfCoronas-c+1);
								for(int i=0; i<3; i++)
								{
									point[i]=pow(fractionAngle,2)*a*normal[i]+center[i]
										+fractionAngle/sqrt(1-l)*(point[i]-a*normal[i]);
								}
							}
							else
							{
								for(int i=0; i<3; i++)
								{
									point[i]=-R*cos(theta)*normal[i]
										+R*sin(theta)/sqrt(l-pow(a,2))*(point[i]-a*normal[i])
										+center[i]+R*normal[i];
								}
							}
						break;
						case SMOOTH_PATCH :
							points->GetPoint(nptsUntilCorona[c-1]+id,point);
							a=0;
							b=0;
							mn=0;
							for(int i=0; i<3; i++)
							{
								point[i]-=center[i];
								a+=point[i]*normalsAtPoint[id][i];
								b+=point[i]*normal[i];
								mn+=normalsAtPoint[id][i]*normal[i];
							}
							l=(mn*a-b)/(1-pow(mn,2));
							signChange = (l>0);

							a=0;
							l=0;
							b=0;
							mn=0;
							for(int i=0; i<3; i++)
							{
								point[i]=-(point1[i]+point2[i])/2+center[i];
								a+=point[i]*normalsAtPoint[id][i];
								l+=pow(point[i],2);
							}
							R=l/a/2;
							theta=asin(a/sqrt(l));
							double fractionAngle=1./(numberOfCoronas-c+1);
							if(signChange) fractionAngle*=-1;
							theta=2*theta*fractionAngle;
							if(fabs(theta)<0.001)
							{
								l=pow(a,2)/l;
								for(int i=0; i<3; i++)
								{
									point[i]=pow(fractionAngle,2)*a*normalsAtPoint[id][i]
										+center[i]-point[i]
										+fractionAngle/sqrt(1-l)*(point[i]-a*normalsAtPoint[id][i]);
								}
							}
							else
							{
								for(int i=0; i<3; i++)
								{
									point[i]=-R*cos(theta)*normalsAtPoint[id][i]
										+R*sin(theta)/sqrt(l-pow(a,2))*(point[i]-a*normalsAtPoint[id][i])
										+center[i]-point[i]+R*normalsAtPoint[id][i];
								}
							}
							for(int i=0; i<3; i++)
							{
								point1[i]-=point[i];
								point2[i]-=point[i];
							}
							for(int i=0; i<3; i++)
							{
								normalsAtPoint[nUsedIds][i]=point1[(i+1)%3]*point2[(i+2)%3]-point1[(i+2)%3]*point2[(i+1)%3];
							}
							area=0;
							for(int i=0; i<3; i++) area+=pow(normalsAtPoint[nUsedIds][i],2);
							area=sqrt(area);
							for(int i=0; i<3; i++) normalsAtPoint[nUsedIds][i]/=area;
						break;
					}
					nUsedIds++;
					points->InsertNextPoint(point);
				}
			}
			nptsInCorona[c]=nUsedIds;
			onlyOneCorrespondingPoint[c].resize(nUsedIds);
		}
		nptsUntilCorona[numberOfCoronas]=nptsUntilCorona[numberOfCoronas-1]+nptsInCorona[numberOfCoronas-1];
		points->InsertNextPoint(center);

		polys = vtkCellArray::New(); //A polygon is created

		for(int c=0; c<numberOfCoronas-1; c++)
		{
			int id2=0;
			for(int id=0; id<nptsInCorona[c] ; id++)
			{
				if(onlyOneCorrespondingPoint[c][id])
				{
					ptIds1->Reset();
					ptIds1->InsertNextId(id+nptsUntilCorona[c]);
					ptIds1->InsertNextId((id+1)%nptsInCorona[c]+nptsUntilCorona[c]);
					ptIds1->InsertNextId(id2%nptsInCorona[c+1]+nptsUntilCorona[c+1]);
					polys->InsertNextCell(ptIds1);
				}
				else
				{
					ptIds1->Reset();
					ptIds1->InsertNextId(id+nptsUntilCorona[c]);
					ptIds1->InsertNextId((id+1)%nptsInCorona[c]+nptsUntilCorona[c]);
					ptIds1->InsertNextId(id2+nptsUntilCorona[c+1]);
					ptIds2->Reset();
					ptIds2->InsertNextId((id+1)%nptsInCorona[c]+nptsUntilCorona[c]);
					ptIds2->InsertNextId(id2+nptsUntilCorona[c+1]);
					ptIds2->InsertNextId((id2+1)%nptsInCorona[c+1]+nptsUntilCorona[c+1]);
					polys->InsertNextCell(ptIds1);
					polys->InsertNextCell(ptIds2);
					id2++;
				}
			}
		}
		for(int id=0; id< nptsInCorona[numberOfCoronas-1]; id++)
		{
			ptIds1->Reset();
			ptIds1->InsertNextId(id+nptsUntilCorona[numberOfCoronas-1]);
			ptIds1->InsertNextId((id+1)%nptsInCorona[numberOfCoronas-1]+nptsUntilCorona[numberOfCoronas-1]);
			ptIds1->InsertNextId(nptsUntilCorona[numberOfCoronas]);
			polys->InsertNextCell(ptIds1);
		}
		patch = vtkPolyData::New();
		patch->SetPoints(points);
		patch->SetPolys(polys);

		//Each sorted hole is stored
		poly[idRegion] = patch;
		patchOutput.at(idRegion) = vtkPolyData::New();
		patchOutput.at(idRegion)->DeepCopy(patch);
		connec->DeleteSpecifiedRegion(idRegion);
	}

	locator->Delete();
	ptIds->Delete();
	ptIds1->Delete();
	ptIds2->Delete();
	ptIdsOrdered->Delete();
	cellIds->Delete();
	cellIdsOrdered->Delete();
	connec->Delete();

	vtkAppendPolyData *append2 = vtkAppendPolyData::New();

	append2->AddInput(this->input);
	if(numRegions != 0)
	{
		vtkAppendPolyData *append = vtkAppendPolyData::New();
		for(int idRegion = 0; idRegion < numRegions;	idRegion++ )
		{
			append->AddInput(poly[idRegion]);
			append->Update();
			poly[idRegion]->Delete();
		}
		append2->AddInput(append->GetOutput());
		append->Delete();
	}
	append2->Update();

	vtkCleanPolyData * clean2 = vtkCleanPolyData::New();
	clean2->SetInput(append2->GetOutput());
	clean2->PointMergingOn();
	clean2->SetTolerance(0.0);
	clean2->PieceInvariantOff();
	clean2->Update();
	append2->Delete();

	normals->SetInput(clean2->GetOutput());
	normals->SplittingOff();
	normals->SetFeatureAngle(360.0);
	normals->ComputePointNormalsOff();
	normals->ComputeCellNormalsOn();
	normals->FlipNormalsOn();
	normals->AutoOrientNormalsOn();
	normals->ConsistencyOn();
	normals->Update();

	this->output->DeepCopy(normals->GetOutput());

	normals->Delete();
	clean->Delete();
	clean2->Delete();
}

void meCloseHoles::Run()
{
	vtkSmartPointer<vtkPolyDataNormals>  normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	normals->SetInput(this->input);
	normals->SplittingOff();
	normals->SetFeatureAngle(180.0);
	normals->ComputePointNormalsOff();
	normals->ComputeCellNormalsOn();
	normals->AutoOrientNormalsOn();
	normals->ConsistencyOn();
	normals->Update();
	normals->GetOutput()->BuildLinks();

	this->patchOutput.clear();
//	vector<vtkPolyData*> poly(numRegions);
//	int numRegions = 1;

	vtkPolyData * patch= vtkPolyData::New();
	vtkPoints*  points= vtkPoints::New();
	vtkCellArray* polys= vtkCellArray::New();

	ComputePatchForHole(normals,points,polys);
	patch->SetPoints(points);
	patch->SetPolys(polys);

	this->patchOutput.push_back(patch);

	vtkSmartPointer<vtkAppendPolyData> append2 = vtkSmartPointer<vtkAppendPolyData>::New();

	append2->AddInput(this->input);
	append2->AddInput(patch);
	append2->Update();

	vtkCleanPolyData * clean2 = vtkCleanPolyData::New();
	clean2->SetInput(append2->GetOutput());
	clean2->PointMergingOn();
	clean2->SetTolerance(0.0);
	clean2->PieceInvariantOff();
	clean2->Update();

	normals->SetInput(clean2->GetOutput());
	normals->SplittingOff();
	normals->SetFeatureAngle(360.0);
	normals->ComputePointNormalsOff();
	normals->ComputeCellNormalsOn();
	normals->FlipNormalsOn();
	normals->AutoOrientNormalsOn();
	normals->ConsistencyOn();
	normals->Update();

	this->output->DeepCopy(clean2->GetOutput());

}

void meCloseHoles::ComputePatchForHole(vtkPolyDataNormals* normals, vtkPoints* points, vtkCellArray* polys)
{
	vtkSmartPointer<vtkPointLocator> locator = vtkSmartPointer<vtkPointLocator>::New();
	locator->SetDataSet(this->input);
	locator->BuildLocator();

	int numEdgesInRegion = 0,
		nUsedIds,
		pointId,
		pointId1,
		pointId2;

	vtkSmartPointer<vtkCleanPolyData> clean = vtkSmartPointer<vtkCleanPolyData>::New();
	vtkIdList* ptIds = vtkIdList::New(),
		* ptIds1 = vtkIdList::New(),
		* ptIds2 = vtkIdList::New(),
		* ptIdsOrdered = vtkIdList::New(),
		* cellIds = vtkIdList::New(),
		* cellIdsOrdered = vtkIdList::New();

	vector<int> nptsInCorona,
		nptsUntilCorona;

	vector< vector<bool> > onlyOneCorrespondingPoint;
	vector< vector<double> > normalsAtPoint;

	////For each hole (region) the points are stored and sorted
	//for(int idRegion = 0; idRegion < numRegions;	idRegion++ )
	//{
	numEdgesInRegion = inputHole->GetNumberOfLines();
	clean->SetInput(inputHole);
	clean->Update();    

	//For each hole, point and connectivity lists (not sorted) are created.
	int npts = clean->GetOutput()->GetNumberOfPoints();
	clean->GetOutput()->BuildLinks(); //in order to sort
	clean->GetOutput()->BuildCells();
	clean->GetOutput()->Update();

	double center[3]={0,0,0},
		point[3],
		point1[3],
		point2[3];

	//points = vtkPoints::New();
	ptIdsOrdered->Reset();
	cellIdsOrdered->Reset();

	int idUsed = 0;
	for(int id=0; id< numEdgesInRegion; id++)
	{
		cellIdsOrdered->InsertNextId(idUsed);
		clean->GetOutput()->GetCellPoints(idUsed,ptIds);
		ptIdsOrdered->InsertNextId(ptIds->GetId(0));
		clean->GetOutput()->GetPointCells(ptIds->GetId(1),cellIds);
		idUsed = (cellIds->GetId(0) == idUsed ? cellIds->GetId(1) : idUsed=cellIds->GetId(0));
		clean->GetOutput()->GetPoint(ptIds->GetId(0),point);
		points->InsertNextPoint(point);
	}
	for(int id=0; id<npts; id++)
	{
		clean->GetOutput()->GetPoint(id,point);
		for(int i=0; i<3; i++) center[i]+=point[i];
	}
	for(int i=0; i<3; i++) center[i]/=npts;

	double normal[3]={0,0,0};
	double area=0,
		theta,
		R,
		a,
		l,
		b,
		mn;
	if(this->closeHoleAlgorithm == SMOOTH_TO_CENTER || this->closeHoleAlgorithm == SMOOTH_PATCH)
	{
		points->GetPoint(npts-1,point1);
		for(int i=0; i<3; i++) point1[i]-=center[i];
		for(int id=0; id< npts; id++)
		{
			points->GetPoint(id,point);
			for(int i=0; i<3; i++)
			{
				point[i]-=center[i];
				point2[i]=point[i]-point1[i];
			}
			for(int i=0; i<3; i++) normal[i]+=point1[(i+1)%3]*point2[(i+2)%3]-point1[(i+2)%3]*point2[(i+1)%3];
			for(int i=0; i<3; i++) point1[i]=point2[i];
		}
		for(int i=0; i<3; i++) area+=pow(normal[i],2);
		area=sqrt(area);
		for(int i=0; i<3; i++) normal[i]/=area;
	}
	int numberOfCoronas=( this->closeHoleAlgorithm==SINGLE_CENTER_POINT ? 1 : npts/6+1 );
	if(this->closeHoleAlgorithm == SMOOTH_PATCH) numberOfCoronas*=5;
	nptsInCorona.clear();
	nptsInCorona.resize(numberOfCoronas);
	nptsUntilCorona.clear();
	nptsUntilCorona.resize(numberOfCoronas+1);
	nptsUntilCorona[0]=0;
	nptsInCorona[0]=npts;
	onlyOneCorrespondingPoint.clear();
	onlyOneCorrespondingPoint.resize(numberOfCoronas);
	onlyOneCorrespondingPoint[0].resize(npts);
	if(this->closeHoleAlgorithm == SMOOTH_PATCH)
	{
		normalsAtPoint.resize(npts);
		for(int id=0; id<npts; id++)
		{
			normalsAtPoint[id].clear();
			normalsAtPoint[id].resize(3);
			points->GetPoint(id,point);
			pointId=locator->FindClosestPoint(point);
			normals->GetOutput()->GetPointCells(pointId,cellIds);
			//				for(int i=0; i<3; i++) normalsAtPoint[id][i]=0;
			a=cellIds->GetNumberOfIds();
			for(int j=0; j<a; j++)
			{
				normals->GetOutput()->GetCellPoints(cellIds->GetId(j),ptIds);
				int sameId;
				if(ptIds->GetId(0)==pointId) sameId=0;
				else if(ptIds->GetId(1)==pointId) sameId=1;
				else sameId=2;
				this->input->GetPoint(ptIds->GetId((sameId+1)%3),point1);
				this->input->GetPoint(ptIds->GetId((sameId+2)%3),point2);
				for(int i=0; i<3; i++)
				{
					point1[i]-=point[i];
					point2[i]-=point[i];
				}
				for(int i=0; i<3; i++)
				{
					normalsAtPoint[id][i]+=point1[(i+1)%3]*point2[(i+2)%3]-point1[(i+2)%3]*point2[(i+1)%3];
				}
				area=0;
				for(int i=0; i<3; i++) area+=pow(normalsAtPoint[id][i],2);
				area=sqrt(area);
				for(int i=0; i<3; i++) normalsAtPoint[id][i]/=area;
			}
		}
	}

	double typicalLengthSq=0,
		lengthSq;
	clean->GetOutput()->GetPoint(0,point);
	for(int i=0; i<3; i++) typicalLengthSq+=pow(point[i]-center[i],2);
	typicalLengthSq/=numberOfCoronas*numberOfCoronas;
	bool signChange = true;

	for(int c=1; c<numberOfCoronas; c++)
	{
		nptsUntilCorona[c]=nptsUntilCorona[c-1]+nptsInCorona[c-1];
		nUsedIds=0;
		if(this->closeHoleAlgorithm == SMOOTH_PATCH)
		{
			/*				double displacement=1e10;
			for(int id=0; id<nptsInCorona[c-1]; id++)
			{
			points->GetPoint(nptsUntilCorona[c-1]+id,point);
			a=0;
			b=0;
			mn=0;
			for(int i=0; i<3; i++)
			{
			point[i]-=center[i];
			a+=point[i]*normalsAtPoint[id][i];
			b+=point[i]*normal[i];
			mn+=normalsAtPoint[id][i]*normal[i];
			}
			l=(mn*a-b)/(1-pow(mn,2));
			if(firstDisplacement)
			{
			if(displacement>l) displacement=l;
			firstDisplacement = false;
			}
			else
			{
			if(displacement>l) displacement=l;
			} 
			}
			for(int i=0; i<3; i++)
			{
			center[i]+=displacement*normal[i];
			}*/
			for(int i=0; i<3; i++)
			{
				center[i]=0;
				for(int id=0; id<nptsInCorona[c-1]; id++) center[i]+=points->GetPoint(nptsUntilCorona[c-1]+id)[i];
				center[i]/=nptsInCorona[c-1];
			}
		}
		for(int id=0; id<nptsInCorona[c-1]; id++)
		{
			points->GetPoint(id+nptsUntilCorona[c-1],point1);
			points->GetPoint((id+1)%nptsInCorona[c-1]+nptsUntilCorona[c-1],point2);
			lengthSq=0;
			for(int i=0; i<3; i++) lengthSq+=pow(point1[i]-point2[i],2);
			onlyOneCorrespondingPoint[c-1][id]=
				((this->closeHoleAlgorithm==LINEAR_TO_CENTER || this->closeHoleAlgorithm==SMOOTH_TO_CENTER ) 
				&& lengthSq<typicalLengthSq && (id==0 || !onlyOneCorrespondingPoint[c-1][id-1]) 
				&& nptsInCorona[c-1]>3	// Added to correct bad behaviour in extrange contours.
				);
			if(!onlyOneCorrespondingPoint[c-1][id])
			{
				switch(this->closeHoleAlgorithm)
				{
				case RADIAL_TRIANGLES :
					for(int i=0; i<3; i++)
					{
						point[i]=(point1[i]*(numberOfCoronas-c)+center[i])/(numberOfCoronas-c+1);
					}
					break;
				case LINEAR_TO_CENTER :
				case LINEAR_TO_CENTER_NONREDUCING_TRIANGLES :
					for(int i=0; i<3; i++)
					{
						point[i]=((point1[i]+point2[i])/2*(numberOfCoronas-c)+center[i])/(numberOfCoronas-c+1);
					}
					break;
				case SMOOTH_TO_CENTER :
					a=0;
					l=0;
					for(int i=0; i<3; i++)
					{
						point[i]=(point1[i]+point2[i])/2-center[i];
						a+=point[i]*normal[i];
						l+=pow(point[i],2);
					}
					R=l/a/2;
					theta=asin(a/sqrt(l));
					theta=2*theta*(1.-1./(numberOfCoronas-c+1));
					if(fabs(theta)<0.001)
					{
						l=pow(a,2)/l;
						double fractionAngle=1.-1./(numberOfCoronas-c+1);
						for(int i=0; i<3; i++)
						{
							point[i]=pow(fractionAngle,2)*a*normal[i]+center[i]
							+fractionAngle/sqrt(1-l)*(point[i]-a*normal[i]);
						}
					}
					else
					{
						for(int i=0; i<3; i++)
						{
							point[i]=-R*cos(theta)*normal[i]
							+R*sin(theta)/sqrt(l-pow(a,2))*(point[i]-a*normal[i])
								+center[i]+R*normal[i];
						}
					}
					break;
				case SMOOTH_PATCH :
					points->GetPoint(nptsUntilCorona[c-1]+id,point);
					a=0;
					b=0;
					mn=0;
					for(int i=0; i<3; i++)
					{
						point[i]-=center[i];
						a+=point[i]*normalsAtPoint[id][i];
						b+=point[i]*normal[i];
						mn+=normalsAtPoint[id][i]*normal[i];
					}
					l=(mn*a-b)/(1-pow(mn,2));
					signChange = (l>0);

					a=0;
					l=0;
					b=0;
					mn=0;
					for(int i=0; i<3; i++)
					{
						point[i]=-(point1[i]+point2[i])/2+center[i];
						a+=point[i]*normalsAtPoint[id][i];
						l+=pow(point[i],2);
					}
					R=l/a/2;
					theta=asin(a/sqrt(l));
					double fractionAngle=1./(numberOfCoronas-c+1);
					if(signChange) fractionAngle*=-1;
					theta=2*theta*fractionAngle;
					if(fabs(theta)<0.001)
					{
						l=pow(a,2)/l;
						for(int i=0; i<3; i++)
						{
							point[i]=pow(fractionAngle,2)*a*normalsAtPoint[id][i]
							+center[i]-point[i]
							+fractionAngle/sqrt(1-l)*(point[i]-a*normalsAtPoint[id][i]);
						}
					}
					else
					{
						for(int i=0; i<3; i++)
						{
							point[i]=-R*cos(theta)*normalsAtPoint[id][i]
							+R*sin(theta)/sqrt(l-pow(a,2))*(point[i]-a*normalsAtPoint[id][i])
								+center[i]-point[i]+R*normalsAtPoint[id][i];
						}
					}
					for(int i=0; i<3; i++)
					{
						point1[i]-=point[i];
						point2[i]-=point[i];
					}
					for(int i=0; i<3; i++)
					{
						normalsAtPoint[nUsedIds][i]=point1[(i+1)%3]*point2[(i+2)%3]-point1[(i+2)%3]*point2[(i+1)%3];
					}
					area=0;
					for(int i=0; i<3; i++) area+=pow(normalsAtPoint[nUsedIds][i],2);
					area=sqrt(area);
					for(int i=0; i<3; i++) normalsAtPoint[nUsedIds][i]/=area;
					break;
				}
				nUsedIds++;
				points->InsertNextPoint(point);
			}
		}
		nptsInCorona[c]=nUsedIds;
		onlyOneCorrespondingPoint[c].resize(nUsedIds);
	}
	nptsUntilCorona[numberOfCoronas]=nptsUntilCorona[numberOfCoronas-1]+nptsInCorona[numberOfCoronas-1];
	points->InsertNextPoint(center);

//	polys = vtkCellArray::New(); //A polygon is created

	for(int c=0; c<numberOfCoronas-1; c++)
	{
		int id2=0;
		for(int id=0; id<nptsInCorona[c] ; id++)
		{
			if(onlyOneCorrespondingPoint[c][id])
			{
				ptIds1->Reset();
				ptIds1->InsertNextId(id+nptsUntilCorona[c]);
				ptIds1->InsertNextId((id+1)%nptsInCorona[c]+nptsUntilCorona[c]);
				ptIds1->InsertNextId(id2%nptsInCorona[c+1]+nptsUntilCorona[c+1]);
				polys->InsertNextCell(ptIds1);
			}
			else
			{
				ptIds1->Reset();
				ptIds1->InsertNextId(id+nptsUntilCorona[c]);
				ptIds1->InsertNextId((id+1)%nptsInCorona[c]+nptsUntilCorona[c]);
				ptIds1->InsertNextId(id2+nptsUntilCorona[c+1]);
				ptIds2->Reset();
				ptIds2->InsertNextId((id+1)%nptsInCorona[c]+nptsUntilCorona[c]);
				ptIds2->InsertNextId(id2+nptsUntilCorona[c+1]);
				ptIds2->InsertNextId((id2+1)%nptsInCorona[c+1]+nptsUntilCorona[c+1]);
				polys->InsertNextCell(ptIds1);
				polys->InsertNextCell(ptIds2);
				id2++;
			}
		}
	}
	for(int id=0; id< nptsInCorona[numberOfCoronas-1]; id++)
	{
		ptIds1->Reset();
		ptIds1->InsertNextId(id+nptsUntilCorona[numberOfCoronas-1]);
		ptIds1->InsertNextId((id+1)%nptsInCorona[numberOfCoronas-1]+nptsUntilCorona[numberOfCoronas-1]);
		ptIds1->InsertNextId(nptsUntilCorona[numberOfCoronas]);
		polys->InsertNextCell(ptIds1);
	}
}
