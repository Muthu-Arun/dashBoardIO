#include <atomic>
#include <cstddef>
#include <drogon/HttpClient.h>
#include <iterator>
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
private:
  std::variant<std::string, Json::Value> response_body;
  std::unique_ptr<HttpClient> client;
  ReqResult result;
  mutable std::mutex mtx;
  std::atomic<bool> is_new_body_available = 0;
  static std::unique_ptr<std::thread> poll_thread;

public:
  auto getBody() const;
  bool is_data_available() const;
};

} // namespace HttpPoll
