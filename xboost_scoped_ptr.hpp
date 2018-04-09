#ifndef XBOOST_SCOPED_PTR_HPP_INCLUDED
#define XBOOST_SCOPED_PTR_HPP_INCLUDED

// xboost_scoped_ptr.hpp
// Based on scoped_ptr.hpp, from Boost
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

// Original file scoped_ptr.hpp contained the following notice:
//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001, 2002 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  http://www.boost.org/libs/smart_ptr/scoped_ptr.htm
//

#include "xboost_checked_delete.hpp"

#include <memory>          // for std::auto_ptr

#ifdef XBOOST_OLD_STYLE_C_HEADERS  // old-style C headers support
# include <assert.h>            // for assert
#else
# include <cassert>             // for assert
#endif


namespace xboost
{

// Debug hooks

//  scoped_ptr mimics a built-in pointer except that it guarantees deletion
//  of the object pointed to, either on destruction of the scoped_ptr or via
//  an explicit reset(). scoped_ptr is a simple solution for simple needs;
//  use shared_ptr or std::auto_ptr if your needs are more complex.

template<class T> class scoped_ptr // noncopyable
{
private:

    T * ptr;

    scoped_ptr(scoped_ptr const &);
    scoped_ptr & operator=(scoped_ptr const &);

    typedef scoped_ptr<T> this_type;

public:

    typedef T element_type;

    explicit scoped_ptr(T * p = 0): ptr(p) // never throws
    {
    }

    explicit scoped_ptr(std::auto_ptr<T> p): ptr(p.release()) // never throws
    {
    }

    ~scoped_ptr() // never throws
    {
        xboost::checked_delete(ptr);
    }

    void reset(T * p = 0) // never throws
    {
        assert(p == 0 || p != ptr); // catch self-reset errors
        this_type(p).swap(*this);
    }

    T & operator*() const // never throws
    {
        assert(ptr != 0);
        return *ptr;
    }

    T * operator->() const // never throws
    {
        assert(ptr != 0);
        return ptr;
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

    void swap(scoped_ptr & b) // never throws
    {
        T * tmp = b.ptr;
        b.ptr = ptr;
        ptr = tmp;
    }
};

template<class T> inline void swap(scoped_ptr<T> & a, scoped_ptr<T> & b) // never throws
{
    a.swap(b);
}

// get_pointer(p) is a generic way to say p.get()

template<class T> inline T * get_pointer(scoped_ptr<T> const & p)
{
    return p.get();
}

} // namespace xboost

#endif // #ifndef XBOOST_SCOPED_PTR_HPP_INCLUDED
