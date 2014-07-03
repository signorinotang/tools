//
//  UIFileBrowser.h
//  test
//
//  Created by Wayne on 13-12-25.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __test__UIFileBrowser__
#define __test__UIFileBrowser__

#include "vnUILogic.h"
#include "vnUILabel.h"
#include "vnUITextField.h"
#include "vnUIList.h"

class UIFileBrowser : public vn::UILogic {
    VN_DECL_UI_LOGIC()
public:
    
    UIFileBrowser();
    virtual UIFileBrowser * clone() const;
    virtual void init(const vn::TreeDataObject *object);
    virtual void update(vn::f32 deltaTime);
    
    void setTitle(const vn::str8 &title);
    void setReadOnly(bool readOnly);
    void setFileName(const vn::str8 &name);
    void setImagesMode(bool images);
    void setExtensionFilter(const vn::str8 &filter);
    
    void show(vn::UIElement *parent);
    void hide();
    bool visible() const;
    
    void bindAction_FileOpen(const vn::function<void(const vn::FilePath &, const vn::str8 &)> &func);
    
protected:
    virtual bool _startup(vn::UIFactory::ElementMap &namedElements);
    
    void _onClose(vn::UIElement *);
    void _onOk(vn::UIElement *);
    void _onUp(vn::UIElement *);
    void _onItemClick(vn::UIElement *, vn::UIListItem *item);
    void _onString(vn::UIElement *, const vn::str32 &);
    
    vn::UILabel *m_title;
    vn::UITextField *m_path, *m_file;
    vn::UIList *m_list;
    vn::UIElement *m_ok;
    vn::UIElement *m_up;
    
    vn::FilePath m_filePath;
    vn::str8 m_fileName;
    enum ListMode {
        _c_FileManager = 0,
        _c_FileSystem,
        _c_Images
    } m_mode;
    
    void _updateList();
    void _updateListFileManager();
    void _updateListFileSystem();
    void _updateListImages();
    void _enterFileSystem();
    void _enterFile();
    void _enterImage();
    void _open();
    bool _checkExtension(const vn::str8 &name);
    
    
    vn::UIListItem *m_clickedItem;
    vn::f32 m_clickedTimer;
    
    bool m_readOnly;
    bool m_imagesMode;
    vn::str8 m_extensionFilter;
    
    vn::function<void(const vn::FilePath &, const vn::str8 &)> m_fnFileOpen;
};

typedef vn::RefCountedPtr<UIFileBrowser> UIFileBrowserPtr;

#endif /* defined(__test__UIFileBrowser__) */
