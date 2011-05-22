/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blSmartPointer_h
#define __blSmartPointer_h

/**
 * \brief Implements transparent reference counting (copy of itk::SmartPointer).
 * \ingroup blCommon
 *
 * blAutoPointer implements reference counting by overloading
 * operator -> (and *) among others. This allows natural interface
 * to the class referred to by the pointer without having to invoke
 * special Register()/UnRegister() methods directly.
 *
 */
template <class ObjectType>
class blSmartPointer 
{
	public:
 
		/** Constructor  */
		blSmartPointer () 
		{ m_Pointer = 0; }

		/** Copy constructor  */
		blSmartPointer (const blSmartPointer<ObjectType> &p):
			m_Pointer(p.m_Pointer)
		{ this->Register(); }
  
		/** Constructor to pointer p, also copy  */
		blSmartPointer (ObjectType *p):
			m_Pointer(p)
		{ this->Register(); }                             
  
		/** Destructor  */
		~blSmartPointer ()
		{
			this->UnRegister();
			m_Pointer = 0;  
		}
  
		/** Overload operator ->  */
		ObjectType *operator -> () const
		{ return m_Pointer; }

		/** Return pointer to object.  */
		operator ObjectType * () const 
		{ return m_Pointer; }
  
		/** Test if the pointer has been initialized */
		bool IsNotNull() const
		{ return m_Pointer != 0; }
		bool IsNull() const
		{ return m_Pointer == 0; }

		/** Template comparison operators. */
		template <typename R>
		bool operator == ( R r ) const
		{ return (m_Pointer == (ObjectType*)(r) ); }

		template <typename R>
		bool operator != ( R r ) const
		{ return (m_Pointer != (ObjectType*)(r) ); }
    
		/** Access function to pointer. */
		ObjectType *GetPointer () const 
		{ return m_Pointer; }
  
		/** Comparison of pointers. Less than comparison.  */
		bool operator < (const blSmartPointer &r) const
		{ return (void*)m_Pointer < (void*) r.m_Pointer; }
  
		/** Comparison of pointers. Greater than comparison.  */
		bool operator > (const blSmartPointer &r) const
		{ return (void*)m_Pointer > (void*) r.m_Pointer; }

		/** Comparison of pointers. Less than or equal to comparison.  */
		bool operator <= (const blSmartPointer &r) const
		{ return (void*)m_Pointer <= (void*) r.m_Pointer; }

		/** Comparison of pointers. Greater than or equal to comparison.  */
		bool operator >= (const blSmartPointer &r) const
		{ return (void*)m_Pointer >= (void*) r.m_Pointer; }

		/** Overload operator assignment.  */
		blSmartPointer &operator = (const blSmartPointer &r)
		{ return this->operator = (r.GetPointer()); }
  

		/** Overload operator assignment.  */
		blSmartPointer &operator = (ObjectType *r)
		{                                                              
			if (m_Pointer != r)
			{
				ObjectType* tmp = m_Pointer; //avoid recursive unregisters by retaining temporarily
				m_Pointer = r;
				this->Register();
				if ( tmp ) { tmp->UnRegister(); }
			}
			return *this;
		}
  
		void Delete()
		{
			UnRegister();
			m_Pointer = 0;
		}

	private:
		/** The pointer to the object referrred to by this smart pointer. */
		ObjectType* m_Pointer;

		void Register()
		{ 
			if(m_Pointer) { m_Pointer->Register(); }
		}
  
		void UnRegister()
		{
			if(m_Pointer) { m_Pointer->UnRegister(); }
		}
};  

  
#endif
