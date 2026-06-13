#include "http/common/config_file_util.h"

#include "logsystem/logger.h"

#include <httplib.h>

#include <cstdlib>
#include <fstream>
#include <mutex>
#include <sstream>

namespace gcontrol {
namespace http {
namespace common {
namespace {

std::mutex config_mutex;

}  // namespace

/**
 * @brief 获取 userdata 根目录路径。
 * @return std::filesystem::path userdata 目录路径。
 */
std::filesystem::path GetUserdataRoot() {
  if (const char* env_dir = std::getenv("ZP1_USERDATA_DIR")) {
    return env_dir;
  }
  return "userdata";
}

/**
 * @brief 读取配置文件内容为字符串。
 * @param path 配置文件路径。
 * @return std::optional<std::string> 成功时返回文件内容，失败返回
 *     std::nullopt。
 */
std::optional<std::string> ReadConfigFile(
    const std::filesystem::path& path) {
  std::ifstream input(path, std::ios::binary);
  if (!input.is_open()) {
    return std::nullopt;
  }

  std::ostringstream buffer;
  buffer << input.rdbuf();
  return buffer.str();
}

/**
 * @brief 将内容写入配置文件，必要时创建父目录。
 * @param path 配置文件路径。
 * @param content 待写入内容。
 * @return bool 写入成功返回 true，否则 false。
 */
bool WriteConfigFile(const std::filesystem::path& path,
                     const std::string& content) {
  const std::filesystem::path parent = path.parent_path();
  if (!parent.empty() && !std::filesystem::exists(parent)) {
    std::error_code ec;
    if (!std::filesystem::create_directories(parent, ec) && ec) {
      LOG_ERROR("Failed to create config directory {}: {}", parent.string(),
                ec.message());
      return false;
    }
  }

  std::ofstream output(path, std::ios::binary | std::ios::trunc);
  if (!output.is_open()) {
    LOG_ERROR("Failed to open config file for writing: {}", path.string());
    return false;
  }

  output << content;
  return output.good();
}

/**
 * @brief 读取配置文件并写入 HTTP 响应。
 * @param res HTTP 响应。
 * @param path 配置文件路径。
 */
void SendConfigFile(httplib::Response& res,
                    const std::filesystem::path& path) {
  std::lock_guard<std::mutex> lock(config_mutex);
  const auto content = ReadConfigFile(path);
  if (!content.has_value()) {
    res.status = 404;
    res.set_content(R"({"error":"config file not found"})", "application/json");
    return;
  }

  res.status = 200;
  res.set_content(*content, "application/json");
}

/**
 * @brief 将请求体写入配置文件并返回 HTTP 响应。
 * @param req HTTP 请求。
 * @param res HTTP 响应。
 * @param path 配置文件路径。
 */
void UpdateConfigFile(const httplib::Request& req, httplib::Response& res,
                      const std::filesystem::path& path) {
  if (req.body.empty()) {
    res.status = 400;
    res.set_content(R"({"error":"empty body"})", "application/json");
    return;
  }

  std::lock_guard<std::mutex> lock(config_mutex);
  if (!WriteConfigFile(path, req.body)) {
    res.status = 500;
    res.set_content(R"({"error":"failed to write config"})",
                    "application/json");
    return;
  }

  res.status = 200;
  res.set_content(R"({"status":"ok"})", "application/json");
}

}  // namespace common
}  // namespace http
}  // namespace gcontrol
