#include "http/routes/config/gcontrol_route.h"

#include "http/common/config_file_util.h"

#include <httplib.h>

namespace gcontrol {
namespace http {
namespace routes {
namespace config {
namespace {

constexpr const char* kGetPath = "/config/gcontrol";
constexpr const char* kPutPath = "/config/gcontrol";
constexpr const char* kConfigRelativePath = "gcontrol_config/gcontrol.json";

/**
 * @brief 获取 gcontrol 配置文件路径。
 * @return std::filesystem::path 配置文件完整路径。
 */
std::filesystem::path GcontrolConfigPath() {
  return common::GetUserdataRoot() / kConfigRelativePath;
}

}  // namespace

/**
 * @brief 注册 gcontrol 配置文件读写 HTTP 路由。
 * @param server 待注册路由的服务器实例。
 */
void RegisterConfigGcontrolRoutes(httplib::Server& server) {
  server.Get(kGetPath, [](const httplib::Request& /*req*/,
                          httplib::Response& res) {
    common::SendConfigFile(res, GcontrolConfigPath());
  });

  server.Put(kPutPath, [](const httplib::Request& req,
                          httplib::Response& res) {
    common::UpdateConfigFile(req, res, GcontrolConfigPath());
  });
}

}  // namespace config
}  // namespace routes
}  // namespace http
}  // namespace gcontrol
