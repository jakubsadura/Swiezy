// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef coreDRRarams_h
#define coreDRRParams_h

#include <string>

/**
* \brief Parameters for DRR visualizaton processor
* \ingroup MeshEditorPlugin
* \author  Chiara Riccobene
* \date 28 08 2009
*/

namespace Core{

class PLUGIN_EXPORT DRRParams
{
  public:

	// Spacing of output image
	float spacing[2];
	// Dimension of output image
	unsigned int size[2];
	// Distance of ray source (focal point)
	double distance; //sid
	double distance2; //sod
	//Translation parameter of the camera
	double translation[3];
	// Rotation around x,y,z axis in degrees
	double rx;
	double ry;
	double rz;
	//The 2D projection normal position 
	double normal[2] ;
	//  The centre of rotation relative to centre of volume
	float center[3];
	// range min, range max (for mview lib)
	int rangeMin;
	int rangeMax;
	// threshold
	double threshold;
	//Flip image axis
	int flipAxis[2];

	double angle1;
	double angle2;

	DRRParams()
	{
		spacing[0] = spacing [1] = 1; //mm
		size[0] = size[1]  = 512;
		distance =  1195. ; // 400 itk
		distance2 = 810. ;
		translation[0] = translation[1] = translation[2] = 0;
		rx = ry = rz = 0.0;
		normal[0] = normal[1] = 0;
		center[0] = center[1] = center[2] =0;
		rangeMin = 0;
		rangeMax =255;
		threshold =0;
		flipAxis[0] =  0;
		flipAxis[1] = 1;
		angle1 = 0;
		angle2 = 90;
    }
};

typedef boost::shared_ptr<DRRParams> DRRParamsPtr;

} // namespace Core
#endif // coreDRRParams_h
