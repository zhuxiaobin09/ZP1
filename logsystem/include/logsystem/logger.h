#ifndef LOGSYSTEM_INCLUDE_LOGSYSTEM_LOGGER_H_
#define LOGSYSTEM_INCLUDE_LOGSYSTEM_LOGGER_H_

#include <format>
#include <fstream>
#include <mutex>
#include <source_location>
#include <string>
#include <string_view>

namespace logsystem {

enum class LogLevel {
  kDebug = 0,
  kInfo = 1,
  kWarning = 2,
  kError = 3,
};

class Logger {
 public:
  static Logger& GetInstance();

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  void SetLogDirectory(std::string_view directory);
  void SetMinLevel(LogLevel level);
  void SetLogToConsole(bool enabled);

  void Log(LogLevel level, std::string_view message,
           const std::source_location& location);
  void Debug(std::string_view message, const std::source_location& location);
  void Info(std::string_view message, const std::source_location& location);
  void Warning(std::string_view message, const std::source_location& location);
  void Error(std::string_view message, const std::source_location& location);

 private:
  Logger();
  ~Logger();

  void EnsureLogFileOpen();
  void WriteLog(LogLevel level, std::string_view message,
                const std::source_location& location);
  static std::string LevelToString(LogLevel level);
  static std::string CurrentTimestamp();
  static std::string_view BaseFileName(std::string_view path);

  std::mutex mutex_;
  std::ofstream log_file_;
  std::string log_directory_;
  std::string current_log_path_;
  LogLevel min_level_ = LogLevel::kDebug;
  bool log_to_console_ = true;
};

}  // namespace logsystem

#define LOG_DEBUG(...) \
  ::logsystem::Logger::GetInstance().Debug( \
      std::format(__VA_ARGS__), std::source_location::current())
#define LOG_INFO(...) \
  ::logsystem::Logger::GetInstance().Info( \
      std::format(__VA_ARGS__), std::source_location::current())
#define LOG_WARNING(...) \
  ::logsystem::Logger::GetInstance().Warning( \
      std::format(__VA_ARGS__), std::source_location::current())
#define LOG_ERROR(...) \
  ::logsystem::Logger::GetInstance().Error( \
      std::format(__VA_ARGS__), std::source_location::current())

#endif  // LOGSYSTEM_INCLUDE_LOGSYSTEM_LOGGER_H_
