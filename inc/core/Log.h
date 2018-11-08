#pragma once
#define SPDLOG_FMT_EXTERNAL
#include "spdlog/async.h"
#include "spdlog/spdlog.h"
#include <cassert>
#include <memory>
#include <thread>

namespace CR::Core::Log {
	enum class Level {
		Info,
		Warn,
		Error,    // will abort in debug and release builds
		Fail      // will abort in all build types
	};
#ifdef CR_DEBUG
	inline constexpr Level c_level = Level::Info;
#else
	inline constexpr Level c_level = Level::Warn;
#endif

	template<typename... ArgTs>
	void Info(const char* a_fmt, ArgTs&&... a_args) {
		detail::GetLogger().Info(a_fmt, std::forward<ArgTs>(a_args)...);
	}

	template<typename... ArgTs>
	void Warn(const char* a_fmt, ArgTs&&... a_args) {
		detail::GetLogger().Warn(a_fmt, std::forward<ArgTs>(a_args)...);
	}

	template<typename... ArgTs>
	void Error(const char* a_fmt, ArgTs&&... a_args) {
		detail::GetLogger().Error(a_fmt, std::forward<ArgTs>(a_args)...);
	}

	template<typename... ArgTs>
	void Fail(const char* a_fmt, ArgTs&&... a_args) {
		detail::GetLogger().Fail(a_fmt, std::forward<ArgTs>(a_args)...);
	}

	namespace detail {
		class Logger {
		  public:
			Logger();
			~Logger();
			Logger(const Logger&) = delete;
			Logger& operator=(const Logger&) = delete;

			template<typename... ArgTs>
			void Info(const char* a_fmt, ArgTs&&... a_args) {
				m_logger->info(a_fmt, std::forward<ArgTs>(a_args)...);
			}

			template<typename... ArgTs>
			void Warn(const char* a_fmt, ArgTs&&... a_args) {
				m_logger->warn(a_fmt, std::forward<ArgTs>(a_args)...);
			}

			template<typename... ArgTs>
			void Error(const char* a_fmt, ArgTs&&... a_args) {
				m_logger->flush();
				m_logger->error(a_fmt, std::forward<ArgTs>(a_args)...);
#if defined(CR_DEBUG) || defined(CR_RELEASE)
				Free();
				abort();
#endif
			}

			template<typename... ArgTs>
			void Fail(const char* a_fmt, ArgTs&&... a_args) {
				m_logger->critical(a_fmt, std::forward<ArgTs>(a_args)...);
				Free();
				abort();
			}

		  private:
			void Free();

			std::shared_ptr<spdlog::async_logger> m_logger;
		};

		Logger& GetLogger();
	}    // namespace detail
}    // namespace CR::Core::Log
