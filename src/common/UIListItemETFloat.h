//
//  UIListItemETFloat.h
//  test
//
//  Created by Wayne on 14-1-4.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __test__UIListItemETFloat__
#define __test__UIListItemETFloat__

#include "vnUIListItem.h"
#include "vnUITextField.h"
#include "vnEditType.h"

class UIListItemETFloat : public vn::UIListItemText, public vn::EditValue::Observer {
	VN_DECL_UI_LOGIC()
public:
	UIListItemETFloat();
	UIListItemETFloat(const UIListItemETFloat &other);
	virtual UIListItemETFloat * clone() const;
	virtual ~UIListItemETFloat();
	
	virtual void ev_destroyed(vn::EditValue *);
	virtual void ev_changed(vn::EditValue *);
	
	void setTarget(vn::EditValueFloat *value);
	void setEditable(bool editable);
	
protected:
    virtual bool _startup(vn::UIFactory::ElementMap &namedElements);
	
	vn::UITextField *m_editbox; // named "editbox"
	
	vn::EditValueFloat *m_target;
	
	bool m_editable;
	
	void _onEditBoxGotFocus(vn::UIElement *);
	void _onEditBoxLoseFocus(vn::UIElement *);
	void _onEditBoxString(vn::UIElement *, const vn::str32 &);
	virtual void _setValue(vn::f32 value);
};

#endif /* defined(__test__UIListItemETFloat__) */
