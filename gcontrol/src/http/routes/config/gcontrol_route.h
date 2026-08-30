#ifndef GCONTROL_SRC_HTTP_ROUTES_CONFIG_GCONTROL_ROUTE_H_
#define GCONTROL_SRC_HTTP_ROUTES_CONFIG_GCONTROL_ROUTE_H_

namespace httplib {
class Server;
}

namespace gcontrol {
namespace http {
namespace routes {
namespace config {

/**
 * @brief 注册 gcontrol 配置文件读写 HTTP 路由。
 * @param server 待注册路由的服务器实例。
 */
void RegisterConfigGcontrolRoutes(httplib::Server& server);

}  // namespace config
}  // namespace routes
}  // namespace http
}  // namespace gcontrol

#endif  // GCONTROL_SRC_HTTP_ROUTES_CONFIG_GCONTROL_ROUTE_H_
