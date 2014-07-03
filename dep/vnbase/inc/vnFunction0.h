//
//  vnFunction0.h
//  vnbase
//
//  Created by Wayne on 13-6-5.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnFunction0_h
#define _vnFunction0_h

#include "vnFunctionBase.h"

_vn_begin

namespace internal {
	
	template <typename R>
	class FuncImpl0 : virtual public RefCounted {
	public:
		virtual R operator ()(void) const = 0;
	};
	
	template <typename F, typename R>
	class FuncHandler0 : public FuncImpl0<R> {
	public:
		FuncHandler0(const F &func) : m_func(func) {}
		virtual R operator ()(void) const {
			return m_func();
		};
	private:
		mutable F m_func;
	};
	
	template <typename T, typename F, typename R>
	class MemFuncHandler0 : public FuncImpl0<R> {
	public:
		MemFuncHandler0(const T &ptr, const F &func) : m_ptr(ptr), m_func(func) {}
		virtual R operator ()(void) const {
			return ((*m_ptr).*m_func)();
		}
		
	private:
		mutable T m_ptr;
		mutable F m_func;
	};
	
	template <class T, typename R>
	struct MemFuncTraits<R (T::*)(void)> {
		typedef T ObjectType;
		typedef function<R()> FunctionType;
	};
	
	template <class T, typename R>
	struct MemFuncTraits<R (T::*)(void) const> {
		typedef const T ObjectType;
		typedef function<R()> FunctionType;
	};
	
} // namespace internal

template <typename R>
class function<R()> {
public:
	typedef internal::FuncImpl0<R> impl_type;
	
	function() {}
    function(int) {}
    
	template <typename F>
	function(const F &func) : m_func(vnnew internal::FuncHandler0<F, R>(func)) {}
	
	template <typename T, typename F>
	function(const T &ptr, const F &func) : m_func(vnnew internal::MemFuncHandler0<T, F, R>(ptr, func)) {}
	
    explicit function(impl_type *impl) : m_func(impl) {}
    
	function(const function &other) : m_func(other.m_func) {}
	
	function & operator = (const function &rhs) {
		m_func = rhs.m_func;
		return *this;
	}
	
	R operator ()(void) const {
		return (*m_func)();
	}
	
	bool null() const {
		return m_func.null();
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
    
	void release() {
		m_func.release();
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