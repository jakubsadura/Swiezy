/*=========================================================================

  Module    : BaseLib
  File      : $RCSfile: blMatlabReader.h,v $
  Copyright : (C)opyright (University Pompeu Fabra) 2004++
              See COPYRIGHT statement in top level directory.
  Authors   : agonzalez
  Modified  : $Author: xplanes $
  Purpose   : This class defines tokenizer for strings.
  Date      : $Date: 2007-10-17 17:39:47 $
  Version   : $Revision: 1.5 $
  Changes   : $Locker:  $
              $Log: not supported by cvs2svn $
              Revision 1.4  2007/10/01 09:44:41  jmoya
              Baselib template errors in .net 2003 were fixed

              Revision 1.3  2007/08/10 15:36:09  vdialinos
               Modifications to compile under linux (gcc-4.1.2) and fixed the BASELIB_EXPORT macro where was mispelled.

              Revision 1.2  2007/04/19 12:53:06  vdialinos
              *** empty log message ***

              Revision 1.1  2007/04/18 14:18:36  vdialinos
              *** empty log message ***

              Revision 1.2  2007/04/18 11:57:08  xmellado
              Character error fixed.

              Revision 1.1  2007/03/29 13:47:16  vdialinos
              New Base lib version

              Revision 1.1  2004/12/03 20:17:31  cvsadmin
              initial import in BaseLib

               
              Revision 0.4  2004/06/22 13:46:46  spavani
              Minor changes

              Revision 0.3  2004/06/21 16:07:44  fsukno
              Minor changes

              Revision 0.2  2004/02/26 20:53:00  agonzalez
              shared/static compiling. matlab6.5 conditional compiling.

              Revision 0.1  2004/02/23 15:44:29  agonzalez
              First Upload.

              
=========================================================================*/
#ifndef _blMatlabReader_h
#define _blMatlabReader_h

#include <iostream>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>

#include <stdio.h>
#include <stdlib.h>
#include <mat.h>


#include "BaseLibWin32Header.h"



/**
 * \brief This implements reading from Matlab binary files
 * \ingroup blMatlabUtils
 *
 *  Implements Open, Read of vnl_vector<double> and vnl_matrix<double> and Close
 *
 * \note based on the libmx and libmat dll libraries available on matlabR13
 */
class BASELIB_EXPORT blMatlabReader
{
public:
	/** \brief reads a vector
	 *
	 *	\param theVector will be resized into the function
	 *	\return -1 if varname is not in theMatFile or is not a vector
	 */
	static vnl_vector<double> readVectorFromMatlabFile(const char *filename,
												const char *varname,
												int &error);
	
	/** \brief reads a matrix
	 *
	 *	\param theMatrix will be resized into the function
	 *	\return -1 if varname is not in theMatFile
	 */
	static vnl_matrix<double> readMatrixFromMatlabFile(const char *filename,
												const char *varname,
												int &error);

	/** \brief should be private. NEVER INVOKE.
	 *
	 */
	static int closeMatlabFile(MATFile *theMatFile);

	/** \brief should be private. NEVER INVOKE.
	 *
	 */
	static MATFile *openMatlabFile(const char *filename);
};

#endif
