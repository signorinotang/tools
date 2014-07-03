//
//  TableRecordDesc.cpp
//  tblcoder
//
//  Created by Wayne on 14-2-8.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "TableRecordDesc.h"

namespace tbl {
    
    void RecordDesc::init(const TreeDataNode *root) {
        if (!root->queryAttributeString("name", name)) {
            name.clear();
        }
        version = root->queryAttributeInteger("version");
        fields.clear();
        for (const TreeDataNode *p = root->firstChild("Field"); p; p = p->nextNeighbour("Field")) {
            fields.push_back(FieldDesc());
            FieldDesc &desc = fields.back();
            p->queryAttributeString("name", desc.name);
            const TreeDataString *tdstr = p->queryAttributeString("type");
            if (!tdstr) {
                desc.format.type = kFT_Invalid;
            } else if (tdstr->value() == "string") {
                desc.format.type = kFT_String;
            } else if (tdstr->value() == "int") {
                desc.format.type = kFT_Int;
            } else if (tdstr->value() == "uint") {
                desc.format.type = kFT_UInt;
            } else if (tdstr->value() == "float") {
                desc.format.type = kFT_Float;
            } else {
                desc.format.type = kFT_Invalid;
            }
            if (p->queryAttributeBoolean("key", false)) {
                desc.format.key = 1;
            } else {
                desc.format.key = 0;
            }
        }
    }
    
}