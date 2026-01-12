#include "widget.h"
#include <memory>
#include <string>
#include <unordered_map>
namespace ParseJson {

    struct DataStore{
        std::unordered_map<std::string, std::shared_ptr<int>> MapInt;
        std::unordered_map<std::string, std::shared_ptr<float>> MapFloat;
        std::unordered_map<std::string, std::shared_ptr<std::string>> MapString;
    };
    class HttpWindowWrapper {
        std::unordered_map<std::string, std::unique_ptr<Widgets::Widget>> widgets;

    };
}
