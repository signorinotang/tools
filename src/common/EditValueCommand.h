//
//  EditValueCommand.h
//  test
//
//  Created by Wayne on 14-1-6.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __test__EditValueCommand__
#define __test__EditValueCommand__

#include "vnEditValue.h"
#include "vnCommandList.h"
#include "vnSingleton.h"

class EditValueCommand : public vn::Command, public vn::AllocatedObject {
public:
    EditValueCommand(vn::EditValue *value, const vn::EditValue::BackupData *oldData);
    virtual void _free();
	virtual void _do();
	virtual void _undo();
	virtual void _redo();
protected:
    vn::RefCountedPtr<vn::EditValue> m_value;
    vn::RefCountedPtr<const vn::EditValue::BackupData> m_oldData;
    vn::RefCountedPtr<const vn::EditValue::BackupData> m_newData;
};

class EditValueCommandGenerator : public vn::EditValue::Observer, public vn::Singleton<EditValueCommandGenerator> {
public:
    EditValueCommandGenerator();
    
    void setCommandList(vn::CommandList *commandList);
    
    virtual void ev_changed_backup(vn::EditValue *value, const vn::EditValue::BackupData *data);
    
private:
    vn::CommandList *m_commandList;
};

#endif /* defined(__test__EditValueCommand__) */
