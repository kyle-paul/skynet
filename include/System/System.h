#ifndef SKYNET_CORE_H
#define SKYNET_CORE_H

#include <memory>
#include <format>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <thread>
#include <filesystem>
#include <cmath>
#include <variant>

#define STATE_SIZE 18

#define LOG_RED    "\033[1;31m"
#define LOG_PINK   "\033[1;35m"
#define LOG_GREEN  "\033[1;32m"
#define LOG_YELLOW "\033[1;33m"
#define LOG_BLUE   "\033[1;34m"
#define LOG_CYAN   "\033[1;36m"
#define LOG_RESET  "\033[0m"

#define BIND_EVENT_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)

#define ERROR(fmt, ...) \
    fprintf(stderr, LOG_RED "%s" LOG_RESET "\n", std::format(fmt, ##__VA_ARGS__).c_str()); \
    exit(1);

#define INFO(fmt, ...) \
    fprintf(stdout, LOG_BLUE "%s" LOG_RESET "\n", std::format(fmt, ##__VA_ARGS__).c_str());

#define LOG(fmt, ...) \
    fprintf(stdout, LOG_PINK "%s" LOG_RESET, std::format(fmt, ##__VA_ARGS__).c_str());

#define TRACE(fmt) \
    fprintf(stdout, LOG_CYAN "%s" LOG_RESET "\n", std::format("{}", fmt).c_str());

#define WARN(fmt, ...) \
    fprintf(stdout, LOG_YELLOW "%s" LOG_RESET "\n", std::format(fmt, ##__VA_ARGS__).c_str());

#define SUCCESS(fmt, ...) \
    fprintf(stdout, LOG_GREEN "%s" LOG_RESET "\n", std::format(fmt, ##__VA_ARGS__).c_str());

#define ASSERT(condition, error_msg) \
    if (!(condition)) { \
        fprintf(stderr, LOG_RED "FAILED: %s" LOG_RESET "\n", std::format(error_msg).c_str()); \
        exit(1); }

namespace Skynet
{
    template<typename T>
    using scope = std::unique_ptr<T>;

    template <typename T>
    using ref = std::shared_ptr<T>;


    template<typename T, typename ... Args>
    constexpr scope<T> cscope(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename ... Args>
    constexpr ref<T> cref(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    using byte = unsigned char;
}

#endif