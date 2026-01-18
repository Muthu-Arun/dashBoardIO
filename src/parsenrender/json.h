#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include "poll.h"

#include "widget.h"
#include "window.h"
namespace ParseJson {

class HttpWindowWrapper {
protected:
    std::unordered_map<std::string, int> map_int;
    std::unordered_map<std::string, float> map_float;
    std::unordered_map<std::string, std::string> map_string;
    std::unordered_map<std::string, std::mutex> network_buffer_mtx;
    std::string host, host_endpoint;
    bool in_init_phase = 0;

public:
    static uint32_t window_count;
    uint32_t win_idx;
    std::string win_label;
    HttpWindowWrapper();
    std::optional<HttpPoll::Poll> poll;
    std::optional<Window::Window> window;
    void addText(const std::string& _label, std::string_view data);
    void addRadialGauge(const std::string& _label, int data, int min, int max);
    void addRadialGauge(const std::string& _label, float data, float min, float max);
    void addPlot(const std::string& _label, float data);
    void renderHeader();
    void parseJSON();
};
}  // namespace ParseJson
