//
//  TableRecordDesc.h
//  tblcoder
//
//  Created by Wayne on 14-2-8.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __tblcoder__TableRecordDesc__
#define __tblcoder__TableRecordDesc__

#include "TableBase.h"
#include "vnTreeData.h"

namespace tbl {
    using namespace vn;
    
    struct FieldDesc {
        str8 name;
        FieldFormat format;
    };
    
    struct RecordDesc {
        str8 name;
        u32 version;
        typedef std::vector<FieldDesc> Fields;
        Fields fields;
        
        void init(const TreeDataNode *root);
    };
}

#endif /* defined(__tblcoder__TableRecordDesc__) */
