//
//  UIFileBrowser.cpp
//  test
//
//  Created by Wayne on 13-12-25.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "UIFileBrowser.h"
#include "vnUIListItem.h"
#include "vnUIFrame.h"
#include "vnUIPicture.h"
#include "vnUILocatorBorder.h"
#include "vnFileManager.h"
#include "vnFontManager.h"
#include "vnPictureManager.h"
#include "vnUnicode.h"

VN_IMPL_UI_LOGIC(UIFileBrowser, "FileBrowser")

namespace internal {
    using namespace vn;
    class FileItem : public UIListItem {
    public:
        enum Type {
            kFile = 0,
            kFolder,
            kImages
        };
        
        FileItem(Type type) : m_type(type) {}
        virtual FileItem * clone() const { return 0; }
        
        void init() {
            m_root = vnnew UIFrame();
            m_bkg = vnnew UIPicture();
            vnnew UILocatorBorder(m_bkg);
            m_bkg->setEnabled(false);
            m_root->addChild(m_bkg, false);
            
            m_text = vnnew UILabel();
            vnnew UILocatorBorder(m_text);
            m_text->setFont(FontManager::instance().defaultFont());
            m_root->addChild(m_text, false);
            
            _applyUnselected();
        }
        
        virtual f32 calculateWidth() { return 0; }
        virtual void setPositionAndHeight(const vector2f &pos, f32 height) {
            
        }
        virtual f32 calculateHeight() {
            return 24;
        }
        virtual void setPositionAndWidth(const vector2f &pos, f32 width) {
            aabox2f rect;
            rect.min_corner = pos;
            rect.max_corner = pos + vector2f(width, 24);
            m_root->setLocation(rect);
        }
        
        virtual void setText(const str8 &text) {
            m_text->setText(text);
        }
        virtual void setText(const str32 &text) {
            m_text->setText(text);
        }
        virtual void getText(str8 &text) {
            text = m_text->getText();
        }
        
        virtual void onSelected() {
            _applySelected();
        }
        
        virtual void onUnselected() {
            _applyUnselected();
        }
        
        Type type() const {
            return m_type;
        }
        
        const str8 & getText() const {
            return m_text->getText();
        }
        
    protected:
        Type m_type;
        UILabel *m_text;
        UIPicture *m_bkg;
        
        void _applyUnselected() {
            m_bkg->setVisible(false);
            switch (m_type) {
                case kFile:
                    m_text->setTextColor(color_t(0x0, 0xE0, 0xE0));
                    break;
                    
                case kFolder:
                    m_text->setTextColor(color_t(0, 0xE0, 0));
                    break;
                    
                case kImages:
                    m_text->setTextColor(color_t(0xE0, 0xE0, 0));
                    break;
            }
        }
        
        void _applySelected() {
            m_bkg->setVisible(true);
            switch (m_type) {
                case kFile:
                    m_bkg->setColor(color_t(0x0, 0xE0, 0xE0));
                    break;
                    
                case kFolder:
                    m_bkg->setColor(color_t(0, 0xE0, 0));
                    break;
                    
                case kImages:
                    m_bkg->setColor(color_t(0xE0, 0xE0, 0));
                    break;
            }
            m_text->setTextColor(color_t::kBlack);
        }
    };
}

UIFileBrowser::UIFileBrowser()
: m_title(0)
, m_path(0)
, m_file(0)
, m_list(0)
, m_mode(_c_FileManager)
, m_readOnly(true)
, m_imagesMode(false) {
    
}

UIFileBrowser * UIFileBrowser::clone() const {
    return vnnew UIFileBrowser(*this);
}

void UIFileBrowser::init(const vn::TreeDataObject *object) {
    UILogic::init(object);
}

void UIFileBrowser::update(vn::f32 deltaTime) {
    if (m_clickedItem) {
        m_clickedTimer -= deltaTime;
        if (m_clickedTimer <= 0) {
            m_clickedItem = 0;
            m_clickedTimer = 0;
        }
    }
}

bool UIFileBrowser::_startup(vn::UIFactory::ElementMap &namedElements) {
    m_title = (vn::UILabel *)namedElements["title"];
    namedElements["close"]->bindAction("clicked", vn::make_memfn(this, &UIFileBrowser::_onClose));
    m_ok = namedElements["ok"];
    m_ok->bindAction("clicked", vn::make_memfn(this, &UIFileBrowser::_onOk));
    m_ok->setEnabled(false);
   
    m_up = namedElements["up"];
    m_up->bindAction("clicked", vn::make_memfn(this, &UIFileBrowser::_onUp));
    m_up->setEnabled(false);
    
    m_path = (vn::UITextField *)namedElements["path"];
    m_file = (vn::UITextField *)namedElements["file"];
    m_file->setReadOnly(m_readOnly);
    m_file->bindAction_TextCompleted(vn::make_memfn(this, &UIFileBrowser::_onString));
    
    m_list = (vn::UIList *)namedElements["list"];
    m_list->bindAction_ItemClicked(vn::make_memfn(this, &UIFileBrowser::_onItemClick));
    
    return true;
}

void UIFileBrowser::setTitle(const vn::str8 &title) {
    m_title->setText(title);
}

void UIFileBrowser::setReadOnly(bool readOnly) {
    m_readOnly = readOnly;
    m_file->setReadOnly(readOnly);
}

void UIFileBrowser::setFileName(const vn::str8 &name) {
    m_fileName = name;
    m_file->setText(name);
}

void UIFileBrowser::setImagesMode(bool images) {
    if ((m_imagesMode = images)) {
        m_readOnly = true;
    }
}

void UIFileBrowser::setExtensionFilter(const vn::str8 &filter) {
    m_extensionFilter = filter;
}

void UIFileBrowser::show(vn::UIElement *parent) {
    vnassert(parent);
    parent->addChild(m_root);
    _updateList();
}

void UIFileBrowser::hide() {
    if (m_root->parent()) {
        m_root->parent()->removeChild(m_root);
    }
}

bool UIFileBrowser::visible() const {
    return m_root->parent() != 0;
}

void UIFileBrowser::bindAction_FileOpen(const vn::function<void (const vn::FilePath &, const vn::str8 &)> &func) {
    m_fnFileOpen = func;
}

void UIFileBrowser::_onClose(vn::UIElement *) {
    hide();
}

void UIFileBrowser::_onOk(vn::UIElement *) {
    _open();
}

void UIFileBrowser::_onUp(vn::UIElement *) {
    if (m_mode == _c_FileManager) {
        return ;
    }
    
    if (m_mode == _c_FileSystem) {
        if (m_filePath.fileName.empty()) {
            m_mode = _c_FileManager;
            _updateListFileManager();
            m_file->setEnabled(false);
            if (m_readOnly) {
                setFileName("");
            }
            m_ok->setEnabled(false);
            m_up->setEnabled(false);
        } else {
            m_filePath.fileName.pop_back();
            size_t pos = m_filePath.fileName.find_last_of('/');
            if (pos == vn::str8::npos) {
                m_filePath.fileName.clear();
            } else {
                m_filePath.fileName.erase(m_filePath.fileName.begin() + pos + 1, m_filePath.fileName.end());
            }
            _updateListFileSystem();
            if (m_readOnly) {
               setFileName("");
            }
        }
    } else {
        size_t pos = m_filePath.fileName.find_last_of('/');
        if (pos == vn::str8::npos) {
            m_filePath.fileName.clear();
        } else {
            m_filePath.fileName.erase(m_filePath.fileName.begin() + pos + 1, m_filePath.fileName.end());
        }
        m_mode = _c_FileSystem;
        _updateListFileSystem();
        setFileName("");
    }
}

void UIFileBrowser::_onItemClick(vn::UIElement *, vn::UIListItem *item) {
    if (!item) {
        m_clickedItem = 0;
        m_clickedTimer = 0;
        m_list->setSelectedItem(0);
        if (m_readOnly) {
            setFileName("");
        }
        return ;
    }
    m_list->setSelectedItem(item);
    if (m_clickedItem != item) {
        m_clickedItem = item;
        m_clickedTimer = 0.5f;
        internal::FileItem *p = (internal::FileItem *)item;
        switch (p->type()) {
            case internal::FileItem::kFile:
                setFileName(p->getText());
                break;
            case internal::FileItem::kFolder:
                if (m_readOnly) {
                    setFileName("");
                }
                break;
            case internal::FileItem::kImages:
                if (!m_imagesMode) {
                    setFileName(p->getText());
                } else {
                    setFileName("");
                }
                break;
        }
    } else {
        switch (m_mode) {
            case _c_FileManager:
                _enterFileSystem();
                break;
                
            case _c_FileSystem:
                _enterFile();
                break;
                
            case _c_Images:
                _enterImage();
                break;
        }
        m_clickedItem = 0;
        m_clickedTimer = 0;
    }
}

void UIFileBrowser::_onString(vn::UIElement *, const vn::str32 &) {
    if (m_mode != _c_FileManager) {
        _open();
    }
}

void UIFileBrowser::_updateList() {
    switch (m_mode) {
        case _c_FileManager:
            _updateListFileManager();
            break;
            
        case _c_FileSystem:
            _updateListFileSystem();
            break;
            
        case _c_Images:
            _updateListImages();
            break;
    }
}

void UIFileBrowser::_updateListFileManager() {
    m_list->clearItems();
    m_path->setText("");
    std::list<vn::u32> result;
    vn::FileManager::instance().listFileSystemId(result);
    for (std::list<vn::u32>::iterator it = result.begin(); it != result.end(); ++it) {
        vn::c8 buf[16];
        internal::FileItem *item = vnnew internal::FileItem(internal::FileItem::kFolder);
        item->init();
        item->setText(vn::str8(buf, sprintf(buf, "%u:/", *it)));
        item->setUserData(vnnew vn::TreeDataInteger(*it), false);
        m_list->addItem(item, false);
    }
}

void UIFileBrowser::_updateListFileSystem() {
    m_list->clearItems();
    vn::str8 path;
    m_filePath.getFullPath(path);
    if (path.back() != '/') {
        path += '/';
    }
    m_path->setText(path);
    std::list<vn::str8> result;
    vn::FileManager::instance().list(m_filePath, result);
    for (std::list<vn::str8>::iterator it = result.begin(); it != result.end(); ++it) {
        
        internal::FileItem::Type type;
        if (it->back() == '/') {
            if (it->length() > 6 && !strcmp(it->c_str() + it->length() - 6, ".imgs/")) {
                type = internal::FileItem::kImages;
                it->pop_back();
                if (!m_imagesMode && !_checkExtension(*it)) {
                    continue;
                }
            } else {
                type = internal::FileItem::kFolder;
            }
        } else {
            type = internal::FileItem::kFile;
            if (m_imagesMode || !_checkExtension(*it)) {
                continue;
            }
        }
        internal::FileItem *item = vnnew internal::FileItem(type);
        item->init();
        item->setText(*it);
        m_list->addItem(item, false);
    }
}

void UIFileBrowser::_updateListImages() {
    m_list->clearItems();
    vn::str8 path;
    m_filePath.getFullPath(path);
    m_path->setText(path);
    std::list<vn::str8> result;
    vn::PictureManager::instance().list(m_filePath, result);
    for (std::list<vn::str8>::iterator it = result.begin(); it != result.end(); ++it) {
        if (!_checkExtension(*it)) {
            continue;
        }
        internal::FileItem *item = vnnew internal::FileItem(internal::FileItem::kFile);
        item->init();
        item->setText(*it);
        m_list->addItem(item, false);
    }
}

void UIFileBrowser::_enterFileSystem() {
    m_mode = _c_FileSystem;
    m_filePath.fsId = static_cast<vn::TreeDataInteger *>(m_clickedItem->getUserData())->value();
    _updateListFileSystem();
    m_file->setEnabled(true);
    m_ok->setEnabled(true);
    m_up->setEnabled(true);
}

void UIFileBrowser::_enterFile() {
    internal::FileItem *p = (internal::FileItem *)m_clickedItem;
    switch (p->type()) {
        case internal::FileItem::kFile:
            _open();
            break;
            
        case internal::FileItem::kFolder:
            m_filePath.fileName += p->getText();
            _updateListFileSystem();
            break;
            
        case internal::FileItem::kImages:
            if (m_imagesMode) {
                m_filePath.fileName += p->getText();
                m_mode = _c_Images;
                _updateListImages();
            } else {
                _open();
            }
            break;
    }
}

void UIFileBrowser::_enterImage() {
    _open();
}

void UIFileBrowser::_open() {
    if (!m_readOnly) {
        const vn::str32 &txt = m_file->getText();
        vn::Unicode::convert(txt.c_str(), txt.length(), m_fileName);
    }
    hide();
    if (!m_fnFileOpen.null()) {
        m_fnFileOpen(m_filePath, m_fileName);
    }
}

bool UIFileBrowser::_checkExtension(const vn::str8 &name) {
    if (m_extensionFilter.empty()) {
        return true;
    }
    if (name.length() < m_extensionFilter.length()) {
        return false;
    }
    if (strcmp(name.c_str() + (name.length() - m_extensionFilter.length()), m_extensionFilter.c_str())) {
        return false;
    }
    return true;
}