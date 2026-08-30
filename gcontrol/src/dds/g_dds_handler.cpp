#include "dds/g_dds_handler.h"

#include "GControlMessage.hpp"
#include "logsystem/logger.h"

#include <chrono>
#include <dds/dds.hpp>
#include <format>

namespace gcontrol {
namespace dds {
namespace {

constexpr const char* kTopicName = "GControlTopic";
constexpr auto kPublishInterval = std::chrono::seconds(1);

/**
 * @brief 获取当前系统时间的毫秒级 Unix 时间戳。
 * @return int64_t 自 epoch 起的毫秒数。
 */
int64_t CurrentTimestampMs() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

}  // namespace

/**
 * @brief 获取 GDDSHandler 单例实例。
 * @return GDDSHandler& 全局唯一实例的引用。
 */
GDDSHandler& GDDSHandler::GetInstance() {
  static GDDSHandler instance;
  return instance;
}

/**
 * @brief 析构时停止 DDS 发布线程，释放资源。
 */
GDDSHandler::~GDDSHandler() {
  Stop();
}

/**
 * @brief 启动后台线程，周期性向 DDS Topic 发布消息。
 */
void GDDSHandler::Start() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (running_) {
    return;
  }

  running_ = true;
  thread_ = std::thread(&GDDSHandler::RunLoop, this);
  LOG_INFO("GDDSHandler started");
}

/**
 * @brief 停止发布循环并等待后台线程退出。
 */
void GDDSHandler::Stop() {
  std::thread worker;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!running_) {
      return;
    }
    running_ = false;
    worker = std::move(thread_);
  }

  if (worker.joinable()) {
    worker.join();
  }
  LOG_INFO("GDDSHandler stopped");
}

/**
 * @brief DDS 发布主循环，创建 Writer 并每秒发送一条 GControlMessage。
 */
void GDDSHandler::RunLoop() {
  ::dds::domain::DomainParticipant participant(0);
  ::dds::pub::Publisher publisher(participant);
  ::dds::topic::Topic<zp1::msg::GControlMessage> topic(
      participant, kTopicName);
  ::dds::pub::DataWriter<zp1::msg::GControlMessage> writer(publisher, topic);

  int32_t seq_id = 0;
  while (running_) {
    zp1::msg::GControlMessage message;
    message.seq_id(++seq_id);
    message.content(std::format("gcontrol message {}", seq_id));
    message.timestamp_ms(CurrentTimestampMs());

    writer.write(message);
    LOG_INFO("Published seq_id={} content={}",
             message.seq_id(),
             message.content());

    std::this_thread::sleep_for(kPublishInterval);
  }
}

}  // namespace dds
}  // namespace gcontrol
