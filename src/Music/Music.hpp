#ifndef Music_H
#define Music_H
#include "MusicNote.hpp"
#include <Saveable.hpp>

template <typename T>
struct Music : Saveable {
    Music(void) {}
    Music(const Hertz<T>& sampleRate_, const T& amplitude, const Array<MusicNote<T>>& notes) : sampleRate(sampleRate_) {
        for (const MusicNote<T>& note : notes) {
            const size_t numSamples = (note.duration * sampleRate).GetValue();
            const Hertz<T> frequency = note.GetFrequency();
            for (size_t j = 0; j < numSamples; j++)
                samples.Add((int16_t)(amplitude * Sin((frequency * 2 * pi * j / sampleRate).GetValue())));
        }
    }
    /// @brief Saves data
    /// @param file File to save data into
    /// @return Status
    virtual bool Save(ByteDevice& file) const override {
        return (
            file.WriteBuffer("RIFF", 4) &&
            file.Write<uint32_t>(36 + samples.GetSize() * sizeof(int16_t)) &&
            file.WriteBuffer("WAVE", 4) &&
            file.WriteBuffer("fmt ", 4) &&
            file.Write<uint32_t>(16) &&
            file.Write<uint16_t>(1) &&
            file.Write<uint16_t>(1) &&
            file.Write<uint32_t>(sampleRate.GetValue()) &&
            file.Write<uint32_t>(sampleRate.GetValue() * 2) &&
            file.Write<uint16_t>(2) &&
            file.Write<uint16_t>(16) &&
            file.WriteBuffer("data", 4) &&
            file.Write<uint32_t>(samples.GetSize() * sizeof(int16_t)) &&
            file.WriteBuffer(samples.GetValue(), samples.GetSize() * sizeof(int16_t))
        );
    }
    /// @brief Loads data
    /// @param file File to load data from
    /// @return Status
    virtual bool Load(ByteDevice& file) override {
        char buff[5] = { '\0', };
        uint32_t size;
        uint32_t tmp32;
        uint16_t tmp16;
        if (
            !file.ReadBuffer(buff, 4) || String(buff) != "RIFF" ||
            !file.Read<uint32_t>(size)
        ) return false;
        size = (size - 36) / sizeof(int16_t);
        if (
            !file.ReadBuffer(buff, 4) || String(buff) != "WAVE" ||
            !file.ReadBuffer(buff, 4) || String(buff) != "fmt " ||
            !file.Read<uint32_t>(tmp32) || tmp32 != 16 ||
            !file.Read<uint16_t>(tmp16) || tmp16 != 1 ||
            !file.Read<uint16_t>(tmp16) || tmp16 != 1 ||
            !file.Read<uint32_t>(tmp32)
        ) return false;
        sampleRate = Hertz<T>(tmp32);
        if (
            !file.Read<uint32_t>(tmp32) || !FloatsEqual<T>(sampleRate.GetValue() * 2, tmp32) ||
            !file.Read<uint16_t>(tmp16) || tmp16 != 2 ||
            !file.Read<uint16_t>(tmp16) || tmp16 != 16 ||
            !file.ReadBuffer(buff, 4) || String(buff) != "data" ||
            !file.Read<uint32_t>(tmp32) || (size * sizeof(int16_t)) != tmp32
        ) return false;
        int16_t tmp[size];
        if (!file.ReadBuffer(tmp, size * sizeof(int16_t))) return false;
        samples = Array<int16_t>(tmp, size);
        return true;
    }

    private:
    Array<int16_t> samples;
    Hertz<T> sampleRate;
};

#endif