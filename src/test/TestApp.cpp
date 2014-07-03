//
//  TestApp.cpp
//  test
//
//  Created by Wayne on 13-12-25.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "TestApp.h"
#include "vnFontManager.h"
#include "vnTreeDataScript.h"
#include "vnLog.h"
#include "vnUIButton.h"
#include "vnEditTypeFactory.h"

#include "../common/EditValueCommand.h"

VN_DEFINE_ET_ENUMERATION_BEGIN(test, "Test")
    VN_DEFINE_ET_ENUMERATION_ITEM("0 - default", 0)
    VN_DEFINE_ET_ENUMERATION_ITEM("1 - horizontal", 1)
    VN_DEFINE_ET_ENUMERATION_ITEM("2 - vertical", 2)
VN_DEFINE_ET_ENUMERATION_END(test)

VN_DEFINE_ET_OBJECT_BEGIN(tobj0, "TOBJ0")
    VN_DEFINE_ET_OBJECT_ATTRIB_BOOL("Visible", false)
    VN_DEFINE_ET_OBJECT_ATTRIB_BOOL("Enabled", false)
    VN_DEFINE_ET_OBJECT_ATTRIB_FLOAT("Left", false)
    VN_DEFINE_ET_OBJECT_ATTRIB_FLOAT("Top", false)
    VN_DEFINE_ET_OBJECT_ATTRIB_FLOAT("Width", false)
    VN_DEFINE_ET_OBJECT_ATTRIB_FLOAT("Height", false)
VN_DEFINE_ET_OBJECT_END(tobj0)

VN_DEFINE_ET_OBJECT_BEGIN(tobj1, "TOBJ1")
    VN_DEFINE_ET_OBJECT_ATTRIB_BOOL("Visible", false)
    VN_DEFINE_ET_OBJECT_ATTRIB_BOOL("Enabled", false)
    VN_DEFINE_ET_OBJECT_ATTRIB_FLOAT("Left", false)
    VN_DEFINE_ET_OBJECT_ATTRIB_FLOAT("Top", false)
    VN_DEFINE_ET_OBJECT_ATTRIB_FLOAT("Width", false)
    VN_DEFINE_ET_OBJECT_ATTRIB_FLOAT("Height", false)
    VN_DEFINE_ET_OBJECT_ATTRIB_NAMED_TYPE("Style", false, "Test");
VN_DEFINE_ET_OBJECT_END(tobj1)

TestApp::TestApp()
: m_fileBrowser(0)
, m_properties(0)
, m_tobj0(0) {
	
}

TestApp::~TestApp() {

}

void TestApp::_init(const vn::vector2f &size) {
    vn::FontManager::instance().createFontFT("", "1:/common/msyh.ttf", 24);
    
    vn::UIFactory::instance().createElementsFromTDL(m_uiRootElement, "1:/test/ui.tdl");
    
    vn::UIFactory::ElementMap namedElements;
    
    m_uiRootElement->buildElementMap(namedElements);
    
    namedElements["text"]->bindAction("string", vn::make_memfn(this, &TestApp::_onString));
    namedElements["button"]->bindAction("clicked", vn::make_memfn(this, &TestApp::_onClicked));
    namedElements["checkbox"]->bindAction("clicked", vn::make_memfn(this, &TestApp::_onChecked));
	
	m_popList = (UIPopList *)namedElements["poplist"];
    
    vn::UILogic *logic = vn::UIFactory::instance().createLogicFromTDL("1:/common/file_browser.tdl");
    if (logic) {
        m_fileBrowser = dynamic_cast<UIFileBrowser *>(logic);
        if (!m_fileBrowser) {
            logic->drop();
        } else {
            m_fileBrowser->startup();
            m_fileBrowser->bindAction_FileOpen(vn::make_memfn(this, &TestApp::_onFileOpen));
        }
    }
	
    logic = vn::UIFactory::instance().createLogicFromTDL("1:/common/property_list.tdl");
    if (logic) {
        m_properties = dynamic_cast<UIPropertyList *>(logic);
        if (!m_properties) {
            logic->drop();
        } else {
            m_properties->startup();
            namedElements["list_bkg"]->addChild(m_properties->root());
        }
    }
    
    EditValueCommandGenerator::instance().setCommandList(&m_commandList);
    
	m_tobj0 = vn::EditTypeFactory::instance().etObject("TOBJ0")->generate();
    m_tobj1 = vn::EditTypeFactory::instance().etObject("TOBJ1")->generate();
    
    for (size_t i = 0, c = m_tobj0->type()->numAttributes(); i < c; ++i) {
        m_tobj0->attribute(i)->addObserver(&EditValueCommandGenerator::instance());
    }
    for (size_t i = 0, c = m_tobj1->type()->numAttributes(); i < c; ++i) {
        m_tobj1->attribute(i)->addObserver(&EditValueCommandGenerator::instance());
    }
    
    m_properties->setTarget(m_tobj1);
    
}

void TestApp::_fini() {
    m_commandList.clear();
    VN_SAFE_DROP(m_tobj0);
    VN_SAFE_DROP(m_tobj1);
    VN_SAFE_DROP(m_properties);
    VN_SAFE_DROP(m_fileBrowser);
}

void TestApp::_update(vn::f32 deltaTime) {
    m_fileBrowser->update(deltaTime);
}

void TestApp::_render() {
    
}

void TestApp::_resize(const vn::vector2f &size) {
    
}

void TestApp::do_undo() {
    m_commandList.undo();
}

void TestApp::do_redo() {
    m_commandList.redo();
}

void TestApp::_onString(vn::UIElement *sender, const vn::str32 &test) {
    
}

void TestApp::_onClicked(vn::UIElement *sender) {
    m_fileBrowser->setTitle("Open TDL File");
    m_fileBrowser->setReadOnly(true);
    m_fileBrowser->setImagesMode(false);
    //m_fileBrowser->setExtensionFilter(".tdl");
    m_fileBrowser->show(m_uiRootElement);
}

void TestApp::_onFileOpen(const vn::FilePath &path, const vn::str8 &name) {
    VN_LOG_DEBUG("path: \"" << path.fsId << ":/" << path.fileName << "\"");
    VN_LOG_DEBUG("name: \"" << name << "\"");
}

void TestApp::_onChecked(vn::UIElement *sender) {
	vn::UIButton *btn = (vn::UIButton *)sender;
	if (btn->isPushed()) {
        btn->setPushed(false);
        m_properties->setTarget(m_tobj1);
    } else {
        btn->setPushed(true);
        m_properties->setTarget(m_tobj0);
    }
	//m_popList->pop();
    
}