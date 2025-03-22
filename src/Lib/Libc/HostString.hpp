#ifndef Freestanding
#ifndef MathLib_Libc_HostString_H
#define MathLib_Libc_HostString_H
#include "../Interfaces/Collection.hpp"
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
        HostString(char chr) : HostString((HostString() + chr).GetValue()) {}
        [[nodiscard]] virtual char At(size_t index) const override {
            return string.at(index);
        }
        [[nodiscard]] virtual char& At(size_t index) override {
            return string.at(index);
        }
        [[nodiscard]] virtual bool Add(const char& val) override {
            string += val;
            return true;
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return string.size();
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
            HostString tmp = *this;
            tmp += other;
            return tmp;
        }
        [[nodiscard]] HostString operator+(const char* other) const {
            HostString tmp = *this;
            tmp += other;
            return tmp;
        }
        [[nodiscard]] HostString operator+(const char& chr) const {
            HostString tmp = *this;
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