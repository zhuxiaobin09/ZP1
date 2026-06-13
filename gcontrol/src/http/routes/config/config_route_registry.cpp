#include "http/routes/config/config_route_registry.h"

#include "http/routes/config/gcontrol_route.h"
#include "http/routes/config/host_route.h"
#include "http/routes/config/log_route.h"

#include <httplib.h>

namespace gcontrol {
namespace http {
namespace routes {
namespace config {

/**
 * @brief 注册 config 域全部 HTTP 路由。
 * @param server 待注册路由的服务器实例。
 */
void RegisterConfigRoutes(httplib::Server& server) {
  RegisterConfigGcontrolRoutes(server);
  RegisterConfigHostRoutes(server);
  RegisterConfigLogRoutes(server);
}

}  // namespace config
}  // namespace routes
}  // namespace http
}  // namespace gcontrol
