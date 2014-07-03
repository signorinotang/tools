//
//  vnTreeDataScript.cpp
//  vnlex
//
//  Created by Wayne on 13-12-19.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnTreeDataScript.h"
#include "vnLexicalAnalizer.h"
#include "vnFileManager.h"


_vn_begin

namespace internal {
#include "vnTreeDataScriptProductions.h"
#include "vnTreeDataScriptStates.hpp"
}

TreeDataScript::TreeDataScript()
: m_parser(internal::g_states) {
    m_lexer.add(vnnew LA_Space(), false);
    m_lexer.add(vnnew LA_Identifier(internal::g_keywordInfo), false);
    m_lexer.add(vnnew LA_Number(), false);
    m_lexer.add(vnnew LA_Comment(), false);
    m_lexer.add(vnnew LA_String(), false);
    m_lexer.add(vnnew LA_Symbol(internal::g_symbolInfo), false);
}

const str8 & TreeDataScript::error() const {
    return m_errorMsg;
}


void TreeDataScript::_makeErrorMsg(const c8 *format, ...) {
    va_list args;
	va_start(args, format);
    
    c8 buf[512];
    
    vsprintf(buf, format, args);
    m_errorMsg = buf;
	
	va_end(args);
}

void TreeDataScript::_appendErrorMsg(const c8 *format, ...) {
    va_list args;
	va_start(args, format);
    
    c8 buf[512];
    
    vsprintf(buf, format, args);
    m_errorMsg += buf;
	
	va_end(args);
}

TreeDataNode * TreeDataScript::parse(const FilePath &file) {
    FileStream *fs = FileManager::instance().open(file);
    if (!fs) {
        _makeErrorMsg("open file \"%u:/%s\" failed!", file.fsId, file.fileName.c_str());
        return 0;
    }
    fs->seek(0, DataStream::kEnd);
    size_t size = (size_t)fs->tell();
    fs->seek(0);
    c8 *ptr = vnmalloc(c8, size);
    if (fs->read(ptr, size) != size) {
        _makeErrorMsg("read file \"%u:/%s\" failed!", file.fsId, file.fileName.c_str());
        fs->close();
        vnfree(ptr);
        return 0;
    }
    fs->close();
    
    c8 *txt = ptr;
    // utf8 bom
    if (strncmp(txt, "\xEF\xBB\xBF", 3) == 0) {
        txt += 3;
        size -= 3;
    }
    
    Token::File *tf = vnnew Token::File();
    file.getFullPath(tf->name);
    
    TreeDataNode *ret = parse(txt, size, tf);
    
    vnfree(ptr);
    tf->drop();
    
    return ret;
}

TreeDataNode * TreeDataScript::parse(const c8 *text, size_t length, const Token::File *file) {
    TokenList tokens;
    if (m_lexer.scan(text, length, file, tokens) < 0) {
        _makeErrorMsg("lexical error at %s", m_lexer.error().c_str());
        return 0;
    }
    tokens.push_back(vnnew Token(file));
    for (TokenList::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        LR_Parser::Result ret = m_parser.input(it->ptr());
        if (ret == LR_Parser::kAccept) {
            break;
        }
        if (ret == LR_Parser::kFailed) {
            _makeErrorMsg("syntax error at %s(%u): %s", (file ? file->name.c_str() : ""), it->ptr()->row + 1, it->ptr()->text.c_str());
            break;
        }
    }
    LR_Node *root = m_parser.result();
    if (root) {
        root->grab();
        m_parser.reset();
    } else {
        m_parser.reset();
        return 0;
    }
    m_errorMsg.clear();
    TreeDataNode *last = 0;
    TreeDataNode *ret = _build_node_children(root->child(0), last);
    root->drop();
    
    return ret;
}

TreeDataNode * TreeDataScript::_build_node_children(LR_Node *node, TreeDataNode *&last) {
    switch (node->productionId()) {
        case internal::PROD_node_children_0: {
            vnassert(!last);
            last = _build_node(node->child(0));
            return last;
        }
            
        case internal::PROD_node_children_1: {
            TreeDataNode *first = _build_node_children(node->child(0), last);
            TreeDataNode *tdn = _build_node(node->child(1));
            last->_setNextNeighbour(tdn, false);
            last = tdn;
            return first;
        }
    }
    return 0;
}

TreeDataNode * TreeDataScript::_build_node(LR_Node *node) {
    switch (node->productionId()) {
        case internal::PROD_node_0: {
            if (node->child(1)->token()->text != node->child(3)->token()->text) {
                // error !
            }
            TreeDataNode *tdn = vnnew TreeDataNode();
            tdn->_setName(node->child(1)->token()->text);
            return tdn;
        }
        case internal::PROD_node_1: {
            if (node->child(1)->token()->text != node->child(4)->token()->text) {
                // error !
            }
            TreeDataNode *tdn = vnnew TreeDataNode();
            tdn->_setName(node->child(1)->token()->text);
            _build_attributes(node->child(2), tdn);
            return tdn;
        }
        case internal::PROD_node_2: {
            if (node->child(1)->token()->text != node->child(4)->token()->text) {
                // error !
            }
            TreeDataNode *tdn = vnnew TreeDataNode();
            tdn->_setName(node->child(1)->token()->text);
            TreeDataNode *last = 0;
            TreeDataNode *first = _build_node_children(node->child(2), last);
            tdn->_setFirstChild(first, false);
            return tdn;
        }
        case internal::PROD_node_3: {
            if (node->child(1)->token()->text != node->child(5)->token()->text) {
                // error !
            }
            TreeDataNode *tdn = vnnew TreeDataNode();
            tdn->_setName(node->child(1)->token()->text);
            _build_attributes(node->child(2), tdn);
            TreeDataNode *last = 0;
            TreeDataNode *first = _build_node_children(node->child(3), last);
            tdn->_setFirstChild(first, false);
            return tdn;
        }
    }
    return 0;
}

void TreeDataScript::_build_attributes(LR_Node *node, TreeDataObject *object) {
    switch (node->productionId()) {
        case internal::PROD_attributes_0: {
            return ;
        }
            
        case internal::PROD_attributes_1: {
            return _build_attribute_list(node->child(1), object);
        }
    }
}

void TreeDataScript::_build_attribute_list(LR_Node *node, TreeDataObject *object) {
    switch (node->productionId()) {
        case internal::PROD_attribute_list_0: {
            _build_attribute(node->child(0), object);
            return ;
        }
            
        case internal::PROD_attribute_list_1: {
            _build_attribute_list(node->child(0), object);
            _build_attribute(node->child(1), object);
            return ;
        }
    }
}

void TreeDataScript::_build_attribute(LR_Node *node, TreeDataObject *object) {
    TreeData *value = _build_value(node->child(2));
    object->_setAttribute(node->child(0)->token()->text, value, false);
}

TreeData * TreeDataScript::_build_value(vn::LR_Node *node) {
    switch (node->productionId()) {
        case internal::PROD_value_0: {
            return vnnew TreeDataBoolean(true);
        }
        case internal::PROD_value_1: {
            return vnnew TreeDataBoolean(false);
        }
        case internal::PROD_value_2:
        case internal::PROD_value_3: {
            return _build_number(node->child(0));
        }
        case internal::PROD_value_4: {
            return _build_minus_number(node->child(1));
        }
        case internal::PROD_value_5: {
            return vnnew TreeDataString(node->child(0)->token()->text);
        }
        case internal::PROD_value_6: {
            return _build_array(node->child(0));
        }
        case internal::PROD_value_7: {
            return _build_object(node->child(0));
        }
        case internal::PROD_value_8: {
            return _build_node(node->child(0));
        }
    }
    return 0;
}

TreeData * TreeDataScript::_build_number(LR_Node *node) {
    switch (node->productionId()) {
        case internal::PROD_number_0: {
            return vnnew TreeDataInteger(node->child(0)->token()->int32);
        }
        case internal::PROD_number_1: {
            return vnnew TreeDataLongInteger(node->child(0)->token()->int64);
        }
        case internal::PROD_number_2: {
            return vnnew TreeDataFloat(node->child(0)->token()->float32);
        }
        case internal::PROD_number_3: {
            return vnnew TreeDataDouble(node->child(0)->token()->float64);
        }
    }
    return 0;
}

TreeData * TreeDataScript::_build_minus_number(LR_Node *node) {
    switch (node->productionId()) {
        case internal::PROD_number_0: {
            return vnnew TreeDataInteger(-(s32)node->child(0)->token()->int32);
        }
        case internal::PROD_number_1: {
            return vnnew TreeDataLongInteger(-(s64)node->child(0)->token()->int64);
        }
        case internal::PROD_number_2: {
            return vnnew TreeDataFloat(-node->child(0)->token()->float32);
        }
        case internal::PROD_number_3: {
            return vnnew TreeDataDouble(-node->child(0)->token()->float64);
        }
    }
    return 0;
}

TreeData * TreeDataScript::_build_array(LR_Node *node) {
    switch (node->productionId()) {
        case internal::PROD_array_0: {
            return vnnew TreeDataArray();
        }
        case internal::PROD_array_1: {
            TreeDataArray *ret = vnnew TreeDataArray();
            _build_value_list(node->child(1), ret);
            return ret;
        }
    }
    return 0;
}

void TreeDataScript::_build_value_list(LR_Node *node, TreeDataArray *array) {
    switch (node->productionId()) {
        case internal::PROD_value_list_0: {
            TreeData *value = _build_value(node->child(0));
            array->push_back(value, false);
            return ;
        }
        case internal::PROD_value_list_1: {
            _build_value_list(node->child(0), array);
            TreeData *value = _build_value(node->child(1));
            array->push_back(value, false);
            return ;
        }
    }
}

TreeData * TreeDataScript::_build_object(vn::LR_Node *node) {
    TreeDataObject *object = vnnew TreeDataObject();
    object->_setName(node->child(1)->token()->text);
    _build_attributes(node->child(2), object);
    return object;
}

_vn_end