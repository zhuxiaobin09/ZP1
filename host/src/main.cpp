#include "dds/h_dds_handler.h"
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

  LOG_INFO("host process started");

  auto& handler = host::dds::HDDSHandler::GetInstance();
  handler.Start();

  while (g_running) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }

  handler.Stop();
  LOG_INFO("host process exited");
  return 0;
}
