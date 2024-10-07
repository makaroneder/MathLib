#include "TGA.hpp"
#include "Color.hpp"
#include "TGAHeader.hpp"
#include "TGAFooter.hpp"
#include "TGAExtensionArea.hpp"

const String identifier = "MathLib";
TGA::TGA(size_t width, size_t height) : SaveableImage(width, height) {}
bool TGA::Save(Writeable& file) const {
    TGAHeader header = {};
    header.idLength = identifier.GetSize();
    header.imageType = 2;
    header.width = GetWidth();
    header.height = header.yOrigin = GetHeight();
    header.pixelDepth = 32;
    header.imageDescriptor = 1 << 5;
    if (!file.Write<TGAHeader>(header)) return false;
    if (!file.Puts(identifier)) return false;
    for (size_t y = 0; y < header.height; y++) {
        for (size_t x = 0; x < header.width; x++) {
            const Color pixel = pixels.At(x, y);
            if (!file.Write<uint8_t>(pixel.rgba.b)) return false;
            if (!file.Write<uint8_t>(pixel.rgba.g)) return false;
            if (!file.Write<uint8_t>(pixel.rgba.r)) return false;
            if (!file.Write<uint8_t>(pixel.rgba.a)) return false;
        }
    }
    TGAExtensionArea extension = {};
    extension.extensionSize = sizeof(TGAExtensionArea);
    extension.attributesType = 3;
    for (size_t i = 0; i < identifier.GetSize() && i < 41; i++) {
        extension.authorName[i] = identifier.At(i);
        extension.softwareId[i] = identifier.At(i);
        extension.jobName[i] = identifier.At(i);
    }
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 80; j++) extension.authorComments[i * 81 + j] = 'A';
        extension.authorComments[i * 81 + 80] = '\0';
    }
    for (size_t i = 0; i < 6; i++) extension.timestamp[i] = 0;
    for (size_t i = 0; i < 3; i++) extension.jobTime[i] = 0;
    if (!file.Write<TGAExtensionArea>(extension)) return false;
    TGAFooter footer = {};
    footer.extensionOffset = sizeof(TGAHeader) + header.width * header.height * sizeof(uint32_t);
    footer.developerOffset = 0;
    for (size_t i = 0; footer.expectedSignature[i]; i++) footer.signature[i] = footer.expectedSignature[i];
    if (!file.Write<TGAFooter>(footer)) return false;
    return true;
}
bool TGA::Load(Readable& file) {
    TGAHeader header;
    if (!file.Read<TGAHeader>(header)) return false;
    if (header.imageType != 2) return false;
    if (header.pixelDepth != 32) return false;
    bool createdByMathLib = false;
    if (header.idLength) {
        char id[header.idLength];
        if (!file.ReadBuffer(id, header.idLength)) return false;
        if (String(id) == identifier) createdByMathLib = true;
    }
    pixels = Matrix<uint32_t>(header.width, header.height);
    for (size_t y = 0; y < header.height; y++) {
        for (size_t x = 0; x < header.width; x++) {
            Color pixel;
            if (!file.Read<uint8_t>(pixel.rgba.b)) return false;
            if (!file.Read<uint8_t>(pixel.rgba.g)) return false;
            if (!file.Read<uint8_t>(pixel.rgba.r)) return false;
            if (!file.Read<uint8_t>(pixel.rgba.a)) return false;
            pixels.At(x, y) = pixel.hex;
        }
    }
    if (createdByMathLib) {
        TGAExtensionArea extension;
        if (!file.Read<TGAExtensionArea>(extension)) return false;
        if (extension.extensionSize != sizeof(TGAExtensionArea)) return false;
        if (extension.attributesType != 3) return false;
        if (String(extension.authorName) != identifier) return false;
        if (String(extension.softwareId) != identifier) return false;
        if (String(extension.jobName) != identifier) return false;
        TGAFooter footer;
        if (!file.Read<TGAFooter>(footer)) return false;
        if (String(footer.signature) != String(footer.expectedSignature)) return false;
        if (footer.extensionOffset != sizeof(TGAHeader) + header.width * header.height * sizeof(uint32_t)) return false;
    }
    return true;
}