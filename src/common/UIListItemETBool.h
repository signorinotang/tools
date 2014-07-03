//
//  UIListItemETBool.h
//  test
//
//  Created by Wayne on 14-1-4.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __test__UIListItemETBool__
#define __test__UIListItemETBool__

#include "vnUIListItem.h"
#include "vnUIButton.h"
#include "vnEditType.h"

class UIListItemETBool : public vn::UIListItemText, public vn::EditValue::Observer {
	VN_DECL_UI_LOGIC()
public:
	UIListItemETBool();
	UIListItemETBool(const UIListItemETBool &other);
	virtual UIListItemETBool * clone() const;
	virtual ~UIListItemETBool();
	
	virtual void ev_destroyed(vn::EditValue *);
	virtual void ev_changed(vn::EditValue *);
	
	void setTarget(vn::EditValueBool *value);
	void setEditable(bool editable);
	
protected:
    virtual bool _startup(vn::UIFactory::ElementMap &namedElements);
	
	vn::UIButton *m_checkbox; // named "checkbox"
	
	vn::EditValueBool *m_target;
	
	void _onClicked(vn::UIElement *sender);
	
	bool m_editable;
};

#endif /* defined(__test__UIListItemETBool__) */
