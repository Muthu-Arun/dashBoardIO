#include "json.h"

#include <format>

#include "imgui.h"
namespace ParseJson {
HttpWindowWrapper::HttpWindowWrapper() {
    win_idx = window_count++;
    win_label = std::format("Window - {}", win_idx++);
    host_endpoint.resize(200, ' ');
    host.resize(200, ' ');
}
void HttpWindowWrapper::renderHeader() {
    ImGui::Begin(win_label.c_str());
    if (ImGui::InputText("Host URL", host_endpoint.data(), host_endpoint.length())) {
        poll.emplace(host, host_endpoint);
        // Probably logic to add widgets
    }
}

}  // namespace ParseJson