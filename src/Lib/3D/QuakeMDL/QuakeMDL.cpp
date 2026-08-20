#include "QuakeMDL.hpp"
#include "../../Color.hpp"

namespace MathLib {
    Array<uint8_t> quakeMDLPalette;
    [[nodiscard]] bool QuakeMDL::Save(Writable& file) const {
        // TODO:
        (void)file;
        return false;
    }
    [[nodiscard]] bool QuakeMDL::Load(Readable& file) {
        QuakeMDLHeader header;
        if (!file.Read<QuakeMDLHeader>(header) || !header.IsValid()) return false;
        for (uint64_t i = 0; i < header.textures; i++) {
            QuakeMDLSkinType type;
            if (!file.Read<QuakeMDLSkinType>(type)) return false;
            uint32_t arrays = 1;
            if (type != QuakeMDLSkinType::Single && !(file.Read<uint32_t>(arrays) && file.Skip(arrays * sizeof(float)))) return false;
            for (uint64_t j = 0; j < arrays; j++) {
                Array<uint8_t> texture = Array<uint8_t>((uint64_t)header.textureWidth * header.textureHeight);
                if (!file.ReadCollection<uint8_t>(texture)) return false;
                Image image = Image(header.textureWidth, header.textureHeight);
                for (uint64_t y = 0; y < header.textureHeight; y++) {
                    for (uint64_t x = 0; x < header.textureWidth; x++) {
                        const uint8_t index = texture.AtUnsafe(y * header.textureWidth + x);
                        if (index == UINT8_MAX) image.AtUnsafe(x, y) = 0;
                        else image.AtUnsafe(x, y) = Color(quakeMDLPalette.At(index * 3 + 0), quakeMDLPalette.At(index * 3 + 1), quakeMDLPalette.At(index * 3 + 2), 0xff).hex;
                    }
                }
                if (!textures.Add(image)) return false;
            }
        }
        QuakeMDLTextureCoordinate mdlTextureCoordinates[header.vertices];
        if (!file.ReadBuffer(mdlTextureCoordinates, header.vertices * sizeof(QuakeMDLTextureCoordinate))) return false;
        QuakeMDLTriangle mdlTriangles[header.triangles];
        if (!file.ReadBuffer(mdlTriangles, header.triangles * sizeof(QuakeMDLTriangle))) return false;
        Array<uint32_t> baseFace = Array<uint32_t>(header.triangles * 3);
        for (uint64_t i = 0; i < header.triangles; i++)
            for (uint8_t j = 0; j < 3; j++)
                baseFace.AtUnsafe(3 * i + j) = mdlTriangles[i].vertices[j];
        for (uint64_t i = 0; i < header.frames; i++) {
            QuakeMDLFrameType type;
            if (!file.Read<QuakeMDLFrameType>(type)) return false;
            uint32_t arrays = 1;
            if (type != QuakeMDLFrameType::Single) {
                QuakeMDLFrameGroup frameGroup;
                if (!file.Read<QuakeMDLFrameGroup>(frameGroup) || !file.Skip(frameGroup.frames * sizeof(float))) return false;
                arrays = frameGroup.frames;
            }
            for (uint64_t j = 0; j < arrays; j++) {
                QuakeMDLFrame frame;
                if (!file.Read<QuakeMDLFrame>(frame)) return false;
                QuakeMDLVertex mdlVertices[header.vertices];
                if (!file.ReadBuffer(mdlVertices, header.vertices * sizeof(QuakeMDLVertex))) return false;
                Array<uint32_t> face = baseFace;
                Array<float> verts = Array<float>(header.vertices * 5);
                for (uint64_t k = 0; k < header.vertices; k++) {
                    for (uint8_t l = 0; l < 3; l++)
                        verts.AtUnsafe(k * 5 + l) = (float)mdlVertices[k].data[l] * header.scale.data[l] + header.translate.data[l];
                    verts.AtUnsafe(k * 5 + 3) = MathLib::nan;
                    verts.AtUnsafe(k * 5 + 4) = MathLib::nan;
                }
                for (uint64_t k = 0; k < header.triangles; k++) {
                    uint32_t newFace[] = { UINT32_MAX, UINT32_MAX, UINT32_MAX, };
                    for (uint8_t l = 0; l < 3; l++) {
                        const uint32_t index = mdlTriangles[k].vertices[l];
                        float s = mdlTextureCoordinates[index].s;
                        if (!mdlTriangles[k].isFrontFace && mdlTextureCoordinates[index].isOnBoundary)
                            s += header.textureWidth * 0.5;
                        s = (s + 0.5) / header.textureWidth;
                        const float t = ((float)mdlTextureCoordinates[index].t + 0.5) / header.textureHeight;
                        if (MathLib::IsNaN(verts.At(index * 5 + 3))) {
                            verts.At(index * 5 + 3) = s;
                            verts.At(index * 5 + 4) = t;
                            continue;
                        }
                        if (MathLib::FloatsEqual<float>(verts.At(index * 5 + 3), s) && MathLib::FloatsEqual<float>(verts.At(index * 5 + 4), t)) continue;
                        newFace[l] = verts.GetSize() / 5;
                        for (uint8_t h = 0; h < 3; h++)
                            if (!verts.Add(verts.At(index * 5 + h))) return false;
                        if (!verts.Add(s)) return false;
                        if (!verts.Add(t)) return false;
                    }
                    for (uint8_t l = 0; l < 3; l++)
                        if (newFace[l] != UINT32_MAX) face.AtUnsafe(k * 3 + l) = newFace[l];
                }
                if (!vertices.Add(verts)) return false;
                if (!faces.Add(face)) return false;
            }
        }
        return true;
    }
}