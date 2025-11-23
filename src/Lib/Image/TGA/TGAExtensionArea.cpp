#include "TGAExtensionArea.hpp"
#include "../../Typedefs.hpp"

namespace MathLib {
    TGAExtensionArea::TGAExtensionArea(void) :  extensionSize(0), softwareVersion(0), keyColor(0), colorCorrectionOffset(0), postageStampOffset(0), scanLineOffset(0), attributesType(0) {}
    TGAExtensionArea::TGAExtensionArea(const Sequence<char>& authorNameStr, const Sequence<char>& jobNameStr, const Sequence<char>& softwareIDStr, const Sequence<char>& authorComments1, const Sequence<char>& authorComments2, const Sequence<char>& authorComments3, const Sequence<char>& authorComments4, uint8_t attributesType) : extensionSize(sizeof(TGAExtensionArea)), softwareVersion(0), keyColor(0), colorCorrectionOffset(0), postageStampOffset(0), scanLineOffset(0), attributesType(attributesType) {
        *(uint32_t*)gammaValue = 0;
        *(uint32_t*)pixelAspectRatio = 0;
        for (uint8_t i = 0; i < SizeOfArray(timestamp); i++) timestamp[i] = 0;
        for (uint8_t i = 0; i < SizeOfArray(jobTime); i++) jobTime[i] = 0;
        const uint8_t authorNameSize = Min<size_t>(authorNameStr.GetSize(), SizeOfArray(authorName));
        for (uint8_t i = 0; i < authorNameSize; i++) authorName[i] = authorNameStr.At(i);
        for (uint8_t i = authorNameSize; i < SizeOfArray(authorName); i++) authorName[i] = '\0';
        const uint8_t jobNameSize = Min<size_t>(jobNameStr.GetSize(), SizeOfArray(jobName));
        for (uint8_t i = 0; i < jobNameSize; i++) jobName[i] = jobNameStr.At(i);
        for (uint8_t i = jobNameSize; i < SizeOfArray(jobName); i++) jobName[i] = '\0';
        const uint8_t softwareIDSize = Min<size_t>(softwareIDStr.GetSize(), SizeOfArray(softwareID));
        for (uint8_t i = 0; i < softwareIDSize; i++) softwareID[i] = softwareIDStr.At(i);
        for (uint8_t i = softwareIDSize; i < SizeOfArray(softwareID); i++) softwareID[i] = '\0';
        const uint8_t authorComments1Size = Min<size_t>(authorComments1.GetSize(), 80);
        const uint8_t authorComments2Size = Min<size_t>(authorComments2.GetSize(), 80);
        const uint8_t authorComments3Size = Min<size_t>(authorComments3.GetSize(), 80);
        const uint8_t authorComments4Size = Min<size_t>(authorComments4.GetSize(), 80);
        for (uint8_t i = 0; i < authorComments1Size; i++) authorComments[i + 0 * 81] = authorComments1.At(i);
        for (uint8_t i = authorComments1Size; i < 81; i++) authorComments[i + 0 * 81] = '\0';
        for (uint8_t i = 0; i < authorComments2Size; i++) authorComments[i + 1 * 81] = authorComments2.At(i);
        for (uint8_t i = authorComments2Size; i < 81; i++) authorComments[i + 1 * 81] = '\0';
        for (uint8_t i = 0; i < authorComments3Size; i++) authorComments[i + 2 * 81] = authorComments3.At(i);
        for (uint8_t i = authorComments3Size; i < 81; i++) authorComments[i + 2 * 81] = '\0';
        for (uint8_t i = 0; i < authorComments4Size; i++) authorComments[i + 3 * 81] = authorComments4.At(i);
        for (uint8_t i = authorComments4Size; i < 81; i++) authorComments[i + 3 * 81] = '\0';
    }
}