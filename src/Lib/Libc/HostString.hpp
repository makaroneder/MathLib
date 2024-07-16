#ifndef Freestanding
#ifndef HostString_H
#define HostString_H
#include "../Collection.hpp"
#include <string>

struct HostString : Collection<char> {
    HostString(void) {}
    HostString(const char* str) : string(str) {}
    HostString(const std::string& str) : string(str) {}
    HostString(const char& chr) : HostString((HostString() + chr).GetValue()) {}
    virtual char At(const size_t& index) const override {
        return string.at(index);
    }
    virtual char& At(const size_t& index) override {
        return string.at(index);
    }
    virtual bool Add(const char& val) override {
        string += val;
        return true;
    }
    virtual size_t GetSize(void) const override {
        return string.size();
    }
    virtual const char* GetValue(void) const override {
        return string.c_str();
    }
    std::string ToStdString(void) const {
        return string;
    }
    HostString& operator+=(const HostString& other) {
        string += other.string;
        return *this;
    }
    HostString& operator+=(const char* other) {
        string += other;
        return *this;
    }
    HostString& operator+=(const char& chr) {
        string += chr;
        return *this;
    }
    HostString operator+(const HostString& other) const {
        HostString tmp = *this;
        tmp += other;
        return tmp;
    }
    HostString operator+(const char* other) const {
        HostString tmp = *this;
        tmp += other;
        return tmp;
    }
    HostString operator+(const char& chr) const {
        HostString tmp = *this;
        tmp += chr;
        return tmp;
    }
    bool operator==(const HostString& other) const {
        return string == other.string;
    }
    bool operator==(const char* other) const {
        return string == other;
    }
    bool operator!=(const HostString& other) const {
        return string != other.string;
    }
    bool operator!=(const char* other) const {
        return string != other;
    }

    private:
    std::string string;
};

#endif
#endif