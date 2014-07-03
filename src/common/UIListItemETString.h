//
//  UIListItemETString.h
//  test
//
//  Created by Wayne on 14-1-4.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __test__UIListItemETString__
#define __test__UIListItemETString__

#include "vnUIListItem.h"
#include "vnUITextField.h"
#include "vnEditType.h"

class UIListItemETString : public vn::UIListItemText, public vn::EditValue::Observer {
	VN_DECL_UI_LOGIC()
public:
	UIListItemETString();
	UIListItemETString(const UIListItemETString &other);
	virtual UIListItemETString * clone() const;
	virtual ~UIListItemETString();
	
	virtual void ev_destroyed(vn::EditValue *);
	virtual void ev_changed(vn::EditValue *);
	
	void setTarget(vn::EditValueString *value);
	void setEditable(bool editable);
	
protected:
    virtual bool _startup(vn::UIFactory::ElementMap &namedElements);
	
	vn::UITextField *m_editbox; // named "editbox"
	
	vn::EditValueString *m_target;
	
	bool m_editable;
	
	void _onEditBoxGotFocus(vn::UIElement *);
	void _onEditBoxLoseFocus(vn::UIElement *);
	void _onEditBoxString(vn::UIElement *, const vn::str32 &);
};

#endif /* defined(__test__UIListItemETString__) */
