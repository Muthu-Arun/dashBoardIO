#include "json.h"

#include <json/value.h>

#include <format>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "imgui.h"
#include "widget.h"
#include "window.h"
namespace ParseJson {
enum class WidgetType : uint16_t { Unknown, Plot, Histogram, Text, Image, RadialGauge };
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

void HttpWindowWrapper::addText(const std::string& _label, std::string_view data) {
    map_string[_label] = data;  // IG using [] oprator is not a problem here as if there aren't any
                                // we need to create one
    network_buffer_mtx[_label];
    window->addWidget(_label, std::make_unique<Widgets::Text<>>(_label, map_string[_label],
                                                                network_buffer_mtx[_label]));
}
void HttpWindowWrapper::addRadialGauge(const std::string& _label, int data, int min, int max) {
    map_int[_label] = data;
    network_buffer_mtx[_label];
    window->addWidget(
        _label, std::make_unique<Widgets::RadialGauge<int>>(_label, min, max, map_int.at(_label),
                                                            network_buffer_mtx[_label]));
}
void HttpWindowWrapper::addRadialGauge(const std::string& _label, float data, float min,
                                       float max) {
    map_float[_label] = data;
    network_buffer_mtx[_label];
    window->addWidget(
        _label, std::make_unique<Widgets::RadialGauge<int>>(_label, min, max, map_float.at(_label),
                                                            network_buffer_mtx[_label]));
}
void HttpWindowWrapper::addPlot(const std::string& _label, float data,
                                Widgets::Plot<float>::type ptype) {
    map_float[_label] = data;
    network_buffer_mtx[_label];
    window->addWidget(_label, std::make_unique<Widgets::Plot<float>>(
                                  _label, ptype, map_float[_label], network_buffer_mtx[_label]));
}

void parseDynamicJson(const Json::Value& root) {
    if (root.isArray()) {
    }
    if (root.isObject()) {
        // It's a map/dictionary: { "key": "value" }
        for (std::string& id : root.getMemberNames()) {
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
void HttpWindowWrapper::initFRs() {
    widget_updates_fr["text"] = [this](const std::string& id, const Json::Value& params) {
        if (!window->isWidgetPresent(id)) {
            addText(id, params["data"].asString());
        }
        else {
            std::lock_guard<std::mutex> lock_(network_buffer_mtx[id]);
            map_string[id] = params["data"].asString();
        }
    };
}
void HttpWindowWrapper::parseJSON() {
    auto jsonptr = poll->getJSONBodyPtr();
    const Json::Value& json = *jsonptr;
    for (std::string& id : json.getMemberNames()) {
        widget_updates_fr[json[id]["type"].asString()];
    }
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
