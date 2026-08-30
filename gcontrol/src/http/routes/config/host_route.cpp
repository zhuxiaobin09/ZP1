#include "http/routes/config/host_route.h"

#include "http/common/config_file_util.h"

#include <httplib.h>

namespace gcontrol {
namespace http {
namespace routes {
namespace config {
namespace {

constexpr const char* kGetPath = "/config/host";
constexpr const char* kPutPath = "/config/host";
constexpr const char* kConfigRelativePath = "host_config/host.json";

/**
 * @brief 获取 host 配置文件路径。
 * @return std::filesystem::path 配置文件完整路径。
 */
std::filesystem::path HostConfigPath() {
  return common::GetUserdataRoot() / kConfigRelativePath;
}

}  // namespace

/**
 * @brief 注册 host 配置文件读写 HTTP 路由。
 * @param server 待注册路由的服务器实例。
 */
void RegisterConfigHostRoutes(httplib::Server& server) {
  server.Get(kGetPath, [](const httplib::Request& /*req*/,
                          httplib::Response& res) {
    common::SendConfigFile(res, HostConfigPath());
  });

  server.Put(kPutPath, [](const httplib::Request& req,
                          httplib::Response& res) {
    common::UpdateConfigFile(req, res, HostConfigPath());
  });
}

}  // namespace config
}  // namespace routes
}  // namespace http
}  // namespace gcontrol
