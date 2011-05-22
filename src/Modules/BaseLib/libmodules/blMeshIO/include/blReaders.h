// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


#ifndef BLReaders_H
#define BLReaders_H


// BaseLib-includes
// ...none at the moment...

// includes from other Toolkit-Libs
// ...none at the moment...

// Third Party Library includes
// ...none at the moment...

// C/C++ includes
#include <cstring>


namespace base {
namespace meshio {


/**
2D-Integer-Vector that can be used to run through a discrete rectangle with lower bound(s) (0, 0) and
upper (excluded) bound(s) (m_numMajorElements, m_numMinorElements); using the advance-functions you can step
forward respective a lexicographic order. The position (m_numMajorElements, 0) designates the only
invalid position at which that this class can be (kind of EOF).
*/
class position2D {
private:
	//! upper bound for major component (>= 0)
	int m_numMajorElements;

	//! upper bound for minor component (>= 0)
	int m_numMinorElements;

	//! number of points in the discrete rectangle (== m_numMajorElements * m_numMinorElements)
	int m_numElements;


	//! current major position ("first component" of position vector) (0 <= majorPosition <= m_numMajorElements)
	int m_majorPosition;

	//! current major position ("first component" of position vector) (0 <= majorPosition < m_numMinorElements)
	int m_minorPosition;

	/**
	overall position in rectangle according to lexicographic order
	(m_position == m_majorPosition * m_numMinorElements + m_minorPosition)
	*/
	int m_position;

public:
	/*
	constructor generating an empty rectangle: lower bound (0, 0) and upper (excluded) bound (0, 0); there is
	no valid position in this rectangle but the EOF-position (0, 0). The object will be initialized at this
	only valid position.
	*/
	position2D();

	/*
	constructor generating a rectangle with lower bound (0, 0) and upper (excluded) bound
	(numMajorElements, numMinorElements). The position will be initialized at position (0, 0),
	which may be (but usually is not) EOF.
	*/
	position2D(int numMajorElements, int numMinorElements);

	//! Sets the position to (0, 0); note that this position is always valid (but may be EOF)
	void reset();

	//! Advance one step respective lexicographic order
	void advance();

	/**
	Store first the current position (combined to 1D) in *out_position and then advance one step
	respective lexicographic order
	*/
	void advance(int *out_position);
	/**
	Store the current position (first!) in (*out_majorPosition, *out_minorPosition) and then advance one step
	respective lexicographic order
	*/
	void advance(int *out_majorPosition, int *out_minorPosition);

	//! Return the major component of the current position (>= 0 and <= major component of rectangle size)
	int getMajorPosition();

	//! Return the major component of the current position (>= 0 and < major component of rectangle size or ==0)
	int getMinorPosition();

	//! Return the current position reduced to 1D (>= 0 and <= major comp. of rect. size * minor comp. of rect. size)
	int getPosition();

	//! Return the first (major) component of the rectangle upper bound
	int getNumMajorElements();

	//! Return the second (minor) component of the rectangle upper bound
	int getNumMinorElements();

	//! Return the number of elements in the rectangle (>= 0)
	int getNumElements();

	//! Tell, if the current position is NOT EOF
	bool hasNext();
};


//! Interface for reading 3D data in an iterative (no random access) manner
template<typename D>
class iDataReaderStaticI {
public:
    //!
    virtual ~iDataReaderStaticI() {}

	//! Returns the number spatial elements for which exists data (>= 0)
	virtual int getNumSpaceElements() const = 0;

	/**
	Reset to the starting point of the iteration. After calling this function
	it is guaranteed that using the readNextData() function the user can obtain
	the data for every spatial element (in {0,...,getNumSpaceElements()-1} ) exactly
	once (but the order can vary) before hitting EOF (means: before hasNext() == false)

	If you "synchronize" several readers (see iDataReaderStaticRAPosition), this condition
	does not have to hold for every single reader, but only for the whole set of synchronized readers.
	*/
	virtual void reset() = 0;

	//! Tells, if we already hit "EOF"
	virtual bool hasNext() const = 0;

	/**
	Returns the data value for an arbitray spatial element that has not yet been
	returned since the last reset(); This function can only be called, if still
	hasNext()==true holds.
	\param out_id       out-Parameter: id of the spatial element of the read value
	\return the data value
	*/
	virtual D readNextData(int *out_id) = 0;
};


//! Interface for reading 3D+T data in an iterative (no random access) manner
template<typename D>
class iDataReaderDynamicI {
public:
    //!
    virtual ~iDataReaderDynamicI() {}

	//! Returns the number spatial elements for which exists data (>= 0)
	virtual int getNumSpaceElements() const = 0;

	//! Returns the number of existing time steps for which exists data (>= 0)
	virtual int getNumTimeSteps() const = 0;

	/**
	Reset to the starting point of the iteration. After calling this function
	it is guaranteed that using the readNextData() function the user can obtain
	the data for every time step (in {0,...,getNumTimeSteps()-1} ) and space element
	(in {0,...,getNumSpaceElements()-1} ) exactly once (but the order can vary)
	before hitting EOF (means: before hasNext() == false)

	If you "synchronize" several readers (see iDataReaderDynamicRAPosition), this condition
	does not have to hold for every single reader, but only for the whole set of synchronized readers.
	*/
	virtual void reset() = 0;

	//! Tells, if we already hit "EOF"
	virtual bool hasNext() const = 0;

	/**
	Returns the data value for an arbitray time step and space element that
	has not yet been returned since the last reset(); This function can only be
	called, if still hasNext()==true holds.
	\param out_time     out-Parameter: time-step of the read value
	\param out_id       out-Parameter: id of the spatial element of the read value
	\return the data value
	*/
	virtual D readNextData(int *out_time, int *out_id) = 0;
};


//! Interface for reading 3D data in an iterative as well as random access manner
template<typename D>
class iDataReaderStaticRA : public iDataReaderStaticI<D> {
public:
	/**
	Read the data for a particular spatial element
	\param id      Spatial element id (0 <= id < getNumSpaceElements())
	\return the data the given id
	*/
	virtual D readData(int id) const = 0;
};


//! Interface for reading 3D+T data in an iterative as well as random access manner
template<typename D>
class iDataReaderDynamicRA : public iDataReaderDynamicI<D> {
public:
	/**
	Read the data at a particular point in time and space
	\param time    Time step (0 <= time < getNumTimeSteps())
	\param id      Spatial element id (0 <= id < getNumSpaceElements())
	\return the data at (time, id)
	*/
	virtual D readData(int time, int id) const = 0;
};


/**
Abstract class that already defines all the functions that the iDataReaderStaticRA
inherits from iDataReaderStaticI. So this class can be considered a wrapper from
iDataReaderStaticI to iDataReaderStaticRA. It does the complete position management
and the "user" (which means: a sub-class) of this class has only to define the
actual random-access-reading function that is then called by this abstract class.

The class provides the possibility to synchronize the position with the position of other
readers. This may be useful, if there exist several readers for the same file. In this case
reading data using one reader advances the read position in the file. This also affects the
other readers and so their position should be updated as well.
*/
template<typename D>
class iDataReaderStaticRAPosition : public iDataReaderStaticRA<D> {

protected:
	//! current (iterative) read position in the data rectangle (may be shared with other readers)
	int *m_position;

	//! number of elements in the data (used to determine EOF in the hasNext()-function)
	int m_numElements;

private:
	//! flag that tells, if the m_position object has to be deleted in our destructor (depending on if it is shared or not)
	bool m_deletePosition;

public:
	/*
	Constructor
	\param numElements      number of elements to be read (used to determine EOF in hasNext())
	\param externalPosition pointer to an external position object for synchronizatin with other readers
	                        (may be NULL, if no synchronization is needed)
	*/
	iDataReaderStaticRAPosition(int numElements, int *externalPosition = NULL);
	
	virtual ~iDataReaderStaticRAPosition();

	// Documentation: see super-class
	virtual int getNumSpaceElements() const;

	// Documentation: see super-class
	virtual void reset();

	// Documentation: see super-class
	virtual bool hasNext() const;

	// Documentation: see super-class
	virtual D readNextData(int *out_id);
};


/**
Abstract class that already defines all the functions that the iDataReaderDynamicRA
inherits from iDataReaderDynamicI. So this class can be considered a wrapper from
iDataReaderDynamicI to iDataReaderDynamicRA. It does the complete 2D position management
and the "user" (which means: a sub-class) of this class has only to define the
actual random-access-reading function that is then called by this abstract class.

The class provides the possibility to synchronize the position with the position of other
readers. This may be useful, if there exist several readers for the same file. In this case
reading data using one reader advances the read position in the file. This also affects the
other readers and so their position should be updated as well.
*/
template<typename D>
class iDataReaderDynamicRAPosition : public iDataReaderDynamicRA<D> {

protected:
	//! current (iterative) read position in the data rectangle (may be shared with other readers)
	position2D *m_position;

private:
	//! flag that tells, if the m_position object has to be deleted in our destructor (depending on if it is shared or not)
	bool m_deletePosition;

public:
	//! constructor taking over an external position object (for synchronization with other readers)
	iDataReaderDynamicRAPosition(position2D *externalPosition);
	
	virtual ~iDataReaderDynamicRAPosition();

	/*
	constructor; no external position object is used, but the reader uses its own one starting at (0, 0) and using
	numTimeElement as (excluded) upper bound for the time position and numSpaceElement for the space position
	*/
	iDataReaderDynamicRAPosition(int numTimeElements, int numSpaceElements);

	// Documentation: see super-class
	virtual int getNumSpaceElements() const;

	// Documentation: see super-class
	virtual int getNumTimeSteps() const;

	// Documentation: see super-class
	virtual void reset();

	// Documentation: see super-class
	virtual bool hasNext() const;

	// Documentation: see super-class
	virtual D readNextData(int *out_time, int *out_id);
};


//! Random-Access-Reader for 3D data from a given array
template <typename D>
class staticArrayReader : public iDataReaderStaticRAPosition<D> {

private:
	//! data array
	const D *m_data;

public:
	/*
	Creates a reader for the given array (data) with a given length (numElements); can be synchronized with
	other readers by providing an external position object (externalPosition)
	*/
	staticArrayReader(const D *data, int numElements, int *externalPosition = NULL);

	// Documentation: see super-class; note: this override is only for efficiency, the superclass-method would do!
	virtual D readNextData(int *out_id);

	// Documentation: see super-class
	virtual D readData(int id) const;
};


//! Random-Access-Reader for 3D+T data from a given array (in time-major lexicographic order)
template <typename D>
class dynamicArrayReader : public iDataReaderDynamicRAPosition<D> {
private:
	//! data array in time-major lexicographic order
	const D *m_data;

	//! data array size
	int m_numSpaceElements;
public:
	//!	Creates a reader for the given array (data) with a given size (numTimeSteps * numSpaceElements)
	dynamicArrayReader(const D *data, int numTimeSteps, int numSpaceElements);

	/**
	Creates a reader for the given array (data). It is synchronized with other readers by the given
	external position object (externalPosition).
	*/
	dynamicArrayReader(const D *data, position2D *externalPosition);

	// Documentation: see super-class; note: this override is only for efficiency, the superclass-method would do!
	virtual D readNextData(int *out_time, int *out_id);

	// Documentation: see super-class
	virtual D readData(int time, int id) const;
};


template <typename D>
class staticConstantReader : public iDataReaderStaticRAPosition<D> {
private:
    D value;
public:
    staticConstantReader(D value, int num, int *externalPosition = NULL);
    virtual D readData(int id) const;
};


class staticIDReader : public iDataReaderStaticRAPosition<int> {
private:
    int m_offset;
public:
    staticIDReader(int value, int offset, int *externalPosition = NULL);
    virtual int readData(int id) const;
};


template <typename D>
class staticPermutedReader : public iDataReaderStaticRAPosition<D> {
private:
    iDataReaderStaticRA<int> *m_permutation;
    iDataReaderStaticRA<D> *m_values;
public:
    staticPermutedReader(iDataReaderStaticRA<int> *permutation, iDataReaderStaticRA<D> *values);
    virtual D readData(int id) const;
};


template <typename D>
class dynamicPermutedReader : public iDataReaderDynamicRAPosition<D> {
private:
    iDataReaderStaticRA<int> *m_permutation;
    iDataReaderDynamicRA<D> *m_values;
public:
    dynamicPermutedReader(iDataReaderStaticRA<int> *permutation, iDataReaderDynamicRA<D> *values);
    virtual D readData(int time, int id) const;
};


} // meshio
} // base


#endif // BLReaders_H
