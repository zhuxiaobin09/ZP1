#ifndef GCONTROL_SRC_HTTP_G_HTTP_HANDLER_H_
#define GCONTROL_SRC_HTTP_G_HTTP_HANDLER_H_

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace httplib {
class Server;
}

namespace gcontrol {
namespace http {

class GHttpHandler {
 public:
  static GHttpHandler& GetInstance();

  GHttpHandler(const GHttpHandler&) = delete;
  GHttpHandler& operator=(const GHttpHandler&) = delete;

  void SetListenPort(int port);
  void Start();
  void Stop();

 private:
  GHttpHandler() = default;
  ~GHttpHandler();

  void RunServer();

  std::mutex mutex_;
  std::thread thread_;
  std::atomic<bool> running_{false};
  int listen_port_{8080};
  std::unique_ptr<httplib::Server> server_;
};

}  // namespace http
}  // namespace gcontrol

#endif  // GCONTROL_SRC_HTTP_G_HTTP_HANDLER_H_
