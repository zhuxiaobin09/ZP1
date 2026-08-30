#ifndef GCONTROL_SRC_HTTP_COMMON_CONFIG_FILE_UTIL_H_
#define GCONTROL_SRC_HTTP_COMMON_CONFIG_FILE_UTIL_H_

#include <filesystem>
#include <optional>
#include <string>

namespace httplib {
class Request;
class Response;
}

namespace gcontrol {
namespace http {
namespace common {

/**
 * @brief 获取 userdata 根目录路径。
 * @return std::filesystem::path userdata 目录路径。
 */
std::filesystem::path GetUserdataRoot();

/**
 * @brief 读取配置文件内容为字符串。
 * @param path 配置文件路径。
 * @return std::optional<std::string> 成功时返回文件内容，失败返回
 *     std::nullopt。
 */
std::optional<std::string> ReadConfigFile(
    const std::filesystem::path& path);

/**
 * @brief 将内容写入配置文件，必要时创建父目录。
 * @param path 配置文件路径。
 * @param content 待写入内容。
 * @return bool 写入成功返回 true，否则 false。
 */
bool WriteConfigFile(const std::filesystem::path& path,
                     const std::string& content);

/**
 * @brief 读取配置文件并写入 HTTP 响应。
 * @param res HTTP 响应。
 * @param path 配置文件路径。
 */
void SendConfigFile(httplib::Response& res,
                    const std::filesystem::path& path);

/**
 * @brief 将请求体写入配置文件并返回 HTTP 响应。
 * @param req HTTP 请求。
 * @param res HTTP 响应。
 * @param path 配置文件路径。
 */
void UpdateConfigFile(const httplib::Request& req, httplib::Response& res,
                      const std::filesystem::path& path);

}  // namespace common
}  // namespace http
}  // namespace gcontrol

#endif  // GCONTROL_SRC_HTTP_COMMON_CONFIG_FILE_UTIL_H_
