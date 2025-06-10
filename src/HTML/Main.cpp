#include "HTMLObject.hpp"
#include <Interfaces/ByteDeviceSequence.hpp>
#include <Interfaces/Saveable.hpp>
#include <iostream>
#include <curl/curl.h>

struct Curl {
    Curl(void) {
        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "MathLib/1.0");
    }
    ~Curl(void) {
        curl_easy_cleanup(curl);
    }
    template <typename T>
    MathLib::Array<T> Get(const MathLib::Sequence<char>& url) {
        const size_t urlSize = url.GetSize();
        char buff[urlSize + 1];
        for (size_t i = 0; i < urlSize; i++) buff[i] = url.At(i);
        buff[urlSize] = '\0';
        curl_easy_setopt(curl, CURLOPT_URL, buff);
        MathLib::Array<uint8_t> tmp;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tmp);
        if (curl_easy_perform(curl) != CURLE_OK) return MathLib::Array<T>();
        const size_t size = tmp.GetSize();
        if (size % sizeof(T)) return MathLib::Array<T>();
        return MathLib::Array<T>((const T*)tmp.GetValue(), size / sizeof(T));
    }

    private:
    static size_t WriteCallback(uint8_t* data, size_t count, size_t elementSize, MathLib::WritableSequence<uint8_t>* output) {
        const size_t size = count * elementSize;
        for (size_t i = 0; i < size; i++)
            if (!output->Add(data[i])) return i;
        return size;
    }

    CURL* curl;
};
HTMLObject LoadHTML(const MathLib::Sequence<char>& str) {
    size_t i = 0;
    MathLib::SkipWhiteSpace(str, i);
    const char* signature = "<!DOCTYPE html>";
    size_t tmp = 0;
    while (signature[tmp])
        if (str.At(i++) != signature[tmp++]) return HTMLObject();
    return LoadHTMLObject(str, i);
}
MathLib::String SaveHTML(const HTMLObject& root) {
    return "<!DOCTYPE html>\n"_M + root.ToString();
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        Curl curl;
        // std::cout << curl.Get<char>("https://en.wikipedia.org/wiki/Linear_partial_information"_M) << std::endl;
        std::cout << SaveHTML(LoadHTML(curl.Get<char>("https://en.wikipedia.org/wiki/Linear_partial_information"_M))) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}