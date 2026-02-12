#include <cstddef>
#include <vector>
#include <iostream>
#include <format>
namespace Utils {
    namespace Log{
        template <typename t>
        void logVec(const std::vector<t>& vec){
            std::cerr << std::format("Vector Size : {}", vec.size());
            std::cin.get();
            for(const t& elem : vec){
                std::cerr << elem;
            }
            std::cerr << '\n';
        }
    }
}