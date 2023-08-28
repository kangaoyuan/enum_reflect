#include <iostream>
#include <string>
#include "impl_enum.hpp"

enum class Color {
    RED, GREEN, BLUE = 7, YELLOW,
};

int main() {
    try{
        std::cout << reflect_enum::get_enum_name(1) << std::endl;
    } catch(...){}
    auto var = Color::GREEN;
    std::cout << reflect_enum::get_enum_name(var) << std::endl;
    std::cout << reflect_enum::get_enum_name(Color::YELLOW) << std::endl;
    std::cout << static_cast<size_t>(reflect_enum::enum_from_name<Color>("GREEN")) << std::endl;
    std::cout << static_cast<size_t>(reflect_enum::enum_from_name<Color>("YELLOW")) << std::endl;
    return 0;
}
