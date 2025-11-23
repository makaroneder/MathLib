#include "TGA.hpp"
#include "TGAHeader.hpp"
#include "TGAFooter.hpp"
#include "../../Color.hpp"
#include "TGAExtensionArea.hpp"

namespace MathLib {
    const String identifier = "MathLib";
    TGA::TGA(size_t width, size_t height) : SaveableImage(width, height) {}
    bool TGA::Save(Writable& file) const {
        TGAHeader header = TGAHeader(identifier.GetSize(), TGAHeader::ImageType::UncompressedTrueColor, GetWidth(), GetHeight(), 32);
        if (!file.Write<TGAHeader>(header) || !file.Puts(identifier)) return false;
        for (size_t y = 0; y < header.height; y++) {
            for (size_t x = 0; x < header.width; x++) {
                const Color pixel = pixels.AtUnsafe(x, y);
                const uint8_t tmp[] = {
                    pixel.rgba.b,
                    pixel.rgba.g,
                    pixel.rgba.r,
                    pixel.rgba.a,
                };
                if (!file.WriteBuffer(tmp, SizeOfArray(tmp) * sizeof(uint8_t))) return false;
            }
        }
        return file.Write<TGAExtensionArea>(TGAExtensionArea(identifier, identifier, identifier, "A"_M, "A"_M, "A"_M, "A"_M, 3)) && file.Write<TGAFooter>(TGAFooter(sizeof(TGAHeader) + header.width * header.height * sizeof(uint32_t), 0));
    }
    bool TGA::Load(Readable& file) {
        TGAHeader header;
        if (!file.Read<TGAHeader>(header) || header.imageType != TGAHeader::ImageType::UncompressedTrueColor || (header.pixelDepth != 32 && header.pixelDepth != 24)) return false;
        bool createdByMathLib = false;
        if (header.idLength) {
            char id[header.idLength];
            if (!file.ReadBuffer(id, header.idLength)) return false;
            if (String(id) == identifier) createdByMathLib = true;
        }
        pixels = Matrix<uint32_t>(header.width, header.height);
        for (size_t y = 0; y < header.height; y++) {
            for (size_t x = 0; x < header.width; x++) {
                uint8_t tmp[4] = { 0, 0, 0, 0xff, };
                if (!file.ReadBuffer(tmp, header.pixelDepth / 8 * sizeof(uint8_t))) return false;
                pixels.AtUnsafe(x, y) = Color(tmp[2], tmp[1], tmp[0], tmp[3]).hex;
            }
        }
        if (createdByMathLib) {
            TGAExtensionArea extension;
            if (!file.Read<TGAExtensionArea>(extension) || extension.extensionSize != sizeof(TGAExtensionArea) || extension.attributesType != 3 || String(extension.authorName) != identifier || String(extension.softwareID) != identifier || String(extension.jobName) != identifier) return false;
            TGAFooter footer;
            if (!file.Read<TGAFooter>(footer) || String(footer.signature) != String(footer.expectedSignature) || footer.extensionOffset != sizeof(TGAHeader) + header.width * header.height * sizeof(uint32_t)) return false;
        }
        return true;
    }
}