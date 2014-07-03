//
//  EditValueCommand.cpp
//  test
//
//  Created by Wayne on 14-1-6.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "EditValueCommand.h"


EditValueCommand::EditValueCommand(vn::EditValue *value, const vn::EditValue::BackupData *oldData)
: m_value(value)
, m_oldData(oldData)
, m_newData(value->backup()) {
    oldData->grab();
    m_value->grab();
}

void EditValueCommand::_free() {
    delete this;
}

void EditValueCommand::_do() {
    
}

void EditValueCommand::_undo() {
    m_oldData->restore(m_value.ptr());
}

void EditValueCommand::_redo() {
    m_newData->restore(m_value.ptr());
}




EditValueCommandGenerator::EditValueCommandGenerator()
: m_commandList(0) {
    
}

void EditValueCommandGenerator::setCommandList(vn::CommandList *commandList) {
    m_commandList = commandList;
}

void EditValueCommandGenerator::ev_changed_backup(vn::EditValue *value, const vn::EditValue::BackupData *data) {
    if (m_commandList) {
        m_commandList->do_(vnnew EditValueCommand(value, data));
    }
}