#pragma once

#include "assignments.h"


#include <fstream>
#include <memory>
#include <iostream>
#include <queue>
#include <condition_variable>
#include <thread>
#include <atomic>

namespace logger {
    using bulk_t = std::queue<bulk::Cmd>;

    class Logger;

    using LogPtr = std::shared_ptr<Logger>;

    class ILogger {
    public: // Methods
        virtual void Process(const bulk_t &bulk) = 0;

        virtual void Stop() = 0;

        virtual void Worker() = 0;

    };

    class Logger : public ILogger {
    public: // Constructor
        Logger();

    public: // Destructor
        virtual ~Logger() = default;

    public: /// <- Methods
        void Process(const bulk_t &) override;

        void Stop() override;

    protected: /// <- Methods
        void Worker() override;

        static void ProcessOne(std::ostream &os, bulk_t &bulk);

    protected: /// <- Fields
        std::queue<bulk_t> m_bulks_;
        std::condition_variable m_cv_;
        std::mutex m_mutex_;
        std::atomic<bool> m_stopped_;

    };

    class Console : public Logger {
    public: // Constructor and Destructor
        Console(std::ostream &os = std::cout);

        ~Console();

    public: // Methods
        void Process(const bulk_t &bulk) override;

        void Stop() override;

    private: // Fields
        std::ostream &m_os_;
        std::thread m_threadLog_;
    private: // Methods
        void Worker() override;

        void ProcessBulk(std::ostream &os, bulk_t &bulk);


    };

    class LogFile : public Logger {
    public: // Constructor and Destructor
        LogFile();

        ~LogFile();

    public: // Methods
        void Process(const bulk_t &bulk) override;

        void Stop() override;

    private: // Fields
        std::ofstream m_logFile_;
        std::string m_logFileName_;
        std::thread m_threadFile1_;
        std::thread m_threadFile2_;

    private: // Methods
        void Worker() override;

        void OpenNewLogFile();

    };
}