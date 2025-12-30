#include <atomic>
#include <cstdint>
#include <drogon/HttpClient.h>
#include <functional>
#include <json/value.h>
#include <memory>
#include <mutex>
#include <queue>
#include <string_view>
#include <thread>
#include <variant>
namespace HttpPoll {
using HttpClient = drogon::HttpClient;
using HttpRequest = drogon::HttpRequest;
using ReqResult = drogon::ReqResult;
using HttpClientPtr = drogon::HttpClientPtr;
using HttpResponsePtr = drogon::HttpResponsePtr;
inline std::atomic<bool> is_data_available = 0;
inline std::mutex data_mtx;
inline std::queue<std::string> data;
inline std::unique_ptr<std::thread> bg_thread;
void init();
void pollData();
void pollHttp(std::string_view url,
              const std::function<void(const std::string &)> &callback);

void add_timed_event();

class Poll {
protected:
  std::variant<std::string, Json::Value> response_body;
  HttpClientPtr client;
  ReqResult result;
  mutable std::mutex mtx;
  std::atomic<bool> is_new_body_available = 0;
  std::string remote_url;
  uint64_t port;

public:
  Poll(std::string_view remote_url, uint16_t port);
  static void init() noexcept;
  bool is_data_available() const noexcept;
  std::pair<const std::variant<std::string, Json::Value>&, std::mutex&> getBody() const noexcept;
private:
};

} // namespace HttpPoll
