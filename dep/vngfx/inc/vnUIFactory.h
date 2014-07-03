//
//  vnUIFactory.h
//  vngfx
//
//  Created by Wayne on 13-12-17.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUIFactory_h
#define vngfx_vnUIFactory_h

#include "vnGfxDef.h"
#include "vnSingleton.h"
#include "vnTreeData.h"
#include "vnFilePath.h"

#include <map>

_vn_begin

class UIElement;
class UILocator;
class UIRenderPicture;
class UILogic;

class _vn_gfx_api UIFactory : public Singleton<UIFactory> {
public:
    typedef std::map<str8, UIElement *> ElementMap;
    UIElement * createElement(const str8 &name);
    UIElement * createElement(const TreeDataObject *object);
    UIElement * createElement(const TreeDataNode *node);
	UILocator * createLocator(const str8 &name, UIElement *owner);
	UIRenderPicture * createRenderPicture(const str8 &name);
	UIRenderPicture * createRenderPicture(const TreeDataObject *object);
    
    UILogic * createLogic(const str8 &name);
    UILogic * createLogic(const TreeDataObject *object);
    UILogic * createLogicFromTDL(const FilePath &path);
    
    void createElements(UIElement *root, const TreeDataNode *node);
    
    bool createElementsFromTDL(UIElement *root, const FilePath &path);

    bool createElementsFromBinaryTDL(UIElement *root, const FilePath &path);
	
	void _register(const c8 *name, UIElement *(*creator)());
	void _register(const c8 *name, UILocator *(*creator)(UIElement *));
	void _register(const c8 *name, UIRenderPicture *(*creator)());
    void _register(const c8 *name, UILogic *(*creator)());
private:
	typedef std::map<str8, UIElement *(*)()> ElementFactory;
	typedef std::map<str8, UILocator *(*)(UIElement *)> LocatorFactory;
	typedef std::map<str8, UIRenderPicture *(*)()> RenderPictureFactory;
    typedef std::map<str8, UILogic *(*)()> LogicFactory;
	ElementFactory m_factoryE;
	LocatorFactory m_factoryL;
	RenderPictureFactory m_factoryRP;
    LogicFactory m_factoryO;
};

enum TextAlignment {
	kTA_Default = 0,
    kTA_Left = 1,
    kTA_Right = 2,
    kTA_HorizontalCenter = 3,
    kTA_Top = 4,
    kTA_Bottom = 8,
    kTA_VerticalCenter = 12,
    kTA_Center = 15
};

enum ScrollType {
	kST_Default = 0,
	kST_Horizontal = 1,
	kST_Vertical = 2
};


_vn_end

#define VN_DECL_UI_ELEMENT() \
    public:\
        static _vn_ns::UIElement * create();

#define VN_IMPL_UI_ELEMENT(cls, name) \
    struct _auto_reg_##cls {\
        _auto_reg_##cls() {\
            _vn_ns::UIFactory::instance()._register(name, cls::create);\
        }\
    } _auto_reg_##cls##_ins;\
    _vn_ns::UIElement * cls::create() { return vnnew cls(); }

#define VN_DECL_UI_LOCATOR() \
    public:\
        static _vn_ns::UILocator * create(_vn_ns::UIElement *owner);

#define VN_IMPL_UI_LOCATOR(cls, name) \
    struct _auto_reg_##cls {\
        _auto_reg_##cls() {\
            _vn_ns::UIFactory::instance()._register(name, cls::create);\
        }\
    } _auto_reg_##cls##_ins;\
    _vn_ns::UILocator * cls::create(_vn_ns::UIElement *owner) { return vnnew cls(owner); }

#define VN_DECL_UI_RENDER_PICTURE() \
	public:\
		static _vn_ns::UIRenderPicture * create();

#define VN_IMPL_UI_RENDER_PICTURE(cls, name) \
	struct _auto_reg_##cls {\
		_auto_reg_##cls() {\
			_vn_ns::UIFactory::instance()._register(name, cls::create);\
		}\
	} _auto_reg_##cls##_ins;\
	_vn_ns::UIRenderPicture * cls::create() { return vnnew cls(); }

#define VN_DECL_UI_LOGIC() \
    public:\
        static _vn_ns::UILogic * create();

#define VN_IMPL_UI_LOGIC(cls, name) \
    struct _auto_reg_##cls {\
        _auto_reg_##cls() {\
            _vn_ns::UIFactory::instance()._register(name, cls::create);\
        }\
    } _auto_reg_##cls##_ins;\
    _vn_ns::UILogic * cls::create() { return vnnew cls(); }

#endif