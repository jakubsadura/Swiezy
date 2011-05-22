/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <blIALinearSampler3D.h>
#include <blVTKHelperTools.h>

#include <vtkPolyData.h>
#include <vtkProbeFilter.h>
#include <vtkDataSet.h>
#include <vtkType.h>
#include <vtkPointData.h>


#include <vnl/vnl_vector.h>


/*-------------------------------------------



		The sampler



----------------------------------------------*/
blIALinearSampler3D::blIALinearSampler3D(): 
	m_Nsamples(21), 
	m_Spacing(1),
	m_NormalizeProfile(false),
	m_AdditionalSamples(1),
	m_SamplerPositions( NULL ),
	m_SamplerType( sampLinearRaw ),
	m_ProfileNormalization( normZeroMeanUnitVar )
{
	//----------debug----------
#ifdef ASM3D_SAVEPROFILELINES
	m_ProfileLines = vtkAppendPolyData::New();
#endif
	//----------debug----------
}
	
blIALinearSampler3D::~blIALinearSampler3D()
{
	if( m_SamplerPositions!=NULL ) m_SamplerPositions->Delete();


	//----------debug----------
#ifdef ASM3D_SAVEPROFILELINES
	m_ProfileLines->Delete();
#endif
	//----------debug----------
}


vtkPolyData* blIALinearSampler3D::GenerateSamplerPositions()
{
	//add additional samples on both sides
	const int nsamples = m_Nsamples + this->m_AdditionalSamples*2;

	vtkPolyData* sample_ptset; // = vtkPolyData::New();
	vtkPoints* sample_pts = vtkPoints::New();
	sample_pts->SetNumberOfPoints(nsamples);

	vnl_vector<double> pt(nsamples);

	for( int i=0; i<nsamples; i++)
	{
		pt = m_Point + m_Normal*m_Spacing*(i-floor((double)nsamples/2));
		sample_pts->SetPoint(i, pt.begin());
	}

	//sample_ptset->SetPoints( sample_pts );
	sample_ptset = blVTKHelperTools::Points2Polydata(sample_pts);

	sample_pts->Delete();

	//----------debug----------
#ifdef ASM3D_SAVEPROFILELINES
	//vtkLineSource* line = vtkLineSource::New();
	//line->SetPoint1( sample_pts->GetPoint(0) );
	//line->SetPoint2( sample_pts->GetPoint(nsamples-1) );

	//m_ProfileLines->AddInput( line->GetOutput() );
	m_ProfileLines->AddInput( blVTKHelperTools::Points2Polydata( sample_pts ) );
	//line->Delete();
#endif
	//----------debug----------

	return sample_ptset;
}


void blIALinearSampler3D::Sample( )
{
	if( m_SamplerPositions!=NULL ) m_SamplerPositions->Delete();
	m_SamplerPositions = GenerateSamplerPositions();

	vnl_vector<double> profile;

	switch( m_SamplerType )
	{
	case sampLinearDerivative:
	case sampLinearRaw:
		SampleLinearRaw( profile );
		break;
	default:
		std::cerr<<"Undefined sampling method"<<std::endl;
		throw "Undefined sampling method";
	}

	if( m_NormalizeProfile ) NormalizeProfile( profile );

	//differentiate
	vnl_vector<double> derivative;
	if( this->m_SamplerType == sampLinearDerivative )
		blVTKHelperTools::Diff3Points( profile, derivative );
	else 
		derivative = profile;

	//truncate the profile
	m_SampledProfile.set_size(this->m_Nsamples);
	m_SampledProfile.copy_in( &derivative[this->m_AdditionalSamples] );

}


void blIALinearSampler3D::NormalizeProfile( vnl_vector<double>& profile, ProfileNormalizationType method )
{
	double norm = 0;

	switch( method )
	{
	case normZeroMeanUnitVar: 
		{
			const double mean = profile.mean();
			profile = profile - mean;
			
			const double var = profile.squared_magnitude()/(profile.size()-1); //unbiased
			if( var==0 && profile[0]!=0 ) profile.normalize();
			else if( var!=0 ) profile /= sqrt(var);
		}
		break;
	case normZeroMeanL1:
		profile = profile - profile.mean();
		norm = profile.one_norm();
		if( norm!=0 ) profile /= profile.one_norm();
		break;
	case normL1:
		norm = profile.one_norm();
		if( norm!=0 ) profile /= profile.one_norm();
		break;
	case normNone:
		break;
	default: 
			std::cerr<<"Unknown profile normalization method"<<std::endl;
			throw "Unknown profile normalization method";
	}
}



void blIALinearSampler3D::SampleLinearRaw( vnl_vector<double>& profile )
{
	vtkProbeFilter* probe = vtkProbeFilter::New();

	probe->SetSource( m_Image );
	probe->SetInput(m_SamplerPositions);

	
	probe->Update();

	vtkDataSet* samples = probe->GetOutput();

	vtkIdType nsamples = samples->GetPointData()->GetScalars()->GetNumberOfTuples();
	
	profile.set_size(nsamples);

	for(int i=0; i<nsamples; i++ )
	{
		double *value = samples->GetPointData()->GetScalars()->GetTuple(i);
		profile[i] = value[0];
	}

	probe->Delete();
}








