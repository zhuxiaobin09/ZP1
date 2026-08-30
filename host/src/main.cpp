#include "dds/h_dds_handler.h"
#include "logsystem/logger.h"

#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>

namespace {

std::atomic<bool> g_running{true};

/**
 * @brief 处理 SIGINT/SIGTERM 信号，通知主循环退出。
 * @param signal 收到的信号编号（未使用）。
 */
void SignalHandler(int /*signal*/) {
  g_running = false;
}

}  // namespace

/**
 * @brief host 进程入口，启动 DDS 订阅并等待退出信号。
 * @return int 正常退出返回 0。
 */
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
