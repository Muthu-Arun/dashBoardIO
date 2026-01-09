#include "imgui.h"
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
namespace Widget {
// Data expressed by the widget is owned by it to create a double buffer
class widget {
public:
  std::string label;
  widget(std::string_view _label) : label(_label) {}
  virtual void draw() = 0;
  virtual void action();
  virtual ~widget() = 0;
};
template <typename _data_type>
  requires /*std::integral<_data_type> || */ std::floating_point<_data_type>
class Plot : public widget {
public:
  enum class type : uint8_t {LINES, HISTOGRAM};
  type ptype;
  size_t buffer_max_limit = 10000;
  std::vector<_data_type> data = std::vector<_data_type>(buffer_max_limit, 0);
  Plot(std::string_view _label, type _ptype) : widget(_label), ptype(_ptype) {
  }

  void draw() override {
    switch (ptype) {
      case type::LINES:
        ImGui::PlotLines(label.c_str(), data.data(), buffer_max_limit);
    }
  }
  ~Plot() {}
};

template <typename _callable = std::function<void()>>
class button : public widget {
private:
  _callable call_on_event;

public:
  enum class event : uint8_t { ON_CLICK, ON_RELEASE };

  button(std::string_view _label) : widget(_label) {}

  void draw() override {
    if (ImGui::Button(label.c_str())) {
      call_on_event();
    }
  }
  // void action() override { call_on_event(); }
  ~button() {}
};

template <typename _data_type = std::string>
class text : public widget {
public:
  text(std::string_view _label) : widget(_label) {}
  _data_type data;
  void draw() override {

  }
  ~text() {}
};

template <typename _data_type>
  requires std::integral<_data_type> || std::floating_point<_data_type>
class radial_gauge : public widget {
public:
  radial_gauge(std::string_view _label) : widget(_label) {}

  _data_type data;
  void draw() override {}
  ~radial_gauge() {}
};

} // namespace Widget
