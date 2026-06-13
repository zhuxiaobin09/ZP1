#include "logsystem/logger.h"

#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <mutex>

namespace logsystem {
namespace {

constexpr const char* kDefaultLogDirectory = "log";

// GCC/Clang 的 function_name() 形如 "void ns::Class::RunLoop()"。
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

Logger& Logger::GetInstance() {
  static Logger instance;
  return instance;
}

Logger::Logger() : log_directory_(kDefaultLogDirectory) {}

Logger::~Logger() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (log_file_.is_open()) {
    log_file_.close();
  }
}

void Logger::SetLogDirectory(std::string_view directory) {
  std::lock_guard<std::mutex> lock(mutex_);
  log_directory_ = directory;
  if (log_file_.is_open()) {
    log_file_.close();
  }
  current_log_path_.clear();
}

void Logger::SetMinLevel(LogLevel level) {
  std::lock_guard<std::mutex> lock(mutex_);
  min_level_ = level;
}

void Logger::SetLogToConsole(bool enabled) {
  std::lock_guard<std::mutex> lock(mutex_);
  log_to_console_ = enabled;
}

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

void Logger::Debug(std::string_view message,
                   const std::source_location& location) {
  Log(LogLevel::kDebug, message, location);
}

void Logger::Info(std::string_view message,
                  const std::source_location& location) {
  Log(LogLevel::kInfo, message, location);
}

void Logger::Warning(std::string_view message,
                     const std::source_location& location) {
  Log(LogLevel::kWarning, message, location);
}

void Logger::Error(std::string_view message,
                   const std::source_location& location) {
  Log(LogLevel::kError, message, location);
}

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

std::string_view Logger::BaseFileName(std::string_view path) {
  const auto pos = path.find_last_of("/\\");
  if (pos == std::string_view::npos) {
    return path;
  }
  return path.substr(pos + 1);
}

}  // namespace logsystem
