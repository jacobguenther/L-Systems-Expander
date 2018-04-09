#ifndef XBOOST_SHARED_COUNT_HPP_INCLUDED
#define XBOOST_SHARED_COUNT_HPP_INCLUDED

#if _MSC_VER >= 1020
#pragma once
#endif

// xboost_shared_count.hpp
// Based on detail/shared_count.hpp, from Boost
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

// Original file detail/shared_count.hpp contained the following notice:
//
//  detail/shared_count.hpp
//
//  Copyright (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#include "xboost_checked_delete.hpp"

#include <memory>           // for std::auto_ptr, std::allocator
#include <functional>       // for std::less
#include <exception>        // for std::exception
#include <new>              // for std::bad_alloc
#include <typeinfo>         // for std::type_info in get_deleter

#ifdef XBOOST_OLD_STYLE_C_HEADERS  // old-style C headers support
# include <stddef.h>            // for size_t
# define XBOOST_SIZE_T_INTERNAL size_t       // #undef'd at end of file
#else
# include <cstddef>             // for std::size_t
# define XBOOST_SIZE_T_INTERNAL std::size_t  // #undef'd at end of file
#endif

#ifdef __BORLANDC__
# pragma warn -8026     // Functions with excep. spec. are not expanded inline
# pragma warn -8027     // Functions containing try are not expanded inline
#endif


namespace xboost
{

// The standard library that comes with Borland C++ 5.5.1
// defines std::exception and its members as having C calling
// convention (-pc). When the definition of bad_weak_ptr
// is compiled with -ps, the compiler issues an error.
// Hence, the temporary #pragma option -pc below. The version
// check is deliberately conservative.

#if defined(__BORLANDC__) && __BORLANDC__ == 0x551
# pragma option push -pc
#endif

class bad_weak_ptr: public std::exception
{
public:

    virtual char const * what() const throw()
    {
        return "xboost::bad_weak_ptr";
    }
};

#if defined(__BORLANDC__) && __BORLANDC__ == 0x551
# pragma option pop
#endif

namespace detail
{

class sp_counted_base
{
public:

    sp_counted_base(): use_count_(1), weak_count_(1)
    {
    }

    virtual ~sp_counted_base() // nothrow
    {
    }

    // dispose() is called when use_count_ drops to zero, to release
    // the resources managed by *this.

    virtual void dispose() = 0; // nothrow

    // destruct() is called when weak_count_ drops to zero.

    virtual void destruct() // nothrow
    {
        delete this;
    }

    virtual void * get_deleter(std::type_info const & ti) = 0;

    void add_ref()
    {
        if(use_count_ == 0 && weak_count_ != 0) throw xboost::bad_weak_ptr();
        ++use_count_;
        ++weak_count_;
    }

    void release() // nothrow
    {
        {
            long new_use_count = --use_count_;

            if(new_use_count != 0)
            {
                --weak_count_;
                return;
            }
        }

        dispose();
        weak_release();
    }

    void weak_add_ref() // nothrow
    {
        ++weak_count_;
    }

    void weak_release() // nothrow
    {
        long new_weak_count;

        {
            new_weak_count = --weak_count_;
        }

        if(new_weak_count == 0)
        {
            destruct();
        }
    }

    long use_count() const // nothrow
    {
        return use_count_;
    }

private:

    sp_counted_base(sp_counted_base const &);
    sp_counted_base & operator= (sp_counted_base const &);

    // inv: use_count_ <= weak_count_

    long use_count_;
    long weak_count_;
};

//
// Borland's Codeguard trips up over the -Vx- option here:
//
#ifdef __CODEGUARD__
# pragma option push -Vx-
#endif

template<class P, class D> class sp_counted_base_impl: public sp_counted_base
{
private:

    P ptr; // copy constructor must not throw
    D del; // copy constructor must not throw

    sp_counted_base_impl(sp_counted_base_impl const &);
    sp_counted_base_impl & operator= (sp_counted_base_impl const &);

    typedef sp_counted_base_impl<P, D> this_type;

public:

    // pre: initial_use_count <= initial_weak_count, d(p) must not throw

    sp_counted_base_impl(P p, D d): ptr(p), del(d)
    {
    }

    virtual void dispose() // nothrow
    {
        del(ptr);
    }

    virtual void * get_deleter(std::type_info const & ti)
    {
        return ti == typeid(D)? &del: 0;
    }

    void * operator new(XBOOST_SIZE_T_INTERNAL)
    {
        return std::allocator<this_type>().allocate(1, static_cast<this_type *>(0));
    }

    void operator delete(void * p)
    {
        std::allocator<this_type>().deallocate(static_cast<this_type *>(p), 1);
    }
};

class weak_count;

class shared_count
{
private:

    sp_counted_base * pi_;

    friend class weak_count;

public:

    shared_count(): pi_(0) // nothrow
    {
    }

    template<class P, class D> shared_count(P p, D d): pi_(0)
    {
        try
        {
            pi_ = new sp_counted_base_impl<P, D>(p, d);
        }
        catch(...)
        {
            d(p); // delete p
            throw;
        }

    }

    // auto_ptr<Y> is special cased to provide the strong guarantee

    template<class Y>
    explicit shared_count(std::auto_ptr<Y> & r): pi_(new sp_counted_base_impl< Y *, checked_deleter<Y> >(r.get(), checked_deleter<Y>()))
    {
        r.release();
    }

    ~shared_count() // nothrow
    {
        if(pi_ != 0) pi_->release();
    }

    shared_count(shared_count const & r): pi_(r.pi_) // nothrow
    {
        if(pi_ != 0) pi_->add_ref();
    }

    explicit shared_count(weak_count const & r); // throws bad_weak_ptr when r.use_count() == 0

    shared_count & operator= (shared_count const & r) // nothrow
    {
        sp_counted_base * tmp = r.pi_;
        if(tmp != 0) tmp->add_ref();
        if(pi_ != 0) pi_->release();
        pi_ = tmp;

        return *this;
    }

    void swap(shared_count & r) // nothrow
    {
        sp_counted_base * tmp = r.pi_;
        r.pi_ = pi_;
        pi_ = tmp;
    }

    long use_count() const // nothrow
    {
        return pi_ != 0? pi_->use_count(): 0;
    }

    bool unique() const // nothrow
    {
        return use_count() == 1;
    }

    friend inline bool operator==(shared_count const & a, shared_count const & b)
    {
        return a.pi_ == b.pi_;
    }

    friend inline bool operator<(shared_count const & a, shared_count const & b)
    {
        return std::less<sp_counted_base *>()(a.pi_, b.pi_);
    }

    void * get_deleter(std::type_info const & ti) const
    {
        return pi_? pi_->get_deleter(ti): 0;
    }
};

#ifdef __CODEGUARD__
# pragma option pop
#endif


class weak_count
{
private:

    sp_counted_base * pi_;

    friend class shared_count;

public:

    weak_count(): pi_(0) // nothrow
    {
    }

    weak_count(shared_count const & r): pi_(r.pi_) // nothrow
    {
        if(pi_ != 0) pi_->weak_add_ref();
    }

    weak_count(weak_count const & r): pi_(r.pi_) // nothrow
    {
        if(pi_ != 0) pi_->weak_add_ref();
    }

    ~weak_count() // nothrow
    {
        if(pi_ != 0) pi_->weak_release();
    }

    weak_count & operator= (shared_count const & r) // nothrow
    {
        sp_counted_base * tmp = r.pi_;
        if(tmp != 0) tmp->weak_add_ref();
        if(pi_ != 0) pi_->weak_release();
        pi_ = tmp;

        return *this;
    }

    weak_count & operator= (weak_count const & r) // nothrow
    {
        sp_counted_base * tmp = r.pi_;
        if(tmp != 0) tmp->weak_add_ref();
        if(pi_ != 0) pi_->weak_release();
        pi_ = tmp;

        return *this;
    }

    void swap(weak_count & r) // nothrow
    {
        sp_counted_base * tmp = r.pi_;
        r.pi_ = pi_;
        pi_ = tmp;
    }

    long use_count() const // nothrow
    {
        return pi_ != 0? pi_->use_count(): 0;
    }

    friend inline bool operator==(weak_count const & a, weak_count const & b)
    {
        return a.pi_ == b.pi_;
    }

    friend inline bool operator<(weak_count const & a, weak_count const & b)
    {
        return std::less<sp_counted_base *>()(a.pi_, b.pi_);
    }
};

inline shared_count::shared_count(weak_count const & r): pi_(r.pi_)
{
    if(pi_ != 0)
    {
        pi_->add_ref();
    }
    else
    {
        throw xboost::bad_weak_ptr();
    }
}

} // namespace detail

} // namespace xboost

#ifdef __BORLANDC__
# pragma warn .8027     // Functions containing try are not expanded inline
# pragma warn .8026     // Functions with excep. spec. are not expanded inline
#endif

#undef XBOOST_SIZE_T_INTERNAL

#endif  // #ifndef XBOOST_SHARED_COUNT_HPP_INCLUDED

