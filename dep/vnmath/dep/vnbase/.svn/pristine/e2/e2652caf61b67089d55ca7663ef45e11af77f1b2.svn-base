//
//  vnFunction1.h
//  vnbase
//
//  Created by Wayne on 13-6-5.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnFunction1_h
#define _vnFunction1_h

#include "vnFunctionBase.h"

_vn_begin

namespace internal {
	
	template <typename R, typename P0>
	class FuncImpl1 : virtual public RefCounted {
	public:
		virtual R operator ()(P0) const = 0;
	};
	
	template < typename F, typename R, typename P0 >
	class FuncHandler1 : public FuncImpl1<R, P0> {
	public:
		FuncHandler1(const F &func) : m_func(func) {}
		virtual R operator ()(P0 p0) const {
			return m_func(p0);
		};
	private:
		mutable F m_func;
	};
	
	template <typename T, typename F, typename R, typename P0>
	class MemFuncHandler1 : public FuncImpl1<R, P0> {
	public:
		MemFuncHandler1(const T &ptr, const F &func) : m_ptr(ptr), m_func(func) {}
		virtual R operator ()(P0 p0) const {
			return ((*m_ptr).*m_func)(p0);
		}
		
	private:
		mutable T m_ptr;
		mutable F m_func;
	};
	
	template <class T, typename R, typename P0>
	struct MemFuncTraits<R (T::*)(P0)> {
		typedef T ObjectType;
		typedef function<R(P0)> FunctionType;
	};
	
	template <class T, typename R, typename P0>
	struct MemFuncTraits<R (T::*)(P0) const> {
		typedef const T ObjectType;
		typedef function<R(P0)> FunctionType;
	};
	
} // namespace internal

template <typename R, typename P0>
class function<R(P0)> {
public:
	typedef internal::FuncImpl1<R, P0> impl_type;
	
	function() {}
    function(int) {}
	
	template <typename F>
	function(const F &func) : m_func(vnnew internal::FuncHandler1<F, R, P0>(func)) {}
	
	template <typename T, typename F>
	function(const T &ptr, const F &func) : m_func(vnnew internal::MemFuncHandler1<T, F, R, P0>( ptr, func ) ) {}
	
    explicit function(impl_type *impl) : m_func(impl) {}
    
	function(const function &other) : m_func(other.m_func) {}
	
	function & operator = (const function &rhs) {
		m_func = rhs.m_func;
		return *this;
	}
	
	R operator ()(P0 p0) const {
		return (*m_func)(p0);
	}
	
	bool null() const {
		return m_func.null();
	}
	
	void release() {
		m_func.release();
	}
    
    void set(impl_type *impl, bool grab = true) {
        if (impl && grab) {
            impl->grab();
        }
        m_func = impl;
    }
    
    void set(RefCounted *impl, bool grab = true) {
        set(dynamic_cast<impl_type *>(impl), grab);
    }
	
	void swap(function &other) {
		m_func.swap( other );
	}
	
	impl_type * impl_ptr() const {
		return m_func.ptr();
	}
	
private:
	RefCountedPtr<impl_type> m_func;
};


_vn_end

#endif