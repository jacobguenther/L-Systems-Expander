#ifndef XBOOST_CHECKED_DELETE_HPP_INCLUDED
#define XBOOST_CHECKED_DELETE_HPP_INCLUDED

#if _MSC_VER >= 1020
#pragma once
#endif

// xboost_checked_delete.hpp
// Based on checked_delete.hpp, from Boost
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

// Original file checked_delete.hpp contained the following notice:
//
//  boost/checked_delete.hpp
//
//  Copyright (c) 1999, 2000, 2001, 2002 boost.org
//  Copyright (c) 2002, 2003 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org/libs/utility/checked_delete.html for documentation.
//


namespace xboost
{

// verify that types are complete for increased safety

template<class T> inline void checked_delete(T * x)
{
    typedef char type_must_be_complete[sizeof(T)];
    delete x;
}

template<class T> inline void checked_array_delete(T * x)
{
    typedef char type_must_be_complete[sizeof(T)];
    delete [] x;
}

template<class T> struct checked_deleter
{
    typedef void result_type;
    typedef T * argument_type;

    void operator()(T * x) const
    {
        xboost::checked_delete(x);
    }
};

template<class T> struct checked_array_deleter
{
    typedef void result_type;
    typedef T * argument_type;

    void operator()(T * x) const
    {
        xboost::checked_array_delete(x);
    }
};

} // namespace xboost

#endif  // #ifndef XBOOST_CHECKED_DELETE_HPP_INCLUDED

