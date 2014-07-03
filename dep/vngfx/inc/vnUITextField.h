//
//  vnUITextField.h
//  vngfx
//
//  Created by Wayne on 13-12-20.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUITextField_h
#define vngfx_vnUITextField_h

#include "vnUIElement.h"
#include "vnUIRenderText.h"
#include "vnColor.h"
#include "vnUIRoot.h"

_vn_begin

class _vn_gfx_api UITextField : public UIElement, public UIRoot::MouseEventHook {
    VN_DECL_UI_ELEMENT()
public:
    // ctor & dtor
    UITextField();
    virtual UITextField * clone() const;
    
    
    // properties
    void setFont(const FontPtr &font);
    const FontPtr & getFont() const;
    void setText(const str8 &text);
    void setText(const str32 &text);
    const str32 & getText() const;
    void setTextColor(const color_t &color);
    const color_t & getTextColor() const;
    void setSelectedTextColor(const color_t &color);
    const color_t & getSelectedTextColor() const;
    void setMarkedTextColor(const color_t &color);
    const color_t & getMarkedTextColor() const;
    void setSelectionColor(const color_t &color);
    const color_t & getSelectionColor() const;
    
    void setTextAlignment(u32 alignment);
    u32 getTextAlignment() const;
    
    virtual void setClipping(bool clipping);
    
	void setAutoReleaseFocus(bool autoReleaseFocus);
	bool isAutoReleaseFocus() const;
    
    void setReadOnly(bool readOnly);
    bool isReadOnly() const;
	
    // methods
    virtual void init(const TreeDataObject *object);
	//	named "string"
	void bindAction_TextCompleted(const function<void(UIElement *, const str32 &)> &func);
	//	named "focus_in"
	void bindAction_FocusIn(const function<void(UIElement *)> &func);
	//	named "focus_out"
	void bindAction_FocusOut(const function<void(UIElement *)> &func);
	
	void selectAll();
	void selectNone();
	
    // events
    virtual void onMouseLeftDown(const vector2f &position);
    virtual void onMouseLeftUp(const vector2f &position);
    virtual void onMouseCaptureCancelled();
    
    virtual void onFocusIn();
    virtual void onFocusOut();
    
    virtual void onTextInsert(const str32 &text);
    virtual void onTextMark(const str32 &text, u32 caretPos);
    virtual void onTextUnmark();
    virtual void onTextNewLine();
    virtual void onTextDeleteBackward();
    virtual void onTextDeleteForward();
	virtual void onTextMoveLeft();
	virtual void onTextMoveRight();
	
	virtual void hookMouseLeftDown(UIElement *target, const vector2f &pos);
	virtual void hookMouseLeftUp(UIElement *target, const vector2f &pos);
	virtual void hookMouseRightDown(UIElement *target, const vector2f &pos);
	virtual void hookMouseRightUp(UIElement *target, const vector2f &pos);
	
protected:
    
    virtual void _onUpdate(f32 deltaTime);
    virtual void _onRender(UIRenderer *renderer);
	virtual bool _bindAction(const c8 *name, RefCounted *func_impl);

	bool m_autoReleaseFocus;
    UIRenderText m_renderText;
    str32 m_text;
    str32 m_markedText;
    u32 m_textAlignment;
    size_t m_caretIndex, m_insertIndex, m_selectionIndex, m_selectionLength;
    size_t m_renderIndex0, m_renderIndex1, m_renderIndex2, m_renderIndex3;
    
    color_t m_textColor, m_selectedTextColor, m_markedTextColor;
    color_t m_selectionColor;
    
    bool m_readOnly;
    
    bool m_hasFocus;
    bool m_mouseDown;
    f32 m_caretTimer;
    
    bool m_textDirty;
    vector2f m_textPosition;
    color_t m_caretColor;
    aabox2f m_caretRect;
	aabox2f m_selectionRect;
	
	function<void(UIElement *, const str32 &)> m_fnTextCompleted;
	function<void(UIElement *)> m_fnFocusIn, m_fnFocusOut;
	
	void _updateText();
};

_vn_end

#endif
