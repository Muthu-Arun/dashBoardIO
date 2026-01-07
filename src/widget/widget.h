#include <concepts>
#include <deque>
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

class button : public widget {
public:
  void draw() override;
  void action() override;
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