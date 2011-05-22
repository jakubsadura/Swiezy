/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blTextUtils_h
#define __blTextUtils_h

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <stdarg.h>

/**
\brief   Text utilities
\ingroup blCommon
\author  Federico Sukno
\date    2007-11-23
*/

/** 
 * \brief Converts a time in sec. to a string
 * \ingroup blCommon
 */
std::string blTimeString ( double nSeconds );

std::vector<float> blGetFloatsFromString (std::string theString, char* theTokens, unsigned int nToExtract);

/** 
 * \brief Converts a time in sec. to a string
 * \ingroup blCommon
 */
std::string AddFileNumber( const std::string& fileName, int iNumber );

namespace blTextUtils
{
	//! Convert "\\" -> "/"
	void ConvertPath(std::string& path);


	/**
	* Return file extension of a full filename (dot included).
	* Warning: this is the shortest extension (for example: .tar.gz)
	*/
	std::string GetFilenameLastExtension(const std::string& filename);


	/**
	Extract all words from line with a delimiter
	*/
	void ParseLine( const std::string &line, char delimiter, std::list<std::string> &data );

	//! Replace all sub strings for_this -> sub_this
	void StrSub(std::string& cp,std::string for_this,std::string sub_this);
	//! 
	std::string format_arg_list(const char *fmt, va_list args);
	//!
	std::string format(const char *fmt, ...);

}


/** 
 * \brief This class generates a progress bar in text mode
 * \ingroup blCommon
 *
 *  Usage example:
 *  -------------
 *				int N = 50; 
 *				blTextProgressBar::Pointer myBar = blTextProgressBar::New();
 *				myBar->DrawBackground ();
 *				myBar->SetRange ( 0, N );
 *				for (int i = 0; i < N; i++)
 *				{
 *					// do_task_for_this_iteration
 *					myBar->Add (1):
 *				}
 *				std::cout << std::endl << "Finished" << std::endl;
 */
class blTextProgressBar: public blLightObject
{

    public:
		typedef blTextProgressBar Self;
		typedef blSmartPointer<Self> Pointer;

		blNewMacro(Self);

		// Updates the progress bar status
		void SetStatus ( float completed );

		// Increments the status by increment
		void Add ( float increment );

		// Set bar limits for input
		void SetRange (float minV, float maxV)
		{
			if (! (minV < maxV))
			{
				return;
			}			
			this->minValue = minV;
			this->maxValue = maxV;
		};

		// Set bar size (default is 79)
		void SetLength (unsigned int newL)
		{
			if (newL > 1)
			{
				this->barLength = newL;
			}
		};

		// Displays the progress bar status on the screen
		void Draw ();
		void DrawBackground ();		
	

	protected:
		/** \brief Empty Constructor */
		blTextProgressBar();   


		/** \brief Destructor */
		~blTextProgressBar();  

    private:        
		blTextProgressBar(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented		

		unsigned char displayCharacter;
		unsigned char backgroundCharacter;

		float maxValue;
		float minValue;
		float currentValue;	// ALLWAYS from 0 to 1

		unsigned int barLength;		// number of steps in the bar
		unsigned int stepIncs;		// number of pending steps to draw
		unsigned int currentStep;	// in barLength range
	
};

#endif
