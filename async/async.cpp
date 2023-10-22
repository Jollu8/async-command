
#include "async.h"
#include "handler.h"

namespace async {
    logger::LogPtr m_console = nullptr;
    logger::LogPtr m_logfile = nullptr;
    std::mutex m_mutex;

    handle_t connect(std::size_t bulk) {
        std::lock_guard<std::mutex> lk(m_mutex);
        if (m_console == nullptr) m_console = logger::LogPtr{new logger::Console(std::cout)};
        if (m_logfile == nullptr) m_logfile = logger::LogPtr{new logger::LogFile};

        bulk::Handler *context{new bulk::Handler(bulk)};
        context->RegisterLogger(m_console);
        context->RegisterLogger(m_logfile);
        return context;
    }

    void receive(handle_t handle, const char *data, std::size_t size) {
        std::lock_guard<std::mutex> lk(m_mutex);
        bulk::Handler *context = static_cast<bulk::Handler *>(handle);
        context->Receive(data, size);
    }

    void disconnect(handle_t handle) {
        std::lock_guard<std::mutex> lk(m_mutex);
        bulk::Handler *context = static_cast<bulk::Handler *>(handle);
        context->ReceiveEof();
        delete context;

    }
}