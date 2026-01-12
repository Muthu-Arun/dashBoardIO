#pragma once
#include "imgui.h"
#include <array>
#include <atomic>
#include <cmath>
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
  // std::mutex data_mtx;
  std::atomic<bool> is_being_copied = 0;
  std::atomic<bool> is_data_available = 0;
  widget(std::string_view _label);
  virtual void draw() = 0;
  // virtual void action();
  virtual void copyFromSource() = 0;
  virtual ~widget();
};
template <typename _data_type>
  requires /*std::integral<_data_type> || */ std::floating_point<_data_type>
class Plot : public widget {
public:
  enum class type : uint8_t { LINES, HISTOGRAM };
  std::shared_ptr<_data_type> src;
  std::mutex& src_mtx;
  type ptype;
  size_t buffer_max_limit = 16384; // 2^14
  std::vector<_data_type> data = std::vector<_data_type>(buffer_max_limit, 0);

  Plot(std::string_view _label, type _ptype,
       std::shared_ptr<_data_type> &_data_source,
      std::mutex& src_mtx)
      : widget(_label), ptype(_ptype), src(_data_source), src_mtx(src_mtx) {}

  void draw() override {
    switch (ptype) {
    case type::LINES:
      ImGui::PlotLines(label.c_str(), data.data(), buffer_max_limit, head);
      break;
    case type::HISTOGRAM:
      ImGui::PlotHistogram(label.c_str(), data.data(), buffer_max_limit, head);
      break;
    }
  }

  void copyFromSource() override {
    if (is_data_available.load()) {
      is_being_copied.store(true);
      std::lock_guard<std::mutex> lock(src_mtx);
    }
  }
  ~Plot() {}

private:
  size_t head = 0;
  void pushData() {
    data[head] = *src;
    head = (head + 1) & (buffer_max_limit - 1);
  }
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
  ~button() {}
};

template <typename _data_type = std::string> class text : public widget {
public:
  _data_type data = std::string("hello");

  std::shared_ptr<_data_type> src;
  std::mutex &src_mtx;

  text(std::string_view _label, std::mutex &src_mtx) : widget(_label), src_mtx(src_mtx) {}
  void draw() override { ImGui::Text("%s", data.c_str()); }
  void copyFromSource() override {
    if (is_data_available.load()) {
      is_being_copied.store(true);
      std::lock_guard<std::mutex> lock(src_mtx);
      data = *src;
      is_data_available.store(false);
      is_being_copied.store(false);
    }
  }
  ~text() {}
};

template <typename _data_type>
  requires std::integral<_data_type> || std::floating_point<_data_type>
class radial_gauge : public widget {
protected:
  struct Coordinates {
    float width;
    float radius;
    float start_angle;
    float end_angle;
  };

public:
  std::array<_data_type, 2> range;
  std::shared_ptr<_data_type> src;
  std::mutex &src_mtx;
  _data_type data;

  radial_gauge(std::string_view _label, _data_type min, _data_type max, std::mutex &src_mtx)
      : widget(_label), range{min, max}, src_mtx(src_mtx) {}

  Coordinates coordinates{200.0f, 200.0f / 2.0f, 3.14159f * 0.75f,
                          3.14159f * 2.25f};
  void draw() override {
    // INIT
    ImVec2 pos = ImGui::GetCursorScreenPos(); // Top-left corner of the widget
    ImVec2 center =
        ImVec2(pos.x + coordinates.radius, pos.y + coordinates.radius);
    ImGui::Dummy(
        ImVec2(coordinates.width, coordinates.width)); // Reserve the space

    // DRAW
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    // Draw the grey "background" arc
    draw_list->PathArcTo(center, coordinates.radius, coordinates.start_angle,
                         coordinates.end_angle,
                         32); // 32 segments for smoothness
    draw_list->PathStroke(ImGui::GetColorU32(ImGuiCol_FrameBg), 0,
                          10.0f); // 10px thickness
    float current_angle =
        coordinates.start_angle +
        (coordinates.end_angle - coordinates.start_angle) * (data / range[1]);

    draw_list->PathArcTo(center, coordinates.radius, coordinates.start_angle,
                         current_angle, 32);
    draw_list->PathStroke(ImGui::GetColorU32(ImGuiCol_PlotHistogram), 0, 10.0f);
  }

  void copyFromSource() override {
    if (is_data_available.load()) {
      is_being_copied.store(true);
      std::lock_guard<std::mutex> lock(src_mtx);
      data = *src;
      is_data_available.store(false);
      is_being_copied.store(false);
    }
  }
  ~radial_gauge() {}
};

} // namespace Widget
