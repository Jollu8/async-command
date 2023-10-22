#pragma once

#include "assignments.h"

namespace bulk {
    class Handler;

    class StateBase {
    public: // Constructor
        explicit StateBase(Handler *handler);

    public: // Destructor
        virtual ~StateBase() = default;

    public: // Methods
        virtual void CmdOpenedBracket() = 0;

        virtual void CmdClosedBracket() = 0;

        virtual void CmdEof() = 0;

        virtual void CmdOther(const bulk::Cmd &cmd) = 0;

    protected:
        Handler *m_handler_;

    };

    class StateEmpty : public StateBase {
    public: // Constructor
        explicit StateEmpty(Handler *handler);

    public: // Methods
        void CmdOpenedBracket() override;

        void CmdClosedBracket() override;

        void CmdEof() override;

        void CmdOther(const bulk::Cmd &cmd) override;

    private: // Methods
        void PushCmd(const bulk::Cmd &cmd);
    };

    class StateStatic : public StateBase {
    public: // Constructor
        explicit StateStatic(Handler *handler);

    public: // Methods

        void CmdOpenedBracket() override;

        void CmdClosedBracket() override;

        void CmdEof() override;

        void CmdOther(const bulk::Cmd &cmd) override;

    private: // Methods
        void PushCmd(const bulk::Cmd &cmd);

    };

    class StateDynamic : public StateBase {
    public: // Constructor
        explicit StateDynamic(Handler *handler);

    public: // Methods
        void CmdOpenedBracket() override;

        void CmdClosedBracket() override;

        void CmdEof() override;

        void CmdOther(const bulk::Cmd &cmd) override;
    };


    using StateBasePtr = std::unique_ptr<StateBase>;
}