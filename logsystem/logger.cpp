#include "logsystem/logger.h"

#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <mutex>

namespace logsystem {
namespace {

constexpr const char* kDefaultLogDirectory = "log";

/**
 * @brief 从编译器提供的函数全名中提取简短函数名（含括号）。
 * @param pretty_name __PRETTY_FUNCTION__ 或 function_name() 字符串。
 * @return std::string 形如 "RunLoop()" 的简短名称。
 */
std::string ShortFunctionName(std::string_view pretty_name) {
  const auto paren = pretty_name.find('(');
  if (paren == std::string_view::npos) {
    return std::string(pretty_name);
  }

  const std::string_view before_paren = pretty_name.substr(0, paren);
  const auto scope_pos = before_paren.rfind("::");
  std::string_view name = before_paren;
  if (scope_pos != std::string_view::npos) {
    name = before_paren.substr(scope_pos + 2);
  } else {
    const auto space_pos = before_paren.rfind(' ');
    if (space_pos != std::string_view::npos) {
      name = before_paren.substr(space_pos + 1);
    }
  }
  return std::format("{}()", name);
}

}  // namespace

/**
 * @brief 获取 Logger 单例实例。
 * @return Logger& 全局唯一实例的引用。
 */
Logger& Logger::GetInstance() {
  static Logger instance;
  return instance;
}

/**
 * @brief 构造 Logger，初始化默认日志目录。
 */
Logger::Logger() : log_directory_(kDefaultLogDirectory) {}

/**
 * @brief 析构时关闭已打开的日志文件。
 */
Logger::~Logger() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (log_file_.is_open()) {
    log_file_.close();
  }
}

/**
 * @brief 设置日志文件存放目录，并关闭当前已打开的文件。
 * @param directory 日志目录路径。
 */
void Logger::SetLogDirectory(std::string_view directory) {
  std::lock_guard<std::mutex> lock(mutex_);
  log_directory_ = directory;
  if (log_file_.is_open()) {
    log_file_.close();
  }
  current_log_path_.clear();
}

/**
 * @brief 设置最低输出日志级别，低于该级别的日志将被丢弃。
 * @param level 最低日志级别。
 */
void Logger::SetMinLevel(LogLevel level) {
  std::lock_guard<std::mutex> lock(mutex_);
  min_level_ = level;
}

/**
 * @brief 设置是否同时将日志输出到控制台。
 * @param enabled true 表示输出到 stdout。
 */
void Logger::SetLogToConsole(bool enabled) {
  std::lock_guard<std::mutex> lock(mutex_);
  log_to_console_ = enabled;
}

/**
 * @brief 按级别写入日志，低于最低级别时直接返回。
 * @param level 日志级别。
 * @param message 日志内容。
 * @param location 调用位置（文件名、行号、函数名）。
 */
void Logger::Log(LogLevel level, std::string_view message,
                 const std::source_location& location) {
  LogLevel current_min_level;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    current_min_level = min_level_;
  }
  if (level < current_min_level) {
    return;
  }
  WriteLog(level, message, location);
}

/**
 * @brief 写入 DEBUG 级别日志。
 * @param message 日志内容。
 * @param location 调用位置。
 */
void Logger::Debug(std::string_view message,
                   const std::source_location& location) {
  Log(LogLevel::kDebug, message, location);
}

/**
 * @brief 写入 INFO 级别日志。
 * @param message 日志内容。
 * @param location 调用位置。
 */
void Logger::Info(std::string_view message,
                  const std::source_location& location) {
  Log(LogLevel::kInfo, message, location);
}

/**
 * @brief 写入 WARNING 级别日志。
 * @param message 日志内容。
 * @param location 调用位置。
 */
void Logger::Warning(std::string_view message,
                     const std::source_location& location) {
  Log(LogLevel::kWarning, message, location);
}

/**
 * @brief 写入 ERROR 级别日志。
 * @param message 日志内容。
 * @param location 调用位置。
 */
void Logger::Error(std::string_view message,
                   const std::source_location& location) {
  Log(LogLevel::kError, message, location);
}

/**
 * @brief 确保按日期命名的日志文件已创建并打开（懒加载）。
 */
void Logger::EnsureLogFileOpen() {
  if (log_file_.is_open()) {
    return;
  }

  std::error_code ec;
  std::filesystem::create_directories(log_directory_, ec);
  if (ec) {
    std::cerr << "Failed to create log directory: " << ec.message()
              << std::endl;
    return;
  }

  const auto now = std::chrono::system_clock::now();
  const auto time_t_now = std::chrono::system_clock::to_time_t(now);
  std::tm local_time{};
  localtime_r(&time_t_now, &local_time);

  current_log_path_ = std::format(
      "{}/log_{:04d}{:02d}{:02d}.txt",
      log_directory_,
      local_time.tm_year + 1900,
      local_time.tm_mon + 1,
      local_time.tm_mday);

  log_file_.open(current_log_path_, std::ios::app);
  if (!log_file_.is_open()) {
    std::cerr << "Failed to open log file: " << current_log_path_
              << std::endl;
  }
}

/**
 * @brief 格式化日志并写入文件，可选同时输出到控制台。
 * @param level 日志级别。
 * @param message 日志内容。
 * @param location 调用位置（文件名、行号、函数名）。
 */
void Logger::WriteLog(LogLevel level, std::string_view message,
                      const std::source_location& location) {
  const std::string formatted = std::format(
      "[{}] [{}] [{}:{} {}] {}\n",
      CurrentTimestamp(),
      LevelToString(level),
      BaseFileName(location.file_name()),
      location.line(),
      ShortFunctionName(location.function_name()),
      message);

  std::lock_guard<std::mutex> lock(mutex_);
  EnsureLogFileOpen();
  if (log_file_.is_open()) {
    log_file_ << formatted;
    log_file_.flush();
  }
  if (log_to_console_) {
    std::cout << formatted;
  }
}

/**
 * @brief 将 LogLevel 枚举转换为字符串标签。
 * @param level 日志级别枚举值。
 * @return std::string 如 "INFO"、"ERROR" 等。
 */
std::string Logger::LevelToString(LogLevel level) {
  switch (level) {
    case LogLevel::kDebug:
      return "DEBUG";
    case LogLevel::kInfo:
      return "INFO";
    case LogLevel::kWarning:
      return "WARNING";
    case LogLevel::kError:
      return "ERROR";
    default:
      return "UNKNOWN";
  }
}

/**
 * @brief 生成当前本地时间的格式化字符串（含毫秒）。
 * @return std::string 形如 "2026-06-13 12:34:56.789"。
 */
std::string Logger::CurrentTimestamp() {
  const auto now = std::chrono::system_clock::now();
  const auto time_t_now = std::chrono::system_clock::to_time_t(now);
  const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now.time_since_epoch()) %
                  1000;

  std::tm local_time{};
  localtime_r(&time_t_now, &local_time);

  return std::format(
      "{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}.{:03d}",
      local_time.tm_year + 1900,
      local_time.tm_mon + 1,
      local_time.tm_mday,
      local_time.tm_hour,
      local_time.tm_min,
      local_time.tm_sec,
      ms.count());
}

/**
 * @brief 从完整路径中提取文件名部分。
 * @param path 文件路径（可含目录分隔符）。
 * @return std::string_view 不含目录的文件名。
 */
std::string_view Logger::BaseFileName(std::string_view path) {
  const auto pos = path.find_last_of("/\\");
  if (pos == std::string_view::npos) {
    return path;
  }
  return path.substr(pos + 1);
}

}  // namespace logsystem
