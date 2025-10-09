#pragma once

#include <string>
#include <stdexcept>
#include <utility>

template <typename T>
class Result {
private:
    bool m_is_success;
    T m_value;
    std::string m_error;

    Result(bool success, T value, std::string error)
        : m_is_success(success), m_value(std::move(value)), m_error(std::move(error)) {}

public:

    static Result<T> success(T value) {
        return Result(true, std::move(value), "");
    }

    static Result<T> error(const std::string& error_message) {
        return Result(false, T{}, error_message);
    }

    bool is_success() const {
        return m_is_success;
    }

    const T& get_value() const {
        if (!m_is_success) {
            throw std::runtime_error("Attempted to get value from a failed Result: " + m_error);
        }
        return m_value;
    }

    const std::string& get_error() const {
        return m_error;
    }
};