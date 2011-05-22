/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blImageMask_txx
#define __blImageMask_txx

//#include "blImageMask.h"


template <class TPixel, unsigned int VImageDimension>
blImageMask<TPixel, VImageDimension>::blImageMask():
	_maskRegion(NULL) 
{
};

template <class TPixel, unsigned int VImageDimension>
blImageMask<TPixel, VImageDimension>::~blImageMask() 
{
	if (_maskRegion != NULL) delete(_maskRegion); 
};



template <class TPixel, unsigned int VImageDimension>
void blImageMask<TPixel, VImageDimension>::ZeroMask( bool zeroWholeMask ) 
{
	TPixel *buffer;
	int pixelTypeSize = sizeof(TPixel);

	buffer = itk::Image< TPixel,VImageDimension >::GetBufferPointer();

	if ( zeroWholeMask || (_maskRegion == NULL) ) //zero whole image
	{
		memset(buffer, 0, itk::Image< TPixel,VImageDimension >::GetBufferedRegion().GetNumberOfPixels() * pixelTypeSize );
	}
	else //zero each row separately
	{
		if( VImageDimension == 2 ) 
		{
			const SizeType& regionSize = _maskRegion->GetSize();
			const IndexType& index = _maskRegion->GetIndex();

			unsigned long regionRowLengthInBytes = regionSize[0] * pixelTypeSize;

			unsigned long rowLength = itk::Image< TPixel,VImageDimension >::GetBufferedRegion().GetSize(0);// * pixelTypeSize;

			unsigned long position = rowLength * index[1] + index[0];// * pixelTypeSize; //rowLength*currentRow+currentCol

			unsigned long numberOfRows = regionSize[1];

			int NumOPixels = itk::Image< TPixel,VImageDimension >::GetBufferedRegion().GetNumberOfPixels();

			buffer += position;

			for (int i = 0; i<numberOfRows; i++)
			{
				memset(buffer, 0, regionRowLengthInBytes);
				buffer += rowLength; //increment in pixelType not bytes !
			}
		}
		else assert(false); //other dimensions of mask are not supported yet
	}
}

#endif
