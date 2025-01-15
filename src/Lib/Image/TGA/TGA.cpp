#include "TGA.hpp"
#include "TGAHeader.hpp"
#include "TGAFooter.hpp"
#include "../../Color.hpp"
#include "TGAExtensionArea.hpp"

namespace MathLib {
    const String identifier = "MathLib";
    TGA::TGA(size_t width, size_t height) : SaveableImage(width, height) {
        EmptyBenchmark
    }
    bool TGA::Save(Writable& file) const {
        StartBenchmark
        TGAHeader header = {};
        header.idLength = identifier.GetSize();
        header.imageType = 2;
        header.width = GetWidth();
        header.height = header.yOrigin = GetHeight();
        header.pixelDepth = 32;
        header.imageDescriptor = 1 << 5;
        if (!file.Write<TGAHeader>(header) || !file.Puts(identifier)) ReturnFromBenchmark(false);
        for (size_t y = 0; y < header.height; y++) {
            for (size_t x = 0; x < header.width; x++) {
                const Color pixel = pixels.At(x, y);
                const uint8_t tmp[] = {
                    pixel.rgba.b,
                    pixel.rgba.g,
                    pixel.rgba.r,
                    pixel.rgba.a,
                };
                if (!file.WriteBuffer(tmp, SizeOfArray(tmp) * sizeof(uint8_t))) ReturnFromBenchmark(false);
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
        if (!file.Write<TGAExtensionArea>(extension)) ReturnFromBenchmark(false);
        TGAFooter footer = {};
        footer.extensionOffset = sizeof(TGAHeader) + header.width * header.height * sizeof(uint32_t);
        footer.developerOffset = 0;
        for (size_t i = 0; footer.expectedSignature[i]; i++) footer.signature[i] = footer.expectedSignature[i];
        ReturnFromBenchmark(file.Write<TGAFooter>(footer));
    }
    bool TGA::Load(Readable& file) {
        StartBenchmark
        TGAHeader header;
        if (!file.Read<TGAHeader>(header) || header.imageType != 2 || header.pixelDepth != 32) ReturnFromBenchmark(false);
        bool createdByMathLib = false;
        if (header.idLength) {
            char id[header.idLength];
            if (!file.ReadBuffer(id, header.idLength)) ReturnFromBenchmark(false);
            if (String(id) == identifier) createdByMathLib = true;
        }
        pixels = Matrix<uint32_t>(header.width, header.height);
        for (size_t y = 0; y < header.height; y++) {
            for (size_t x = 0; x < header.width; x++) {
                uint8_t tmp[4] = { 0, 0, 0, 0, };
                if (!file.ReadBuffer(tmp, SizeOfArray(tmp) * sizeof(uint8_t))) ReturnFromBenchmark(false);
                pixels.At(x, y) = Color(tmp[2], tmp[1], tmp[0], tmp[3]).hex;
            }
        }
        if (createdByMathLib) {
            TGAExtensionArea extension;
            if (!file.Read<TGAExtensionArea>(extension) || extension.extensionSize != sizeof(TGAExtensionArea) || extension.attributesType != 3 || String(extension.authorName) != identifier || String(extension.softwareId) != identifier || String(extension.jobName) != identifier) ReturnFromBenchmark(false);
            TGAFooter footer;
            if (!file.Read<TGAFooter>(footer) || String(footer.signature) != String(footer.expectedSignature) || footer.extensionOffset != sizeof(TGAHeader) + header.width * header.height * sizeof(uint32_t)) ReturnFromBenchmark(false);
        }
        ReturnFromBenchmark(true);
    }
}