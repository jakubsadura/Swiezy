/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blMatlabWriter_h
#define _blMatlabWriter_h

#include <fstream>				// for blMat5FileWriter class
#include "blLightObject.h"		// for SmartPointers in blMat5FileWriter
#include "blMacro.h"			// for SmartPointers in blMat5FileWriter
#include "blSmartPointer.h"		// for SmartPointers in blMat5FileWriter

#include <vnl/vnl_matlab_filewrite.h>
#include "BaseLibWin32Header.h"

typedef vnl_matlab_filewrite vnlMatlabWriter;
typedef vnl_matrix<double> vnlMatrixDouble;
typedef vnl_vector<double> vnlVectorDouble;



/**
 * \brief This implements output to Matlab binary files
 * \ingroup blUtilities
 *
 *  Implements Open, Write and Close
 *
 * based on vnl_matlab_write.h
 */
class BASELIB_EXPORT blMatlabWriter 
{
public:
	static vnlMatlabWriter *openMatlabFile(const char *filename);

	static int writeToMatlabFile(vnlMatlabWriter * myWriter, 
									vnlVectorDouble theVector, 
									const char *name);
	
	static int writeToMatlabFile(vnlMatlabWriter * myWriter, 
									vnlMatrixDouble theMatrix, 
									const char *name);

	static int closeMatlabFile(vnlMatlabWriter *myWriter);
};

// #endif
// #ifndef __mat5filewriter_h
// #define __mat5filewriter_h
// #include <fstream.h>

// #define DEBUG_MATLAB5_NWRITES
#define SHORT_MATLAB5_NWRITES
#define ENABLE_MAT5WRITER_EXAMPLES

// fsukno 20040421
/**
 *	This class writes multidimensional arrays to Matlab files
 *      according to Version 5 MAT-File specifications
 */
// ___________________________________________________
// ---------------------------------------------------
class blMat5FileWriter:public blLightObject
// ___________________________________________________
// ---------------------------------------------------
{
        public:

		/* Smart pointers typedef for this class*/
		typedef blMat5FileWriter Self;
		typedef blSmartPointer<Self> Pointer;

		/* \brief Smart poitner class constructor*/
		static Pointer New( const char *fileName )  
		{ 
			Pointer smartPtr; 
			Self* rawPtr = new Self( fileName ); 
			smartPtr = rawPtr; 
			rawPtr->UnRegister(); 
			return smartPtr; 
		};

      
        /** \brief Returns current file status (opened = true) */
        bool IsOpened ()
        {
			return this->fileStream.is_open();
        };

		/** \brief Returns TRUE if there is an array currently opened
		 * This means that no other array can be opened for this file,
		 * since current one is not completely filled yet
		*/
		bool IsArrayOpened ()
		{
			return this->arrayOpened;
		};

        /** \brief Creates a double (multidimensional) array*/
        int CreateDoubleArray (const int numberOfDimensions,
                const long *dimensionSizes, const char *name);

        /** \breif Stores a muldidimensional array at once*/
        int StoreArray (const int numberOfDimensions,
                const long *dimensionSizes, const char *name,
                const double *data);

        /** \breif Adds a vector to the first dimension of a double array*/
        bool AppendVectorToArray (const double *data);

        /** \breif Adds ona element to the first dimension of a double array*/
        bool AppendToArray (const double data);

		/** \brief Stores a vnl matrix*/
		int StoreVnl (vnl_matrix<double> *data, const char *name);

// ---------------------------------------------------
        protected:
// ---------------------------------------------------
		/** \brief Constructor */
        blMat5FileWriter ( const char *fileName );

        /** \brief Descrutcor */
        virtual ~blMat5FileWriter ();


// ---------------------------------------------------
        private:
// ---------------------------------------------------
		/** \brief Copy constructor*/
		blMat5FileWriter (const blMat5FileWriter&); //purposely not implemented
		/** \brief = constructor*/
		void operator = (const blMat5FileWriter&);  //purposely not implemented			

		// Methods
        // ---------------------------------------------------
        /** \brief Writes file header*/
        void WriteHeader();

        /** \brief Creates array support variables*/
        void CreateArrayVariables (const long *dimensionSizes);

        /** \brief Free up array temporary memory*/
        void FreeArrayVariables();

        /** \brief Updates number of writes counter*/
        void UpdateWritten ( const long numberOfElements );

        // Members
        // ---------------------------------------------------
		std::ofstream fileStream;    // Output file handler

        bool arrayOpened;       // Flag for opened array
        long nextVectorSize;    // The next data vector size to append an array
        long currentNDims;      // The number of dimensions of the currently opened array
        long *currentDims;      // The dimensions of the currently opened array
        long *nWritten;         // The number of writes counter

        char *fName;            // The current file name
        char *empty8;           // 8 zero-characters to pad file
};

namespace blMat5FileTypes
{
	/**\brief Features IDs */
	enum MatFile_DataType
        {
                miINT8 = 1,     miUINT8,        miINT16,
                miUINT16,       miINT32,        miUINT32,
                miSINGLE,       miDOUBLE = 9,   miINT64 = 12,
                miUINT64,       miMATRIX
        };
}

#ifdef ENABLE_MAT5WRITER_EXAMPLES
	namespace blClassesTests
	{
		int blMat5FileWriter_Example ();
	}
#endif // ENABLE_MAT5WRITER_EXAMPLES

#endif // _blMatlabWriter_h

