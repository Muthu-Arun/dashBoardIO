#include <concepts>
#include <cstdint>
#include <deque>
#include <functional>
#include <string>
namespace Widget {
// Data expressed by the widget is owned by it to create a double buffer
class widget {
public:
  virtual void draw() = 0;
  virtual void action();
  virtual ~widget() = 0;
};
template <typename _data_type>
  requires std::integral<_data_type> || std::floating_point<_data_type>
class graph : public widget {
public:
  std::deque<_data_type> data;
  void draw() override {}
  ~graph() {}
};

template <typename _callable = std::function<void()>> class button : public widget {
private:
  _callable call_on_event;

public:
  enum class event : uint8_t {ON_CLICK, ON_RELEASE};

  void draw() override {}
  void action() override { call_on_event(); }
  ~button() {}
};

template <typename _data_type = std::string> class text : public widget {
public:
  _data_type data;
  void draw() override {}
  ~text() {}
};

template <typename _data_type>
  requires std::integral<_data_type> || std::floating_point<_data_type>
class radial_gauge : public widget {
public:
  _data_type data;
  void draw() override {}
  ~radial_gauge() {}
};

} // namespace Widget