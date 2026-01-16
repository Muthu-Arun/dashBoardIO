#include "json.h"

#include <json/value.h>

#include <format>
#include <string_view>
#include <vector>

#include "imgui.h"
#include "window.h"
namespace ParseJson {
enum class WidgetType :uint16_t {
    Unknown,
    Plot,
    Histogram,
    Text,
    Image
};
/*
void parseDynamicJson(const Json::Value& root) {
    if (root.isObject()) {
        // It's a map/dictionary: { "key": "value" }
        for (auto const& id : root.getMemberNames()) {
            std::cout << "Key: " << id << " -> ";
            parseDynamicJson(root[id]);  // Recurse!
        }
    } else if (root.isArray()) {
        // It's a list: [1, 2, 3]
        for (unsigned int i = 0; i < root.size(); i++) {
            parseDynamicJson(root[i]);  // Recurse!
        }
    } else {
        // It's a "leaf" (Value): string, int, bool
        std::cout << "Value: " << root.asString() << std::endl;
    }
}
*/
/*
JSON Structure
{
    widget_label1: {type: widget_type, value: data},

    widget_label2: {type: widget_type, value: data},

    ....
}
*/

static void addTextWidget(Window::Window& window, std::string_view label_, ){
    window.addWidget(std::string_view id, std::unique_ptr<Widgets::Widget> widget)
}

static void parseDynamicJson(const Json::Value& root) {
    if (root.isArray()) {
    
    }
    if (root.isObject()) {
        // It's a map/dictionary: { "key": "value" }
        for (auto const& id : root.getMemberNames()) {
            std::cout << "Key: " << id << " -> ";
            parseDynamicJson(root[id]);  // Recurse!
        }
    } else if (root.isArray()) {
        // It's a list: [1, 2, 3]
        for (unsigned int i = 0; i < root.size(); i++) {
            parseDynamicJson(root[i]);  // Recurse!
        }
    } else {
        // It's a "leaf" (Value): string, int, bool
        std::cout << "Value: " << root.asString() << std::endl;

    }
}
uint32_t HttpWindowWrapper::window_count = 0;

HttpWindowWrapper::HttpWindowWrapper() {
    win_idx = window_count++;
    win_label = std::format("Window - {}", win_idx);
    host_endpoint.resize(200, ' ');
    host.resize(200, ' ');
    window.emplace(win_label);
}
void HttpWindowWrapper::parseJSON() {
    auto jsonptr = poll->getJSONBodyPtr();
    const Json::Value& json = *jsonptr;
}

void HttpWindowWrapper::renderHeader() {
    ImGui::Begin(win_label.c_str());
    if (in_init_phase) [[unlikely]] {
        if (ImGui::InputText("Host URL", host_endpoint.data(), host_endpoint.capacity() + 1,
                             ImGuiInputTextFlags_EnterReturnsTrue)) {
            poll.emplace(host, host_endpoint);
            in_init_phase = false;
            ImGui::End();
            return;
        }
    }
    if (poll->is_data_available()) {
        parseJSON();
    }
    window->render();
    ImGui::End();
}

}  // namespace ParseJson