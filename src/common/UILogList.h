//
//  UILogList.h
//  TDMapEditor
//
//  Created by Wayne on 13-12-30.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__UILogList__
#define __TDMapEditor__UILogList__

#include "vnUILogic.h"
#include "vnLog.h"

class UILogList : public vn::UILogic, public vn::LogPrinter {
    VN_DECL_UI_LOGIC()
public:
    UILogList();
    UILogList(const UILogList &other);
    virtual UILogList * clone() const;
    
protected:
    virtual bool _startup(vn::UIFactory::ElementMap &namedElements);
    virtual void _print(vn::Log::Level, const vn::str8 &);
};

#endif /* defined(__TDMapEditor__UILogList__) */
