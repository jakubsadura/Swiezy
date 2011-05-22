/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __BL_SINGLETON_H__
#define __BL_SINGLETON_H__


/**
\brief The blSingleton class is a template class for creating singleton objects.
\ingroup blUtilities
\author Xavi Planes
\date 3/7/2008
 */
template<typename Derived>
class blSingleton {

public:
	//! Return the static instance
	static Derived* Instance() { static Derived the_inst; return &the_inst; }    

protected:
	//! Do nothing
	blSingleton()  {}

	//! Do nothing
	~blSingleton() {}

private:  
	//! Purposely not implemented
    blSingleton( const blSingleton& );

	//! Purposely not implemented
	const blSingleton& operator=( const blSingleton& );
};


#endif // ! defined __BL_SINGLETON_H__

