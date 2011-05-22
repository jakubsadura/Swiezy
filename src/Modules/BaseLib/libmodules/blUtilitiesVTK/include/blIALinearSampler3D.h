/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef ___blIALinearSampler3D_h
#define ___blIALinearSampler3D_h

#include "BaseLibVTKWin32Header.h"

#include <vtkDataSet.h>
#include <vnl/vnl_vector.h>

#include <vtkPolyData.h>

#include <blLightObject.h>
#include <blSmartPointer.h>
#include <blMacro.h>

//#define ASM3D_SAVEPROFILELINES



#ifdef ASM3D_SAVEPROFILELINES
#include <vtkAppendPolyData.h>
#include "blShapeUtils.h"
#endif


/*-------------------------------------------



		The sampler



----------------------------------------------*/
/**
 * \brief Samples a 3D vtk image
 * \ingroup blUtilitiesVTK
 *
 * Typical use:
 * \code
 * SetImage()
 *   SetPoint()
 *	 SetNormal()
 *   SetNumberOfPositions()
 *   SetSpacing()
 *   Sample()
 *   GetSampledProfile()
 *
 *	 NormalizeProfile() is also implemented as static member and can be 
 *		used separately from the class
 * \endcode
 *
 */
class BASELIBVTK_EXPORT blIALinearSampler3D: public blLightObject
{
public:
	typedef blIALinearSampler3D Self;
	typedef blSmartPointer<Self> Pointer;


	typedef enum { sampLinearRaw, sampLinearDerivative } SamplerType; 
	typedef enum { normZeroMeanUnitVar, normL1, normZeroMeanL1, normNone } ProfileNormalizationType; 

	blNewMacro(Self);


	void SetImage( vtkDataSet* image )
	{
		m_Image = image;
	}
	
	vtkDataSet* GetImage()
	{
		return m_Image;
	}

	void SetPoint( vnl_vector<double>& point )
	{
		m_Point = point;
	}

	vnl_vector<double> GetPoint( ) const
	{
		return m_Point;
	}


	void SetNormal( vnl_vector<double>& normal )
	{
		m_Normal = normal;
	}

	vnl_vector<double> GetNormal( ) const
	{
		return m_Normal;
	}


	void SetNumberOfPositions(int nsamples)
	{
		m_Nsamples = nsamples;
	}

	int GetNumberOfPositions() const
	{
		return m_Nsamples;
	}


	void SetSpacing(double spacing)
	{
		m_Spacing = spacing;
	}
	
	double GetSpacing() const
	{
		return m_Spacing;
	}

	void SetProfileNormalizationMethod( ProfileNormalizationType method )
	{
		m_ProfileNormalization = method;
	}

	ProfileNormalizationType GetProfileNormalizationMethod( ) const
	{
		return m_ProfileNormalization;
	}

	void SetNormalizeProfile( bool normalize )
	{
		m_NormalizeProfile = normalize;
	}

	bool GetNormalizeProfile() const
	{
		return m_NormalizeProfile;
	}

	vnl_vector<double> GetSampledProfile()
	{
		vnl_vector<double> result;
		GetSampledProfile(result);
		return result;
	}

	void GetSampledProfile( vnl_vector<double>& profile )
	{
		profile = m_SampledProfile;
	}

	//returns sampled positions - point coordinates
	double* GetSampledPointCoordinates( vtkIdType point )
	{
		return m_SamplerPositions->GetPoint( point + m_AdditionalSamples );
	}

	virtual void Sample();


	void NormalizeProfile( vnl_vector<double>& profile )
	{
		NormalizeProfile( profile, m_ProfileNormalization ); 
	}

	static void NormalizeProfile( vnl_vector<double>& profile, ProfileNormalizationType method ); //not the same as normalize sample profile (candidate profiles can be subsets of sampled ones)

#ifdef ASM3D_SAVEPROFILELINES
	void SaveProfileLines( const char* filename )
	{
		m_ProfileLines->Modified();
		blShapeUtils::ShapeUtils::SaveShapeToFile( m_ProfileLines->GetOutput(), filename );
	}

	void ResetProfileLineStorage( )
	{
		m_ProfileLines->RemoveAllInputs();
	}
#endif


	SamplerType GetSamplerType() const 
	{
		return m_SamplerType;
	}

	void SetSamplerType( SamplerType sampler_type )
	{
		m_SamplerType = sampler_type;
	}


	//reserved for future use
	void SetSamplerSpecificParameters( void *params )
	{
		/*switch( m_SamplerType )
		{
		}*/
	}

	//reserved for future use
	void GetSamplerSpecificParameters( void *params )
	{
		/*switch( m_SamplerType )
		{
		}*/
	}

	vtkPolyData* GetSamplerPositions() { return m_SamplerPositions; }


protected:
	blIALinearSampler3D();
	virtual ~blIALinearSampler3D();


	//additional_pts is the number of additional elements to sample from each side
	vtkPolyData* GenerateSamplerPositions();

	void SampleLinearRaw( vnl_vector<double>& profile );


private:
	blIALinearSampler3D(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented


	vtkDataSet* m_Image;
	vnl_vector<double> m_Point;
	vnl_vector<double> m_Normal;
	int m_Nsamples;
	double m_Spacing;
	bool m_NormalizeProfile;

	int m_AdditionalSamples; //additioanl samples on each side of the profile (for the derivative)

	vnl_vector<double> m_SampledProfile;
	vtkPolyData* m_SamplerPositions;
	SamplerType m_SamplerType;
	ProfileNormalizationType m_ProfileNormalization;

#ifdef ASM3D_SAVEPROFILELINES
	vtkAppendPolyData* m_ProfileLines;
#endif
};






#endif

