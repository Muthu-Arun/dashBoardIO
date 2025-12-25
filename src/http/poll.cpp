
#include "poll.h"
#include <drogon/HttpAppFramework.h>

namespace HttpPoll {
    // Keep the client alive to reuse TCP connections
    static drogon::HttpClientPtr shared_client;

void init() {
    // 1. Initialize the client
    shared_client = drogon::HttpClient::newHttpClient("https://build-it-together.click/");

    // 2. Schedule the recurring task
    drogon::app().getLoop()->runEvery(std::chrono::milliseconds(500), []() {
        pollData(); 
    });

    // 3. START THE EVENT LOOP
    // We launch this in a separate thread so it doesn't block your ImGui/Main loop.
    std::thread dr_thread([]() {
        // This will block this specific thread and process all 
        // network events and timers until the app shuts down.
        drogon::app().run(); 
    });
    
    // Detach so the thread lives as long as the process
    dr_thread.detach(); 
}

    void pollData() {
        auto req = drogon::HttpRequest::newHttpRequest();
        req->setPath("/");

        // Note: No need to pass 'url' if using the shared_client
        shared_client->sendRequest(req, [](drogon::ReqResult result, const drogon::HttpResponsePtr &response) {
            if (result == drogon::ReqResult::Ok) {
                std::cout << "Exeecuting Callback";
                std::lock_guard<std::mutex> lock(data_mtx);
                data.emplace(response->getBody());
                is_data_available.store(true);
            }
        });
    }
}