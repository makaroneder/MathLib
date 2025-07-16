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
        HostString(void) {
            EmptyBenchmark
        }
        HostString(const char* str) : string(str) {
            EmptyBenchmark
        }
        HostString(const char* str, size_t size) : string(str, size) {
            EmptyBenchmark
        }
        HostString(const std::string& str) : string(str) {
            EmptyBenchmark
        }
        HostString(char chr) : HostString((HostString() + chr).GetValue()) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual bool Add(const char& val) override {
            StartBenchmark
            string += val;
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] virtual bool Reset(void) override {
            StartBenchmark
            string.clear();
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(string.size());
        }
        [[nodiscard]] virtual char* GetValue(void) override {
            StartAndReturnFromBenchmark(string.data());
        }
        [[nodiscard]] virtual const char* GetValue(void) const override {
            StartAndReturnFromBenchmark(string.c_str());
        }
        [[nodiscard]] std::string ToStdString(void) const {
            StartAndReturnFromBenchmark(string);
        }
        HostString& operator+=(const char* other) {
            StartBenchmark
            string += other;
            ReturnFromBenchmark(*this);
        }
        [[nodiscard]] HostString operator+(const Sequence<char>& other) const {
            StartBenchmark
            HostString tmp = *this;
            tmp += other;
            ReturnFromBenchmark(tmp);
        }
        [[nodiscard]] HostString operator+(const char* other) const {
            StartBenchmark
            HostString tmp = *this;
            tmp += other;
            ReturnFromBenchmark(tmp);
        }
        [[nodiscard]] HostString operator+(const char& chr) const {
            StartBenchmark
            HostString tmp = *this;
            tmp += chr;
            ReturnFromBenchmark(tmp);
        }
        [[nodiscard]] bool operator==(const char* other) const {
            StartAndReturnFromBenchmark(string == other);
        }
        [[nodiscard]] bool operator!=(const char* other) const {
            StartAndReturnFromBenchmark(!(*this == other));
        }

        private:
        std::string string;
    };
}

#endif
#endif