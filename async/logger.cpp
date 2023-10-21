
#include "logger.h"

#include <chrono>
#include <sstream>
#include <boost/filesystem.hpp>


namespace fs = boost::filesystem;

logger::Logger::Logger() : m_stopped_(false) {}

void logger::Logger::Process(const logger::bulk_t &) {}

void logger::Logger::Worker() {}

void logger::Logger::Stop() {}

void logger::Logger::ProcessOne(std::ostream &os, logger::bulk_t &bulk) {
    os << bulk.front();
    bulk.pop();
    if (!bulk.empty()) os << ", ";
}

logger::Console::Console(std::ostream &os) :
        Logger(), m_os_(os), m_threadLog_(std::thread([this] { Worker(); })) {}

logger::Console::~Console() {
    Console::Stop();
}

void logger::Console::Process(const logger::bulk_t &bulk) {
    std::lock_guard<std::mutex> lk(m_mutex_);
    m_bulks_.push(bulk);
    m_cv_.notify_one();
}

void logger::Console::Stop() {
    if (!m_stopped_) {
        m_stopped_ = true;
        m_cv_.notify_all();
        m_threadLog_.join();
    }
}

void logger::Console::Worker() {
    do {
        std::unique_lock<std::mutex> lk(m_mutex_);
        m_cv_.wait(lk, [this]() { return m_stopped_ || !m_bulks_.empty(); });
        while (!m_bulks_.empty()) {
            ProcessBulk(m_os_, m_bulks_.front());
            m_bulks_.pop();
        }
        if (m_stopped_)break;
    } while (!m_stopped_);
}

void logger::Console::ProcessBulk(std::ostream &os, logger::bulk_t &bulk) {
    os << "bulk: ";
    while (!bulk.empty()) {
        os << bulk.front();
        bulk.pop();
        if (!bulk.empty())os << ", ";
    }
    os << std::endl;
}

logger::LogFile::LogFile()
        : Logger(),
          m_threadFile1_(std::thread([this] { Worker(); })),
          m_threadFile2_(std::thread([this] { Worker(); })) {

    fs::create_directory("./log");
}

logger::LogFile::~LogFile() {
    LogFile::Stop();
}

void logger::LogFile::Process(const logger::bulk_t &bulk) {
    std::lock_guard<std::mutex> lk(m_mutex_);
    m_bulks_.push(bulk);
    m_cv_.notify_all();
}

void logger::LogFile::Stop() {
    if (!m_stopped_) {
        m_stopped_ = true;
        m_cv_.notify_all();
        m_threadFile1_.join();
        m_threadFile2_.join();

    }
}

void logger::LogFile::Worker() {
    do {

        std::unique_lock<std::mutex> lk(m_mutex_);
        m_cv_.wait(lk, [this]() { return m_stopped_ || !m_bulks_.empty(); });

        if (!m_bulks_.empty()) {

            auto &bulk = m_bulks_.front();

            if (!m_logFile_.is_open()) {
                OpenNewLogFile();
            }

            ProcessOne(m_logFile_, bulk);

            if (bulk.empty()) {
                m_bulks_.pop();
                m_logFile_.close();
            }
        }

    } while (!m_stopped_ || !m_bulks_.empty());
}

void logger::LogFile::OpenNewLogFile() {
    static auto fileNum = 0;
    auto result = std::chrono::system_clock::now().time_since_epoch().count();
    std::ostringstream ossFilename;
    std::ostream &osFilename = ossFilename;
    osFilename << "log/bulk" << result
               << "_" << std::this_thread::get_id() << fileNum++
               << ".log";
    m_logFileName_ = ossFilename.str();
    m_logFile_.open(m_logFileName_);

    // write some service info to file
    if (m_logFile_.is_open()) {
        m_logFile_ << "bulk: ";
    } else {
        using namespace std::string_view_literals;
        throw "Error! File " + m_logFileName_ + "is not opened";
    }
}