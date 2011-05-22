/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __PACSAPIDCMTKINCLUDED
#define __PACSAPIDCMTKINCLUDED

#if !defined(_WIN32)
  #define HAVE_CONFIG_H
#endif

//For DIMSE generic services (network connection and association)
namespace PACS
{
#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */
}

#include "dcmtk/ofstd/ofcond.h"

#include "dcmtk/dcmnet/assoc.h"

#include "dcmtk/dcmdata/dcdict.h"
#include "dcmtk/dcmnet/diutil.h"
#include "dcmtk/dcmdata/dcuid.h"    /* for dcmtk version name */
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdebug.h"
#include "dcmtk/dcmdata/dcdicent.h"
#include "dcmtk/dcmnet/dimse.h"
#include "dcmtk/ofstd/ofcmdln.h" 

#include "dcmtk/ofstd/ofstdinc.h"


#include "dcmtk/dcmnet/dicom.h"
//#include "dcmtk/dcmdata/cmdlnarg.h"
#include "dcmtk/ofstd/ofconapp.h"
#include "dcmtk/ofstd/ofstd.h"

#include "dcmtk/ofstd/ofstring.h"
#include "dcmtk/dcmnet/dimse.h"
#include "dcmtk/dcmnet/diutil.h"
#include "dcmtk/dcmdata/dcdatset.h"
#include "dcmtk/dcmdata/dcmetinf.h"
#include "dcmtk/dcmdata/dcdeftag.h"

//standard includefile needed both MoveSCU and FindSCU Classes
#ifdef HAVE_GUSI_H
#include <GUSI.h>
#endif

#ifdef WITH_ZLIB
#include <zlib.h>     /* for zlibVersion() */
#endif

#endif  //#ifndef __PACSAPIDCMTKINCLUDED