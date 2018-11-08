#include "core/Log.h"
#include "spdlog/async_logger.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/dist_sink.h"
#include "spdlog/sinks/msvc_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using namespace CR::Core::Log;
using namespace std;

detail::Logger::Logger() {
	spdlog::init_thread_pool(8192, 1);
	auto stdSink  = make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto msvcSink = make_shared<spdlog::sinks::msvc_sink_mt>();
	std::vector<spdlog::sink_ptr> sinks{stdSink, msvcSink};

	m_logger = make_shared<spdlog::async_logger>("default_logger", sinks.begin(), sinks.end(), spdlog::thread_pool(),
	                                             spdlog::async_overflow_policy::block);

	m_logger->set_pattern("[%H:%M:%S:%f] [thread %t] [%^%l%$] %v ");
	m_logger->flush_on(spdlog::level::warn);
	spdlog::register_logger(m_logger);
}

detail::Logger::~Logger() {
	Free();
}

void detail::Logger::Free() {
	if(m_logger) {
		m_logger->flush();
		m_logger.reset();
		spdlog::drop_all();
		spdlog::shutdown();
	}
}

CR::Core::Log::detail::Logger& CR::Core::Log::detail::GetLogger() {
	static CR::Core::Log::detail::Logger logger;
	return logger;
}
