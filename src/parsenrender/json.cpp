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
    window.emplace(win_label);
}
void HttpWindowWrapper::initFRs() {
    widget_updates_fr["text"] = [this](const std::string& label_, const Json::Value& params) {
        if (!window->isWidgetPresent(label_)) {
            addText(label_, params["data"].asString());
        } else [[likely]] {
            std::lock_guard<std::mutex> lock_(network_buffer_mtx[label_]);
            map_string[label_] = params["data"].asString();
            window->widgets.at(label_)->is_data_available.store(true);
        }
    };
    widget_updates_fr["radial_gauge"] = [this](const std::string& label_,
                                               const Json::Value& params) {
        if (!window->isWidgetPresent(label_)) {
            if (params["dtype"].asString() == "int") {
                addRadialGauge(label_, params["data"].asInt(), params["min"].asInt(),
                               params["max"].asInt());
            } else {
                addRadialGauge(label_, params["data"].asFloat(), params["min"].asFloat(),
                               params["max"].asFloat());
            }
        } else [[likely]] {
            if (map_int.find(label_) != map_int.end()) {
                std::lock_guard<std::mutex> lock_(network_buffer_mtx[label_]);
                map_int[label_] = params["data"].asInt();
                window->widgets.at(label_)->is_data_available.store(true);
            } else {
                std::lock_guard<std::mutex> lock_(network_buffer_mtx[label_]);
                map_float[label_] = params["data"].asFloat();
                window->widgets.at(label_)->is_data_available.store(true);
            }
        }
    };
    widget_updates_fr["plot"] = [this](const std::string& label_, const Json::Value& params) {
        if (!window->isWidgetPresent(label_)) {
            addPlot(label_, params["data"].asFloat());
        } else [[likely]] {
            std::lock_guard<std::mutex> lock_(network_buffer_mtx[label_]);
            map_float[label_] = params["data"].asFloat();
            window->widgets.at(label_)->is_data_available.store(true);
        }
    };
}
void HttpWindowWrapper::parseJSON() {
    auto jsonptr = poll->getJSONBodyPtr();
    const Json::Value& json = *jsonptr;
    for (std::string& id : json.getMemberNames()) {
        widget_updates_fr.at(json[id]["type"].asString())(id, json[id]);
    }
}

void HttpWindowWrapper::renderHeader() {
    ImGui::Begin(win_label.c_str());
    if (in_init_phase) [[unlikely]] {
        ImGui::InputText("Host URL", host.data(), 250, ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::InputText("Host Endpoint", host_endpoint.data(), 250,
                         ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::InputScalar("Port", ImGuiDataType_U64, &port);
        if (ImGui::Button("Enter Host URL")) {
            poll.emplace(host.data(), host_endpoint.data(), port);
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
