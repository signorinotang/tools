//
//  vnFunction3.h
//  vnbase
//
//  Created by Wayne on 13-6-5.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnFunction3_h
#define _vnFunction3_h

#include "vnFunctionBase.h"

_vn_begin

namespace internal {
	
	template <typename R, typename P0, typename P1, typename P2>
	class FuncImpl3 : virtual public RefCounted {
	public:
		virtual R operator ()(P0, P1, P2) const = 0;
	};
	
	template <typename F, typename R, typename P0, typename P1, typename P2>
	class FuncHandler3 : public FuncImpl3<R, P0, P1, P2> {
	public:
		FuncHandler3(const F &func) : m_func(func) {}
		virtual R operator ()(P0 p0, P1 p1, P2 p2) const {
			return m_func( p0, p1, p2 );
		};
	private:
		mutable F m_func;
	};
	
	template <typename T, typename F, typename R, typename P0, typename P1, typename P2>
	class MemFuncHandler3 : public FuncImpl3<R, P0, P1, P2> {
	public:
		MemFuncHandler3(const T &ptr, const F &func) : m_ptr(ptr), m_func(func) {}
		virtual R operator ()(P0 p0, P1 p1, P2 p2) const {
			return ((*m_ptr).*m_func)(p0, p1, p2);
		}
	private:
		mutable T m_ptr;
		mutable F m_func;
	};
	
	template <class T, typename R, typename P0, typename P1, typename P2>
	struct MemFuncTraits<R (T::*)(P0, P1, P2)> {
		typedef T ObjectType;
		typedef function<R(P0, P1, P2)> FunctionType;
	};
	
	template <class T, typename R, typename P0, typename P1, typename P2>
	struct MemFuncTraits<R (T::*)(P0, P1, P2) const> {
		typedef const T ObjectType;
		typedef function<R(P0, P1, P2)> FunctionType;
	};
	
} // namespace internal

template <typename R, typename P0, typename P1, typename P2>
class function<R(P0, P1, P2)> {
public:
	typedef internal::FuncImpl3<R, P0, P1, P2> impl_type;
	
	function() {}
    function(int) {}
	
	template <typename F>
	function(const F &func) : m_func(vnnew internal::FuncHandler3<F, R, P0, P1, P2>(func)) {}
	
	template <typename T, typename F>
	function(const T &ptr, const F &func) : m_func(vnnew internal::MemFuncHandler3<T, F, R, P0, P1, P2>(ptr, func)) {}
	
    explicit function(impl_type *impl) : m_func(impl) {}
    
	function(const function &other) : m_func(other.m_func) {}
	
	function & operator = (const function &rhs) {
		m_func = rhs.m_func;
		return *this;
	}
	
	R operator ()(P0 p0, P1 p1, P2 p2) const {
		return (*m_func)(p0, p1, p2);
	}
	
	bool null() const {
		return m_func.null();
	}
	
	void release() {
		m_func.setNull();
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