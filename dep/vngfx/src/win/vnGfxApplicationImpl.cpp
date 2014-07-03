//
//  vnGfxApplicationImpl.cpp
//  vngfx
//
//  Created by Wayne on 13-12-4.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnGfxApplicationImpl.h"
#include <Windows.h>
#include <process.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Imm32.lib")




_vn_begin

namespace internal {

	GfxApplicationImpl* GfxApplicationImpl::m_self = 0;
	

	GfxApplicationImpl::GfxApplicationImpl(GfxApplication::Delegate *delegate)
		: m_delegate(delegate)
		, m_deltaTime(0.f)
		, m_hDC(NULL)
		, m_hRC(NULL)
		, m_wnd(NULL)
		, m_delay(0)
		, m_interval(0.0166666666666f)
		, m_isTextInputEnabled(false)
		, m_mousePosition(vector2f(0,0))
		, m_height(0)
		, m_width(0)
		, m_quit(false)
		{
			m_self = this;
		}

	GfxApplicationImpl::~GfxApplicationImpl() {

	}

	void GfxApplicationImpl::setDelegate(GfxApplication::Delegate *delegate) {
		m_delegate = delegate;
		
	}

	void * GfxApplicationImpl::createWindow(u32 width, u32 height, const char* title)
	{
		
		m_width = width;
		m_height = height;

		if (m_wnd){
			::SetWindowPos(m_wnd, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
			::SetWindowTextA(m_wnd, title);
			return m_wnd;
		}

	
		//m_wndThreadId = ::GetCurrentThreadId();

		WNDCLASSW wc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = windowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = ::GetModuleHandleW(NULL);
		wc.hIcon = LoadIconW(0, IDI_APPLICATION);
		wc.hCursor = LoadCursorW(0, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wc.lpszMenuName = 0;
		wc.lpszClassName = L"VN_GFX_WND";

		if (!::RegisterClassW(&wc))
		{
			return 0;
		}
		
		//DWORD wndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU ;
		DWORD wndStyle = WS_OVERLAPPEDWINDOW;

		RECT r = { 0, 0, (LONG)width, (LONG)height };
		::AdjustWindowRect(&r, wndStyle, FALSE);
		r.right -= r.left;
		r.bottom -= r.top;


		wchar_t buf[256];
		swprintf_s(buf, L"%S", title);

		m_wnd = ::CreateWindowW(
			wc.lpszClassName
			, buf
			, wndStyle
			, CW_USEDEFAULT
			, CW_USEDEFAULT
			, r.right
			, r.bottom
			, 0
			, 0
			, wc.hInstance
			, 0);

		if (!m_wnd)	{
			return 0;
		}
	
		m_track.cbSize = sizeof(TRACKMOUSEEVENT);
		m_track.dwFlags = TME_LEAVE;
		m_track.hwndTrack = m_wnd;
		m_track.dwHoverTime = HOVER_DEFAULT;
		
		BOOL isTrack = ::TrackMouseEvent(&m_track); 

		if (!isTrack)
		{
			printf("Mouse Event Track filed");
		}		
		CreateCaret(m_wnd, NULL, 0, 0);
		HideCaret(m_wnd);

		//forbidden HIMC
		m_hIMC = ::ImmAssociateContext(m_wnd, NULL);

        return m_wnd;
    }
   

 
    int GfxApplicationImpl::run() {

		if (!m_wnd)
		{
			return 0;
		}
		m_delegate->initialize(m_wnd, m_width, m_height);

		ShowWindow(m_wnd, SW_NORMAL);

		::timeBeginPeriod(1);
		u32 oTime = ::timeGetTime();
		u32 nTime = 0;

		//_beginthread(_testThread, 0, NULL);
		/*for (;;) { 	
			if (::GetMessageW(&m_msg, 0, 0, 0)) {		
				::TranslateMessage(&m_msg);
				::DispatchMessageW(&m_msg);
			}
			if (m_msg.message == WM_QUIT) {
				quit(WM_QUIT);
				break;
			}
		}*/

		for (;;) {
			if (::PeekMessageW(&m_msg, NULL, 0, 0, PM_REMOVE)) {
				if (m_msg.message == WM_QUIT) {
					quit(WM_QUIT);
					break;
				}
				::TranslateMessage(&m_msg);
				::DispatchMessageW(&m_msg);
			}
			else {
				u32 nTime = ::timeGetTime();
				m_self->m_deltaTime = (nTime - oTime) / 1000.f;
				m_self->m_delay = m_self->m_deltaTime;
				if (m_self->m_delay > m_self->m_interval)
				{
					oTime = nTime;
					m_self->thread_lock();
					m_self->m_delegate->update(m_self->m_deltaTime);
					m_self->m_delegate->draw();
					m_self->thread_unlock();
				}
				else
				{
					::Sleep(1);
				}		
			} 
		}

	

		::timeEndPeriod(1);	
        return 0;
    }

	void GfxApplicationImpl::_testThread(void *parameters) {
		u32 oTime = ::timeGetTime();
		u32 nTime = 0;
		while (!(m_self->m_quit)) {
			u32 nTime = ::timeGetTime();
			m_self->m_deltaTime = (nTime - oTime) / 1000.f;
			m_self->m_delay = m_self->m_deltaTime;
			if (m_self->m_delay > m_self->m_interval)
			{
				oTime = nTime;
				m_self->thread_lock();
				m_self->m_delegate->update(m_self->m_deltaTime);
				m_self->m_delegate->draw();
				m_self->thread_unlock();
			}
			else
			{
				::Sleep(1);
			}
		}
		return;
	}

    void GfxApplicationImpl::quit(int code) {
		m_quit = true;
		m_delegate->finalize();
		if (m_wnd) {
			::DestroyWindow(m_wnd);
			m_wnd = 0;
		}
    }

	vector2f GfxApplicationImpl::getMousePosition()
	{
		return m_mousePosition;
	}

	void GfxApplicationImpl::performUndo() {
		m_delegate->do_undo();
	}

	void GfxApplicationImpl::performRedo() {
		m_delegate->do_undo();
	}

	void GfxApplicationImpl::performCut() {

	}

	void GfxApplicationImpl::performCopy() {

	}

	void GfxApplicationImpl::performPaste() {

	}

//window event
 LRESULT CALLBACK GfxApplicationImpl::windowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		static HDC hDC;
		static HGLRC hRC;
		
		int width, height;

		switch (msg)
		{
		case WM_PAINT:
			m_self->thread_lock();
			m_self->m_delegate->draw();
			m_self->thread_unlock();
			break;
		case WM_ERASEBKGND:
			return true;
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_SIZE:
			width = LOWORD(lparam);
			height = HIWORD(lparam);
			m_self->m_delegate->resize(width, height);
			break;
		default:
			break;
		}

		if (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST || msg == WM_MOUSELEAVE) {
			onMouseMessage(wnd, msg, wparam, lparam);
		}
		else if (msg >= WM_KEYFIRST && msg <= WM_KEYLAST) {
			onKeyboardMessage(wnd, msg, wparam, lparam);
		}	
		return DefWindowProc(wnd, msg, wparam, lparam);
	}


	//mouse event
	void GfxApplicationImpl::onMouseMessage(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		static s32 capture = 0;
		static bool ldown = false;
		static bool rdown = false;
		static bool leave = false;

		switch (msg)
		{
		case WM_MOUSELEAVE:
			{
				if (!leave)
				{
					leave = true;
				}
			}
		    break;
		case WM_LBUTTONDOWN:
			{	
				if (capture++ == 0)
				{
					::SetCapture(wnd);
				}
				ldown = true;
				m_self->m_delegate->mouse_button_down(0, vector2f((short)LOWORD(lparam), (short)HIWORD(lparam)));
			}
			break;
		case WM_LBUTTONUP:
			{
				
				if (ldown)
				{
					ldown = false;
					if (--capture == 0) 
					{
						::ReleaseCapture();
					}
					m_self->m_delegate->mouse_button_up(0, vector2f((short)LOWORD(lparam), (short)HIWORD(lparam)));
				}

			}
			break;
		case WM_RBUTTONDOWN:
			{
				if (capture++ == 0) {
					::SetCapture(wnd);
				}
				rdown = true;
				m_self->m_delegate->mouse_button_down(1, vector2f((short)LOWORD(lparam), (short)HIWORD(lparam)));
			}
			break;
		case WM_RBUTTONUP:
			{
				if (rdown) {
					if (--capture == 0) { 
						::ReleaseCapture();
					}
					rdown = false;
					m_self->m_delegate->mouse_button_up(1, vector2f((short)LOWORD(lparam), (short)HIWORD(lparam)));
				}
			}
			break;
		case WM_MOUSEMOVE:
			{
				if (leave)
				{	
					leave = false;
					::TrackMouseEvent(&(m_self->m_track));
				}
				m_self->m_mousePosition = vector2f((short)LOWORD(lparam), (short)HIWORD(lparam));
			}
			break;
		case WM_MOUSEWHEEL:
			{
				POINT point = { (short)LOWORD(lparam), (short)HIWORD(lparam) };
				::ScreenToClient(wnd, &point);
				m_self->m_delegate->mouse_wheel(vector2f((short)point.x, (short)point.y), vector2f(0, (short)HIWORD(wparam) / 40.f));
			}
			break;
		case WM_MOUSEHWHEEL:
			{
				POINT point = { (short)LOWORD(lparam), (short)HIWORD(lparam) };
				::ScreenToClient(wnd, &point);
				m_self->m_delegate->mouse_wheel(vector2f((short)point.x, (short)point.y), vector2f((short)HIWORD(wparam) / 40.f, 0));

			}
			break;
		}
	
	}

	//keyboard event
	void GfxApplicationImpl::onKeyboardMessage(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg) {
		case WM_KEYDOWN:
		{	
			if (m_self->m_isTextInputEnabled)
			{
				if (wparam == 46)
				{
					m_self->m_delegate->text_delete_forward();
				}
				else if (wparam == 37)
				{
					m_self->m_delegate->text_move_left();
				}
				else if (wparam == 39)
				{
					m_self->m_delegate->text_move_right();
				}
				else if (wparam == 38)
				{
					m_self->m_delegate->text_move_up();
				}
				else if (wparam == 40)
				{
					m_self->m_delegate->text_move_down();
				}
			}

			if (lparam & (1 << 30))
			{
				m_self->m_delegate->key_down((c32)wparam, true);
			}
			else
			{	
				m_self->m_delegate->key_down((c32)wparam, false);
			}	
		}
			break;
		case WM_KEYUP:
		{
			m_self->m_delegate->key_up((c32)wparam);
		}
			break;
		case WM_CHAR:
		{
			if (m_self->m_isTextInputEnabled)
			{
				if (wparam >= 0x20)
				{
					str32 text;
					text.push_back((c32)wparam);
					m_self->m_delegate->text_insert(text);
				}
				else if (wparam == '\r')
				{
					m_self->m_delegate->text_new_line();
				}
				else if (wparam == '\b')
				{
					m_self->m_delegate->text_delete_backward();
				}
			}
			if (wparam == 25) {
				// ctrl + y
				m_self->m_delegate->do_redo();
			} else if (wparam == 26) {
				// ctrl + z
				m_self->m_delegate->do_undo();
			}
			break;
		}

		}
	}
	

	bool GfxApplicationImpl::isTextInputEnabled()
	{
		return m_isTextInputEnabled;
	}

	void GfxApplicationImpl::setTextInputEnabled(bool enabled)
	{
		if (enabled)
		{
			::ImmAssociateContext(m_wnd, m_hIMC);
			m_isTextInputEnabled = true;
		}
		else
		{
			m_hIMC = ::ImmAssociateContext(m_wnd, NULL);
			m_isTextInputEnabled = false;
		} 
	}

	void GfxApplicationImpl::setTextInputPosition(f32 x, f32 y)
	{
		::SetCaretPos(int(x), int(y));
	}

	void GfxApplicationImpl::thread_init()
	{
		/*m_hRC = ::wglCreateContext(m_hDC);
		wglMakeCurrent(m_hDC, m_hRC);*/
	}

	void GfxApplicationImpl::thread_lock()
	{
		m_mutex.lock();
	}

	void GfxApplicationImpl::thread_unlock()
	{
		m_mutex.unlock();
	}    
}
_vn_end