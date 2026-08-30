#include "http/routes/route_registry.h"

#include "http/routes/config/config_route_registry.h"

#include <httplib.h>

namespace gcontrol {
namespace http {
namespace routes {

/**
 * @brief 注册全部 HTTP 路由模块。
 * @param server 待注册路由的服务器实例。
 */
void RegisterRouteModules(httplib::Server& server) {
  config::RegisterConfigRoutes(server);
}

}  // namespace routes
}  // namespace http
}  // namespace gcontrol
