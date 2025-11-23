#ifndef QuakeMDL_H
#define QuakeMDL_H
#include "QuakeMDLFrame.hpp"
#include "../Animation3D.hpp"
#include "QuakeMDLHeader.hpp"
#include "QuakeMDLTriangle.hpp"
#include "QuakeMDLSkinType.hpp"
#include "QuakeMDLFrameType.hpp"
#include "QuakeMDLFrameGroup.hpp"
#include "QuakeMDLTextureCoordinate.hpp"

template <typename T>
struct QuakeMDL : Animation3D<T>, MathLib::Saveable {
    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override {
        // TODO:
        (void)file;
        return false;
    }
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override {
        QuakeMDLHeader header;
        if (!file.Read<QuakeMDLHeader>(header) || !header.IsValid()) return false;
        for (uint64_t i = 0; i < header.textures; i++) {
            QuakeMDLSkinType type;
            if (!file.Read<QuakeMDLSkinType>(type)) return false;
            uint32_t arrays = 1;
            if (type != QuakeMDLSkinType::Single && !(file.Read<uint32_t>(arrays) && file.Skip(arrays * sizeof(float)))) return false;
            if (!file.Skip(arrays * header.textureWidth * header.textureHeight)) return false;
        }
        if (!file.Skip(header.vertices * sizeof(QuakeMDLTextureCoordinate))) return false;
        QuakeMDLTriangle mdlTriangles[header.triangles];
        if (!file.ReadBuffer(mdlTriangles, header.triangles * sizeof(QuakeMDLTriangle))) return false;
        for (uint64_t i = 0; i < header.frames; i++) {
            QuakeMDLFrameType type;
            if (!file.Read<QuakeMDLFrameType>(type)) return false;
            uint32_t arrays = 1;
            if (type != QuakeMDLFrameType::Single) {
                QuakeMDLFrameGroup frameGroup;
                if (!file.Read<QuakeMDLFrameGroup>(frameGroup) || !file.Skip(frameGroup.frames * sizeof(float))) return false;
                arrays = frameGroup.frames;
            }
            QuakeMDLFrame frame[arrays];
            for (uint64_t j = 0; j < arrays; j++) {
                if (!file.Read<QuakeMDLFrame>(frame[j])) return false;
                QuakeMDLVertex vertices[header.vertices];
                if (!file.ReadBuffer(vertices, header.vertices * sizeof(QuakeMDLVertex))) return false;
                Model3D<T> model;
                model.triangles = MathLib::Array<Triangle<T>>(header.triangles);
                for (uint64_t k = 0; k < header.triangles; k++) {
                    for (uint8_t l = 0; l < SizeOfArray(Triangle<T>::vertices); l++) {
                        model.triangles.At(k).vertices[l].x = vertices[mdlTriangles[k].vertices[l]].x * header.scale.x + header.translate.x;
                        model.triangles.At(k).vertices[l].y = vertices[mdlTriangles[k].vertices[l]].y * header.scale.y + header.translate.y;
                        model.triangles.At(k).vertices[l].z = vertices[mdlTriangles[k].vertices[l]].z * header.scale.z + header.translate.z;
                    }
                }
                if (!this->frames.Add(model)) return false;
            }
        }
        return true;
    }
};

#endif