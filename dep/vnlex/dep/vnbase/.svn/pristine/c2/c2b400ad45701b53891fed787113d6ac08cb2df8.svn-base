//
//  vnFunction4.h
//  vnbase
//
//  Created by Wayne on 13-6-5.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnFunction4_h
#define _vnFunction4_h

#include "vnFunctionBase.h"

_vn_begin

namespace internal {
	
	template <typename R, typename P0, typename P1, typename P2, typename P3>
	class FuncImpl4 : virtual public RefCounted {
	public:
		virtual R operator ()(P0, P1, P2, P3) const = 0;
	};
	
	template <typename F, typename R, typename P0, typename P1, typename P2, typename P3>
	class FuncHandler4 : public FuncImpl4<R, P0, P1, P2, P3> {
	public:
		FuncHandler4(const F &func) : m_func(func) {}
		virtual R operator ()(P0 p0, P1 p1, P2 p2, P3 p3) const {
			return m_func(p0, p1, p2, p3);
		};
	private:
		mutable F m_func;
	};
	
	template <typename T, typename F, typename R, typename P0, typename P1, typename P2, typename P3>
	class MemFuncHandler4 : public FuncImpl4<R, P0, P1, P2, P3> {
	public:
		MemFuncHandler4(const T &ptr, const F &func) : m_ptr(ptr), m_func(func) {}
		virtual R operator ()(P0 p0, P1 p1, P2 p2, P3 p3) const {
			return ((*m_ptr).*m_func)(p0, p1, p2, p3);
		}
	private:
		mutable T m_ptr;
		mutable F m_func;
	};
	
	template <class T, typename R, typename P0, typename P1, typename P2, typename P3>
	struct MemFuncTraits<R (T::*)(P0, P1, P2, P3)> {
		typedef T ObjectType;
		typedef function<R(P0, P1, P2, P3)> FunctionType;
	};
	
	template <class T, typename R, typename P0, typename P1, typename P2, typename P3>
	struct MemFuncTraits<R (T::*)(P0, P1, P2, P3) const> {
		typedef const T ObjectType;
		typedef function<R(P0, P1, P2, P3)> FunctionType;
	};
	
} // namespace internal

template <typename R, typename P0, typename P1, typename P2, typename P3>
class function<R(P0, P1, P2, P3)> {
public:
	typedef internal::FuncImpl4<R, P0, P1, P2, P3> impl_type;
	
	function() {}
    function(int) {}
	
	template <typename F>
	function(const F &func) : m_func(vnnew internal::FuncHandler4<F, R, P0, P1, P2, P3>(func)) {}
	
	template <typename T, typename F>
	function(const T &ptr, const F &func) : m_func(vnnew internal::MemFuncHandler4<T, F, R, P0, P1, P2, P3>(ptr, func)) {}
	
    explicit function(impl_type *impl) : m_func(impl) {}
    
	function(const function &other) : m_func(other.m_func) {}
	
	function & operator = (const function &rhs) {
		m_func = rhs.m_func;
		return *this;
	}
	
	R operator ()(P0 p0, P1 p1, P2 p2, P3 p3) const {
		return (*m_func)(p0, p1, p2, p3);
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
		m_func.swap(other);
	}
	
	impl_type * impl_ptr() const {
		return m_func.ptr();
	}
	
private:
	RefCountedPtr<impl_type> m_func;
};


_vn_end

#endif