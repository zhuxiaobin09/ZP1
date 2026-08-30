#ifndef GCONTROL_SRC_HTTP_ROUTES_CONFIG_CONFIG_ROUTE_REGISTRY_H_
#define GCONTROL_SRC_HTTP_ROUTES_CONFIG_CONFIG_ROUTE_REGISTRY_H_

namespace httplib {
class Server;
}

namespace gcontrol {
namespace http {
namespace routes {
namespace config {

/**
 * @brief 注册 config 域全部 HTTP 路由。
 * @param server 待注册路由的服务器实例。
 */
void RegisterConfigRoutes(httplib::Server& server);

}  // namespace config
}  // namespace routes
}  // namespace http
}  // namespace gcontrol

#endif  // GCONTROL_SRC_HTTP_ROUTES_CONFIG_CONFIG_ROUTE_REGISTRY_H_
