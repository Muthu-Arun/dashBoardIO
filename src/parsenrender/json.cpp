#include "json.h"

#include <format>

#include "imgui.h"
namespace ParseJson {
HttpWindowWrapper::HttpWindowWrapper() {
    win_idx = window_count++;
    win_label = std::format("Window - {}", win_idx);
    host_endpoint.resize(200, ' ');
    host.resize(200, ' ');
    window.emplace(win_label);
}
void HttpWindowWrapper::parseJSON() {}

void HttpWindowWrapper::renderHeader() {
    ImGui::Begin(win_label.c_str());
    if (in_init_phase) {
        if (ImGui::InputText("Host URL", host_endpoint.data(), host_endpoint.capacity() + 1,
                             ImGuiInputTextFlags_EnterReturnsTrue)) {
            poll.emplace(host, host_endpoint);
            in_init_phase = false;
        }
    } else [[likely]] {
        window->render();
    }
    ImGui::End();
}

}  // namespace ParseJson