#pragma once
#include <drogon/HttpClient.h>
#include <drogon/HttpRequest.h>
#include <json/value.h>
#include <trantor/net/EventLoop.h>

#include <atomic>
#include <cstdint>
#include <functional>
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
using HttpRequestPtr = drogon::HttpRequestPtr;
inline std::atomic<bool> is_data_available = 0;
inline std::mutex data_mtx;
inline std::queue<std::string> data;
inline std::unique_ptr<std::thread> bg_thread;
void init();
void pollData();
void pollHttp(std::string_view url, const std::function<void(const std::string&)>& callback);

void add_timed_event();

class Poll {
protected:
    std::variant<std::string, Json::Value> response_body;
    std::shared_ptr<Json::Value> json_ptr;
    mutable std::mutex res_body_mtx;
    trantor::TimerId timer_id;
    std::atomic<bool> is_new_data_available = 0;
    std::string remote_url;
    std::string endpoint;
    uint64_t port;
    HttpRequestPtr request;

public:
    Poll(std::string_view remote_url, std::string_view endpoint, uint16_t port = 80);
    bool is_data_available() const noexcept;
    std::shared_ptr<Json::Value>& getJSONBodyPtr() noexcept;
    std::pair<const std::variant<std::string, Json::Value>&, std::mutex&> getBody() const noexcept;
    std::function<void(const std::string&, drogon::HttpMethod)> getButtonCallback();
    ~Poll();

private:
    HttpClientPtr client;
    ReqResult result;
};

}  // namespace HttpPoll
