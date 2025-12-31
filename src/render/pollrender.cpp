#include "pollrender.h"
#include "imgui.h"
#include <cstdint>
#include <mutex>
#include <string>
#include <string_view>
#include <variant>

namespace Render {
PollRender::PollRender(std::string_view remote_url, std::string_view endpoint,
               uint16_t port)
    : HttpPoll::Poll(remote_url, endpoint, port) {}

void PollRender::Render(){
    if (is_new_data_available.load() && std::holds_alternative<std::string>(response_body)) {
        // ImGui::Text("%s",std::get<std::string>(response_body).c_str());
        // Append Entity
        is_new_data_available.store(false);
        
    }
}
} // namespace Render