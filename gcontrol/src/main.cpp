#include "dds/g_dds_handler.h"
#include "http/g_http_handler.h"
#include "logsystem/logger.h"

#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>

namespace {

std::atomic<bool> g_running{true};

void SignalHandler(int /*signal*/) {
  g_running = false;
}

}  // namespace

int main() {
  std::signal(SIGINT, SignalHandler);
  std::signal(SIGTERM, SignalHandler);

  LOG_INFO("gcontrol process started");

  auto& dds_handler = gcontrol::dds::GDDSHandler::GetInstance();
  auto& http_handler = gcontrol::http::GHttpHandler::GetInstance();

  http_handler.Start();
  dds_handler.Start();

  while (g_running) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }

  dds_handler.Stop();
  http_handler.Stop();
  LOG_INFO("gcontrol process exited");
  return 0;
}
