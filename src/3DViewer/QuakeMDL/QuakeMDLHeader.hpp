#ifndef QuakeMDLHeader_H
#define QuakeMDLHeader_H
#include "QuakeMDLVector.hpp"
#include <stdint.h>

struct QuakeMDLHeader {
    static constexpr const char* expectedSignature = "IDPO";
    static constexpr uint32_t expectedVersion = 6;
    enum class SynchronizationType : uint32_t {
        Synchronized = 0,
        Random,
    };

    char signature[4];
    uint32_t version;
    QuakeMDLVector scale;
    QuakeMDLVector translate;
    float boundingRadius;
    QuakeMDLVector camera;
    uint32_t textures;
    uint32_t textureWidth;
    uint32_t textureHeight;
    uint32_t vertices;
    uint32_t triangles;
    uint32_t frames;
    SynchronizationType synchronizationType;
    uint32_t flags;
    float size;

    [[nodiscard]] bool IsValid(void) const;
} __attribute__((packed));

#endif