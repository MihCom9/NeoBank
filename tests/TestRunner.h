#pragma once
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

class TestRunner {
    int passed_ = 0, failed_ = 0;
public:
    void suite(const std::string& name) {
        std::cout << "\n[Suite] " << name << "\n";
    }

    void run(const std::string& name, std::function<void()> fn) {
        try {
            fn();
            ++passed_;
            std::cout << "  \033[32m[PASS]\033[0m " << name << "\n";
        } catch (const std::exception& e) {
            ++failed_;
            std::cout << "  \033[31m[FAIL]\033[0m " << name << " — " << e.what() << "\n";
        } catch (...) {
            ++failed_;
            std::cout << "  \033[31m[FAIL]\033[0m " << name << " — unknown exception\n";
        }
    }

    void summary() const {
        std::cout << "\n" << std::string(44, '=') << "\n";
        if (failed_ == 0)
            std::cout << "\033[32mAll " << passed_ << " tests passed.\033[0m\n";
        else
            std::cout << "\033[31m" << failed_ << " failed\033[0m, "
                      << passed_ << " passed  (" << (passed_ + failed_) << " total)\n";
        std::cout << std::string(44, '=') << "\n";
    }

    int exitCode() const { return failed_ > 0 ? 1 : 0; }
};

#define ASSERT(cond) \
    do { if (!(cond)) throw std::runtime_error("Assertion failed: " #cond " (line " + std::to_string(__LINE__) + ")"); } while(0)

#define ASSERT_EQ(a, b) \
    do { if ((a) != (b)) throw std::runtime_error(std::string("Not equal: " #a " != " #b) + " (line " + std::to_string(__LINE__) + ")"); } while(0)

#define ASSERT_NEAR(a, b, eps) \
    do { double _a = (double)(a), _b = (double)(b), _e = (double)(eps); \
         if ((_a - _b) > _e || (_b - _a) > _e) \
             throw std::runtime_error(std::string("Not near: " #a " vs " #b) + " (line " + std::to_string(__LINE__) + ")"); } while(0)

#define ASSERT_THROWS(expr) \
    do { bool _caught = false; try { expr; } catch (...) { _caught = true; } \
         if (!_caught) throw std::runtime_error("Expected exception: " #expr " (line " + std::to_string(__LINE__) + ")"); } while(0)
