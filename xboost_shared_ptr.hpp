#ifndef XBOOST_SHARED_PTR_HPP_INCLUDED
#define XBOOST_SHARED_PTR_HPP_INCLUDED

// xboost_shared_ptr.hpp
// Based on shared_ptr.hpp, from Boost
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

// Original file shared_ptr.hpp contained the following notice:
//
//  shared_ptr.hpp
//
//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001, 2002, 2003 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org/libs/smart_ptr/shared_ptr.htm for documentation.
//

#include "xboost_checked_delete.hpp"
#include "xboost_shared_count.hpp"

#include <memory>               // for std::auto_ptr
#include <algorithm>            // for std::swap
#include <functional>           // for std::less
#include <typeinfo>             // for std::bad_cast
#include <iosfwd>               // for std::basic_ostream

#ifdef XBOOST_OLD_STYLE_C_HEADERS  // old-style C headers support
# include <assert.h>            // for assert
#else
# include <cassert>             // for assert
#endif

#ifdef _MSC_VER  // Special stuff for MS-Visual C++
# define XBOOST_NO_CV_VOID_SPECIALIZATIONS
# define XBOOST_NO_MEMBER_TEMPLATE_FRIENDS
# pragma warning(push)
# pragma warning(disable:4284) // odd return type for operator->
#endif


namespace xboost
{

template<class T> class weak_ptr;
template<class T> class enable_shared_from_this;

namespace detail
{

struct static_cast_tag {};
struct dynamic_cast_tag {};
struct polymorphic_cast_tag {};

template<class T> struct shared_ptr_traits
{
    typedef T & reference;
};

template<> struct shared_ptr_traits<void>
{
    typedef void reference;
};

#if !defined(XBOOST_NO_CV_VOID_SPECIALIZATIONS)

template<> struct shared_ptr_traits<void const>
{
    typedef void reference;
};

#endif

// enable_shared_from_this support

template<class T, class Y> void sp_enable_shared_from_this(xboost::enable_shared_from_this<T> * pe, Y * px, shared_count const & pn)
{
    if(pe != 0) pe->_internal_weak_this._internal_assign(px, pn);
}

inline void sp_enable_shared_from_this(void const *, void const *, shared_count const &)
{
}

} // namespace detail


//
//  shared_ptr
//
//  An enhanced relative of scoped_ptr with reference counted copy semantics.
//  The object pointed to is deleted when the last shared_ptr pointing to it
//  is destroyed or reset.
//

template<class T> class shared_ptr
{
private:

    // Borland 5.5.1 specific workaround
    typedef shared_ptr<T> this_type;

public:

    typedef T element_type;
    typedef T value_type;
    typedef T * pointer;
    typedef typename detail::shared_ptr_traits<T>::reference reference;

    shared_ptr(): px(0), pn() // never throws in 1.30+
    {
    }

    template<class Y>
    explicit shared_ptr(Y * p): px(p), pn(p, checked_deleter<Y>()) // Y must be complete
    {
        detail::sp_enable_shared_from_this(p, p, pn);
    }

    //
    // Requirements: D's copy constructor must not throw
    //
    // shared_ptr will release p by calling d(p)
    //

    template<class Y, class D> shared_ptr(Y * p, D d): px(p), pn(p, d)
    {
        detail::sp_enable_shared_from_this(p, p, pn);
    }

//  generated copy constructor, assignment, destructor are fine...

//  except that Borland C++ has a bug, and g++ with -Wsynth warns
#if defined(__BORLANDC__) || defined(__GNUC__)

    shared_ptr & operator=(shared_ptr const & r) // never throws
    {
        px = r.px;
        pn = r.pn; // shared_count::op= doesn't throw
        return *this;
    }

#endif

    template<class Y>
    explicit shared_ptr(weak_ptr<Y> const & r): pn(r.pn) // may throw
    {
        // it is now safe to copy r.px, as pn(r.pn) did not throw
        px = r.px;
    }

    template<class Y>
    shared_ptr(shared_ptr<Y> const & r): px(r.px), pn(r.pn) // never throws
    {
    }

    template<class Y>
    shared_ptr(shared_ptr<Y> const & r, detail::static_cast_tag): px(static_cast<element_type *>(r.px)), pn(r.pn)
    {
    }

    template<class Y>
    shared_ptr(shared_ptr<Y> const & r, detail::dynamic_cast_tag): px(dynamic_cast<element_type *>(r.px)), pn(r.pn)
    {
        if(px == 0) // need to allocate new counter -- the cast failed
        {
            pn = detail::shared_count();
        }
    }

    template<class Y>
    shared_ptr(shared_ptr<Y> const & r, detail::polymorphic_cast_tag): px(dynamic_cast<element_type *>(r.px)), pn(r.pn)
    {
        if(px == 0)
        {
            throw std::bad_cast();
        }
    }

    template<class Y>
    explicit shared_ptr(std::auto_ptr<Y> & r): px(r.get()), pn()
    {
        Y * tmp = r.get();
        pn = detail::shared_count(r);
        detail::sp_enable_shared_from_this(tmp, tmp, pn);
    }

    template<class Y>
    shared_ptr & operator=(shared_ptr<Y> const & r) // never throws
    {
        px = r.px;
        pn = r.pn; // shared_count::op= doesn't throw
        return *this;
    }

    template<class Y>
    shared_ptr & operator=(std::auto_ptr<Y> & r)
    {
        this_type(r).swap(*this);
        return *this;
    }

    void reset() // never throws in 1.30+
    {
        this_type().swap(*this);
    }

    template<class Y> void reset(Y * p) // Y must be complete
    {
        assert(p == 0 || p != px); // catch self-reset errors
        this_type(p).swap(*this);
    }

    template<class Y, class D> void reset(Y * p, D d)
    {
        this_type(p, d).swap(*this);
    }

    reference operator* () const // never throws
    {
        assert(px != 0);
        return *px;
    }

    T * operator-> () const // never throws
    {
        assert(px != 0);
        return px;
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

    // operator! is redundant, but some compilers need it

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

    void swap(shared_ptr<T> & other) // never throws
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }

    template<class Y> bool _internal_less(shared_ptr<Y> const & rhs) const
    {
        return pn < rhs.pn;
    }

    void * _internal_get_deleter(std::type_info const & ti) const
    {
        return pn.get_deleter(ti);
    }

// Tasteless as this may seem, making all members public allows member templates
// to work in the absence of member template friends. (Matthew Langston)

#ifndef XBOOST_NO_MEMBER_TEMPLATE_FRIENDS

private:

    template<class Y> friend class shared_ptr;
    template<class Y> friend class weak_ptr;


#endif

    T * px;                     // contained pointer
    detail::shared_count pn;    // reference counter

};  // shared_ptr

template<class T, class U> inline bool operator==(shared_ptr<T> const & a, shared_ptr<U> const & b)
{
    return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(shared_ptr<T> const & a, shared_ptr<U> const & b)
{
    return a.get() != b.get();
}

template<class T, class U> inline bool operator<(shared_ptr<T> const & a, shared_ptr<U> const & b)
{
    return a._internal_less(b);
}

template<class T> inline void swap(shared_ptr<T> & a, shared_ptr<T> & b)
{
    a.swap(b);
}

template<class T, class U> shared_ptr<T> static_pointer_cast(shared_ptr<U> const & r)
{
    return shared_ptr<T>(r, detail::static_cast_tag());
}

template<class T, class U> shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const & r)
{
    return shared_ptr<T>(r, detail::dynamic_cast_tag());
}

// shared_*_cast names are deprecated. Use *_pointer_cast instead.

template<class T, class U> shared_ptr<T> shared_static_cast(shared_ptr<U> const & r)
{
    return shared_ptr<T>(r, detail::static_cast_tag());
}

template<class T, class U> shared_ptr<T> shared_dynamic_cast(shared_ptr<U> const & r)
{
    return shared_ptr<T>(r, detail::dynamic_cast_tag());
}

template<class T, class U> shared_ptr<T> shared_polymorphic_cast(shared_ptr<U> const & r)
{
    return shared_ptr<T>(r, detail::polymorphic_cast_tag());
}

template<class T, class U> shared_ptr<T> shared_polymorphic_downcast(shared_ptr<U> const & r)
{
    assert(dynamic_cast<T *>(r.get()) == r.get());
    return shared_static_cast<T>(r);
}

// get_pointer() enables mem_fn to recognize shared_ptr

template<class T> inline T * get_pointer(shared_ptr<T> const & p)
{
    return p.get();
}

// operator<<

template<class E, class T, class Y> std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, shared_ptr<Y> const & p)
{
    os << p.get();
    return os;
}

// get_deleter (experimental)

template<class D, class T> D * get_deleter(shared_ptr<T> const & p)
{
    return static_cast<D *>(p._internal_get_deleter(typeid(D)));
}

} // namespace xboost

#ifdef _MSC_VER
# pragma warning(pop)
#endif

#endif  // #ifndef XBOOST_SHARED_PTR_HPP_INCLUDED
