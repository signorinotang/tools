//
//  vnUIRoot.h
//  vngfx
//
//  Created by Wayne on 13-12-18.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUIRoot_h
#define vngfx_vnUIRoot_h

#include "vnUIRenderer.h"
#include "vnSingleton.h"

#include <vector>

_vn_begin

class UIElement;

class _vn_gfx_api UIRoot : public Singleton<UIRoot> {
public:
	class MouseEventHook {
	public:
		virtual void hookMouseLeftDown(UIElement *target, const vector2f &pos) {}
		virtual void hookMouseLeftUp(UIElement *target, const vector2f &pos) {}
		virtual void hookMouseRightDown(UIElement *target, const vector2f &pos) {}
		virtual void hookMouseRightUp(UIElement *target, const vector2f &pos) {}
		virtual void hookMouseWheel(const UIElement *target, const vector2f &pos, const vector2f &delta) {}
	};
	
    UIRoot();
    ~UIRoot();
    
    void update(f32 deltaTime);
	void render();
    
	void setRootElement(UIElement *root, bool grab = true);
    UIElement * getRootElement();
	
    const vector2f & getViewSize();
    void setViewSize(const vector2f &size);
    
	void hookMouseEvent(MouseEventHook *hook);
	void unhookMouseEvent(MouseEventHook *hook);
	
    bool dispath_mouseLeftDown(const vector2f &pos);
    bool dispath_mouseLeftUp(const vector2f &pos);
    bool dispath_mouseRightDown(const vector2f &pos);
    bool dispath_mouseRightUp(const vector2f &pos);
    bool dispath_mouseWheel(const vector2f &pos, const vector2f &delta);
    void dispath_keyDown(c32 chr, bool repeat);
    void dispath_keyUp(c32 chr);
    void dispath_textInsert(const str32 &text);
    void dispath_textMark(const str32 &text, u32 caretPos);
    void dispath_textUnmark();
    void dispath_textNewLine();
    void dispath_textDeleteBackward();
    void dispath_textDeleteForward();
	void dispath_textMoveUp();
	void dispath_textMoveDown();
	void dispath_textMoveLeft();
	void dispath_textMoveRight();
    
    void reset();
    
    void captureMouse(UIElement *element);
    void releaseMouse(UIElement *element);
    
    void requireFocus(UIElement *element);
    void releaseFocus(UIElement *element);
    
    void _notify_disappeared(UIElement *element);
private:
    UIRenderer m_renderer;
    UIElement *m_root;
	UIElement *m_focus;
    UIElement *m_hover;
    UIElement *m_capture;
    vector2f m_viewSize;
    typedef std::vector<MouseEventHook *> MouseEventHooks;
	MouseEventHooks m_mouseEventHooks;
	
    void _updateMouse();
	void _sendMouseLeftDown(UIElement *target, const vector2f &pos);
	void _sendMouseLeftUp(UIElement *target, const vector2f &pos);
	void _sendMouseRightDown(UIElement *target, const vector2f &pos);
	void _sendMouseRightUp(UIElement *target, const vector2f &pos);
	void _sendMouseWheel(UIElement *target, const vector2f &pos, const vector2f &delta);
	
};

_vn_end


#endif