#include "http/g_http_router.h"

#include "http/routes/route_registry.h"
#include "logsystem/logger.h"

#include <httplib.h>

namespace gcontrol {
namespace http {
namespace {

/**
 * @brief 注册 HTTP 请求/响应日志回调。
 * @param server 待配置日志的服务器实例。
 */
void SetupRequestLogging(httplib::Server& server) {
  server.set_logger([](const httplib::Request& req,
                       const httplib::Response& res) {
    LOG_INFO("HTTP {} {} -> {}", req.method, req.path, res.status);
    if (!req.body.empty()) {
      LOG_INFO("HTTP body: {}", req.body);
    }
  });
}

}  // namespace

/**
 * @brief 向 httplib 服务器注册全部 HTTP 路由及公共中间件。
 * @param server 待注册路由的服务器实例。
 */
void RegisterAllRoutes(httplib::Server& server) {
  SetupRequestLogging(server);

  routes::RegisterRouteModules(server);
}

}  // namespace http
}  // namespace gcontrol
