#ifndef GCONTROL_SRC_HTTP_ROUTES_HOST_CONFIG_ROUTE_H_
#define GCONTROL_SRC_HTTP_ROUTES_HOST_CONFIG_ROUTE_H_

namespace httplib {
class Server;
}

namespace gcontrol {
namespace http {
namespace routes {

/**
 * @brief 注册 host 配置文件读写 HTTP 路由。
 * @param server 待注册路由的服务器实例。
 */
void RegisterHostConfigRoutes(httplib::Server& server);

}  // namespace routes
}  // namespace http
}  // namespace gcontrol

#endif  // GCONTROL_SRC_HTTP_ROUTES_HOST_CONFIG_ROUTE_H_
