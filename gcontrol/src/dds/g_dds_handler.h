#ifndef GCONTROL_SRC_DDS_G_DDS_HANDLER_H_
#define GCONTROL_SRC_DDS_G_DDS_HANDLER_H_

#include <atomic>
#include <mutex>
#include <thread>

namespace gcontrol {
namespace dds {

class GDDSHandler {
 public:
  static GDDSHandler& GetInstance();

  GDDSHandler(const GDDSHandler&) = delete;
  GDDSHandler& operator=(const GDDSHandler&) = delete;

  void Start();
  void Stop();

 private:
  GDDSHandler() = default;
  ~GDDSHandler();

  void RunLoop();

  std::mutex mutex_;
  std::thread thread_;
  std::atomic<bool> running_{false};
};

}  // namespace dds
}  // namespace gcontrol

#endif  // GCONTROL_SRC_DDS_G_DDS_HANDLER_H_
