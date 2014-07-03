//
//  vnVariableScript.h
//  vnlex
//
//  Created by Wayne on 13-7-30.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnlex_vnVariableScript_h
#define vnlex_vnVariableScript_h

#include "vnLexer.h"
#include "vnLR_Parser.h"
#include "vnVariable.h"
#include "vnFilePath.h"

_vn_begin

class _vn_lex_api VariableScript {
public:
    VariableScript();
    ~VariableScript();
    
    void set_global(Variable_object *object, bool grab = true);
    
    Variable_array * parse(const FilePath &file);
    Variable_array * parse(const c8 *text, size_t length, const Token::File *file);
    
    const str8 & error() const;
protected:
    Lexer m_lexer;
	LR_Parser m_parser;
    str8 m_errorMsg;
    
    
    void _makeErrorMsg(const c8 *format, ...);
    void _appendErrorMsg(const c8 *format, ...);
    
    void _build_object(LR_Node *node, Variable_object *object);
    void _build_item_list(LR_Node *node, Variable_object *object);
    void _build_item(LR_Node *node, Variable_object *object);
    Variable * _build_value(LR_Node *node);
    Variable * _build_array(LR_Node *node);
    void _build_value_list(LR_Node *node, Variable_array *array);
    Variable * _build_reference(LR_Node *node);
    
    Variable_object * m_global;
};

_vn_end

#endif
