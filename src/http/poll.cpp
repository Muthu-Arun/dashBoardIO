#include <drogon/HttpClient.h>
#include <json/value.h>
#include <string_view>

namespace HttpPoll {
    using HttpClient = drogon::HttpClient;
    using HttpRequest = drogon::HttpRequest;
    using ReqResult = drogon::ReqResult;
    using HttpResponsePtr = drogon::HttpResponsePtr;
    Json::Value pollHttp(std::string_view url, const std::function<void(const std::string &)> &callback) {

        auto client = HttpClient::newHttpClient("http://www.example.com");
        auto req = HttpRequest::newHttpRequest();
        req->setPath("/");
        
        // Non-blocking call
        client->sendRequest(req, [](ReqResult result, const HttpResponsePtr &response) {
            if (result == ReqResult::Ok) {
                std::cout << "Received: " << response->getBody() << std::endl;
            }
        });
    }
}