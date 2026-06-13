#include "http/routes/log_config_route.h"

#include "http/routes/config_file_util.h"

#include <httplib.h>

namespace gcontrol {
namespace http {
namespace routes {
namespace {

constexpr const char* kGetPath = "/config/log";
constexpr const char* kPutPath = "/config/log";
constexpr const char* kConfigRelativePath = "log_config/log.json";

/**
 * @brief 获取 log 配置文件路径。
 * @return std::filesystem::path 配置文件完整路径。
 */
std::filesystem::path LogConfigPath() {
  return GetUserdataRoot() / kConfigRelativePath;
}

}  // namespace

/**
 * @brief 注册 log 配置文件读写 HTTP 路由。
 * @param server 待注册路由的服务器实例。
 */
void RegisterLogConfigRoutes(httplib::Server& server) {
  server.Get(kGetPath, [](const httplib::Request& /*req*/,
                          httplib::Response& res) {
    SendConfigFile(res, LogConfigPath());
  });

  server.Put(kPutPath, [](const httplib::Request& req,
                          httplib::Response& res) {
    UpdateConfigFile(req, res, LogConfigPath());
  });
}

}  // namespace routes
}  // namespace http
}  // namespace gcontrol
