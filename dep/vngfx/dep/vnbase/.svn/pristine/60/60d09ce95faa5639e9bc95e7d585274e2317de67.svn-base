//
//  vnFilePath.h
//  vnbase
//
//  Created by Wayne on 13-12-9.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnbase_vnFilePath_h
#define vnbase_vnFilePath_h

#include "vnAllocatedObject.h"

_vn_begin

struct FilePath : AllocatedObject {
    u32 fsId;
    str8 fileName;
    
    FilePath();
    FilePath(u32 _fsId, const str8 &_fileName);
    FilePath(u32 _fsId, const c8 *_fileName);
    FilePath(const c8 *fullPath);
    
    str8 & getFullPath(str8 &fullPath) const;
    str8 getFullPath() const;
    
    bool operator ==(const FilePath &rhs) const;
    bool operator !=(const FilePath &rhs) const;
    bool operator <(const FilePath &rhs) const;
    
    FilePath & operator =(const FilePath &rhs);
    FilePath & operator =(const c8 *fullPath);
};

_vn_end

#endif
