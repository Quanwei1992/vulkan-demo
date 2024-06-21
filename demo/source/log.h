#pragma once

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include <memory>
#include <cstdint>
#include <stdexcept>
#include <cassert>


enum class ELogLevel : uint8_t
{
	Debug,
	Info,
	Warn,
	Error,
	Fatal
};

class LogSystem final
{

public:

	static void initialize();
	static void shutdown();

	LogSystem();
	~LogSystem();

	template<typename... Args>
	void log(ELogLevel level, Args&&... args)
	{
		switch (level)
		{
		case ELogLevel::Debug:
			m_logger->debug(std::forward<Args>(args)...);
			break;
		case ELogLevel::Info:
			m_logger->info(std::forward<Args>(args)...);
			break;
		case ELogLevel::Warn:
			m_logger->warn(std::forward<Args>(args)...);
			break;
		case ELogLevel::Error:
			m_logger->error(std::forward<Args>(args)...);
			break;
		case ELogLevel::Fatal:
			m_logger->critical(std::forward<Args>(args)...);
			fatalCallback(std::forward<Args>(args)...);
			break;
		default:
			break;
		}
	}

	template<typename... TARGS>
	void fatalCallback(TARGS&&... args)
	{
		const std::string format_str = fmt::format(std::forward<TARGS>(args)...);
		throw std::runtime_error(format_str);
	}
private:
	std::shared_ptr<spdlog::logger> m_logger;
};

LogSystem& GetLogSystem();

#define LogDebug(...) GetLogSystem().log(ELogLevel::Debug,__VA_ARGS__)
#define LogInfo(...) GetLogSystem().log(ELogLevel::Info,__VA_ARGS__)
#define LogWarn(...) GetLogSystem().log(ELogLevel::Warn,__VA_ARGS__)
#define LogError(...) GetLogSystem().log(ELogLevel::Error,__VA_ARGS__)
#define LogFatal(...) GetLogSystem().log(ELogLevel::Fatal,__VA_ARGS__)

#define Assert(test) assert(test)