#include "http/g_http_handler.h"

#include "logsystem/logger.h"

#include <httplib.h>

namespace gcontrol {
namespace http {

GHttpHandler& GHttpHandler::GetInstance() {
  static GHttpHandler instance;
  return instance;
}

GHttpHandler::~GHttpHandler() {
  Stop();
}

void GHttpHandler::SetListenPort(int port) {
  std::lock_guard<std::mutex> lock(mutex_);
  listen_port_ = port;
}

void GHttpHandler::Start() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (running_) {
    return;
  }

  running_ = true;
  thread_ = std::thread(&GHttpHandler::RunServer, this);
  LOG_INFO("GHttpHandler started, port={}", listen_port_);
}

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
