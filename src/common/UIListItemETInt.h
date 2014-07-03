//
//  UIListItemETInt.h
//  test
//
//  Created by Wayne on 14-1-4.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __test__UIListItemETInt__
#define __test__UIListItemETInt__

#include "vnUIListItem.h"
#include "vnUITextField.h"
#include "vnEditType.h"

class UIListItemETInt : public vn::UIListItemText, public vn::EditValue::Observer {
	VN_DECL_UI_LOGIC()
public:
	UIListItemETInt();
	UIListItemETInt(const UIListItemETInt &other);
	virtual UIListItemETInt * clone() const;
	virtual ~UIListItemETInt();
	
	virtual void ev_destroyed(vn::EditValue *);
	virtual void ev_changed(vn::EditValue *);
	
	void setTarget(vn::EditValueInt *value);
	void setEditable(bool editable);
	
protected:
    virtual bool _startup(vn::UIFactory::ElementMap &namedElements);
	
	vn::UITextField *m_editbox; // named "editbox"
	
	vn::EditValueInt *m_target;
	
	bool m_editable;
	
	void _onEditBoxGotFocus(vn::UIElement *);
	void _onEditBoxLoseFocus(vn::UIElement *);
	void _onEditBoxString(vn::UIElement *, const vn::str32 &);
	virtual void _setValue(vn::s32 value);
};

#endif /* defined(__test__UIListItemETInt__) */
