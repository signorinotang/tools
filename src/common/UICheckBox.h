//
//  UICheckBox.h
//  test
//
//  Created by Wayne on 14-1-4.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __test__UICheckBox__
#define __test__UICheckBox__

#include "vnUIButton.h"
#include "vnColor.h"

class UICheckBox : public vn::UIButton {
	VN_DECL_UI_ELEMENT()
public:
	virtual UICheckBox * clone() const;
	
protected:
	virtual void _onRender(vn::UIRenderer *renderer);
	
	void _drawFrame(vn::UIRenderer *renderer, const vn::color_t &color);
	void _drawBody(vn::UIRenderer *renderer, const vn::color_t &color);
};

#endif /* defined(__test__UICheckBox__) */
