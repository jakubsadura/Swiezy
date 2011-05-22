#ifndef __meVTKTriangleFilterPaper_h
#define __meVTKTriangleFilterPaper_h


// VTK
#include <vtkType.h>
#include <vtkPolyData.h>

// ITK
#include <itkLightObject.h>


using namespace itk;
using namespace std;

class meVTKTriangleFilterPaper : public LightObject
{
public:
  
       	//-----------------------------------------------------
	// Typedefs
	//-----------------------------------------------------

	typedef meVTKTriangleFilterPaper Self;
	typedef SmartPointer< Self > Pointer;
  
	/// \brief Static constructor.
	itkFactorylessNewMacro( Self );
	
	/*static meVTKTriangleFilterPaper *New()
	  {return new meVTKTriangleFilterPaper;}*/
	void SetInput( vtkPolyData *inputMesh, vtkPolyData *inputPatch )
			{
				this->surrounding->DeepCopy( inputMesh );
				this->input->DeepCopy( inputPatch );
			}
	vtkPolyData* GetOutput()
			{ return this->output; }

	/// \brief Compute mesh filter.
	void Update();
	
protected:
	meVTKTriangleFilterPaper();
	~meVTKTriangleFilterPaper();
	
	vtkPolyData *surrounding, *input;
	vtkPolyData *output;
	
	// Usual data generation method
	//int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

	
	
		
private:
	meVTKTriangleFilterPaper(const meVTKTriangleFilterPaper&);  // Not implemented.
	void operator=(const meVTKTriangleFilterPaper&);  // Not implemented.
};

#endif
