/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef BLSIMPLEXTUBEMESHCREATOR_TXX
#define BLSIMPLEXTUBEMESHCREATOR_TXX

#include "blSimplexTubeMeshCreator.h"

#include "blConstants.h"

template<typename TInputMesh, typename TOutputMesh>
blSimplexTubeMeshCreator<TInputMesh,TOutputMesh>
::blSimplexTubeMeshCreator()
{
	OutputMeshPointer output = TOutputMesh::New();
	this->ProcessObject::SetNumberOfRequiredOutputs(1);
	this->ProcessObject::SetNthOutput(0, output.GetPointer());
	this->start = 0;
	this->tubeLength = 0;
	this->patchLength = 0;
	this->patchesOnSweep = 0;
	this->patchesOnLength = 0;
	this->r = 0.0;
	this->angleShift = 0.0;
	this->units = 1.0;
	this->surf = NULL;
	this->vtkCL = NULL;
	this->finalRadius = 1.4;
}

template <typename TInputMesh, typename TOutputMesh>
blSimplexTubeMeshCreator<TInputMesh, TOutputMesh>
::~blSimplexTubeMeshCreator()
{
	delete pointIndexMatrix;
}

template <typename TInputMesh, typename TOutputMesh>
void blSimplexTubeMeshCreator<TInputMesh, TOutputMesh>
::GenerateData()
{
	// Do all the computation from the path geometry data
	Initialize();
	// Just create the points by sweeping around the polyline
	if(CreateSimplexPoints() == -1)
		throw std::runtime_error::runtime_error( "An error occurred while generating simplex points. Stent is too long or point is to close to the end of the vessel" );
		//itkExceptionMacro(<< "Data could not be generated properly. An error occurred while generating simplex points." );
	if(CreateSimplexPoints()>=0){
		// Walk the points by cell and connect them
		this->CreateSimplexCells();
		this->GetOutput()->BuildCellLinks();
		OutputMeshPointer output = this->GetOutput();
	}else{
		std::cout << "Data could not be generated properly. An error occurred while generating simplex points."<< std::endl;
	}
}

template <typename TInputMesh, typename TOutputMesh>
void blSimplexTubeMeshCreator<TInputMesh, TOutputMesh>
::SetCenterlineOriginalSurface(vtkPolyData *surf)
{
	this->surf = surf;
}

template <typename TInputMesh, typename TOutputMesh>
void blSimplexTubeMeshCreator<TInputMesh, TOutputMesh>
::Initialize()
{
	//This is just a first approach, it will be improved and computed from the patch info
	if (!patchesOnSweep || !patchesOnLength){
		std::cout << "Invalid use of blSimplexTubeMeshCreator class, no number of patches has to be greater than 0 in length and sweep."<< std::endl;
		return;
	}

	//If the tube desired is not valid, stop
	if (!(tubeLength>0)){
		std::cout << "Generated mesh must be greater than 0."<< std::endl;
		return;
	}

	patchAngle = (2 * base::PI)/patchesOnSweep; // The angle covered by one patch (computed)
	patchLength = tubeLength/(patchesOnLength);// The length of one patch
	dx = patchLength/2; // Distance between points
	dTheta = patchAngle/2;  // Angle between points
	this->Rows = 2 * patchesOnSweep;
	this->Cols = (unsigned long)(tubeLength/dx);
	this->pointIndexMatrix = new PointIndexMatrixType(Rows,Cols);
	patchMatrix.reserve(patchesOnSweep);
	
	// compute the starting point of the mesh from the center point
	ComputeStartingPointIdFromCenterPointId();

}

template <typename TInputMesh, typename TOutputMesh>
void blSimplexTubeMeshCreator<TInputMesh,TOutputMesh>
::SetRadius(double r) 
{
	if (r > 0)
	{
		this->r = r;
	}
}

template <typename TInputMesh, typename TOutputMesh>
void blSimplexTubeMeshCreator<TInputMesh,TOutputMesh>
::SetStart(unsigned int start) 
{
	if (start > 0)
	{
		this->start = start;
	}
}

template <typename TInputMesh, typename TOutputMesh>
void blSimplexTubeMeshCreator<TInputMesh,TOutputMesh>
::SetCentralPoint(unsigned int centralPoint) 
{
	if (centralPoint > 0)
	{
		this->centralPoint = centralPoint;
	}
}

template <typename TInputMesh, typename TOutputMesh>
void blSimplexTubeMeshCreator<TInputMesh,TOutputMesh>
::SetTubeLength(double tubeLength ) 
{
	if (tubeLength > 0)
	{
		this->tubeLength = tubeLength;
	}
}

template <typename TInputMesh, typename TOutputMesh>
void blSimplexTubeMeshCreator<TInputMesh,TOutputMesh>
::SetPatchesOnSweep(unsigned long patchesOnSweep ) 
{
	this->patchesOnSweep = patchesOnSweep;
}

template <typename TInputMesh, typename TOutputMesh>
void blSimplexTubeMeshCreator<TInputMesh,TOutputMesh>
::SetUnits(double units ) 
{
	this->units = units;
}

template <typename TInputMesh, typename TOutputMesh>
void blSimplexTubeMeshCreator<TInputMesh,TOutputMesh>
::SetFinalRadius(double finalRadius ) 
{
	this->finalRadius = finalRadius;
}

template <typename TInputMesh, typename TOutputMesh>
void blSimplexTubeMeshCreator<TInputMesh,TOutputMesh>
::SetRadialYoungModulus(double radialYoungModulus ) 
{
	this->radialYoungModulus = radialYoungModulus;
}

template <typename TInputMesh, typename TOutputMesh>
void blSimplexTubeMeshCreator<TInputMesh,TOutputMesh>
::SetPatchesOnLength(unsigned long patchesOnLength ) 
{
	this->patchesOnLength = patchesOnLength;
}

template <typename TInputMesh, typename TOutputMesh>
int blSimplexTubeMeshCreator<TInputMesh,TOutputMesh>
::PreprocessCenterline()
{
	InputPointsContainerConstPointer      polylinePoints = this->GetInput()->GetPoints();
	InputPointsContainerConstIterator     polylineIterator = polylinePoints->Begin();
	unsigned int i = 0;

	double tmpDist = 0.0;
	double minDist = 0.0;
	// maximum minimum distance from the centerline points to the surface
	this->maxDist = 0.0;
	double x[3], closestPoint[3];
	vtkIdType cellId;
    int subId;
	bool firstIter = true;

	// if the surface associated to the centerline is available, 
	// -> compute the min distrance to the surface from the centerline
	if (this->surf) {
		this->locator = vtkCellLocator::New();
		this->locator->SetNumberOfCellsPerBucket(4);
		this->locator->SetDataSet(this->surf);
		this->locator->BuildLocator();
	}

	while ((i<(this->start)) && (polylineIterator!=polylinePoints->End()))
	{
		i++;
		polylineIterator++;
	}
	if (polylineIterator==polylinePoints->End())
	{
		std::cout << " Start point does not exist!!, exiting without generating data." << std::endl;
		return -1;
	}
	InputPointType actPoint,antPoint;
	vnl_vector<double> diff;
	double centerlineLength = 0;
	antPoint = polylineIterator.Value();
	polylineIterator++;

	double meshlength;
	meshlength = dx * this->Cols; // Still needs to add the first segment and the last segment.
	//////////////////////////////////////////////////
	while (polylineIterator!=polylinePoints->End()) {
		actPoint = polylineIterator.Value();
		polylineIterator++;
		diff = antPoint.Get_vnl_vector() - actPoint.Get_vnl_vector();
		if (polylineIterator!=polylinePoints->End()){
			centerlineLength += diff.magnitude();
		}
		// begin computing min distance to surface
		if (this->surf) {
			x[0] = actPoint[0];
			x[1] = actPoint[1];
			x[2] = actPoint[2];
			this->locator->FindClosestPoint(x, closestPoint, cellId, subId, tmpDist);
			tmpDist = sqrt(tmpDist);
			if (firstIter || tmpDist < minDist) {
				firstIter = false;
				minDist = tmpDist;
			}
			if (tmpDist > this->maxDist) {
				this->maxDist = tmpDist;
			}
		}// end compute min distance to surface

		// if we reached rhe end of the centerline
		if (centerlineLength > meshlength){
			// break.
			break;
		}
		antPoint = actPoint;
	}
	//////////////////////////////////////////////////

	if ((centerlineLength) < (meshlength)){
		std::cout << "Simplex mesh too long for the centerline provided. Centerline length : " << (centerlineLength) << ", mesh length : " << meshlength << std::endl;
		return -1;
	}

	if (this->surf){
		// initial radius should not be more 99% of the minimal distance to the surface.
		this->r = minDist * 0.99;
		this->locator->Delete();
	}
	//std::cout << " Minimum tube radius : " << this->r << std::endl;

	return 0;
}

template <typename TInputMesh, typename TOutputMesh>
void blSimplexTubeMeshCreator<TInputMesh,TOutputMesh>
::ComputeStartingPointIdFromCenterPointId()
{
	// we are half way from the starting point
	double distToStart = tubeLength/2;
	bool finished = false;
	//std::cout << "Start point before: " << this->start << std::endl;
	//std::cout << "Center point before: " << this->centralPoint << std::endl;

	//Recover the points and start walking through them until reaching the stent central point
	InputPointsContainerConstPointer polylinePoints = this->GetInput()->GetPoints();
	InputPointsContainerConstIterator polylineIterator = polylinePoints->Begin();
	vtkIdType i = 0;
	while (i<this->centralPoint && polylineIterator!=polylinePoints->End()){
		i++;
		polylineIterator++;
	}
	if (polylineIterator==polylinePoints->End()){
		this->start = 0;
	}else{
		InputPointType ptCurr, ptAnt;
		vnl_vector<double> aux(3);
		ptCurr = polylineIterator.Value();
		for (; !finished && i>0; polylineIterator--,i--){
			// got to the beginning of the centerline or walked enough over the centerline
			if (polylineIterator==polylinePoints->Begin() || distToStart <= 0){
				finished = true;
			// still there is more distance to be walked
			} else {
				ptAnt = ptCurr;
				ptCurr = polylineIterator.Value();
				aux[0] = ptCurr[0] - ptAnt[0]; aux[1] = ptCurr[1] - ptAnt[1]; aux[2] = ptCurr[2] - ptAnt[2];
				distToStart -= aux.two_norm();
			}
		}
		this->start = i;
	}
	//std::cout << "Start point after: " << this->start << std::endl;
}

template <typename TInputMesh, typename TOutputMesh>
int blSimplexTubeMeshCreator<TInputMesh,TOutputMesh>
::CreateSimplexPoints()
{
	/* At this point:
	- compute number of points on the sweep
	- compute number of points on the length
	- create point index matrix.
	- generate points
	*/

	unsigned long pointId = 0;
	unsigned long row, col; // I use these two to walk throw the matrix while creating the points 
	InputPointType actPoint, antPoint, nextPoint;
	OutputPointType newPoint, pointTmp;
	vnl_vector<double> pAct(3), pAnt(3), pNext(3);
	vnl_vector<double> dTmp1(3), dTmp2(3), vAux1(3), vAux2(3), v1(3), v2(3), v3(3);
	double a, rAnt, rAct;
	unsigned int segSteps = 0;

	// Re-sample the centerline? (To Be Implemented)
	// Verify that the centerline is long enough for the tube I'm trying to create
	if (PreprocessCenterline()<0){
		std::cout << "Simplex mesh points could not be created." << std::endl;
		return -1;
	}
    
	// I will start from the (0,0) position
	row = 0; col = 0;
	
	// Distance between the two consecutive points in the polyline being analyzed
	double pDist;
	
	// Percentage of the path between two consecutive points already walked
	double perc;
	double lengthDone = 0;

	// I will use auxiliary lists to store points at the start and at the end of the segment
	typedef std::list<OutputPointType> PointList;
	typedef typename PointList::iterator PointListIterator;
	PointList ptList,ptListAnt;
	PointListIterator ptListIt,ptListAntIt;

	//Recover the points and start walking through them until reaching the starting point
	InputPointsContainerConstPointer polylinePoints = this->GetInput()->GetPoints();
	InputPointsContainerConstIterator polylineIterator = polylinePoints->Begin();
	unsigned int i = 0;
	bool firstIter = true;
	while (i<(start))
	{
		i++;
		polylineIterator++;
	}
	
	// initialize the locator
	// it will be used to compute the local radius of each ring
	if (this->surf) {
		this->locator = vtkCellLocator::New();
		this->locator->SetNumberOfCellsPerBucket(4);
		this->locator->SetDataSet(this->surf);
		this->locator->BuildLocator();
	}
	// local auxiliary variables for the locator
	double closestPoint[3];
	vtkIdType cellId;
	int subId;

	// I should verify that there is more than one point in the polyline
	// Stop conditions: (walked all the tube and filled all the matrix)
	while( lengthDone<tubeLength  && col < Cols && 
	(polylineIterator!=polylinePoints->End())) 
	{
		double patchDistCovered = 0;
		bool patchCovered = false;
		while(!patchCovered){
			// In the first iteration just get the first point and advance
			if (firstIter)
			{
				antPoint = polylineIterator.Value();
				polylineIterator++;
				firstIter = false;
			}
			else
			{
				antPoint = actPoint;
			}
			actPoint = polylineIterator.Value();

			// Get direction at actual point, both backward and forward points need to exist
			polylineIterator++;
			if (polylineIterator!=polylinePoints->End()){
				nextPoint = polylineIterator.Value();
			}
			else
			{
				// If I'm standing at the last oint of the centerline, add a new one pased on 
				// the last and the one before
				nextPoint[0] = actPoint[0] +  (actPoint[0] - antPoint[0]);
				nextPoint[1] = actPoint[1] +  (actPoint[1] - antPoint[1]);
				nextPoint[2] = actPoint[2] +  (actPoint[2] - antPoint[2]);
			}

			//std::cout << " antPoint   : [" << antPoint[0] << ", "   << antPoint[1]   << ", " << antPoint[2]  << "]"<< std::endl;
			//std::cout << " actPoint   : [" << actPoint[0] << ", "   << actPoint[1]   << ", " << actPoint[2]  << "]"<< std::endl;
			//std::cout << " nextPoint   : [" << nextPoint[0] << ", "   << nextPoint[1]   << ", " << nextPoint[2]  << "]"<< std::endl;

			pAnt[0] = antPoint[0]; pAnt[1] = antPoint[1]; pAnt[2] = antPoint[2]; 
			pAct[0] = actPoint[0]; pAct[1] = actPoint[1]; pAct[2] = actPoint[2]; 
			pNext[0] = nextPoint[0]; pNext[1] = nextPoint[1]; pNext[2] = nextPoint[2]; 

			//std::cout << " --- > New segment < --- "<< std::endl;
			//std::cout << " pAnt   : [" << pAnt[0] << ", "   << pAnt[1]   << ", " << pAnt[2]  << "]"<< std::endl;
			//std::cout << " pAct   : [" << pAct[0] << ", "   << pAct[1]   << ", " << pAct[2]  << "]"<< std::endl;
			//std::cout << " pNext   : [" << pNext[0] << ", "   << pNext[1]   << ", " << pNext[2]  << "]"<< std::endl;

			// Compute backward and forward direction
			dTmp1[0] = pAct[0]-pAnt[0]; dTmp1[1] = pAct[1]-pAnt[1]; dTmp1[2] = pAct[2]-pAnt[2];
			dTmp2[0] = pNext[0]-pAct[0]; dTmp2[1] = pNext[1]-pAct[1]; dTmp2[2] = pNext[2]-pAct[2];
			//std::cout << " dTmp1   : [" << dTmp1[0] << ", "   << dTmp1[1]   << ", " << dTmp1[2]  << "]"<< std::endl;
			//std::cout << " dTmp2   : [" << dTmp2[0] << ", "   << dTmp2[1]   << ", " << dTmp2[2]  << "]"<< std::endl;

			// Compute the length of the segment
			pDist = dTmp1.magnitude();
			patchDistCovered +=pDist;

			// Compute the size of the step in between points
			// A problem might arise here, we should be careful for 
			// lines in the polyline that are shorter than dx
			if (dx < patchDistCovered) {
				patchCovered = true;
				segSteps = (unsigned int)(patchDistCovered/dx);
			}
			if (pDist <= 0.0){
				std::cout << "End of the centerline reached, not enough space for the specified mesh." << std::endl;
				return -1;
			}
		}// end While (!patchCovered)
		
		if (polylineIterator == polylinePoints->End())
		{
			std::cout << "End of the centerline reached, not enough space for the specified mesh." << std::endl;
				return -1;
		}
		
		// In the first iteration, generate the anterior point sweep
		if (ptListAnt.empty())
		{
			// In the first node I use as tangencial direction to the centerline the the one computed from the 
			// actual and anterior point
			v1[0] = dTmp1[0];
			v1[1] = dTmp1[1];
			v1[2] = dTmp1[2];

			vAux1[0] = 0; vAux1[1] = 1; vAux1[2] = 0;
			vAux2[0] = 0; vAux2[1] = 0; vAux2[2] = 1;
			v1.normalize();
			// If the dot product is 1 or -1, the vectors are co-linear
			if ((dot_product(v1,vAux1) == 1) || (dot_product(v1,vAux1) == -1)) {
				// Choose another vector
				vAux1[0] = 1; vAux1[1] = 0; vAux1[2] = 0;
			}else if ((dot_product(v1,vAux2) == 1) || (dot_product(v1,vAux2) == -1)) {
				// Choose another vector
				vAux2[0] = 1; vAux2[1] = 0; vAux2[2] = 0;
			}
			
			// Remove the comºponent in the vAux1 and vAux2 directions
			a = dot_product(vAux1,v1);
			v2[0] = vAux1[0] - a * v1[0];
			v2[1] = vAux1[1] - a * v1[1];
			v2[2] = vAux1[2] - a * v1[2];

			v2.normalize();
			vnl_cross_product_matrix cross(v2);
			v3 = cross * v1;
			v3.normalize();

			//std::cout << " v1   : [" << v1[0] << ", "   << v1[1]   << ", " << v1[2]  << "]"<< std::endl;
			//std::cout << " v2   : [" << v2[0] << ", "   << v2[1]   << ", " << v2[2]  << "]"<< std::endl;
			//std::cout << " v3   : [" << v3[0] << ", "   << v3[1]   << ", " << v3[2]  << "]"<< std::endl;

			//std::cout << " v1 . v2 : " << dot_product(v1,v2) << std::endl;
			//std::cout << " v2 . v3 : " << dot_product(v2,v3) << std::endl;
			//std::cout << " v3 . v1 : " << dot_product(v3,v1) << std::endl;

			rAnt =  this->r;
			if (this->surf) {
				this->locator->FindClosestPoint(pAnt.data_block(), closestPoint, cellId, subId, rAnt);
				rAnt = sqrt(rAnt);
				rAnt *=0.95;
			}

			for (row = 0; row<Rows ;row++) {
				double theta = dTheta * row;
				double s,c;
				s = sin(theta + this->angleShift);
				c = cos(theta + this->angleShift);
				pointTmp[0] = pAnt[0] + rAnt * v2[0] * s + rAnt * v3[0] * c;
				pointTmp[1] = pAnt[1] + rAnt * v2[1] * s + rAnt * v3[1] * c;
				pointTmp[2] = pAnt[2] + rAnt * v2[2] * s + rAnt * v3[2] * c;
				//std::cout << " pointTmp   : [" << pointTmp[0] << ", "   <<pointTmp[1]   << ", " <<pointTmp[2]  << "]"<< std::endl;
				//Insert its Id in the point list for posterior use
				ptListAnt.insert(ptListAnt.end(),pointTmp);
			}
		}

		// Compute average direction at the actual node
		v1[0] = (dTmp1[0] + dTmp2[0])/2;
		v1[1] = (dTmp1[1] + dTmp2[1])/2;
		v1[2] = (dTmp1[2] + dTmp2[2])/2;
		//std::cout << " v1   : [" << v1[0] << ", "   << v1[1]   << ", " << v1[2]  << "]"<< std::endl;

		// Normalize the vector
		v1.normalize();
		
		// Compute v2 using the old v2
		vAux1[0] = v2[0]; vAux1[1] = v2[1]; vAux1[2] = v2[2];
		vAux2[0] = v3[0]; vAux2[1] = v3[1]; vAux2[2] = v3[2];

		// Remove the component in the vAux1 and vAux2 directions
		a = dot_product(vAux1,v1);
		v2[0] = vAux1[0] - a * v1[0];
		v2[1] = vAux1[1] - a * v1[1];
		v2[2] = vAux1[2] - a * v1[2];

		v2.normalize();
		vnl_cross_product_matrix cross(v2);
		v3 = cross * v1;

		//std::cout << " v1   : [" << v1[0] << ", "   << v1[1]   << ", " << v1[2]  << "]"<< std::endl;
		//std::cout << " v2   : [" << v2[0] << ", "   << v2[1]   << ", " << v2[2]  << "]"<< std::endl;
		//std::cout << " v3   : [" << v3[0] << ", "   << v3[1]   << ", " << v3[2]  << "]"<< std::endl;

		//std::cout << " v1 . v2 : " << dot_product(v1,v2) << std::endl;
		//std::cout << " v2 . v3 : " << dot_product(v2,v3) << std::endl;
		//std::cout << " v3 . v1 : " << dot_product(v3,v1) << std::endl;

		// It should be normalized, just for precaution
		v3.normalize();

		// compute the local minimum radius and use it for the creatin of the ring.
		rAct =  this->r;
		// use this to compute tha variable initial radius
		if (this->surf) {
			this->locator->FindClosestPoint(pAct.data_block(), closestPoint, cellId, subId, rAct);
			rAct = sqrt(rAct);
			rAct *=0.95;
		}

		ptList.clear();
		unsigned int row;
		for (row = 0; row<Rows ;row++) {
			double theta = dTheta * row;
			double s,c;
			s = sin(theta + this->angleShift);
			c = cos(theta + this->angleShift);
			pointTmp[0] = pAct[0] + rAct * v2[0] * s + rAct * v3[0] * c;
			pointTmp[1] = pAct[1] + rAct * v2[1] * s + rAct * v3[1] * c;
			pointTmp[2] = pAct[2] + rAct * v2[2] * s + rAct * v3[2] * c;
			//std::cout << " pointTmp   : [" << pointTmp[0] << ", "   <<pointTmp[1]   << ", " <<pointTmp[2]  << "]"<< std::endl;
			//Insert its Id in the point list for posterior use
			ptList.insert(ptList.end(),pointTmp);
		}

		OutputPointType avgPoint;
		blSimplexMeshGeometry *data;
		// In the first iteration only creates the first point list
		if (!ptListAnt.empty()) {
			//Sweep and generate all the points
			for (double step=0;step<segSteps;step++) {
				// Compute the percentage of the segment that I've advanced
				perc = (step)/segSteps;
				row = 0;
				avgPoint[0] = 0; avgPoint[1] = 0; avgPoint[2] = 0;
				for (ptListAntIt = ptListAnt.begin(),ptListIt = ptList.begin();ptListAntIt!=ptListAnt.end();ptListAntIt++,ptListIt++) {
					OutputPointType pTmpAnt;
					OutputPointType pTmpAct;
					pTmpAnt[0] = (*ptListAntIt)[0]; pTmpAnt[1] = (*ptListAntIt)[1]; pTmpAnt[2] = (*ptListAntIt)[2]; 
					pTmpAct[0] = (*ptListIt)[0]; pTmpAct[1] = (*ptListIt)[1]; pTmpAct[2] = (*ptListIt)[2]; 
					newPoint[0] = (1-perc) * pTmpAnt[0] + (perc) * pTmpAct[0];
					newPoint[1] = (1-perc) * pTmpAnt[1] + (perc) * pTmpAct[1];
					newPoint[2] = (1-perc) * pTmpAnt[2] + (perc) * pTmpAct[2];
					avgPoint[0] += newPoint[0];
					avgPoint[1] += newPoint[1];
					avgPoint[2] += newPoint[2];
					// This is done to properly handle the boundaries. With this, I do not create
					// boundary points that I will not use. IMPORTANT: this can only be done 
					// because the mesh is very regular on the creation.
					if (!((((col==(Cols-1)) || ((col==0))) && ((row%2)==1)))){
						// Insert the point in the point list
						this->GetOutput()->SetPoint( pointId, newPoint);
						data = new blSimplexMeshGeometry();
						data->pos[0] = newPoint[0];
						data->pos[1] = newPoint[1];
						data->pos[2] = newPoint[2];
						if (this->surf) {
							//data->localFinalRadius = ((1-perc) * rAnt + (perc) * rAct) *  this->finalRadius;
							data->localFinalRadius = this->maxDist * this->finalRadius;
						} else {
							//data->localFinalRadius = this->r * this->finalRadius;
							data->localFinalRadius = this->maxDist * this->finalRadius;
						}
						// compute the idealized area per point
						double cylinderArea = (2 * base::PI * (data->localFinalRadius) * this->tubeLength);
						data->associatedArea = cylinderArea / (this->GetOutput(0)->GetGeometryData()->Size());

						data->trajectory.push_back(data->pos);
						this->GetOutput()->SetGeometryData( pointId,  data);
						// Save the Id of the created point in index matrix.
						//std::cout << " Added point with ID : " <<pointId  << "."<< std::endl;
						pointIndexMatrix->put(row,col,pointId);
						pointId++;
					}else{
						pointIndexMatrix->put(row,col,0);
					}
					//std::cout << " Setting : (Rows,Cols) -> (" << Rows << ", "   << Cols   << ") , (row, col) ->  (" << row << ", "   <<col   << ") to -> " <<pointId  << "."<< std::endl;
					//						  pointId++;
					row++;			
					// And advance one step
				}
				avgPoint[0] /= Rows;
				avgPoint[1] /= Rows;
				avgPoint[2] /= Rows;
				//std::cout << " avgPoint   : [" << avgPoint[0] << ", "   <<avgPoint[1]   << ", " <<avgPoint[2]  << "]"<< std::endl;
				lengthDone+=dx;
				//std::cout << " lengthDone   : " << lengthDone << ", tubeLength : "   << stentLength  << std::endl;
				col++;
				if (col>=Cols) {
					break;
				}
				// all the columns required where generated
			}
		}
		ptListAnt.clear();
		// copy the current min radius to the anterior one
		rAnt = rAct;
		// Copy the new list into the old one and start a new iteration
		for (ptListIt = ptList.begin();ptListIt!=ptList.end();ptListIt++)
		{
			ptListAnt.insert(ptListAnt.begin(),*ptListIt);
		}
		ptListAnt.reverse();
		ptList.clear();
	}
	return 0;
}

template <typename TInputMesh, typename TOutputMesh>
void blSimplexTubeMeshCreator<TInputMesh, TOutputMesh>
::CreateSimplexCells()
{
	double lengthCovered = 0;
	double areaPerPoint, cylinderArea;

	patchMatrix.reserve(patchesOnSweep);
	patchMatrix.resize(patchesOnSweep);
	for (unsigned int i=0; i<patchesOnSweep; i++){
		patchMatrix[i].reserve(patchesOnLength);
		patchMatrix[i].resize(patchesOnLength);
		for (unsigned int j=0; j<patchesOnLength; j++){
			patchMatrix[i].push_back(NULL);
		}
	}

	//this->GetOutput()->SetReferenceCurvatureRadius(stentGeom->GetReferenceStentCurvature());
	//cylinderArea = (2 * base::PI * stentGeom->GetFinalRadius() * simplexTubeGenerator->GetTubeLength());
	cylinderArea = (2 * base::PI * this->r * this->tubeLength);
	areaPerPoint = cylinderArea / (this->GetOutput(0)->GetGeometryData()->Size());
	this->GetOutput()->SetAreaPerPoint( areaPerPoint );
	this->GetOutput()->SetFinalRadius( this->finalRadius );
	this->GetOutput()->SetRadialYoungModulus( this->radialYoungModulus );
	this->GetOutput()->SetRadialStiffnessFromYoungModulusAndUnits(this->radialYoungModulus, this->units);
	//std::cout << "this->r : " << this->r  << std::endl;
	//std::cout << "this->tubeLength : " << this->tubeLength  << std::endl;
	//std::cout << "cylinderArea : " << cylinderArea  << std::endl;
	//std::cout << "this->GetOutput(0)->GetGeometryData()->Size() : " << this->GetOutput(0)->GetGeometryData()->Size()  << std::endl;
	//std::cout << "areaPerPoint : " << areaPerPoint  << std::endl;

	//// compute the elasticity modulus
	//double RadialYoungModulus;
	//RadialYoungModulus = (this->r * 1.5)/(areaPerPoint * abs((this->r*1.5) - this->r));
	//std::cout << "Tube RadialYoungModulus : " << RadialYoungModulus  << std::endl;

	//Initialize the sections' information in all the mesh while creating it.
	OutputCrossSectionMatrixType *crossSections;
	crossSections = new OutputCrossSectionMatrixType(Rows,Cols);
	for (unsigned long auxj = 0;auxj<this->Cols;auxj++){
		for (unsigned long auxi = 0;auxi<this->Rows;auxi++){
			crossSections->put(auxi,auxj,pointIndexMatrix->get(auxi,auxj));
		}
	}
	this->GetOutput()->SetCrossSectionMatrix(crossSections);
	delete crossSections;

	// generate all the patches
	PatchHelperPointer patch;
	for (unsigned int j=0; j<patchesOnLength; j++)
	{
		unsigned int i;
		for (i=0; i<patchesOnSweep; i++)
		{
			patch = PatchHelperType::New();
			// This could be improved, move everything to the cell
			patch->SetPatchHeightStart(2*i);
			patch->SetPatchWidthStart(2*j);
			patch->SetPointIndexMatrix(pointIndexMatrix);

			patch->SetMesh(this->GetOutput());
			if (j==0){
				patch->SetIsFirstPatch(true);
			}
			if (j==patchesOnLength-1){
				patch->SetIsLastPatch(true);
			}
			patch->GenerateData();
			patchMatrix[i][j] = patch;
		}
	}

	// Go over all the patches and 
	for (unsigned int j = 0; j<patchesOnLength; j++)
	{
		unsigned int i;
		unsigned int i0, i1;

		for (i=0; i<patchesOnSweep; i++)
		{
			// the first iteration I have to create the patch using points the first 
			// and last row.
			if (i==0){
				i1 = 0;
				i0 = patchesOnSweep-1;
			}else{
				i1 = i;
				i0 = i-1;
			}
			PatchHelperPointer actPatch;
			PatchHelperPointer antPatch;
			actPatch = patchMatrix[i1][j];
			antPatch = patchMatrix[i0][j];

			PointIndexListType actPatchIds;
			PointIndexListType antPatchIds;
			actPatchIds = actPatch->GetInternalIds();
			antPatchIds = antPatch->GetInternalIds();

			blSimplexMeshGeometry *auxPointGeom;
			NewSimplexCellPointer.TakeOwnership( new OutputPolygonType );
			NewSimplexCellPointer->SetPointId( 0, antPatchIds[4]);
			NewSimplexCellPointer->SetPointId( 1, antPatchIds[5]);
			NewSimplexCellPointer->SetPointId( 2, actPatchIds[4]);
			NewSimplexCellPointer->SetPointId( 3, actPatchIds[7]);
			NewSimplexCellPointer->SetPointId( 4, antPatchIds[8]);
			NewSimplexCellPointer->SetPointId( 5, antPatchIds[7]);

			unsigned long faceIndex = this->GetOutput()->AddFace( NewSimplexCellPointer );

		    auxPointGeom = this->GetOutput()->GetGeometryData(antPatchIds[4]);
			auxPointGeom->neighborCells.push_back(faceIndex);

		    auxPointGeom = this->GetOutput()->GetGeometryData(antPatchIds[5]);
			auxPointGeom->neighborCells.push_back(faceIndex);

		    auxPointGeom = this->GetOutput()->GetGeometryData(actPatchIds[4]);
			auxPointGeom->neighborCells.push_back(faceIndex);

		    auxPointGeom = this->GetOutput()->GetGeometryData(actPatchIds[7]);
			auxPointGeom->neighborCells.push_back(faceIndex);

		    auxPointGeom = this->GetOutput()->GetGeometryData(antPatchIds[8]);
			auxPointGeom->neighborCells.push_back(faceIndex);

		    auxPointGeom = this->GetOutput()->GetGeometryData(antPatchIds[7]);
			auxPointGeom->neighborCells.push_back(faceIndex);
		}

		//// How much of the stent length I have covered?
		lengthCovered += patchLength;
	}
}

/* PrintSelf. */
template <typename TInputMesh, typename TOutputMesh>
void
blSimplexTubeMeshCreator<TInputMesh, TOutputMesh>
::PrintSelf(std::ostream& os, Indent indent) const
{
	Superclass::PrintSelf(os,indent);
	os << indent << "Mesh generation starts at point "<< start <<" of the centerline";    
	os << indent << "Tube length = "<< tubeLength;    
	os << indent << "Patches on sweep = "<< patchesOnSweep;    
	os << indent << "Patches on length = "<< patchesOnLength;    
	os << indent << "Initial tube radius = "<< r <<" of the centerline";    
}

#endif // BLSIMPLEXTUBEMESHCREATOR_TXX

