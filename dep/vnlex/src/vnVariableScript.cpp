//
//  vnVariableScript.cpp
//  vnlex
//
//  Created by Wayne on 13-7-30.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnVariableScript.h"
#include "vnVariable.h"
#include "vnLexicalAnalizer.h"
#include "vnFileManager.h"

_vn_begin

namespace internal {
#include "vnVariableScriptProductions.h"
#include "vnVariableScriptStates.hpp"
}

VariableScript::VariableScript()
: m_parser(internal::g_states)
, m_global(0) {
    m_lexer.add(vnnew LA_Space(), false);
    m_lexer.add(vnnew LA_Identifier(internal::g_keywordInfo), false);
    m_lexer.add(vnnew LA_Number(), false);
    m_lexer.add(vnnew LA_Comment(), false);
    m_lexer.add(vnnew LA_String(), false);
    m_lexer.add(vnnew LA_Symbol(internal::g_symbolInfo), false);
}

VariableScript::~VariableScript() {
    VN_SAFE_DROP(m_global);
}

void VariableScript::set_global(vn::Variable_object *object, bool grab) {
    if (object) {
        if (grab) {
            object->grab();
        }
        if (m_global) {
            m_global->drop();
        }
        m_global = object;
    } else {
        VN_SAFE_DROP(m_global);
    }
}

Variable_array * VariableScript::parse(const FilePath &file) {
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
    c8 buf[512];
    int len = sprintf(buf, "%u:/%s", file.fsId, file.fileName.c_str());
    tf->name.assign(buf, len);
    
    Variable_array *ret = parse(txt, size, tf);
    
    vnfree(ptr);
    tf->drop();
    
    return ret;
}

Variable_array * VariableScript::parse(const c8 *text, size_t length, const Token::File *file) {
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
    Variable_array *ret = vnnew Variable_array();
    _build_value_list(root->child(0), ret);
    root->drop();
    
    return ret;
}

const str8 & VariableScript::error() const {
    return m_errorMsg;
}


void VariableScript::_makeErrorMsg(const c8 *format, ...) {
    va_list args;
	va_start(args, format);
    
    c8 buf[512];
    
    vsprintf(buf, format, args);
    m_errorMsg = buf;
	
	va_end(args);
}

void VariableScript::_appendErrorMsg(const c8 *format, ...) {
    va_list args;
	va_start(args, format);
    
    c8 buf[512];
    
    vsprintf(buf, format, args);
    m_errorMsg += buf;
	
	va_end(args);
}

void VariableScript::_build_object(LR_Node *node, Variable_object *object) {
    switch (node->productionId()) {
        case internal::PROD_object_0:
            break;
            
        case internal::PROD_object_1:
            _build_item_list(node->child(1), object);
            break;
    }
}

void VariableScript::_build_item_list(LR_Node *node, Variable_object *object) {
    switch (node->productionId()) {
        case internal::PROD_item_list_0:
            _build_item(node->child(0), object);
            break;
            
        case internal::PROD_item_list_1:
            _build_item_list(node->child(0), object);
            _build_item(node->child(1), object);
            break;
    }
}

void VariableScript::_build_item(LR_Node *node, Variable_object *object) {
    switch (node->productionId()) {
        case internal::PROD_item_0:
            object->set(node->child(0)->token()->text, Variable_null::instance(), true);
            break;
            
        case internal::PROD_item_1:
            object->set(node->child(0)->token()->text, _build_value(node->child(2)), false);
            break;
    }
}

Variable * VariableScript::_build_value(LR_Node *node) {
    Variable *ret = 0;
    switch (node->productionId()) {
        case internal::PROD_value_0:
            ret = Variable_null::instance();
            ret->grab();
            break;
            
        case internal::PROD_value_1:
            ret = vnnew Variable_bool(true);
            break;
            
        case internal::PROD_value_2:
            ret = vnnew Variable_bool(false);
            break;
            
        case internal::PROD_value_3:
            ret = vnnew Variable_int32(node->child(0)->token()->int32);
            break;
            
        case internal::PROD_value_4:
            ret = vnnew Variable_int64(node->child(0)->token()->int64);
            break;
            
        case internal::PROD_value_5:
            ret = vnnew Variable_float32(node->child(0)->token()->float32);
            break;
            
        case internal::PROD_value_6:
            ret = vnnew Variable_float64(node->child(0)->token()->float64);
            break;
            
        case internal::PROD_value_7:
            ret = vnnew Variable_string(node->child(0)->token()->text);
            break;
            
        case internal::PROD_value_8:
            ret = _build_reference(node->child(0));
            break;
            
        case internal::PROD_value_9:
            ret = _build_array(node->child(0));
            break;
            
        case internal::PROD_value_10: {
            Variable_object *object = vnnew Variable_object();
            _build_object(node->child(0), object);
            ret = object;
            break;
        }
    }
    return ret;
}

Variable * VariableScript::_build_array(LR_Node *node) {
    Variable_array *ret = vnnew Variable_array();
    switch (node->productionId()) {
        case internal::PROD_array_0:
            break;
            
        case internal::PROD_array_1:
            _build_value_list(node->child(1), ret);
            break;
    }
    return ret;
}

void VariableScript::_build_value_list(LR_Node *node, Variable_array *array) {
    switch (node->productionId()) {
        case internal::PROD_value_list_0:
            array->push_back(_build_value(node->child(0)), false);
            break;
            
        case internal::PROD_value_list_1:
            _build_value_list(node->child(0), array);
            array->push_back(_build_value(node->child(1)), false);
            break;
    }
}

Variable * VariableScript::_build_reference(vn::LR_Node *node) {
    Variable *ret = 0;
    switch (node->productionId()) {
        case internal::PROD_reference_0: {
            if (!m_global) {
                ret = Variable_null::instance();
                ret->grab();
            } else {
                size_t idx = m_global->index_of(node->child(1)->token()->text);
                if (idx == -1) {
                    ret = Variable_null::instance();
                } else {
                    ret = m_global->at(idx);
                }
                ret->grab();
            }
            break;
        }
            
        case internal::PROD_reference_1: {
            ret = vnnew Variable_reference(node->child(0)->token()->text);
            break;
        }
            
        case internal::PROD_reference_2: {
            ret = _build_value(node->child(2));
            Variable_reference *ref = vnnew Variable_reference(node->child(0)->token()->text, ret, false);
            ret = ref;
            break;
        }
    }
    return ret;
}

_vn_end