/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef BLSIMPLEXTUBEMESH_TXX
#define BLSIMPLEXTUBEMESH_TXX

#include "blSimplexTubeMesh.h"

#include "itkObjectFactory.h"
#include "itkProcessObject.h"
#include "itkMapContainer.h"
#include <algorithm>

#include <vxl_version.h>
#if VXL_VERSION_DATE_FULL > 20040406
# include <vnl/vnl_cross.h>
# define itk_cross_3d vnl_cross_3d
#else
# define itk_cross_3d cross_3d
#endif

/**
 * A protected default constructor allows the New() routine to create an
 * instance of SimplexMesh. All the containers are initialized to empty
 * containers.
 */
template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::blSimplexTubeMesh() : m_LastCellId ( 0 ) 
{
  this->m_GeometryData = GeometryMapType::New();
  this->m_CrossSections = NULL;
  this->meanSimplexDisplacement.clear();
}

/**
 * Mesh Destructor takes care of releasing the memory of Cells
 * and CellBoundaries objects for which normal pointers are
 * stored.
 */
template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::~blSimplexTubeMesh()
{
  if ( m_CrossSections != NULL)
  {
	delete this->m_CrossSections;
  }

  typename GeometryMapType::Iterator dataIt = m_GeometryData->Begin();

  while (dataIt != m_GeometryData->End())
  {
	  delete dataIt.Value();
	  dataIt++;
  }


  
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
 ::SetCrossSectionMatrix(CrossSectionMatrixType *CrossSections)
{
	this->m_CrossSections = new CrossSectionMatrixType(CrossSections->rows(),CrossSections->cols());
	for (unsigned long auxj = 0;auxj<CrossSections->cols();auxj++){
		for (unsigned long auxi = 0;auxi<CrossSections->rows();auxi++){
			this->m_CrossSections->put(auxi,auxj,CrossSections->get(auxi,auxj));
		}
	}
	this->ComputePointToCenterInfo();
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::ComputePointToCenterInfo()
{
	//std::cout << "Entering ComputePointToCenterInfoEntering() simplex mesh object : " << (unsigned long) this << std::endl;
	this->m_PointToCenterMap = PointToCenterMapType::New();
	for (unsigned long auxj = 0; auxj<this->m_CrossSections->cols(); auxj++){
		for (unsigned long auxi = 0; auxi<this->m_CrossSections->rows(); auxi++){
			unsigned long pId = this->m_CrossSections->get(auxi,auxj); 
			if (pId || ((auxi==0) && (auxj==0))){
				this->m_PointToCenterMap->InsertElement(this->m_CrossSections->get(auxi,auxj),auxj);
				//std::cout << "Inserting pair : " << this->m_CrossSections->get(auxi,auxj) << " ," << auxj << std::endl;
			}
		}
	}
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
typename blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>::PointType
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::GetCorrespondingCrossSectionCenter(unsigned long idx)
{
	unsigned long center;
	if (!this->m_PointToCenterMap)
	{
		this->ComputePointToCenterInfo();
	}
	center = this->m_PointToCenterMap->GetElement(idx);
	return this->m_CrossSectionsCenters[center];
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::UpdateSectionCenters()
{
	//std::cout << "Entering UpdateSectionCenters() simplex mesh object : " << (unsigned long) this << std::endl;
	if (this->m_CrossSections !=NULL) {
		unsigned int nCenters = this->m_CrossSections->cols();
		if (!this->m_CrossSectionsCenters.empty()){
			this->m_CrossSectionsCenters.clear();
		}
		PointType aux, pt;
		unsigned long count;
		blSimplexMeshGeometry* data;
		for (unsigned long center = 0; center<nCenters; center++){
			count = 0;
			aux.Fill(0.0);
			pt.Fill(0.0);
			unsigned long nextId;
			//std::cout << " Center position before : "<< this->m_CrossSectionsCenters[center] << std::endl;
			for (unsigned long ptI = 0; ptI<this->m_CrossSections->rows();ptI++)
			{
				nextId = this->m_CrossSections->get(ptI,center);
				if (nextId || ((ptI==0) && (center==0))) {
					data = this->m_GeometryData->GetElement(nextId);
					pt[0] += data->pos[0];
					pt[1] += data->pos[1];
					pt[2] += data->pos[2];
					//std::cout << "ptI : " << ptI << ", center : " << center << "Id : "<< this->m_CrossSections->get(ptI,center) << ", pos : " << data->pos << std::endl;
					count++;
				}
			}
			pt[0] /= count;
			pt[1] /= count;
			pt[2] /= count;
			this->m_CrossSectionsCenters.push_back(pt);
			//std::cout << " Center position [" << center << "], after  : "<< this->m_CrossSectionsCenters[center] << std::endl;
		}
	}
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
double
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::GetCorrespondingCrossSectionAverageRadio(unsigned long idx) 
{
	unsigned long center;
	if (!this->m_PointToCenterMap)
	{
		this->ComputePointToCenterInfo();
	}
	center = this->m_PointToCenterMap->GetElement(idx);
	return this->m_CrossSectionAverageToCenter[center];
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::UpdateSectionRadius()
{
	if (this->m_CrossSections!=NULL) {
		//std::cout << "Entered UpdateSectionRadius()... " << std::endl;
		unsigned int nCenters = this->m_CrossSections->cols();
		if (!m_CrossSectionAverageToCenter.empty()){
			m_CrossSectionAverageToCenter.clear();
		}
		PointType aux, pt;
		VectorType diffToCenter;
		double dstAvg;
		this->UpdateSectionCenters();
		blSimplexMeshGeometry* data;
		for (unsigned long center = 1; center < nCenters-1; center++)
		{ 
			aux.Fill(0.0);
			dstAvg = 0.0;
			unsigned long count = 0;
 			pt = m_CrossSectionsCenters[center];
			for (unsigned long ptI = 0; ptI < this->m_CrossSections->rows()-1; ptI++)
			{
				data = m_GeometryData->GetElement(this->m_CrossSections->get(ptI,center));
				diffToCenter[0] = pt[0] - data->pos[0];
				diffToCenter[1] = pt[1] - data->pos[1];
				diffToCenter[2] = pt[2] - data->pos[2];
				dstAvg += diffToCenter.GetNorm();
				count++;
			}
			dstAvg /= count;
			if (center==1) {
				this->m_CrossSectionAverageToCenter.push_back(dstAvg);
			}
			this->m_CrossSectionAverageToCenter.push_back(dstAvg);
			//std::cout << " Section radius, center : " << center << ", radius : " << this->m_CrossSectionAverageToCenter[center] << std::endl;
		}
		this->m_CrossSectionAverageToCenter.push_back(dstAvg);
	}
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::CopyInformation(const DataObject *data)
{
  const Superclass *mesh;

  mesh = dynamic_cast<const Superclass*>(data);

  if (mesh)
    {
    this->m_MaximumNumberOfRegions = mesh->GetMaximumNumberOfRegions();
    }
  else
    {
    // Pointer could not be cast back down
    itkExceptionMacro(<< "itk::Mesh::CopyInformation() cannot cast "
                      << typeid(data).name() << " to "
                      << typeid(Superclass*).name() );
    }
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::SetBarycentricCoordinates(unsigned long idx, PointType value)
{
  //blSimplexTubeMeshGeometry* geometry = m_GeometryData->GetElement(idx);
  blSimplexMeshGeometry* geometry = m_GeometryData->GetElement(idx);
  geometry->eps = value;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
typename blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>::PointType
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::GetBarycentricCoordinates(unsigned long idx) const
{
  return m_GeometryData->GetElement(idx)->eps;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::SetReferenceMetrics(unsigned long idx, PointType value)
{
  blSimplexMeshGeometry* geometry = m_GeometryData->GetElement(idx);
  geometry->referenceMetrics = value;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
typename blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>::PointType
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::GetReferenceMetrics(unsigned long idx) const
{
  return m_GeometryData->GetElement(idx)->referenceMetrics;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::SetPhi(unsigned long idx, double value)
{
  blSimplexMeshGeometry* geometry = m_GeometryData->GetElement(idx);
  geometry->phi = value;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
double
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::GetPhi(unsigned long idx) const
{
  blSimplexMeshGeometry* geometry = m_GeometryData->GetElement(idx);
  PointType test;
  this->GetPoint(idx, &test);

  return m_GeometryData->GetElement(idx)->phi;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::SetMeanCurvature(unsigned long idx, double value)
{
  blSimplexMeshGeometry* geometry = m_GeometryData->GetElement(idx);
  geometry->meanCurvature = value;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
double 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::GetMeanCurvature(unsigned long idx) const
{
  return m_GeometryData->GetElement(idx)->meanCurvature;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::SetRadius(unsigned long idx, double value)
{
  blSimplexMeshGeometry* geometry = m_GeometryData->GetElement(idx);
  geometry->circleRadius = value;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
double
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::GetRadius(unsigned long idx) const
{
  return m_GeometryData->GetElement(idx)->circleRadius;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::SetDistance(unsigned long idx, double value)
{
  blSimplexMeshGeometry* geometry = m_GeometryData->GetElement(idx);
  geometry->distance = value;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
double
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::GetDistance(unsigned long idx) const
{
  return m_GeometryData->GetElement(idx)->distance;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
unsigned long 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::AddEdge(unsigned long startPointId, unsigned long endPointId)
{
	return 0;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
unsigned long 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::AddFace(CellAutoPointer &cellPointer)
{
  this->SetCell( m_LastCellId , cellPointer );
  m_LastCellId++;
  return m_LastCellId-1;      
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
unsigned long 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::ReplaceFace(unsigned long replaceIndex, CellAutoPointer &cellPointer)
{
  this->GetCells()->DeleteIndex( replaceIndex );
  this->SetCell( replaceIndex , cellPointer );
  this->SetCellData( replaceIndex , (PixelType) 1.0 );
  return replaceIndex;      
}

/* PrintSelf. */
template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::PrintSelf(std::ostream& os, Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "LastCellId = " << m_LastCellId << std::endl;

  typename Superclass::CellsContainerConstPointer cells = this->GetCells();
  typename Superclass::CellsContainerConstIterator cellIt = cells->Begin();

  os << indent << "Cells Point Ids:" << std::endl;
  while ( cellIt != cells->End() )
    {
    os << indent << "cell id: " << cellIt->Index() << ", point ids: ";
    CellType *nextCell = cellIt->Value();
    typename CellType::PointIdIterator pointIt = nextCell->PointIdsBegin() ;
    while (pointIt != nextCell->PointIdsEnd() ) { os << *pointIt++ << "-"; }
    os << std::endl;
    cellIt++;
    }

  PointsContainerConstIterator pointsIt = this->GetPoints()->Begin();
  os << indent << "Point locations:" << std::endl;
  while ( pointsIt != this->GetPoints()->End() )
    {
    os << indent << "pt index:" << pointsIt->Index() << " , coords: " << pointsIt->Value() << std::endl;
    pointsIt++;
    }

  GeometryMapPointer geometryMap = this->GetGeometryData();
  GeometryMapIterator pointDataIterator = geometryMap->Begin();
  GeometryMapIterator pointDataEnd = geometryMap->End();

  while (pointDataIterator != pointDataEnd)
    {
    blSimplexMeshGeometry* geometry = pointDataIterator->Value();
    os << indent << "Mesh Geometry Data for point:"<< pointDataIterator->Index() << std::endl;
    os << indent << "Direct Neighbors indices: " 
       << geometry->neighborIndices[0] << ", "
       << geometry->neighborIndices[1] << ", " 
       << geometry->neighborIndices[2] << std::endl;
    pointDataIterator++;
    }
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::SetRadialStiffnessFromYoungModulusAndUnits(double radialYoungModulus, double units)
{
	this->m_RadialYoungModulus = radialYoungModulus;
	this->m_RadialStiffness = (this->m_AreaPerPoint * (this->m_RadialYoungModulus/units)) / this->m_FinalRadius;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::SetGeometryData(unsigned long pointId, blSimplexMeshGeometry* geometryData)
{
  if (m_GeometryData->IndexExists(pointId))
    {
    delete m_GeometryData->GetElement(pointId);
    }
  m_GeometryData->InsertElement(pointId, geometryData);
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
blSimplexMeshGeometry* 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::GetGeometryData(unsigned long pointId)
{
  if (m_GeometryData->IndexExists(pointId))
  {
    return m_GeometryData->GetElement(pointId);
  }
  else
  {
	return NULL;
  }
}


template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
typename blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>::IndexArray 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::GetNeighbors(unsigned long idx) const
{
  return m_GeometryData->GetElement(idx)->neighborIndices;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
typename blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>::NeighborListType*  
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::GetNeighbors(unsigned long idx, unsigned int radius, NeighborListType* list ) const
{
  if (list == NULL)
    {
    list = new NeighborListType();
    IndexArray neighborArray = GetNeighbors(idx);
    list->push_back(neighborArray[0]);
    list->push_back(neighborArray[1]);
    list->push_back(neighborArray[2]);

    if(radius>0)
      {
      list = GetNeighbors(neighborArray[0], radius-1, list);
      list = GetNeighbors(neighborArray[1], radius-1, list);
      list = GetNeighbors(neighborArray[2], radius-1, list);
      }
    NeighborListType::iterator it = std::find( list->begin(),list->end(),idx );
    if (it != list->end()) list->erase(it);

    return list;
    }
  else 
    {
    IndexArray neighborArray = GetNeighbors(idx);

    NeighborListType::iterator foundIt1 = std::find( list->begin(),list->end(),neighborArray[0] );
    NeighborListType::iterator foundIt2 = std::find( list->begin(),list->end(),neighborArray[1] );
    NeighborListType::iterator foundIt3 = std::find( list->begin(),list->end(),neighborArray[2] );
    NeighborListType::iterator endIt = list->end();
    bool found1=false, found2=false, found3=false;

    if (foundIt1 != endIt) found1 =true;
    if (foundIt2 != endIt) found2 = true;
    if (foundIt3 != endIt) found3 = true;

    if (!found1) list->push_back(neighborArray[0]);
    if (!found2) list->push_back(neighborArray[1]);
    if (!found3) list->push_back(neighborArray[2]);

    if (radius == 0) 
      {
      return list;
      }
    else
      {
      list = GetNeighbors(neighborArray[0], radius-1, list);
      list = GetNeighbors(neighborArray[1], radius-1, list);
      list = GetNeighbors(neighborArray[2], radius-1, list);
      return list;
      }
    }
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::AddNeighbor(unsigned long pointIdx, unsigned long neighborIdx)
{
  blSimplexMeshGeometry* data = m_GeometryData->GetElement(pointIdx);

  for (int i = 0; i < 3; i++)
    {
    if (data->neighborIndices[i] == ((unsigned long)NumericTraits<unsigned long>::max() ))
      {
      data->neighborIndices[i] = neighborIdx;
      break;
      }
    }
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::ReplaceNeighbor(unsigned long pointIdx, unsigned long oldIdx,unsigned long newIdx)
{
  blSimplexMeshGeometry* data = m_GeometryData->GetElement(pointIdx);

  for (int i = 0; i < 3;i++)
    {
    if (data->neighborIndices[i] == oldIdx)
      {
      data->neighborIndices[i] = newIdx;
      }
    }
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::SwapNeighbors(unsigned long pointIdx, unsigned long firstIdx,unsigned long secondIdx)
{
  blSimplexMeshGeometry* data = m_GeometryData->GetElement(pointIdx);
  int i;
  int firstFound = -1;
  int secondFound = -1;

  for (i = 0; i < 3;i++)
    {
    if (data->neighborIndices[i] == firstIdx)
      {
      firstFound = i;
      }
    else if (data->neighborIndices[i] == secondIdx)
      {
      secondFound = i;
      }
    }
  if(firstFound == -1 || secondFound == -1)
    {
    itkExceptionMacro("first and second not found");
    }
  data->neighborIndices[firstFound] = secondIdx;
  data->neighborIndices[secondFound] = firstIdx;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
typename blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>::CovariantVectorType 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::ComputeNormal(unsigned long idx ) const
{
  PointType p,n1,n2,n3;

  IndexArray neighbors = this->GetNeighbors( idx );  
  this->GetPoint(idx,&p);
  this->GetPoint(neighbors[0],&n1);
  this->GetPoint(neighbors[1],&n2);
  this->GetPoint(neighbors[2],&n3);

  // Compute normals
  CovariantVectorType normal;
  normal.Fill(0.0);
  CovariantVectorType z;
  z.SetVnlVector( itk_cross_3d((n2-n1).GetVnlVector() , (n3-n1).GetVnlVector()) );
  z.Normalize();
  normal += z;

  return normal;
}

template <typename TPixelType, unsigned int VDimension, typename TMeshTraits>
void 
blSimplexTubeMesh<TPixelType, VDimension, TMeshTraits>
::DeepCopy(ConstPointer orig) 
{
	typename GeometryMapType::Iterator  dataIt = orig->m_GeometryData->Begin();
	blSimplexMeshGeometry* auxGeom;
	// Finally, add all the points to the output.
	while ( dataIt != orig->m_GeometryData->End() ) 
	{
		//auxGeom = dataIt.Value();
		this->GetPoints()->InsertElement( dataIt.Index(), ((blSimplexMeshGeometry*)dataIt.Value())->pos);
		dataIt++;
	}

	this->m_FinalRadius = orig->GetFinalRadius();

	// Copy the cell data to the new object from the original mesh
	CellsContainerConstPointer simplexCellContainer;
	CellsContainerConstIterator simplexCellIterator;
	CellAutoPointer auxCell;

	CellAutoPointer  NewSimplexCellPointer;

	simplexCellContainer = orig->GetCells();
	for (simplexCellIterator = simplexCellContainer->Begin();
		 simplexCellIterator != simplexCellContainer->End();
		 simplexCellIterator++){
		NewSimplexCellPointer.TakeOwnership( new PolygonType );
		unsigned long pointsInCell = simplexCellIterator->Value()->GetNumberOfPoints();
		const unsigned long *tp;
		tp = simplexCellIterator->Value()->GetPointIds();
		//std::cout << "recovered cell auxId: " << auxId << ", pointsInCell : " << pointsInCell << std::endl;
		for (unsigned int ii = 0;ii<pointsInCell;ii++) {
			NewSimplexCellPointer->SetPointId( ii, tp[ii] );
		}
		this->AddFace( NewSimplexCellPointer );
	}

	dataIt = orig->m_GeometryData->Begin();
	this->SetCrossSectionMatrix( orig->m_CrossSections );

	while (dataIt != orig->m_GeometryData->End()) {
		auxGeom = new blSimplexMeshGeometry();

		auxGeom->neighborIndices = dataIt.Value()->neighborIndices;
		auxGeom->neighbors = dataIt.Value()->neighbors;
		auxGeom->meanCurvature = dataIt.Value()->meanCurvature;
		auxGeom->pos = dataIt.Value()->pos;
		auxGeom->oldPos = dataIt.Value()->oldPos;
		auxGeom->eps = dataIt.Value()->eps;
		auxGeom->referenceMetrics = dataIt.Value()->referenceMetrics;
		auxGeom->normal = dataIt.Value()->normal;
		auxGeom->closestAttractor = dataIt.Value()->closestAttractor;
		auxGeom->closestAttractorIndex = dataIt.Value()->closestAttractorIndex;
		auxGeom->circleRadius = dataIt.Value()->circleRadius;
		auxGeom->circleCenter = dataIt.Value()->circleCenter;
		auxGeom->sphereRadius = dataIt.Value()->sphereRadius;
		auxGeom->distance = dataIt.Value()->distance;
		auxGeom->phi = dataIt.Value()->phi;
		auxGeom->multiplier = dataIt.Value()->multiplier;
		auxGeom->forceIndex = dataIt.Value()->forceIndex;
		auxGeom->isFixed = dataIt.Value()->isFixed;
		auxGeom->localFinalRadius = dataIt.Value()->localFinalRadius;

		blSimplexMeshGeometry::NeighborSetType::iterator it = dataIt.Value()->neighborSet.begin();
		while(it != dataIt.Value()->neighborSet.end()) {
			auxGeom->neighborSet.insert(*it);
			it++;
		}

		blSimplexMeshGeometry::TrajectoryType::iterator it2 = dataIt.Value()->trajectory.begin();
		while(it2 != dataIt.Value()->trajectory.end()) {
			auxGeom->trajectory.push_back(*it2);
			it2++;
		}

		blSimplexMeshGeometry::NeghborCellsArray::iterator it3 = dataIt.Value()->neighborCells.begin();
		int count = 0;
		while(it3 != dataIt.Value()->neighborCells.end()) {
			auxGeom->neighborCells.push_back(*it3);
			it3++;
			count++;
		}

		this->SetGeometryData( dataIt.Index(), auxGeom);
		dataIt++;
	}

	this->meanSimplexDisplacement = orig->GetMeanSimplexDisplacementList();
	this->m_LastCellId = orig->GetLastCellId();
	this->ComputePointToCenterInfo();
	this->UpdateSectionRadius();
	this->m_AreaPerPoint = orig->GetAreaPerPoint();
	this->m_RadialYoungModulus = orig->GetRadialYoungModulus();
	this->m_RadialStiffness = orig->GetRadialStiffness();
}
	//this->m_RadialYoungModulus = radialYoungModulus;
	//this->m_RadialStiffness = (this->m_AreaPerPoint * (this->m_RadialYoungModulus/units)) / this->m_FinalRadius;

#endif // BLSIMPLEXTUBEMESH_TXX

