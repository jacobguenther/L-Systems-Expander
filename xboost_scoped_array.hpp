#ifndef XBOOST_SCOPED_ARRAY_HPP_INCLUDED
#define XBOOST_SCOPED_ARRAY_HPP_INCLUDED

// xboost_scoped_array.hpp
// Based on scoped_array.hpp, from Boost
// File obtained from boost.org 6 May 2003
// Extricated from Boost by Glenn G. Chappell
// Version: 1.0
// Date: 6 May 2003
//
// Broken compiler work-arounds removed
// Threads support removed
// Debug hooks support removed
// Quick allocator support removed
// BOOST_ASSERT replaced with assert
// throw_exception replaced with throw
// XBOOST_OLD_STYLE_C_HEADERS support added
// Namespace boost changed to xboost

// Original file scoped_array.hpp contained the following notice:
//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001, 2002 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  http://www.boost.org/libs/smart_ptr/scoped_array.htm
//

#include "xboost_checked_delete.hpp"

#ifdef XBOOST_OLD_STYLE_C_HEADERS  // old-style C headers support
# include <assert.h>          // for assert
# include <stddef.h>          // for ptrdiff_t
# define XBOOST_PTRDIFF_T_INTERNAL ptrdiff_t       // #undef'd at end of file
#else
# include <cassert>           // for assert
# include <cstddef>           // for std::ptrdiff_t
# define XBOOST_PTRDIFF_T_INTERNAL std::ptrdiff_t  // #undef'd at end of file
#endif


namespace xboost
{

//  scoped_array extends scoped_ptr to arrays. Deletion of the array pointed to
//  is guaranteed, either on destruction of the scoped_array or via an explicit
//  reset(). Use shared_array or std::vector if your needs are more complex.

template<class T> class scoped_array // noncopyable
{
private:

    T * ptr;

    scoped_array(scoped_array const &);
    scoped_array & operator=(scoped_array const &);

    typedef scoped_array<T> this_type;

public:

    typedef T element_type;

    explicit scoped_array(T * p = 0) : ptr(p) // never throws
    {
    }

    ~scoped_array() // never throws
    {
        xboost::checked_array_delete(ptr);
    }

    void reset(T * p = 0) // never throws
    {
        assert(p == 0 || p != ptr); // catch self-reset errors
        this_type(p).swap(*this);
    }

    T & operator[](XBOOST_PTRDIFF_T_INTERNAL i) const // never throws
    {
        assert(ptr != 0);
        assert(i >= 0);
        return ptr[i];
    }

    T * get() const // never throws
    {
        return ptr;
    }

    // implicit conversion to "bool"

    typedef T * (this_type::*unspecified_bool_type)() const;

    operator unspecified_bool_type() const // never throws
    {
        return ptr == 0? 0: &this_type::get;
    }

    bool operator! () const // never throws
    {
        return ptr == 0;
    }

    void swap(scoped_array & b) // never throws
    {
        T * tmp = b.ptr;
        b.ptr = ptr;
        ptr = tmp;
    }

};

template<class T> inline void swap(scoped_array<T> & a, scoped_array<T> & b) // never throws
{
    a.swap(b);
}

} // namespace xboost

#undef XBOOST_PTRDIFF_T_INTERNAL

#endif  // #ifndef XBOOST_SCOPED_ARRAY_HPP_INCLUDED
