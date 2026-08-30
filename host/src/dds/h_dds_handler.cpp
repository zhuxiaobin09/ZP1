#include "dds/h_dds_handler.h"

#include "GControlMessage.hpp"
#include "logsystem/logger.h"

#include <chrono>
#include <dds/dds.hpp>

namespace host {
namespace dds {
namespace {

constexpr const char* kTopicName = "GControlTopic";
constexpr auto kPollInterval = std::chrono::milliseconds(100);

}  // namespace

/**
 * @brief 获取 HDDSHandler 单例实例。
 * @return HDDSHandler& 全局唯一实例的引用。
 */
HDDSHandler& HDDSHandler::GetInstance() {
  static HDDSHandler instance;
  return instance;
}

/**
 * @brief 析构时停止 DDS 订阅线程，释放资源。
 */
HDDSHandler::~HDDSHandler() {
  Stop();
}

/**
 * @brief 启动后台线程，周期性从 DDS Topic 读取消息。
 */
void HDDSHandler::Start() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (running_) {
    return;
  }

  running_ = true;
  thread_ = std::thread(&HDDSHandler::RunLoop, this);
  LOG_INFO("HDDSHandler started");
}

/**
 * @brief 停止订阅循环并等待后台线程退出。
 */
void HDDSHandler::Stop() {
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
  LOG_INFO("HDDSHandler stopped");
}

/**
 * @brief DDS 订阅主循环，创建 Reader 并轮询接收 GControlMessage。
 */
void HDDSHandler::RunLoop() {
  ::dds::domain::DomainParticipant participant(0);
  ::dds::sub::Subscriber subscriber(participant);
  ::dds::topic::Topic<zp1::msg::GControlMessage> topic(
      participant, kTopicName);
  ::dds::sub::DataReader<zp1::msg::GControlMessage> reader(subscriber, topic);

  while (running_) {
    const auto samples = reader.take();
    for (const auto& sample : samples) {
      if (!sample.info().valid()) {
        continue;
      }

      const auto& message = sample.data();
      LOG_INFO("Received seq_id={} content={} timestamp_ms={}",
               message.seq_id(),
               message.content(),
               message.timestamp_ms());
    }

    std::this_thread::sleep_for(kPollInterval);
  }
}

}  // namespace dds
}  // namespace host
