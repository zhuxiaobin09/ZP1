#include "http/g_http_handler.h"

#include "logsystem/logger.h"

#include <httplib.h>

namespace gcontrol {
namespace http {

/**
 * @brief 获取 GHttpHandler 单例实例。
 * @return GHttpHandler& 全局唯一实例的引用。
 */
GHttpHandler& GHttpHandler::GetInstance() {
  static GHttpHandler instance;
  return instance;
}

/**
 * @brief 析构时停止 HTTP 服务并释放资源。
 */
GHttpHandler::~GHttpHandler() {
  Stop();
}

/**
 * @brief 设置 HTTP 服务监听端口（需在 Start 之前调用）。
 * @param port 监听端口号。
 */
void GHttpHandler::SetListenPort(int port) {
  std::lock_guard<std::mutex> lock(mutex_);
  listen_port_ = port;
}

/**
 * @brief 启动后台线程，在指定端口运行 HTTP 服务。
 */
void GHttpHandler::Start() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (running_) {
    return;
  }

  running_ = true;
  thread_ = std::thread(&GHttpHandler::RunServer, this);
  LOG_INFO("GHttpHandler started, port={}", listen_port_);
}

/**
 * @brief 停止 HTTP 服务并等待后台线程退出。
 */
void GHttpHandler::Stop() {
  std::thread worker;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!running_) {
      return;
    }
    running_ = false;
    if (server_) {
      server_->stop();
    }
    worker = std::move(thread_);
  }

  if (worker.joinable()) {
    worker.join();
  }
  LOG_INFO("GHttpHandler stopped");
}

/**
 * @brief 注册 HTTP 路由与请求日志回调。
 */
void GHttpHandler::RegisterRoutes() {
  server_->set_logger([](const httplib::Request& req,
                         const httplib::Response& res) {
    LOG_INFO("HTTP {} {} -> {}", req.method, req.path, res.status);
    if (!req.body.empty()) {
      LOG_INFO("HTTP body: {}", req.body);
    }
  });

  server_->Get("/health", [](const httplib::Request& /*req*/,
                             httplib::Response& res) {
    res.set_content(R"({"status":"ok"})", "application/json");
  });
}

/**
 * @brief 创建 httplib 服务器、注册路由并阻塞监听。
 */
void GHttpHandler::RunServer() {
  const int port = [&]() {
    std::lock_guard<std::mutex> lock(mutex_);
    return listen_port_;
  }();

  server_ = std::make_unique<httplib::Server>();
  RegisterRoutes();

  if (!server_->listen("0.0.0.0", port)) {
    LOG_ERROR("HTTP server failed to listen on port {}", port);
  }
}

}  // namespace http
}  // namespace gcontrol
