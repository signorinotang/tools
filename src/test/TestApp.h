//
//  TestApp.h
//  test
//
//  Created by Wayne on 13-12-25.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __test__TestApp__
#define __test__TestApp__

#include "../common/GfxAppDelegate.h"
#include "../common/UIFileBrowser.h"
#include "../common/UIPopList.h"
#include "../common/UIPropertyList.h"

#include "vnEditType.h"
#include "vnCommandList.h"

class TestApp
: public GfxAppDelegate
, public vn::Singleton<TestApp> {
public:
    TestApp();
	~TestApp();
    
    virtual void do_undo();
    virtual void do_redo();
    
protected:
    virtual void _init(const vn::vector2f &size);
    virtual void _fini();
    virtual void _update(vn::f32 deltaTime);
    virtual void _render();
    virtual void _resize(const vn::vector2f &size);

    void _onString(vn::UIElement *, const vn::str32 &);
    void _onClicked(vn::UIElement *);
    void _onFileOpen(const vn::FilePath &path, const vn::str8 &name);
	void _onChecked(vn::UIElement *);
    
    UIFileBrowser *m_fileBrowser;
	
	vn::UIList *m_list;
	UIPopList *m_popList;
    
    UIPropertyList *m_properties;
    
    vn::CommandList m_commandList;
    
    vn::EditValueObject *m_tobj0, *m_tobj1;
};

#endif /* defined(__test__TestApp__) */
