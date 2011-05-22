/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blTimeStamp_h
#define __blTimeStamp_h

#include "blSmartPointer.h"
#include "blMacro.h"

/** 
 * \brief Generate a unique, increasing time value.
 *
 * TimeStamp records a unique time when the method Modified() is 
 * executed. This time is guaranteed to be monotonically increasing.
 * Classes use this object to record modified and/or execution time.
 * There is built in support for the binary < and > comparison
 * operators between two TimeStamp objects. 
 *
 * \note This implementation is not Multi threading compatible
 *
 * \author Xavi Planes
 * \date 5 Feb 2010
 * \ingroup blCommon
 */
class blTimeStamp 
{
public:
  /** Standard class typedefs. */
  typedef blTimeStamp Self;
  typedef blSmartPointer<Self> Pointer;

  /** Create an instance of this class. We don't want to use reference
  * counting. */
  static Self* New();

  /** Constructor must remain public because classes instantiate
   * TimeStamps implicitly in their construction.  */
  blTimeStamp() 
    {m_ModifiedTime = 0;}

  /** Destroy this instance. */
  void Delete() 
    {delete this;}

  /** The class name as a string.  */
  static const char *GetNameOfClass() 
    {return "TimeStamp";}

  /** Set this objects time to the current time. The current time is just a
   * monotonically increasing unsigned long integer. It is possible for this
   * number to wrap around back to zero.  This should only happen for
   * processes that have been running for a very long time, while constantly
   * changing objects within the program. When this does occur, the typical
   * consequence should be that some filters will update themselves when
   * really they don't need to.   */
  void Modified();

  /** Return this object's Modified time.  */
  unsigned long GetMTime() const
    {return m_ModifiedTime;};

  /** Support comparisons of time stamp objects directly.  */
  bool operator>(blTimeStamp& ts) 
    {return (m_ModifiedTime > ts.m_ModifiedTime);}
  bool operator<(blTimeStamp& ts) 
    {return (m_ModifiedTime < ts.m_ModifiedTime);}

  /** Allow for typcasting to unsigned long.  */
  operator unsigned long() const
    {return m_ModifiedTime;}

private:
  unsigned long m_ModifiedTime;
};

  
#endif
