#pragma once
#include <iostream>
#include <stdexcept>
#include <exception>
#include <type_traits>

namespace reflect_enum{

// Using a set of Macro to print infos.
#define Log(x) (std::cout << __FILE__ << " : " << __LINE__ << " : " << __PRETTY_FUNCTION__ << " : " << (x) << std::endl)

template<bool Cond, typename T=void>
struct my_enable_if{};

template<typename T>
struct my_enable_if<true, T>{
    using type = T;
};

template<bool Cond, typename T=void>
using my_enable_if_t = typename my_enable_if<Cond, T>::type;

template<typename T, T Val>
struct my_integral_constant{
    T value = Val;
};

// integral type can be evaluated at compile-time.
template<typename T, T N>
std::string get_enum_name_static(){
    return __PRETTY_FUNCTION__;
}

// compile-time loop
template<size_t Beg, size_t End, typename Func>
std::enable_if_t<Beg == End>
static_for(const Func& func) {};

template<size_t Beg, size_t End, typename Func>
std::enable_if_t<Beg < End>
static_for(const Func& func){
    /* if constexpr (Beg == End) {
     *      return;
     * } else{ */
    auto result = func(std::integral_constant<size_t, Beg>{}); 
    if(result) {
        return;
    }
    static_for<Beg + 1, End>(func);
    /* } */
};

template<size_t Beg = 0, size_t End = 256, typename T>
std::string get_enum_name(T num){
    if(!std::is_enum_v<T>){
        Log("You need to put a enumerator as param");
        throw std::exception();
    }

    std::string str;
    bool flag = false;
    // how to convert a run-time variable `num` to compile-time template argument, exhaustively loop for comparison.
    static_for<Beg, End>([&num, &str, &flag](const auto& param) {
        if(static_cast<size_t>(param.value) == static_cast<size_t>(num)){
            flag = true;
            str = get_enum_name_static<T, static_cast<T>(param.value)>();
        } 
        return flag;
    });
    if(str.empty()) {
        return "";
    }
    size_t pos = str.find("N = ");
    size_t pos_ = str.find("; ", pos+4);
    size_t pos__ = str.find("::", pos+4);
    str = str.substr(pos__+2, pos_-pos__-2);
    return str;
}



// Here, T is the type of enum class, which is we need to specify and will be returned an instantiation of it.
template<typename T, size_t Beg = 0, size_t End = 256>
T enum_from_name(const std::string& str){
    for(size_t i = static_cast<size_t>(Beg); i < static_cast<size_t>(End); ++i){
        if(str == get_enum_name(static_cast<T>(i))){
            return static_cast<T>(i); 
        }
    }
    throw std::runtime_error("Not in [Beg, End) size");
}

} // namespace reflect_enum::
