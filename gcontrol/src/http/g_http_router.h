#ifndef GCONTROL_SRC_HTTP_G_HTTP_ROUTER_H_
#define GCONTROL_SRC_HTTP_G_HTTP_ROUTER_H_

namespace httplib {
class Server;
}

namespace gcontrol {
namespace http {

/**
 * @brief 向 httplib 服务器注册全部 HTTP 路由及公共中间件。
 * @param server 待注册路由的服务器实例。
 */
void RegisterAllRoutes(httplib::Server& server);

}  // namespace http
}  // namespace gcontrol

#endif  // GCONTROL_SRC_HTTP_G_HTTP_ROUTER_H_
