#ifndef GCONTROL_SRC_HTTP_ROUTES_ROUTE_REGISTRY_H_
#define GCONTROL_SRC_HTTP_ROUTES_ROUTE_REGISTRY_H_

namespace httplib {
class Server;
}

namespace gcontrol {
namespace http {
namespace routes {

/**
 * @brief 注册全部 HTTP 路由模块。
 *
 * 约定：
 * - 目录：routes/{domain}/，如 routes/config/
 * - 文件：{resource}_route.h / {resource}_route.cpp
 * - 注册函数：Register{Domain}{Resource}Routes()
 * - 域内聚合：{domain}_route_registry.cpp 中注册该域全部路由
 * - 新增接口时，在对应域 registry 追加一行，必要时新建 {resource}_route 文件
 *
 * @param server 待注册路由的服务器实例。
 */
void RegisterRouteModules(httplib::Server& server);

}  // namespace routes
}  // namespace http
}  // namespace gcontrol

#endif  // GCONTROL_SRC_HTTP_ROUTES_ROUTE_REGISTRY_H_
