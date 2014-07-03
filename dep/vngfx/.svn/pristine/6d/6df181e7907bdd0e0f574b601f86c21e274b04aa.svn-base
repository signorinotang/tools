//
//  vnGfxApplicationImpl.h
//  vngfx
//
//  Created by Wayne on 13-12-4.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __vngfx__vnGfxApplicationImpl__
#define __vngfx__vnGfxApplicationImpl__

#include "vnGfxApplication.h"
#include "vnAllocatedObject.h"
#include "vnMutex.h"

_vn_begin

namespace internal {
    
    class GfxApplicationImpl : public AllocatedObject {
    public:
        GfxApplicationImpl(GfxApplication::Delegate *delegate);
        ~GfxApplicationImpl(); 
		void setDelegate(GfxApplication::Delegate *delegate);
		void * createWindow(u32 width, u32 height, const char* title);
        int run();
		void quit(int code);
		vector2f getMousePosition();

		bool isTextInputEnabled();
		void setTextInputEnabled(bool enabled);
		void setTextInputPosition(f32 x, f32 y);

		void thread_init();
		void thread_lock();
		void thread_unlock();

		void performUndo();
		void performRedo();
		void performCut();
		void performCopy();
		void performPaste();

		vector2f m_mousePosition;
    private:
		static void _testThread(void *parameters);
		static GfxApplicationImpl *m_self;
		static LRESULT CALLBACK windowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static void onMouseMessage(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static void onKeyboardMessage(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
		
		volatile bool			m_quit;
		TRACKMOUSEEVENT m_track;
		HDC             m_hDC;
		HGLRC           m_hRC;
		HWND            m_wnd;
		HANDLE          m_hThread;   
		MSG		        m_msg;
		HIMC            m_hIMC;
		f32             m_deltaTime;
		f32             m_delay;
		f32             m_interval;
		bool            m_isTextInputEnabled;
		GfxApplication::Delegate  *m_delegate;	
		Mutex m_mutex;
		u32 m_width, m_height;
    };
    
}

_vn_end


#endif /* defined(__vngfx__vnGfxApplicationImpl__) */
