#ifndef __meVTKPolygon_h
#define __meVTKPolygon_h

#include "vtkCell.h"
#include "vtkPolyData.h"

class vtkDoubleArray;
class vtkLine;
class vtkPoints;
class vtkQuad;
class vtkTriangle;

class meVTKPolygon : public  vtkCell
{
public:
  
  static meVTKPolygon *New();
  int GetCellType() {return VTK_POLYGON;};
  int GetCellDimension() {return 2;};
  int GetNumberOfEdges() {return this->GetNumberOfPoints();};
  int GetNumberOfFaces() {return 0;};
  vtkCell *GetEdge(int edgeId);
  vtkCell *GetFace(int) {return 0;};
  
  static inline double Max(double x, double y){return (x> y) ? x:y; };

  int CellBoundary(int subId, double pcoords[3], vtkIdList *pts);
  void Contour(double value, vtkDataArray *cellScalars,
               vtkIncrementalPointLocator *locator,vtkCellArray *verts,
               vtkCellArray *lines, vtkCellArray *polys,
               vtkPointData *inPd, vtkPointData *outPd,
               vtkCellData *inCd, vtkIdType cellId, vtkCellData *outCd);
  void Clip(double value, vtkDataArray *cellScalars,
            vtkIncrementalPointLocator *locator, vtkCellArray *tris,
            vtkPointData *inPd, vtkPointData *outPd,
            vtkCellData *inCd, vtkIdType cellId, vtkCellData *outCd,
            int insideOut);
  int EvaluatePosition(double x[3], double* closestPoint,
                       int& subId, double pcoords[3],
                       double& dist2, double *weights);
  void EvaluateLocation(int& subId, double pcoords[3], double x[3],
                        double *weights);
  int IntersectWithLine(double p1[3], double p2[3], double tol, double& t,
                        double x[3], double pcoords[3], int& subId);
  int Triangulate(int index, vtkIdList *ptIds, vtkPoints *pts);
  virtual void InterpolateFunctions(double pcoords[3], double *sf);
  virtual void InterpolateDerivs(double pcoords[3], double *derivs);
  void Derivatives(int subId, double pcoords[3], double *values,
                   int dim, double *derivs);
  int IsPrimaryCell() {return 0;}
  static void ComputeNormal(vtkPoints *p, int numPts, vtkIdType *pts,
                            double n[3]);
  static void ComputeNormal(vtkPoints *p, double n[3]);
  static void ComputeNormal(int numPts, double *pts, double n[3]);
  VTK_LEGACY(void ComputeWeights(double x[3], double *weights));
  int ParameterizePolygon(double p0[3], double p10[3], double &l10,
                          double p20[3], double &l20, double n[3]);
  static int PointInPolygon(double x[3], int numPts, double *pts,
                            double bounds[6], double n[3]);
  static int IntersectPolygonWithPolygon(int npts, double *pts, double bounds[6],
                                         int npts2, double *pts2,
                                         double bounds2[3], double tol,
                                         double x[3]);
  int Triangulate(vtkIdList *outTris);
  
  int TriangulatePaper(vtkIdList *outTris, vtkPolyData *surrounding);
protected:
    meVTKPolygon();
    ~meVTKPolygon();

  double   Tolerance; // Intersection tolerance
  int      SuccessfulTriangulation; // Stops recursive tri. if necessary
  double   Normal[3]; //polygon normal
  vtkIdList *Tris;
  vtkTriangle *Triangle;
  vtkQuad *Quad;
  vtkDoubleArray *TriScalars;
  vtkLine *Line;

  
  // Helper methods for triangulation------------------------------
  // Description:
  // A fast triangulation method. Uses recursive divide and
  // conquer based on plane splitting  to reduce loop into triangles.
  // The cell (e.g., triangle) is presumed properly initialized (i.e.,
  // Points and PointIds).
 int Triangulation(vtkPolyData *surrounding);
 int EarCutTriangulation();

private:
  meVTKPolygon(const meVTKPolygon&);  // Not implemented.
  void operator=(const meVTKPolygon&);  // Not implemented.
};

#endif

