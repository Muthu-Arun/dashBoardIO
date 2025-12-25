#include <atomic>
#include <drogon/HttpClient.h>
#include <iterator>
#include <json/value.h>
#include <memory>
#include <mutex>
#include <queue>
#include <string_view>
#include <thread>
namespace HttpPoll {
    using HttpClient = drogon::HttpClient;
    using HttpRequest = drogon::HttpRequest;
    using ReqResult = drogon::ReqResult;
    using HttpResponsePtr = drogon::HttpResponsePtr;
    inline std::atomic<bool> is_data_available = 0;
    inline std::mutex data_mtx;
    inline std::queue<std::string> data;
    inline std::unique_ptr<std::thread> bg_thread;
    void init();
    void pollData();
    void pollHttp(std::string_view url, const std::function<void(const std::string &)> &callback);
}