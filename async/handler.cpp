#include "handler.h"

#include <sstream>

bulk::Handler::Handler(const std::size_t &bulkSize)
        : m_bulkSize_(bulkSize),
          m_state_(StateBasePtr{new StateEmpty(this)}) {}

void bulk::Handler::Receive(const char *data, std::size_t size) {
    for (auto i = 0u; i < size; ++i) {
        if (data[i] == '\n') {
            Handler::ReceiveCmd(m_buffer_);
            m_buffer_.clear();
        } else m_buffer_.append({data[i]});

    }

}

void bulk::Handler::ReceiveCmd(const bulk::Cmd &cmd) {
    if (IsOpenedBracket(cmd))m_state_->CmdOpenedBracket();
    else if (IsClosedBracket(cmd)) m_state_->CmdOpenedBracket();
    else m_state_->CmdOther(cmd);
}

void bulk::Handler::ReceiveEof() {
    m_state_->CmdEof();
}

void bulk::Handler::RegisterLogger(logger::LogPtr logger) {
    m_loggers_.emplace_back(std::move(logger));
}

void bulk::Handler::SetState(bulk::StateBasePtr state) {
    m_state_ = std::move(state);
}

std::size_t bulk::Handler::BulkSize() const { return m_bulkSize_; }

std::size_t bulk::Handler::CmdsSize() const { return m_cmds_.size(); }

std::size_t bulk::Handler::BracketSize() const { return m_bracketSize_.size(); }

void bulk::Handler::PushOpenBracket() {
    m_bracketSize_.push('{');
}

void bulk::Handler::PopOpenBracket() { m_bracketSize_.pop(); }

void bulk::Handler::PushCmd(const bulk::Cmd &cmd) { m_cmds_.push(cmd); }

void bulk::Handler::ProcessBulk() {
    std::for_each(m_loggers_.cbegin(), m_loggers_.cend(), [&](auto &logger) {
        logger->Process(m_cmds_);
    });
    while(!m_cmds_.empty()) m_cmds_.pop();
}

bool bulk::Handler::IsOpenedBracket(const bulk::Cmd &cmd) {
    return IsAnyBracket(cmd, '{');

}

bool bulk::Handler::IsClosedBracket(const bulk::Cmd &cmd) {return IsAnyBracket(cmd, '}');}

bool bulk::Handler::IsAnyBracket(const bulk::Cmd&cmd, bulk::Bracket anyBracket) {
    return (cmd.size() == 1) && (cmd.at(0) == anyBracket);
}


