//
//  vnTreeDataScript.h
//  vnlex
//
//  Created by Wayne on 13-12-19.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnlex_vnTreeDataScript_h
#define vnlex_vnTreeDataScript_h

#include "vnLexer.h"
#include "vnLR_Parser.h"
#include "vnTreeData.h"
#include "vnFilePath.h"

_vn_begin

class _vn_lex_api TreeDataScript : public AllocatedObject {
public:
    TreeDataScript();
    
    TreeDataNode * parse(const FilePath &file);
    TreeDataNode * parse(const c8 *text, size_t length, const Token::File *file);
    
    const str8 & error() const;
protected:
    Lexer m_lexer;
	LR_Parser m_parser;
    str8 m_errorMsg;
    
    
    void _makeErrorMsg(const c8 *format, ...);
    void _appendErrorMsg(const c8 *format, ...);
    TreeDataNode * _build_node_children(LR_Node *node, TreeDataNode *&last);
    TreeDataNode * _build_node(LR_Node *node);
    void _build_attributes(LR_Node *node, TreeDataObject *object);
    void _build_attribute_list(LR_Node *node, TreeDataObject *object);
    void _build_attribute(LR_Node *node, TreeDataObject *object);
    TreeData * _build_value(LR_Node *node);
    TreeData * _build_number(LR_Node *node);
    TreeData * _build_minus_number(LR_Node *node);
    TreeData * _build_array(LR_Node *node);
    TreeData * _build_object(LR_Node *node);
    void _build_value_list(LR_Node *node, TreeDataArray *array);
};

_vn_end

#endif
