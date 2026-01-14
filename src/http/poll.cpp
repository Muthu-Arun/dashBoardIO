
#include "poll.h"

#include <drogon/HttpAppFramework.h>
#include <drogon/HttpTypes.h>
#include <json/value.h>

#include <chrono>
#include <cstdint>
#include <mutex>
#include <string_view>
#include <thread>
#include <utility>

namespace HttpPoll {
// Keep the client alive to reuse TCP connections
static drogon::HttpClientPtr shared_client;

void init() {
    // 1. Initialize the client
    shared_client = drogon::HttpClient::newHttpClient("https://build-it-together.click/");

    // 2. Schedule the recurring task
    /*
    drogon::app().getLoop()->runEvery(std::chrono::milliseconds(500), []() {
        pollData();
    });
    */

    // 3. START THE EVENT LOOP
    // We launch this in a separate thread so it doesn't block your ImGui/Main
    // loop.
    std::thread dr_thread([]() { drogon::app().run(); });

    // Detach so the thread lives as long as the process
    dr_thread.detach();
}

void pollData() {
    auto req = drogon::HttpRequest::newHttpRequest();
    req->setPath("/");

    // Note: No need to pass 'url' if using the shared_client
    shared_client->sendRequest(
        req, [](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
            if (result == drogon::ReqResult::Ok) {
                std::cout << "Exeecuting Callback";
                std::lock_guard<std::mutex> lock(data_mtx);
                data.emplace(response->getBody());
                is_data_available.store(true);
            }
        });
}

std::pair<const std::variant<std::string, Json::Value>&, std::mutex&> Poll::getBody()
    const noexcept {
    return {response_body, res_body_mtx};
}

bool Poll::is_data_available() const noexcept {
    return is_new_data_available.load();
}

Poll::Poll(std::string_view remote_url, std::string_view endpoint, uint16_t port)
    : remote_url(remote_url), endpoint(endpoint), port(port) {
    client = HttpClient::newHttpClient(this->remote_url, port);
    request = HttpRequest::newHttpRequest();
    request->setPath(this->endpoint);
    timer_id = drogon::app().getLoop()->runEvery(std::chrono::milliseconds(100), [this]() {
        client->sendRequest(request,
                            [this](drogon::ReqResult result, const HttpResponsePtr& response) {
                                std::lock_guard<std::mutex> _lock(res_body_mtx);
                                response_body.emplace<std::string>(response->body());
                                is_new_data_available.store(true);
                            });
    });
}
Poll::~Poll() {
    drogon::app().getLoop()->invalidateTimer(timer_id);  // Stops the Polling task
}

}  // namespace HttpPoll