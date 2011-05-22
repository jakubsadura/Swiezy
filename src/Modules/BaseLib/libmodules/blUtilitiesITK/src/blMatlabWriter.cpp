/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//----------------------------------------------------------------------------
/** \brief This implements output to Matlab binary files
 *
 *  Implements Open, Write and Close
 */


#include "blMatlabWriter.h"	// based on vnl_matlab_write.h
#include <iostream>			// for Mat5FileWriter class
#include <string>			// for Mat5FileWriter class
#include <cstring>
//#include "Mat5FileWriter.h"



int blMatlabWriter::writeToMatlabFile(vnlMatlabWriter *myWriter, 
										vnlVectorDouble theVector, const char *name)
{	
	myWriter->write(theVector, name);

	return 1;
}

int blMatlabWriter::writeToMatlabFile(vnlMatlabWriter *myWriter, 
									  vnlMatrixDouble theMatrix, const char *name)
{	
	myWriter->write(theMatrix, name);

	return 1;
}

vnlMatlabWriter *blMatlabWriter::openMatlabFile(const char *filename)
{
	vnl_matlab_filewrite *myWriter = 
				new vnl_matlab_filewrite(filename);
											//"./Files/matlabData.mat");
												//char const *    basename = 0);

	return myWriter;
}

int blMatlabWriter::closeMatlabFile(vnlMatlabWriter *myWriter)
{
	delete myWriter;

	return 1;
}


/**
 * Constructor: A filename must be specified since there's no
 * known use for the object but writing a MAT file
 */
// -------------------------------------------------
blMat5FileWriter::blMat5FileWriter ( const char *fileName )
// -------------------------------------------------
{
        this->fName = new char [strlen (fileName) + 1];
        strcpy (this->fName, fileName);
		
        this->fileStream.open (fName, std::ios::out|std::ios::binary );
		
		if (!this->fileStream.is_open())
		{
			std::cerr << "Could not create MAT file" << this->fName << "\n";
		}

        this->WriteHeader();
        this->arrayOpened = false;

        // Empty 8-bytes to pad to 8 bytes length
        this->empty8 = new char[8];
        for (int i = 0; i < 8; i++)
        {
                //*(empty8 + i) = 0;
				this->empty8[i] = 0;
        }
}

/**
 * Destructor
 */
// -------------------------------------------------
blMat5FileWriter::~blMat5FileWriter ()
// -------------------------------------------------
{
        this->fileStream.close();
        if ( this->arrayOpened )
        {
                // This section should never be reached since impies
                // closing the MAT file bevore filling it with the
                // required number of elements
                this->FreeArrayVariables();
                std::cerr << "blMat5FileWriter Warning:\n";
                std::cerr << "Closing incomplete MAT-File";
                std::cerr << this->fName << "\n";
        }
        delete[] this->empty8;
        delete[] this->fName;
}

/**
 * This function creates the array support variables and stores
 * the memory they need
 */
// -------------------------------------------------
void blMat5FileWriter::CreateArrayVariables (const long *dimensionSizes)
// -------------------------------------------------
{
        if (this->currentNDims < 2)
        {
                // 1-dimensional arrays of N elements are stored as
                // N * 1 matrices
                this->currentNDims = 2;
                this->currentDims = new long [2];
                this->nWritten = new long [2];

                this->currentDims[0] = dimensionSizes[0];
                this->currentDims[1] = 1;
                this->nWritten[0] = 0;
                this->nWritten[1] = 0;
        }
        else
        {
                this->currentDims = new long [this->currentNDims];
                this->nWritten = new long [this->currentNDims];
                for (int i = 0; i < this->currentNDims; i++)
                {
                        this->currentDims[i] = dimensionSizes[i];
                        this->nWritten[i] = 0;
                }
        }

        this->nextVectorSize = dimensionSizes [0];

		#ifdef SHORT_MATLAB5_NWRITES
			std::cout << "Creating Mat5File Double Array of (";
			for (int ii = 0; ii < this->currentNDims; ii++)
			{
				std::cout << this->currentDims[ii];
				if (ii + 1 < this->currentNDims)
				{
					std::cout << " x ";
				}
				else
				{
					std::cout << ")" << std::endl;
				}
			}
		#endif

}

/**
 * To free up some memory reserved when creating the temporary
 * variables needed to wort with an array
 */
// -------------------------------------------------
void blMat5FileWriter::FreeArrayVariables()
// -------------------------------------------------
{
                delete [] this->currentDims;
                delete [] this->nWritten;
}

/**
 * Writes File Header according to MAT-File Format Version 5
 */
// -------------------------------------------------
void blMat5FileWriter::WriteHeader()
// -------------------------------------------------
{
        // MAT file version id = 0x0100
        unsigned short version_id = 0x0100;
        // Endian indicator (characters MI
        // Using M = ASCII(77) ans I = ASCII(73)
        unsigned short endian_indicator = 77 * 256 + 73;

        // MAT-File Description. Length must be 124 bytes and
        // first 4-bytes must not be zero
        const char *descript = "SML Mat-File Version 5 Writer";
        char *descript_124;
        int text_size;

        text_size = strlen ( descript );
        descript_124 = new char [124];
        if (text_size > 124)
        {
                memcpy (descript_124, descript, 124);
        }
        else
        {
                memcpy (descript_124, descript, text_size);
                for (int i = text_size; i < 123; i++)
                {
                        //*(descript_124 + i) = 32;
						descript_124[i] = 32;
                }
                //*(descript_124 + 123) = 32;
				descript_124[123] = 32;
        }

        this->fileStream.write ( descript_124, 124 );
        this->fileStream.write ( (char*) &version_id, 2);
        this->fileStream.write ( (char*) &endian_indicator, 2);

		delete[] descript_124;
}

/**
 * Creates a new array for output. Only one array can be used at
 * a time since no buffer stores the data in memory (elemenst are
 * written to file as soon as they are available)
 * \return  0    on success,
 *         -1:   if already working with an array
 */
// -------------------------------------------------
int blMat5FileWriter::CreateDoubleArray (const int numberOfDimensions,
                const long *dimensionSizes, const char *name)
// -------------------------------------------------
{
        // Check to open a new array and create support variables
        // ------------------------------------------------------
        if ( this->arrayOpened )
        {
				std::cerr << "blMat5FileWriter ERROR: Can not create "
					<< "new array until filling the current one" 
					<< std::endl;
                return -1;
        }

        this->arrayOpened = true;
        this->currentNDims = numberOfDimensions;
        this->CreateArrayVariables (dimensionSizes);

        // Setup ovject header variables
        // ------------------------------------------------------
        // MATLAB array data type
        unsigned long objectDataType = blMat5FileTypes::miMATRIX;
        // Array data type: IEE 754 double format
        unsigned long arrayDataType = blMat5FileTypes::miDOUBLE;

        // Tag data type
        unsigned long tagDataType = blMat5FileTypes::miUINT32;
        // The size of the array tag (bytes)
        unsigned long tagSize = 8;
        // Array Flags
        unsigned long arrayFlags = 0x00000006;

        // Array dimensions data type
        unsigned long arrayDimDataType = blMat5FileTypes::miINT32;
        // Array dimensions
        unsigned long arrayDims = 4 * this->currentNDims;
        // ArrayName Data Type
        unsigned long nameDataType = blMat5FileTypes::miINT8;

        // Number of bytes in array and in the whole object field
        // ------------------------------------------------------
        unsigned long fieldNumberOfBytes;
        unsigned long arrayNumberOfBytes;
        unsigned long nameLength = strlen (name);
        unsigned long nameLength8 =
                8 * (1 + (long) ((nameLength - 1) / 8));
        unsigned long arrayDims8 =
                8 * (1 + (long) ((arrayDims - 1) / 8));

        arrayNumberOfBytes = 8;
        for (int i = 0; i < this->currentNDims; i++)
        {
                arrayNumberOfBytes *= this->currentDims[i];
        }
        fieldNumberOfBytes = 40 + arrayDims8 + nameLength8 +
                arrayNumberOfBytes;

        // Writing tags information
        // ------------------------
        this->fileStream.write ((char *) &objectDataType, 4);
        this->fileStream.write ((char *) &fieldNumberOfBytes, 4);
        this->fileStream.write ((char *) &tagDataType, 4);
        this->fileStream.write ((char *) &tagSize, 4);
        this->fileStream.write ((char *) &arrayFlags, 4);
        this->fileStream.write ( this->empty8, 4);

        // Writing Dimensions Information
        // ------------------------------
        this->fileStream.write ((char *) &arrayDimDataType, 4);
        this->fileStream.write ((char *) &arrayDims, 4);
        for (int i = 0; i < currentNDims; i++)
        {
                this->fileStream.write ((char *) &( this->currentDims[i] ), 4);
        }
        if (arrayDims8 != arrayDims)
        {
                this->fileStream.write ( this->empty8, 4);
        }

        // Writing Array Name and size info
        // --------------------------------
        this->fileStream.write ((char *) &nameDataType, 4);
        this->fileStream.write ((char *) &nameLength, 4);
        this->fileStream.write ( name, nameLength);
        if (nameLength8 != nameLength)
        {
			if (nameLength8 - nameLength > 8)
			{
				std::cerr << "ERROR while writing array name to matlab"
					<< std::endl;
			}
            this->fileStream.write ( this->empty8, nameLength8 - nameLength);
        }
        this->fileStream.write ((char *) &arrayDataType, 4);
        this->fileStream.write ((char *) &arrayNumberOfBytes, 4);

        return 0;
}


/**
 * This function fills a multidimensional array at once
 * using a vnl_matrix as input
 */
// -------------------------------------------------
int blMat5FileWriter::StoreVnl (vnl_matrix<double> *data,
								const char *name)
// -------------------------------------------------
{
	long *dimSizes = new long[2];
	dimSizes[0] = data->rows();
	dimSizes[1] = data->cols();

	if (this->blMat5FileWriter::CreateDoubleArray (2,
		dimSizes, name) == -1)
    {
		return -1;
    }

	double *tempVect = new double [this->currentDims[0]];
    for (int j = 0; j < this->currentDims[1]; j++)
	{
		for (int i = 0; i < this->currentDims[0]; i++)
		{
			tempVect[i] = data->get(i, j);			
		}
		this->AppendVectorToArray ( tempVect );
	}
	delete[] tempVect;
	delete[] dimSizes;
    
    return 0;
}


/**
 * This function fills a multidimensional array at once
 * The method used here is not the most efficient, since
 * data in memory will not be sorted the same way it is
 * in a MAT file (some kind of transposition, see
 * AppendToArray (const double *data) for details)
 *
 * \param *data a pointer to double multidimensional array
 * \return 0 on success, -1 if another array is already opened
 */
// -------------------------------------------------
int blMat5FileWriter::StoreArray (const int numberOfDimensions,
                const long *dimensionSizes, const char *name,
                const double *data)
// -------------------------------------------------
{
        if (this->blMat5FileWriter::CreateDoubleArray (
                numberOfDimensions, dimensionSizes, name) == -1)
        {
                return -1;
        }

        long maxDim = this->currentNDims - 1;

        // dimOffset[k] tells how many memory memory places to move
        // when we increment the k-dimension index of the matrix
        long *dimOffset = new long [this->currentNDims];
        dimOffset[maxDim] = 1;
        for (int k = maxDim - 1; k >= 0; k--)
        {
                dimOffset[k] = dimOffset[k + 1] * this->currentDims[k + 1];
        }

        // Data will not be written at once since its size could
        // fall out of int16
        char *data_ptr = (char *)data;
        for (long i = 0; i < 8 * dimOffset[0]; i++)
        {
                this->fileStream.write (data_ptr, this->currentDims[0]);
                data_ptr += currentDims[0];
        }
        this->arrayOpened = false;
        this->FreeArrayVariables();

        //long offset;
        //double *ptr;
        //ptr = data;
        //while ( this->arrayOpened )
        //{
                //offset = this->nWritten[ maxDim ];
                // this->nWritten[0] should be zero
                //for (int j = 0; j < maxDim; j++)
                //{
                //        offset += (this->nWritten[j]) * dimOffset[j];
                //}
                //for (int i = 0; i < this->currentDims[0]; i++)
                //{
                //        tempVect[i] = *(data + offset);
                //        offset += dimOffset[0];
                //}
                //this->AppendToArray ( tempVect );
        //}

		delete[] dimOffset;
        return 0;
}

/**
 * Adds one vector to the currently opened array
 * It's important to understand that Matlab orders the elements
 * in a MAT file from the last to the first dimension. That is,
 * it we have an array of dimensions (a, b, c), then we will first
 * fill the elements in (:, 1, 1), then (:, 2, 1), then, (:, 3, 1)
 * and so on. Or, the same, we start with (1, 1, 1), (2, 1, 1),
 * (3, 1, 1), ...
 * The elements must be passed to this function in an ordered way
 * since there's no memory buffer but, instead, they're directly
 * wrote to the MAT file
 *
 * WARINING: This function stands for a double array with the right
 * number of elements on it, and will not check for it.
 * You can get the current expected size with GetNextVectorSize()
 *
 * \param *data A pointer to a double array with the data to be written
 * \return FALSE if no array is opened (nothing written), TRUE otherwise
 *
 */
// -------------------------------------------------
bool blMat5FileWriter::AppendVectorToArray (const double *data)
// -------------------------------------------------
{
        if ( !this->arrayOpened )
        {
				std::cerr << "blMat5FileWriter ERROR: No array opened "
					<< "where to append data" << std::endl;
                return false;
        }

        this->fileStream.write ((char *) data, 8 * this->nextVectorSize);
        this->UpdateWritten ( this->nextVectorSize );
        return true;
}

/**
 * This is the single value version of AppendToArray
 * \param data is a single element to add to the array
 * (see AppendToArray (const double *data) for
 * important information).
 */
// -------------------------------------------------
bool blMat5FileWriter::AppendToArray (const double data)
// -------------------------------------------------
{
        if ( !this->arrayOpened )
        {
				std::cerr << "blMat5FileWriter ERROR: No array opened "
					<< "where to append data" << std::endl;
                return false;
        }

        this->fileStream.write ((char *) &data, 8 );
        this->UpdateWritten ( 1 );
        return true;
}

/**
 * Updates the number of writes counter
 * This counter initializes to zero when a new array is created and
 * srotes the number of elements written to each array dimension
 * When the array is filled with the number of elements it was
 * defined, this function CLOSES the array, which means that you
 * can start writing another one. The file, however, is not closed
 *
 * \param numberOfElements The number of elements added to the array
 */
// -------------------------------------------------
void blMat5FileWriter::UpdateWritten ( const long numberOfElements )
// -------------------------------------------------
{
        int nDim = 0;
        int accum = 0;

        this->nWritten[nDim] += numberOfElements;
        while ( this->nWritten[nDim] >= this->currentDims[nDim]
                && nDim < this->currentNDims )
        {
                this->nWritten[nDim] -= this->currentDims[nDim];
                accum++;
                if ( this->nWritten[nDim] < this->currentDims[nDim] )
                {
                        nDim++;
                        if (nDim >= this->currentNDims)
                        {
                                this->arrayOpened = false;
                        }
                        else
                        {
                                this->nWritten[nDim] += accum;
                                accum = 0;
                        }
                }
        }

        if ( !this->arrayOpened )
        {
                this->FreeArrayVariables();
        }

        #ifdef DEBUG_MATLAB5_NWRITES
        cout << "\n";
        for (int j = 0; j < this->currentNDims; j++)
        {
                cout << this->nWritten[j] << "\t";
        }
        #endif
}


#ifdef ENABLE_MAT5WRITER_EXAMPLES
// -------------------------------------------------
// #################################################
//      blMat5FileWriter CLASS TESTS AND EXAMPLES
// #################################################
// -------------------------------------------------
namespace blClassesTests
{

	int blMat5FileWriter_Example ()
	{
	// -----------------------------------------------------------
	// EXAMPLE 1 - prueba.mat, using AppendToArray( vector )
	//             prueba2.mat, using AppendToArray( element )
	//             prueba3.mat, using StoreArray ( full array )
	// Creating a 4 * 3 * 7 matrix, and filling it with
	// doubles starting in .1 and incrementing .1 each
	// time.
	// -----------------------------------------------------------
			long dimSizes[3];
			dimSizes[0] = 4;
			dimSizes[1] = 3;
			dimSizes[2] = 7;
			double vector[4];
			double *matrix3_ptr;
			double *matrix3 = new double [dimSizes[0] * dimSizes[1] *
					dimSizes[2]];

			blMat5FileWriter::Pointer M = 
				blMat5FileWriter::New ("prueba.mat");
			blMat5FileWriter::Pointer M2 = 
				blMat5FileWriter::New ("prueba2.mat");
			blMat5FileWriter::Pointer M3 = 
				blMat5FileWriter::New ("prueba3.mat");

			std::cout << "File Status = " << M->IsOpened();
			M->CreateDoubleArray (3, dimSizes, "Federico");
			M2->CreateDoubleArray (3, dimSizes, "Federico2");

			int ac = 0;

			// Notice the nesting starts with the last dimension
			// end finishes in the first one
			for (int i = 0; i < dimSizes[2]; i++)
			{ // iterate from 0 to 7
					for (int j = 0; j < dimSizes[1]; j++)
					{ // iterate from 0 to 3
							for (int k = 0; k < dimSizes[0]; k++)
							{ // iterate from 0 to 4
								vector[k] = (double) (++ac) / 10;
								// Write one element at a time
								M2->AppendToArray (vector[k]);
							}
							// Write first-dimension vectors at once
							M->AppendVectorToArray( vector );
					}
			}

			// fill M3
			ac = 0;
			matrix3_ptr = matrix3;
			for (int i = 0; i < dimSizes[0]; i++)
			{
					for (int j = 0; j < dimSizes[1]; j++)
					{
							for (int k = 0; k < dimSizes[2]; k++)
							{
									*(matrix3_ptr++) = (double) (++ac) / 10;
							}
					}
			}

			// Write full array at once
			M3->StoreArray (3, dimSizes, "Federico3", matrix3);
			M2->StoreArray (3, dimSizes, "array3", matrix3);
			delete[] matrix3;

	// -----------------------------------------------------------
	// EXAMPLE 2 - prueba1.mat, add "array1d" 120 elements vector and
	//             "array2d" 5 * 7 elemtns matrix
	// -----------------------------------------------------------
			double array2[532][200];
			dimSizes[0] = 532;
			dimSizes[1] = 200;
			M->CreateDoubleArray (2, dimSizes, "array2d");

			for (int i = 0; i < 200; i++)
			{
					for (int j = 0; j < 532; j++)
					{
							array2[j][i] = (double) (i + j);
							M->AppendToArray (double (i + j));
					}
			}

			double *array1 = new double [120];
			for (int i = 0; i < 120; i++)
			{
					*(array1 + i) = i + 1;
			}

			dimSizes[0] = 120;
			M->StoreArray (1, dimSizes, "array1d", array1);

			return 0;
	}
} // closing blClassesTests namespace
#endif // ENABLE_MAT5WRITER_EXAMPLES
