#pragma once
#include <memory>
#include <format>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>


#define RED    "\033[1;31m"
#define GREEN  "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE   "\033[1;34m"
#define RESET  "\033[0m"

#define ERROR(fmt, ...) \
    fprintf(stderr, RED "%s" RESET "\n", std::format(fmt, __VA_ARGS__).c_str()); \
    exit(1)

#define INFO(fmt, ...) \
    fprintf(stdout, BLUE "%s" RESET "\n", std::format(fmt, __VA_ARGS__).c_str());

#define WARN(fmt, ...) \
    fprintf(stdout, YELLOW "%s" RESET "\n", std::format(fmt, __VA_ARGS__).c_str());

#define SUCCESS(fmt, ...) \
    fprintf(stdout, GREEN "%s" RESET "\n", std::format(fmt, __VA_ARGS__).c_str());

#define ASSERT(condition, error_msg) \
    if (!(condition)) { \
        fprintf(stderr, RED "FAILED: %s" RESET "\n", std::format(error_msg).c_str()); \
        exit(1); }


template <typename T>
using ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr ref<T> cref(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}