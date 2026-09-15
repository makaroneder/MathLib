#ifndef Curl_H
#define Curl_H
#include <Typedefs.hpp>
#include <curl/curl.h>

namespace MathLib {
    struct Curl {
        static constexpr const char* userAgent = "MathLib/1.0";
        Curl(void);
        ~Curl(void);
        template <typename T>
        Array<T> Get(const Sequence<char>& url) {
            const size_t urlSize = url.GetSize();
            char buff[urlSize + 1];
            for (size_t i = 0; i < urlSize; i++) buff[i] = url.At(i);
            buff[urlSize] = '\0';
            curl_easy_setopt(curl, CURLOPT_URL, buff);
            Array<uint8_t> tmp;
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tmp);
            if (curl_easy_perform(curl) != CURLE_OK) return Array<T>();
            const size_t size = tmp.GetSize();
            if (size % sizeof(T)) return Array<T>();
            return Array<T>((const T*)tmp.GetValue(), size / sizeof(T));
        }

        private:
        static size_t WriteCallback(uint8_t* data, size_t count, size_t elementSize, WritableSequence<uint8_t>* output);

        CURL* curl;
    };
}

#endif