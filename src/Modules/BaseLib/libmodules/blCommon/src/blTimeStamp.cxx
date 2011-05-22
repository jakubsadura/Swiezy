/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blTimeStamp.h"

blTimeStamp* blTimeStamp::New()
{
	return new Self;
}

void blTimeStamp::Modified()
{
  static unsigned long blTimeStampTime = 0;

  m_ModifiedTime = ++blTimeStampTime;

}


