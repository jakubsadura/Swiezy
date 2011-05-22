/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLUBLASCONVERTIBLEARRAY_H
#define BLUBLASCONVERTIBLEARRAY_H

#include "boost/numeric/ublas/storage.hpp"

namespace Ublas = boost::numeric::ublas;

template<class S, class ALLOC = std::allocator<S> >
class blUblasConvertibleArray : public Ublas::storage_array<blUblasConvertibleArray<S, ALLOC> > 
{
    typedef blUblasConvertibleArray<S, ALLOC> self_type;

public:
    typedef ALLOC allocator_type;
    typedef typename ALLOC::size_type size_type;
    typedef typename ALLOC::difference_type difference_type;
    typedef S value_type;
    typedef const S &const_reference;
    typedef S &reference;
    typedef const S *const_pointer;
    typedef S *pointer;
    typedef const_pointer const_iterator;
    typedef pointer iterator;

    // Construction and destruction
    explicit BOOST_UBLAS_INLINE
    blUblasConvertibleArray (const ALLOC &a = ALLOC()):
        alloc_ (a), size_ (0), mine_(true) {
        data_ = 0;
    }
    explicit BOOST_UBLAS_INLINE
    blUblasConvertibleArray (pointer p, int argsize, const ALLOC &a = ALLOC()):
        alloc_ (a), size_ (argsize), data_(p), mine_(false)
	{
    }
    explicit BOOST_UBLAS_INLINE
    blUblasConvertibleArray (size_type size, const ALLOC &a = ALLOC()):
        alloc_(a), size_ (size), mine_(true) {
        if (size_) {
            data_ = alloc_.allocate (size_);
            // ISSUE some compilers may zero POD here
#ifdef BOOST_UBLAS_USEFUL_ARRAY_PLACEMENT_NEW
            // array form fails on some compilers due to size cookie, is it standard conforming?
            new (data_) value_type[size_];
#else
            for (pointer d = data_; d != data_ + size_; ++d)
                new (d) value_type;
#endif
        }
        else
            data_ = 0;
    }
    // No value initialised, but still be default constructed
    BOOST_UBLAS_INLINE
    blUblasConvertibleArray (size_type size, const value_type &init, const ALLOC &a = ALLOC()):
        alloc_ (a), size_ (size), mine_(true) {
        if (size_) {
            data_ = alloc_.allocate (size_);
            std::uninitialized_fill (begin(), end(), init);
        }
        else
            data_ = 0;
    }
    BOOST_UBLAS_INLINE
    blUblasConvertibleArray (const blUblasConvertibleArray &c):
        alloc_ (c.alloc_), size_ (c.size_), mine_(true) {
        if (size_) {
            data_ = alloc_.allocate (size_);
            std::uninitialized_copy (c.begin(), c.end(), begin());
        }
        else
            data_ = 0;
    }
    BOOST_UBLAS_INLINE
    ~blUblasConvertibleArray () {
        if (size_) {
            const iterator i_end = end();
            for (iterator i = begin (); i != i_end; ++i) {
                iterator_destroy (i); 
            }
			if( mine_ )
				alloc_.deallocate (data_, size_);
        }
    }

    // Resizing
private:
    BOOST_UBLAS_INLINE
    void resize_internal (const size_type size, const value_type init, const bool preserve) {
        if (size != size_) {
            pointer p_data = data_;
            if (size) {
                data_ = alloc_.allocate (size);
				mine_ = true;
                if (preserve) {
                    pointer si = p_data;
                    pointer di = data_;
                    if (size < size_) {
                        for (; di != data_ + size; ++di) {
                            alloc_.construct (di, *si);
                            ++si;
                        }
                    }
                    else {
                        for (pointer si = p_data; si != p_data + size_; ++si) {
                            alloc_.construct (di, *si);
                            ++di;
                        }
                        for (; di != data_ + size; ++di) {
                            alloc_.construct (di, init);
                        }
                    }
                }
                else {
                    // ISSUE some compilers may zero POD here
#ifdef BOOST_UBLAS_USEFUL_ARRAY_PLACEMENT_NEW
                    // array form fails on some compilers due to size cookie, is it standard conforming?
                    new (data_) value_type[size];
#else
                    for (pointer di = data_; di != data_ + size; ++di)
                        new (di) value_type;
#endif
					mine_ = true;
                }
            }

            if (size_ && mine_) {
                for (pointer si = p_data; si != p_data + size_; ++si)
                    alloc_.destroy (si);
                alloc_.deallocate (p_data, size_);
            }

            if (!size)
                data_ = 0;
            size_ = size;
        }
    }
public:
    BOOST_UBLAS_INLINE
    void resize (size_type size) {
        resize_internal (size, value_type (), false);
    }
    BOOST_UBLAS_INLINE
    void resize (size_type size, value_type init) {
        resize_internal (size, init, true);
    }
                
    // Random Access Container
    BOOST_UBLAS_INLINE
    size_type max_size () const {
        return ALLOC ().max_size();
    }
    
    BOOST_UBLAS_INLINE
    bool empty () const {
        return size_ == 0;
    }
        
    BOOST_UBLAS_INLINE
    size_type size () const {
        return size_;
    }

    // Element access
    BOOST_UBLAS_INLINE
    const_reference operator [] (size_type i) const {
        BOOST_UBLAS_CHECK (i < size_, Ublas::bad_index ());
        return data_ [i];
    }
    BOOST_UBLAS_INLINE
    reference operator [] (size_type i) {
        BOOST_UBLAS_CHECK (i < size_, Ublas::bad_index ());
        return data_ [i];
    }

    // Assignment
    BOOST_UBLAS_INLINE
    blUblasConvertibleArray &operator = (const blUblasConvertibleArray &a) {
        if (this != &a) {
            resize (a.size_);
            std::copy (a.data_, a.data_ + a.size_, data_);
        }
        return *this;
    }
    BOOST_UBLAS_INLINE
    blUblasConvertibleArray &assign_temporary (blUblasConvertibleArray &a) {
        swap (a);
        return *this;
    }

    // Swapping
    BOOST_UBLAS_INLINE
    void swap (blUblasConvertibleArray &a) {
        if (this != &a) {
            std::swap (size_, a.size_);
            std::swap (data_, a.data_);
        }
    }
    BOOST_UBLAS_INLINE
    friend void swap (blUblasConvertibleArray &a1, blUblasConvertibleArray &a2) {
        a1.swap (a2);
    }

    BOOST_UBLAS_INLINE
    const_iterator begin () const {
        return data_;
    }
    BOOST_UBLAS_INLINE
    const_iterator end () const {
        return data_ + size_;
    }

    BOOST_UBLAS_INLINE
    iterator begin () {
        return data_;
    }
    BOOST_UBLAS_INLINE
    iterator end () {
        return data_ + size_;
    }

    // Reverse iterators
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;

    BOOST_UBLAS_INLINE
    const_reverse_iterator rbegin () const {
        return const_reverse_iterator (end ());
    }
    BOOST_UBLAS_INLINE
    const_reverse_iterator rend () const {
        return const_reverse_iterator (begin ());
    }
    BOOST_UBLAS_INLINE
    reverse_iterator rbegin () {
        return reverse_iterator (end ());
    }
    BOOST_UBLAS_INLINE
    reverse_iterator rend () {
        return reverse_iterator (begin ());
    }

    // Allocator
    allocator_type get_allocator () {
        return alloc_;
    }

private:
    // Handle explict destroy on a (possibly indexed) iterator
    BOOST_UBLAS_INLINE
    static void iterator_destroy (iterator &i) {
        (&(*i)) -> ~value_type ();
    }
    ALLOC alloc_;
    size_type size_;
    pointer data_;
	bool mine_;
};

#endif //BLUBLASCONVERTIBLEARRAY_H
