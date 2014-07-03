//
//  UILogList.cpp
//  TDMapEditor
//
//  Created by Wayne on 13-12-30.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "UILogList.h"
#include "vnUIList.h"
#include "vnUIListItem.h"

VN_IMPL_UI_LOGIC(UILogList, "LogList")

UILogList::UILogList() {
    
}

UILogList::UILogList(const UILogList &other)
: UILogic(other) {
    
}

UILogList * UILogList::clone() const {
    return vnnew UILogList(*this);
}

bool UILogList::_startup(vn::UIFactory::ElementMap &namedElements) {
    vn::UIList *list = dynamic_cast<vn::UIList *>(m_root);
    return list != 0;
}

void UILogList::_print(vn::Log::Level ll, const vn::str8 &content) {
    vn::UIList *list = static_cast<vn::UIList *>(m_root);
    if (list->itemCount() == 128) {
        list->eraseItem(0);
    }
    vn::UIListItemText *item = (vn::UIListItemText *)list->addItem();
    vn::str8 text;
    vn::color_t color;
    switch (ll) {
        case vn::Log::kDebug:
            text = "[Debug] ";
            color = vn::color_t::kWhite;
            break;
            
        case vn::Log::kInformation:
            text = "[Info] ";
            color = vn::color_t::kGreen;
            break;
            
        case vn::Log::kWarning:
            text = "[Warning] ";
            color = vn::color_t::kYellow;
            break;
            
        case vn::Log::kError:
            text = "[Error] ";
            color = vn::color_t::kRed;
            break;
            
        case vn::Log::kFatalError:
            text = "[FatalError] ";
            color = vn::color_t::kRed;
            break;
    }
    text += content;
    item->setText(text);
    item->setTextNormalColor(color);
    list->makeItemVisible(item);
}