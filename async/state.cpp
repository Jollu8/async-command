
#include "state.h"
#include "handler.h"

bulk::StateBase::StateBase(bulk::Handler *handler) : m_handler_(handler) {}

bulk::StateEmpty::StateEmpty(bulk::Handler *handler) : bulk::StateBase(handler) {}

void bulk::StateEmpty::CmdOpenedBracket() {
    m_handler_->PushOpenBracket();
    m_handler_->SetState(bulk::StateBasePtr{new StateDynamic(m_handler_)});

}

void bulk::StateEmpty::CmdClosedBracket() { PushCmd("}"); }

void bulk::StateEmpty::CmdEof() {//
}

void bulk::StateEmpty::CmdOther(const bulk::Cmd &cmd) {
    PushCmd(cmd);
}

void bulk::StateEmpty::PushCmd(const bulk::Cmd &cmd) {
    m_handler_->PushCmd(cmd);
    if (m_handler_->CmdsSize() == m_handler_->BulkSize()) m_handler_->ProcessBulk();
    else m_handler_->SetState(StateBasePtr{new bulk::StateStatic(m_handler_)});
}

bulk::StateStatic::StateStatic(bulk::Handler *handler) : bulk::StateBase(handler) {}

void bulk::StateStatic::CmdOpenedBracket() {
    m_handler_->PushOpenBracket();
    m_handler_->ProcessBulk();
    m_handler_->SetState(bulk::StateBasePtr{new bulk::StateDynamic(m_handler_)});
}

void bulk::StateStatic::CmdClosedBracket() {
    PushCmd("}");
}

void bulk::StateStatic::CmdEof() { m_handler_->ProcessBulk(); }

void bulk::StateStatic::CmdOther(const bulk::Cmd &cmd) { PushCmd(cmd); }

void bulk::StateStatic::PushCmd(const bulk::Cmd &cmd) {
    m_handler_->PushCmd(cmd);
    if (m_handler_->CmdsSize() == m_handler_->BulkSize()) m_handler_->ProcessBulk();
}

bulk::StateDynamic::StateDynamic(bulk::Handler *handler) : bulk::StateBase(handler) {}

void bulk::StateDynamic::CmdOpenedBracket() { m_handler_->PushOpenBracket(); }

void bulk::StateDynamic::CmdClosedBracket() {
    m_handler_->PopOpenBracket();
    if (m_handler_->BracketSize() == 0) {
        m_handler_->ProcessBulk();
        m_handler_->SetState(bulk::StateBasePtr{new bulk::StateEmpty(m_handler_)});
    }
}

void bulk::StateDynamic::CmdEof() {
    //
}

void bulk::StateDynamic::CmdOther(const bulk::Cmd &cmd) {
    m_handler_->PushCmd(cmd);
}

