#include "imgui.h"
#include <atomic>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>
namespace Widget {
// Data expressed by the widget is owned by it to create a double buffer
class widget {
public:
  std::string label;
  std::mutex data_mtx;
  std::mutex src_mtx;
  std::atomic<bool> is_being_copied = 0;
  std::atomic<bool> is_data_available = 0;
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
  std::weak_ptr<_data_type> src;
  type ptype;
  size_t buffer_max_limit = 10000;
  std::vector<_data_type> data = std::vector<_data_type>(buffer_max_limit, 0);
  Plot(std::string_view _label, type _ptype, std::shared_ptr<_data_type>& _data_source) : widget(_label), ptype(_ptype), src(_data_source) {
  }

  void draw() override {
    switch (ptype) {
      case type::LINES:
        ImGui::PlotLines(label.c_str(), data.data(), buffer_max_limit);
        break;
      case type::HISTOGRAM:
        ImGui::PlotHistogram(label.c_str(), data.data(), buffer_max_limit);
        break;
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
  std::weak_ptr<_data_type> src;
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
