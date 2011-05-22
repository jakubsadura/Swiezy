/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef BLSIMPLEXPATCHHELPER_TXX
#define BLSIMPLEXPATCHHELPER_TXX

#include "blSimplexPatchHelper.h"
#include "blSimplexMeshGeometry.h"

//--------------------------------------------------
template<typename TOutputMesh>
 blSimplexPatchHelper<TOutputMesh>
 ::blSimplexPatchHelper()
//--------------------------------------------------
{
    // Each patch introduces two points
	patchHeightPoints = 2;
	patchWidthPoints = 2;
	patchHeightStart = -1;
	patchWidthStart = -1;
	mesh = NULL;
	isLastPatch = false;
	isFirstPatch = false;
	internalIds.reserve(9); 
}

//--------------------------------------------------
template<typename TOutputMesh>
 blSimplexPatchHelper<TOutputMesh>
 ::~blSimplexPatchHelper()
//--------------------------------------------------
{
}

template<typename TOutputMesh>
 void blSimplexPatchHelper<TOutputMesh>
 ::Initialize() 
{
	internalIds.reserve(9); 
}

template<typename TOutputMesh>
 void blSimplexPatchHelper<TOutputMesh>
 ::SetPointIndexMatrix(PointIndexMatrixType *pointIndexMatrix)
{
	this->pointIndexMatrix = pointIndexMatrix;
	return;
}

template<typename TOutputMesh>
 void blSimplexPatchHelper<TOutputMesh>
 ::GenerateData(){
	 unsigned int pointHeigthIndex;
	 unsigned int pointWidthIndex;
	 // At this point I need to add something that stops the mesh from creating neighbor 
	 // with the first point
	 if (mesh) {
		 pointHeigthIndex = (patchHeightStart)%(pointIndexMatrix->rows());
		 pointWidthIndex = (patchWidthStart)%(pointIndexMatrix->cols());
		 internalIds.push_back(pointIndexMatrix->get(pointHeigthIndex,pointWidthIndex));
		 pointHeigthIndex = (patchHeightStart+1)%(pointIndexMatrix->rows());
		 pointWidthIndex = (patchWidthStart)%(pointIndexMatrix->cols());
		 internalIds.push_back(pointIndexMatrix->get(pointHeigthIndex,pointWidthIndex));
		 pointHeigthIndex = (patchHeightStart+2)%(pointIndexMatrix->rows());
		 pointWidthIndex = (patchWidthStart)%(pointIndexMatrix->cols());
		 internalIds.push_back(pointIndexMatrix->get(pointHeigthIndex,pointWidthIndex));
		 //
		 pointHeigthIndex = (patchHeightStart)%(pointIndexMatrix->rows());
		 pointWidthIndex = (patchWidthStart+1)%(pointIndexMatrix->cols());
		 internalIds.push_back(pointIndexMatrix->get(pointHeigthIndex,pointWidthIndex));
		 pointHeigthIndex = (patchHeightStart+1)%(pointIndexMatrix->rows());
		 pointWidthIndex = (patchWidthStart+1)%(pointIndexMatrix->cols());
		 internalIds.push_back(pointIndexMatrix->get(pointHeigthIndex,pointWidthIndex));
		 pointHeigthIndex = (patchHeightStart+2)%(pointIndexMatrix->rows());
		 pointWidthIndex = (patchWidthStart+1)%(pointIndexMatrix->cols());
		 internalIds.push_back(pointIndexMatrix->get(pointHeigthIndex,pointWidthIndex));
		 //
		 pointHeigthIndex = (patchHeightStart)%(pointIndexMatrix->rows());
		 pointWidthIndex = (patchWidthStart+2)%(pointIndexMatrix->cols());
		 internalIds.push_back(pointIndexMatrix->get(pointHeigthIndex,pointWidthIndex));
		 pointHeigthIndex = (patchHeightStart+1)%(pointIndexMatrix->rows());
		 pointWidthIndex = (patchWidthStart+2)%(pointIndexMatrix->cols());
		 internalIds.push_back(pointIndexMatrix->get(pointHeigthIndex,pointWidthIndex));
		 pointHeigthIndex = (patchHeightStart+2)%(pointIndexMatrix->rows());
		 pointWidthIndex = (patchWidthStart+2)%(pointIndexMatrix->cols());
		 internalIds.push_back(pointIndexMatrix->get(pointHeigthIndex,pointWidthIndex));

		 // The reason for this ordering in the way neighbors are created is that in this way, 
		 // the normal orientation is computed in the appropriate way. If a neighbor is added in 
		 // a different order, then the mesh normal at that mesh location will be computed with 
		 // the wrong direction (inward instead of outward).
		 mesh->AddNeighbor( internalIds[2] , internalIds[5]);
		 if (!isFirstPatch) {
			mesh->AddNeighbor( internalIds[1] , internalIds[2]);
			mesh->AddNeighbor( internalIds[1] , internalIds[0]);
			mesh->AddNeighbor( internalIds[2] , internalIds[1]);
			mesh->AddNeighbor( internalIds[0] , internalIds[1]);
		 }else{
			mesh->AddNeighbor( internalIds[2] , internalIds[0]);
			mesh->AddNeighbor( internalIds[0] , internalIds[2]);
		 }
		 if (!isLastPatch) {
			 mesh->AddNeighbor( internalIds[5] , internalIds[4]);
			 mesh->AddNeighbor( internalIds[4] , internalIds[5]);
			 mesh->AddNeighbor( internalIds[3] , internalIds[4]);
			 mesh->AddNeighbor( internalIds[4] , internalIds[7]);
 			 mesh->AddNeighbor( internalIds[4] , internalIds[3]);
			 mesh->AddNeighbor( internalIds[7] , internalIds[4]);
			 newSimplexCellPointer.TakeOwnership( new LineType );
			 newSimplexCellPointer->SetPointId(0, internalIds[4]);
			 newSimplexCellPointer->SetPointId(1, internalIds[7]);
		 }else{
			 mesh->AddNeighbor( internalIds[5] , internalIds[3]);
			 mesh->AddNeighbor( internalIds[3] , internalIds[5]);
		 }
		 mesh->AddNeighbor( internalIds[5] , internalIds[2]);

		 newSimplexCellPointer.TakeOwnership( new LineType );
		 newSimplexCellPointer->SetPointId(0, internalIds[0]);
		 newSimplexCellPointer->SetPointId(1, internalIds[1]);

		 newSimplexCellPointer.TakeOwnership( new LineType );
		 newSimplexCellPointer->SetPointId(0, internalIds[1]);
		 newSimplexCellPointer->SetPointId(1, internalIds[2]);

		 newSimplexCellPointer.TakeOwnership( new LineType );
		 newSimplexCellPointer->SetPointId(0, internalIds[3]);
		 newSimplexCellPointer->SetPointId(1, internalIds[4]);

		 newSimplexCellPointer.TakeOwnership( new LineType );
		 newSimplexCellPointer->SetPointId(0, internalIds[4]);
		 newSimplexCellPointer->SetPointId(1, internalIds[5]);

		 newSimplexCellPointer.TakeOwnership( new LineType );
		 newSimplexCellPointer->SetPointId(0, internalIds[0]);
		 newSimplexCellPointer->SetPointId(1, internalIds[3]);

		 newSimplexCellPointer.TakeOwnership( new LineType );
		 newSimplexCellPointer->SetPointId(0, internalIds[2]);
		 newSimplexCellPointer->SetPointId(1, internalIds[5]);

		 blSimplexMeshGeometry *auxPointGeom;
		 if (internalIds[0]!=0 && internalIds[1]!=0 && internalIds[2]!=0 &&
			 internalIds[3]!=0 && internalIds[4]!=0 && internalIds[5]!=0){
			newSimplexCellPointer.TakeOwnership( new OutputPolygonType );
			newSimplexCellPointer->SetPointId( 0, internalIds[0]);
			newSimplexCellPointer->SetPointId( 1, internalIds[1]);
			newSimplexCellPointer->SetPointId( 2, internalIds[2]);
			newSimplexCellPointer->SetPointId( 3, internalIds[5]);
			newSimplexCellPointer->SetPointId( 4, internalIds[4]);
			newSimplexCellPointer->SetPointId( 5, internalIds[3]);
			unsigned long faceIndex = mesh->AddFace( newSimplexCellPointer );

 			auxPointGeom = this->mesh->GetGeometryData(internalIds[0]);
			auxPointGeom->neighborCells.push_back(faceIndex);
			 
 			auxPointGeom = this->mesh->GetGeometryData(internalIds[1]);
			auxPointGeom->neighborCells.push_back(faceIndex);

 			auxPointGeom = this->mesh->GetGeometryData(internalIds[2]);
			auxPointGeom->neighborCells.push_back(faceIndex);

 			auxPointGeom = this->mesh->GetGeometryData(internalIds[5]);
			auxPointGeom->neighborCells.push_back(faceIndex);

 			auxPointGeom = this->mesh->GetGeometryData(internalIds[4]);
			auxPointGeom->neighborCells.push_back(faceIndex);

 			auxPointGeom = this->mesh->GetGeometryData(internalIds[3]);
			auxPointGeom->neighborCells.push_back(faceIndex);
			}
	 }else{
		 std::cout << " Output mesh object is empty!." << std::endl;
	 }
}

#endif // BLSIMPLEXPATCHHELPER_TXX

