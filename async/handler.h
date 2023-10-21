#pragma once

#include "logger.h"
#include "assignments.h"
#include "state.h"

#include <list>
#include <iostream>
#include <queue>
#include <stack>

namespace bulk {
    class Handler {
    public: // Constructor
        Handler(const std::size_t &bulkSize);

    public: // methods
        void RegisterLogger(logger::LogPtr logger);

        void Receive(const char *data, std::size_t size);

        void ReceiveEof();

    public: // Setter
        void SetState(StateBasePtr state);

    public: // Properties
        std::size_t BulkSize() const;

        std::size_t CmdsSize() const;

        std::size_t BracketSize() const;

    public: // Handler functionally
        void PushOpenBracket();

        void PopOpenBracket();

        void PushCmd(const bulk::Cmd &cmd);

        void ProcessBulk();

    private: // Fields
        std::size_t m_bulkSize_;
        std::string m_buffer_;
        std::stack<Bracket >m_bracketSize_;
        std::queue<bulk::Cmd> m_cmds_;
        std::list<logger::LogPtr> m_loggers_;
        StateBasePtr m_state_;
    private: // Methods
        void ReceiveCmd(const bulk::Cmd &cmd);

    private: // Static
        static bool IsOpenedBracket(const bulk::Cmd &cmd);

        static bool IsClosedBracket(const bulk::Cmd &cmd);

        static bool IsAnyBracket(const bulk::Cmd&, Bracket anyBracket);


    };

    using HandlerPtr = std::unique_ptr<Handler>;
}