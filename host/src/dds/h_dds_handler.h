#ifndef HOST_SRC_DDS_H_DDS_HANDLER_H_
#define HOST_SRC_DDS_H_DDS_HANDLER_H_

#include <atomic>
#include <mutex>
#include <thread>

namespace host {
namespace dds {

class HDDSHandler {
 public:
  static HDDSHandler& GetInstance();

  HDDSHandler(const HDDSHandler&) = delete;
  HDDSHandler& operator=(const HDDSHandler&) = delete;

  void Start();
  void Stop();

 private:
  HDDSHandler() = default;
  ~HDDSHandler();

  void RunLoop();

  std::mutex mutex_;
  std::thread thread_;
  std::atomic<bool> running_{false};
};

}  // namespace dds
}  // namespace host

#endif  // HOST_SRC_DDS_H_DDS_HANDLER_H_
