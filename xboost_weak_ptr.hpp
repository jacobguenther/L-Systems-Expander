#ifndef XBOOST_WEAK_PTR_HPP_INCLUDED
#define XBOOST_WEAK_PTR_HPP_INCLUDED

// xboost_weak_ptr.hpp
// Based on weak_ptr.hpp, from Boost
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

// Original file weak_ptr.hpp contained the following notice:
//
//  weak_ptr.hpp
//
//  Copyright (c) 2001, 2002, 2003 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org/libs/smart_ptr/weak_ptr.htm for documentation.
//

#include "xboost_shared_ptr.hpp"

#ifdef _MSC_VER  // moved here to work around VC++ compiler crash
# pragma warning(push)
# pragma warning(disable:4284) // odd return type for operator->
#endif


namespace xboost
{

template<class T> class weak_ptr
{
private:

    // Borland 5.5.1 specific workarounds
    typedef weak_ptr<T> this_type;

public:

    typedef T element_type;

    weak_ptr(): px(0), pn() // never throws in 1.30+
    {
    }

//  generated copy constructor, assignment, destructor are fine


//
//  The "obvious" converting constructor implementation:
//
//  template<class Y>
//  weak_ptr(weak_ptr<Y> const & r): px(r.px), pn(r.pn) // never throws
//  {
//  }
//
//  has a serious problem.
//
//  r.px may already have been invalidated. The px(r.px)
//  conversion may require access to *r.px (virtual inheritance).
//
//  It is not possible to avoid spurious access violations since
//  in multithreaded programs r.px may be invalidated at any point.
//

    template<class Y>
    weak_ptr(weak_ptr<Y> const & r): pn(r.pn) // never throws
    {
        px = r.lock().get();
    }

    template<class Y>
    weak_ptr(shared_ptr<Y> const & r): px(r.px), pn(r.pn) // never throws
    {
    }

    template<class Y>
    weak_ptr & operator=(weak_ptr<Y> const & r) // never throws
    {
        px = r.lock().get();
        pn = r.pn;
        return *this;
    }

    template<class Y>
    weak_ptr & operator=(shared_ptr<Y> const & r) // never throws
    {
        px = r.px;
        pn = r.pn;
        return *this;
    }

    shared_ptr<T> lock() const // never throws
    {
        // optimization: avoid try/catch overhead when single threaded
        return expired()? shared_ptr<element_type>(): shared_ptr<element_type>(*this);
    }

    long use_count() const // never throws
    {
        return pn.use_count();
    }

    bool expired() const // never throws
    {
        return pn.use_count() == 0;
    }

    void reset() // never throws in 1.30+
    {
        this_type().swap(*this);
    }

    void swap(this_type & other) // never throws
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }

    void _internal_assign(T * px2, detail::shared_count const & pn2)
    {
        px = px2;
        pn = pn2;
    }

    template<class Y> bool _internal_less(weak_ptr<Y> const & rhs) const
    {
        return pn < rhs.pn;
    }

// Tasteless as this may seem, making all members public allows member templates
// to work in the absence of member template friends. (Matthew Langston)

#ifndef XBOOST_NO_MEMBER_TEMPLATE_FRIENDS

private:

    template<class Y> friend class weak_ptr;
    template<class Y> friend class shared_ptr;

#endif

    T * px;                     // contained pointer
    detail::weak_count pn;      // reference counter

};  // weak_ptr

template<class T, class U> inline bool operator<(weak_ptr<T> const & a, weak_ptr<U> const & b)
{
    return a._internal_less(b);
}

template<class T> void swap(weak_ptr<T> & a, weak_ptr<T> & b)
{
    a.swap(b);
}

// deprecated, provided for backward compatibility
template<class T> shared_ptr<T> make_shared(weak_ptr<T> const & r)
{
    return r.lock();
}

} // namespace xboost

#ifdef _MSC_VER
# pragma warning(pop)
#endif    

#endif  // #ifndef XBOOST_WEAK_PTR_HPP_INCLUDED
