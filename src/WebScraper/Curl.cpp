#include "Curl.hpp"

Curl::Curl(void) {
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "MathLib/1.0");
}
Curl::~Curl(void) {
    curl_easy_cleanup(curl);
}
size_t Curl::WriteCallback(uint8_t* data, size_t count, size_t elementSize, MathLib::WritableSequence<uint8_t>* output) {
    const size_t size = count * elementSize;
    for (size_t i = 0; i < size; i++)
        if (!output->Add(data[i])) return i;
    return size;
}