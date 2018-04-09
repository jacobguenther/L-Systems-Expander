#ifndef XBOOST_SHARED_ARRAY_HPP_INCLUDED
#define XBOOST_SHARED_ARRAY_HPP_INCLUDED

// xboost_shared_array.hpp
// Based on shared_array.hpp, from Boost
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
/// Namespace boost changed to xboost

// Original file shared_array.hpp contained the following notice:
//
//  shared_array.hpp
//
//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001, 2002 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org/libs/smart_ptr/shared_array.htm for documentation.
//

#include "xboost_checked_delete.hpp"
#include "xboost_shared_count.hpp"

#include <algorithm>          // for std::swap
#include <functional>         // for std::less

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

//
//  shared_array
//
//  shared_array extends shared_ptr to arrays.
//  The array pointed to is deleted when the last shared_array pointing to it
//  is destroyed or reset.
//

template<class T> class shared_array
{
private:

    // Borland 5.5.1 specific workarounds
    typedef checked_array_deleter<T> deleter;
    typedef shared_array<T> this_type;

public:

    typedef T element_type;

    explicit shared_array(T * p = 0): px(p), pn(p, deleter())
    {
    }

    //
    // Requirements: D's copy constructor must not throw
    //
    // shared_array will release p by calling d(p)
    //

    template<class D> shared_array(T * p, D d): px(p), pn(p, d)
    {
    }

//  generated copy constructor, assignment, destructor are fine

    void reset(T * p = 0)
    {
        assert(p == 0 || p != px);
        this_type(p).swap(*this);
    }

    template <class D> void reset(T * p, D d)
    {
        this_type(p, d).swap(*this);
    }

    T & operator[] (XBOOST_PTRDIFF_T_INTERNAL i) const // never throws
    {
        assert(px != 0);
        assert(i >= 0);
        return px[i];
    }
    
    T * get() const // never throws
    {
        return px;
    }

    // implicit conversion to "bool"

    typedef T * (this_type::*unspecified_bool_type)() const;

    operator unspecified_bool_type() const // never throws
    {
        return px == 0? 0: &this_type::get;
    }

    bool operator! () const // never throws
    {
        return px == 0;
    }

    bool unique() const // never throws
    {
        return pn.unique();
    }

    long use_count() const // never throws
    {
        return pn.use_count();
    }

    void swap(shared_array<T> & other) // never throws
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }

private:

    T * px;                     // contained pointer
    detail::shared_count pn;    // reference counter

};  // shared_array

template<class T> inline bool operator==(shared_array<T> const & a, shared_array<T> const & b) // never throws
{
    return a.get() == b.get();
}

template<class T> inline bool operator!=(shared_array<T> const & a, shared_array<T> const & b) // never throws
{
    return a.get() != b.get();
}

template<class T> inline bool operator<(shared_array<T> const & a, shared_array<T> const & b) // never throws
{
    return std::less<T*>()(a.get(), b.get());
}

template<class T> void swap(shared_array<T> & a, shared_array<T> & b) // never throws
{
    a.swap(b);
}

} // namespace xboost

#undef XBOOST_PTRDIFF_T_INTERNAL

#endif  // #ifndef XBOOST_SHARED_ARRAY_HPP_INCLUDED
