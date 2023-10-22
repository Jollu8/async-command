#include "handler.h"

#include <sstream>

using namespace bulk;

Handler::Handler(const size_t &bulkSize)
        : m_bulkSize_(bulkSize)
        , m_state_(StateBasePtr{new StateEmpty(this)})
{
}

void Handler::Receive(const char *data, size_t size)
{
    for (auto i = 0u; i < size; i++) {
        if (data[i] == '\n') {
            ReceiveCmd(m_buffer_);
            m_buffer_.clear();
        } else {
            m_buffer_.append({data[i]});
        }
    }
}

void Handler::ReceiveCmd(const Cmd &cmd)
{
    if (IsOpenedBracket(cmd)) {
        m_state_->CmdOpenedBracket();
    } else if (IsClosedBracket(cmd)) {
        m_state_->CmdClosedBracket();
    } else {
        m_state_->CmdOther(cmd);
    }
}

void Handler::ReceiveEof()
{
    m_state_->CmdEof();
}

void Handler::RegisterLogger(logger::LogPtr logger)
{
    m_loggers_.emplace_back(std::move(logger));
}

void Handler::SetState(StateBasePtr state)
{
    m_state_ = std::move(state);
}

size_t Handler::BulkSize() const
{
    return m_bulkSize_;
}

size_t Handler::CmdsSize() const
{
    return m_cmds_.size();
}

size_t Handler::BracketSize() const
{
    return m_brackets_.size();
}

void Handler::PushOpenBracket()
{
    m_brackets_.push('{');
}

void Handler::PopOpenBracket()
{
    m_brackets_.pop();
}

void Handler::PushCmd(const Cmd &cmd)
{
    m_cmds_.push(cmd);
}

void Handler::ProcessBulk()
{
    for (const auto &logger : m_loggers_) {
        logger->Process(m_cmds_);
    }
    while (!m_cmds_.empty()) {
        m_cmds_.pop();
    }
}

bool Handler::IsOpenedBracket(const Cmd &cmd)
{
    return IsAnyBracket(cmd, '{');
}

bool Handler::IsClosedBracket(const Cmd &cmd)
{
    return IsAnyBracket(cmd, '}');
}

bool Handler::IsAnyBracket(const Cmd &cmd, Bracket anyBracket)
{
    if (cmd.size() == 1) {
        return cmd.at(0) == anyBracket;
    } else {
        return false;
    }
}
