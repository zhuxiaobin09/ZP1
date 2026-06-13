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

HDDSHandler& HDDSHandler::GetInstance() {
  static HDDSHandler instance;
  return instance;
}

HDDSHandler::~HDDSHandler() {
  Stop();
}

void HDDSHandler::Start() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (running_) {
    return;
  }

  running_ = true;
  thread_ = std::thread(&HDDSHandler::RunLoop, this);
  LOG_INFO("HDDSHandler started");
}

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
