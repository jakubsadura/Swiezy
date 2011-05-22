/*=========================================================================

  Module    : BaseLib
  File      : $RCSfile: blMatlabReader.cpp,v $
  Copyright : (C)opyright (University Pompeu Fabra) 2004++
              See COPYRIGHT statement in top level directory.
  Authors   : agonzalez
  Modified  : $Author: vdialinos $
  Purpose   : This class defines tokenizer for strings.
  Date      : $Date: 2007-04-19 12:53:06 $
  Version   : $Revision: 1.2 $
  Changes   : $Locker:  $
              $Log: not supported by cvs2svn $
              Revision 1.1  2007/04/18 14:18:36  vdialinos
              *** empty log message ***

              Revision 1.2  2007/04/18 11:57:08  xmellado
              Character error fixed.

              Revision 1.1  2007/03/29 13:47:16  vdialinos
              New Base lib version

              Revision 1.1  2004/12/03 20:17:31  cvsadmin
              initial import in BaseLib

               
              Revision 0.3  2004/04/28 15:42:22  cbutakoff
              Fixed Resize problem (related to ITK migration)

              Revision 0.2  2004/04/12 15:10:24  agonzalez
              Added some error-management functionality to support SCATI-interface

              Revision 0.1  2004/02/23 15:44:36  agonzalez
              First Upload.

              
=========================================================================*/
//----------------------------------------------------------------------------
/** \brief This implements reading from Matlab binary files
 *
 *  Implements Open, Write and Close
 */


#include "blMatlabReader.h"



//----------------------------------------------------------------------------
int blMatlabReader::closeMatlabFile(MATFile *theMatFile)
//----------------------------------------------------------------------------
{
	if (matClose(theMatFile) != 0) 
	{
		std::cerr << "blMatlabReader: Error closing mat-file " << std::endl;
		return -1;
	}
	return 0;
}



//----------------------------------------------------------------------------
MATFile *blMatlabReader::openMatlabFile(const char *filename)
//----------------------------------------------------------------------------
{
	MATFile *theMatFile = matOpen(filename, "r");

	if (theMatFile == NULL) 
	{
		std::cerr << "blMatlabReader: Error opening mat-file: " << filename << std::endl;
		return NULL;
	}
	
	return theMatFile;
}


//----------------------------------------------------------------------------
vnl_vector<double> blMatlabReader::readVectorFromMatlabFile(const char *filename,
						const char *varname,
						int &error)
//----------------------------------------------------------------------------
{
	char **dir;
	const char *name;

	int ndir;
	int i;

	mxArray *auxArray;
	MATFile *theMatFile;

	vnl_vector<double> theVector;	// returned data
	
	theMatFile = blMatlabReader::openMatlabFile(filename);

	// Get directory of MAT-file
	dir = matGetDir(theMatFile, &ndir);
	
	if (dir == NULL) 
	{
		std::cerr << "blMatlabReader: Error reading directory of mat-file" << std::endl;
		error = -1;
		return theVector;
	} 
	
	mxFree(dir);
	
	// In order to use matGetNextXXX correctly, reopen file to read in headers.
	blMatlabReader::closeMatlabFile(theMatFile);
	theMatFile = blMatlabReader::openMatlabFile(filename);

	for (i = 0; i < ndir; i++) 
	{
		auxArray = matGetNextVariable(theMatFile, &name);
		
		if (auxArray == NULL) 
		{
			std::cerr << "blMatlabReader: read vector. Error reading in mat-file" << std::endl;
			error = -2;
			return theVector;
		}
		
		if (strcmp(varname, name) == 0)
		{
			if ( (mxGetM(auxArray) != 1) && (mxGetN(auxArray) != 1) )
			{
				std::cout << "blMatlabReader: ERROR! Reading matrix with vector procedure" 
																	<< std::endl;
				error = -3;
				return theVector;
			}
			else
			{
				// dirty trick: numberOfElements * 1
				int numberOfElements = mxGetM(auxArray) * mxGetN(auxArray); 

				const double *cppVector = (double *)mxGetData(auxArray);
				theVector.set_size(numberOfElements );
		
				// Actual reading of data
				for (i = 0; i < numberOfElements; i++)
				{
					theVector(i) = cppVector[i];	// unneficient but easy.
				}
				mxDestroyArray(auxArray);
				error = 0;
				return theVector;	// normal termination
			}
		}
	}
	error = -4;
	return theVector; // unnormal termination
	mxDestroyArray(auxArray);

	blMatlabReader::closeMatlabFile(theMatFile);
}



//----------------------------------------------------------------------------
vnl_matrix<double> blMatlabReader::readMatrixFromMatlabFile(const char *filename,
						const char *varname,
						int &error)
//----------------------------------------------------------------------------
{
	char **dir;
	const char *name;

	int ndir;
	int i, j;

	mxArray *auxArray;
	MATFile *theMatFile;

	vnl_matrix<double> theMatrix;

	theMatFile = blMatlabReader::openMatlabFile(filename);

	// Get directory of MAT-file
	dir = matGetDir(theMatFile, &ndir);
	
	if (dir == NULL) 
	{
		std::cerr << "blMatlabReader: Error reading directory of mat-file" << std::endl;
		error = -1;
	} 

	mxFree(dir);
	
	// In order to use matGetNextXXX correctly, reopen file to read in headers.
	blMatlabReader::closeMatlabFile(theMatFile);
	theMatFile = blMatlabReader::openMatlabFile(filename);

	for (i = 0; i < ndir; i++) 
	{
		auxArray = matGetNextVariable(theMatFile, &name);
		
		if (auxArray == NULL) 
		{
			std::cerr << "blMatlabReader: read vector. Error reading in mat-file" << std::endl;
			error = -2;
		}

		if ( strcmp(varname, name) == 0) 
		{
			const double *cppVector = (double *)mxGetData(auxArray);
			
			theMatrix.set_size(mxGetM(auxArray), mxGetN(auxArray) );
		
			vnl_matrix<double> theMatrix2(mxGetM(auxArray), mxGetN(auxArray) );
		
			
			// Actual reading of data
			for (i = 0; i < mxGetM(auxArray); i++)
			{
				for (j = 0; j < mxGetN(auxArray); j++)
				{
					theMatrix(i,j) = cppVector[i + j * mxGetM(auxArray)];	// unneficient but easy.
				}
				std::cout << std::endl << std::endl;
			}

			error = 0; // normal termination
			return theMatrix;
		}
	}

	error = -1; // abnormal termination
	mxDestroyArray(auxArray);

	blMatlabReader::closeMatlabFile(theMatFile);
	theMatrix.fill(0.0);
	return theMatrix;
}
