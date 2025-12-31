#include "poll.h"
#include <cstdint>

namespace Render {
class Poller : public HttpPoll::Poll {
public:
  Poller(std::string_view remote_url, std::string_view endpoint, uint16_t port);
};

} // namespace Render