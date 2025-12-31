#include "poll.h"
#include <cstdint>

namespace Render {
class PollRender : public HttpPoll::Poll {
public:
  PollRender(std::string_view remote_url, std::string_view endpoint, uint16_t port);
  void Render();
};

} // namespace Render