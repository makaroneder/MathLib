#ifndef Freestanding
#ifndef MathLib_Libc_HostString_H
#define MathLib_Libc_HostString_H
#include "../Interfaces/Sequence/Collection.hpp"
#include <string>

namespace MathLib {
    struct HostString : Collection<char> {
        using Sequence<char>::operator==;
        using Sequence<char>::operator!=;
        using Collection<char>::operator+=;
        HostString(void) {}
        HostString(const char* str) : string(str) {}
        HostString(const char* str, size_t size) : string(str, size) {}
        HostString(const std::string& str) : string(str) {}
        HostString(char chr) : string(1, chr) {}
        [[nodiscard]] virtual bool Add(const char& val) override {
            string += val;
            return true;
        }
        [[nodiscard]] virtual bool Reset(void) override {
            string.clear();
            return true;
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return string.size();
        }
        [[nodiscard]] virtual char* GetValue(void) override {
            return string.data();
        }
        [[nodiscard]] virtual const char* GetValue(void) const override {
            return string.c_str();
        }
        [[nodiscard]] std::string ToStdString(void) const {
            return string;
        }
        HostString& operator+=(const char* other) {
            string += other;
            return *this;
        }
        [[nodiscard]] HostString operator+(const Sequence<char>& other) const {
            HostString tmp = string;
            tmp += other;
            return tmp;
        }
        [[nodiscard]] HostString operator+(const char* other) const {
            HostString tmp = string;
            tmp += other;
            return tmp;
        }
        [[nodiscard]] HostString operator+(char chr) const {
            HostString tmp = string;
            tmp += chr;
            return tmp;
        }
        [[nodiscard]] bool operator==(const char* other) const {
            return string == other;
        }
        [[nodiscard]] bool operator!=(const char* other) const {
            return string != other;
        }

        private:
        std::string string;
    };
}

#endif
#endif