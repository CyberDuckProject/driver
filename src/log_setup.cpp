#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#define BOOST_LOG_USE_NATIVE_SYSLOG
#include <boost/log/sinks.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace syslog = boost::log::sinks::syslog;
namespace keywords = boost::log::keywords;

namespace {

class log_setup
{
public:
    log_setup()
    {
        init_syslog();
        init_stderr();
        logging::add_common_attributes();
#ifndef NDEBUG
        logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::debug);
#else
        logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
#endif
    }

private:
    void init_syslog()
    {
        // Create a backend
        auto backend{boost::make_shared<sinks::syslog_backend>(
            keywords::facility = syslog::user, keywords::use_impl = syslog::native)};

        // Add mapping from trivial severity levels
        syslog::custom_severity_mapping<logging::trivial::severity_level> mapping{"Severity"};
        mapping[logging::trivial::debug] = syslog::debug;
        mapping[logging::trivial::info] = syslog::info;
        mapping[logging::trivial::warning] = syslog::warning;
        mapping[logging::trivial::error] = syslog::error;
        mapping[logging::trivial::fatal] = syslog::critical;
        backend->set_severity_mapper(mapping);

        // Add to logging core
        logging::core::get()->add_sink(
            boost::make_shared<sinks::synchronous_sink<sinks::syslog_backend>>(backend));
    }

    void init_stderr()
    {
        logging::add_console_log(std::clog,
                                 keywords::format = "[%TimeStamp%] [%Severity%] %Message%");
    }

} log_setup;

} // namespace
