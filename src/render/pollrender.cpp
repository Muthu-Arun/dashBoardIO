#include "pollrender.h"
#include <cstdint>

namespace Render {
    Poller::Poller(std::string_view remote_url, uint16_t port) : HttpPoll::Poll(remote_url, port){}

}