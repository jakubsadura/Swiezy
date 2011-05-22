/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blPDSetReader_templ_H
#define _blPDSetReader_templ_H

#include "blPDSet_templ.h"
#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"

#include "BaseLibWin32Header.h"

// max length of a line in the list file



/**
 * \brief This class reads a new blPDSet from a list of files
 * \ingroup blPDShape
 *
 *	This class reads from a text file (.lst) that contains a number 
 *	of names of shape files, in VTK format, that define PDShapes.
 * 
 *	The file extension should be .lst, with one file name in each line.
 */


template< class ArgMatrixType >
class BASELIB_EXPORT blPDSetReader_templ: public blLightObject
{

    public:
		typedef blPDSetReader_templ Self;
		typedef blSmartPointer<Self> Pointer;

		typedef ArgMatrixType MatrixType;
		typedef typename bllao::Namespace<MatrixType> MatrixNamespace;
		typedef typename MatrixNamespace::Pointer MatrixPointer;
		typedef baselib::VnlVectorType VectorType;

		typedef typename blPDSet_templ<MatrixType>::Pointer blPDSet_templPointer;
		typedef typename blPDSetReader_templ<MatrixType>::Pointer blPDSetReader_templPointer;

		blNewMacro(Self);

		/** \brief Set Filename */
        void SetFilename(const char * fileName);

		
		/** \brief reads from list file and create the PDSet object */
		void Update();

		/** \brief returns the output PDSet */
		blPDSet_templPointer GetOutput() const
			{ return this->pdSet;}	;	

		void SetAllShapesInfoKeepingFlag( bool b )
			{ m_allShapesInfoKeep = b; };

		void SetVervoseOn()
		{
			m_verbose = true;
		}

		void SetVervoseOff()
		{
			m_verbose = false;
		}
						
	protected:
		/** \brief Constructor */
		blPDSetReader_templ();

		/** \brief Destructor */
		virtual ~blPDSetReader_templ();
    private:        
		blPDSetReader_templ(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented		

        char * fileName;		//!< file to be read
		blPDSet_templPointer pdSet;		//!< blPDSet to be built		

		bool m_allShapesInfoKeep;
		bool m_verbose;
		
		MatrixPointer thePoints;

};

#endif // blPDSetReader_templ_H
