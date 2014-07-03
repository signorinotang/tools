//
//  UIListItemETEnum.h
//  test
//
//  Created by Wayne on 14-1-5.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __test__UIListItemETEnum__
#define __test__UIListItemETEnum__

#include "vnUIListItem.h"
#include "vnEditType.h"
#include "vnUIButton.h"

#include "UIPopList.h"

class UIListItemETEnum : public vn::UIListItemText, public vn::EditValue::Observer {
	VN_DECL_UI_LOGIC()
public:
	UIListItemETEnum();
	UIListItemETEnum(const UIListItemETEnum &other);
	virtual UIListItemETEnum * clone() const;
	virtual ~UIListItemETEnum();
	
	virtual void ev_destroyed(vn::EditValue *);
	virtual void ev_changed(vn::EditValue *);
	
	void setTarget(vn::EditValueInt *value);
	void setEditable(bool editable);
	
protected:
    virtual bool _startup(vn::UIFactory::ElementMap &namedElements);
	
	vn::EditValueInt *m_target;
	UIPopList *m_list; // named "list"
	vn::UIButton *m_button; // named "button"
	vn::UILabel *m_value; // named "value"
	
	bool m_editable;
	
	void _onClicked(vn::UIElement *);
	void _onSelectedChanged(vn::UIElement *, size_t index);
};

#endif /* defined(__test__UIListItemETEnum__) */
