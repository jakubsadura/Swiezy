/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blAAMVisualizer_h
#define __blAAMVisualizer_h

// own dependencies
//-----------------------------------------------
#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"

#include "blIAWarp2DFilter.h"
#include "blWarp2DReferenceFrame.h"

#include "blVector.h"

/** 
 * \note USE WITH MULTIBAND IMAGES
 *
 * \brief Class implements visualization of the AAM results
 * when you have texture and shapes and you need to render the texture 
 * patch on the image or render the shape or both.
 * \ingroup blImageUtilities
 *
 * TInImage should be the image type that was used to build a model and 
 * used with the matcher. 
 * Usually this would be bloating point image like blImage<bloat,2>
 * TOutImage is the output image type. This is a type of the image onto 
 * which everything will be rendered usually this would be 
 * blImage< unsigned char, 2> At least these are the values for parameters 
 * that were tested.
 * TShape is the type of shape, usually it would be blPDShape2D
 *
 * Mandatory functions to call before Update()
 * \code
 * SetOutputShape(  )
 * SetOutputTexture(  )
 * SetMeanShape( )
 * \endcode
 *
 * Other functions allow to choose to render shape, patch, how to 
 * transform texture to the correct dynamic range o the output. See below 
 * for function description
 *
 */ 

template <class TInImage, class TOutImage, class TShape>
class blAAMVisualizer : public blLightObject
{
public:
	typedef blAAMVisualizer Self;
	typedef blSmartPointer<Self> Pointer;

	typedef TInImage InputImageType;
	typedef TOutImage OutputImageType;
	typedef TShape ShapeType;
	typedef typename TInImage::SizeType ImageSizeType;

	typedef typename ShapeType::BoundingBoxType BoundingBoxType;
	typedef typename blIAWarp2DFilter<InputImageType>::MaskType MaskType;
	typedef blWarp2DReferenceFrame<InputImageType, ShapeType> ReferenceFrameType;

	typedef blVector<double> VectorNamespace;
	typedef typename blVector<double>::VectorType VectorType;
	typedef typename blVector<double>::Pointer VectorPointer;

	/** \brief Types of re sampling texture from TInImage::PixelType to 
	 * TOutImage::PixelType
	 * LinearStretch - linearly transforms range [texture min, texture 
	 *					max] to [ 0; output image max]. 
	 *						This would work correctly only if 
	 *						TOutImage::PixelType is unsigned. 
	 * 
	 * SampledStatistics - it is important that SetBackgroundImage() is 
	 *				called to et image. Then the texture 
	 * 				from under the specified shape is sampled, its mean 
	 *				and length(as that of the vector) is calculated.
	 * 				The rendered texture then re sampled to zero mean and 
	 *				unit length and then re sampled by: 
	 *					length * texture + mean;
	 */
	enum RenderType { LinearStretch, SampledStatistics };

	blNewMacro(Self);

	/** \brief Shape rendering blag */
	void SetWireframeRender( bool render )
	{
		_renderWireframe = render;
	};
	
	/** \brief Patch rendering blag */
	void SetPatchRender( bool render )
	{
		_renderPatch = render;
	};

	
	/** \brief Set the image on which to render.
	 * If called with NULL as the parameter, then the thing will be generated on empty image(default)
	 */
	void SetBackgroundImage( InputImageType* image )
	{
		if ( image!=NULL )
			_bkImage = image;
		else
			_bkImage.Delete();
	};
	

	/** \brief Set the shape for the rendered patch
	 * 
	 */
	void SetOutputShape( ShapeType* shape )
	{
		_outputShape = (ShapeType*)shape->GetCopy().GetPointer();
	};
	
	/** \brief Set the texture for the rendered patch
	 * 
	 */
	void SetOutputTexture( const VectorPointer texture )
	{
		_texture = VectorNamespace::New( texture->size() );
		_texture -> copy_in ( texture->begin() );
	};

	/** \brief Render!
	 * 	make sure the tesselation information is set. E.g. before calling this call something like
	 *    shape->SetTesselationCells( meanshape->GetTesselationCells() );
	 *    visualizer->SetOutputShape( shape );
	 *    visualizer->SetMeanShape( meanshape, size );
	 */
	void Update();
	void UpdateAndSave( const char* filename );

	/** \brief Returns the result */
	typename OutputImageType::Pointer GetOutput()
	{
		return _outputImage;
	};
	
	/** \brief Sets the mean shape and its size 
	 * These things are stored with the model. If the mean shape is already resized
	 * then 1 can be passed as 1 (I think this should work just fine)
	 */
	void SetMeanShape( ShapeType* shape, double shapeSize )
	{
		_meanShape = (ShapeType*) shape->GetCopy().GetPointer();
		_shapeMeanSize = shapeSize;
		_meanShapeChanged = true;
	};
	
	/** \brief Sets the moethod of texture rescaling, see the types above */
	void SetTextureRenderType( RenderType type )
	{
		_textureRenderType = type;
	}
	
	/** \brief Sets the color of the shape to be rendered. Default is the maximum value of the output pixel type.
	 * But be careful, the default value would work only for unsigned types, i.e. if TOutImage::PixelType is unsigned.
	 */
	void SetWireframeColor( typename OutputImageType::PixelType color )
	{
		_wireframeColor = color;
	};
	
	//can be ignored. Default is 1
	void SetMirroredEdge( int edge )
	{
		_mirroredEdge = edge;
		_meanShapeChanged = true; //referenceframe must be rebuilt
	}

protected:

	blAAMVisualizer();
	virtual ~blAAMVisualizer();

	void GenerateReferenceFrame();

private:

	typename TShape::Pointer _outputShape;
	typename TShape::Pointer _meanShape;

	VectorPointer _texture;

	typename InputImageType::Pointer _bkImage;
	typename OutputImageType::Pointer _outputImage;
	
	bool _renderWireframe;
	bool _renderPatch;
	bool _meanShapeChanged;

	RenderType _textureRenderType;

	int _mirroredEdge; //in pixels. Size of the mirrored edge along the object contour specified by the mask 
							 //in the _referenceImage for correct interpolation

	double _shapeMeanSize;

	typename OutputImageType::PixelType _wireframeColor;

	typename ReferenceFrameType::Pointer _referenceFrame;
	

	blAAMVisualizer(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented


};

#include "blAAMVisualizer.txx"

#endif


