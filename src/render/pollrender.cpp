#include "pollrender.h"
#include <cstdint>
#include <string_view>

namespace Render {
Poller::Poller(std::string_view remote_url, std::string_view endpoint,
               uint16_t port)
    : HttpPoll::Poll(remote_url, endpoint, port) {}

} // namespace Render