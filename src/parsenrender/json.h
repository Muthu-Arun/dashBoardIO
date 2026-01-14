#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include "poll.h"

#include "widget.h"
#include "window.h"
namespace ParseJson {

class HttpWindowWrapper {
protected:
    std::unordered_map<std::string, std::unique_ptr<Widgets::Widget>> widgets;

    std::unordered_map<std::string, std::shared_ptr<int>> map_int;
    std::unordered_map<std::string, std::shared_ptr<float>> map_float;
    std::unordered_map<std::string, std::shared_ptr<std::string>> map_string;
    std::string host_endpoint;

public:
    void renderHeader();
    std::optional<HttpPoll::Poll> poll;
    std::optional<Window::Window> window;
};
}  // namespace ParseJson
