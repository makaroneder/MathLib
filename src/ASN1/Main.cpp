#include <FileSystem/FileSystem.hpp>
#include <iostream>

enum class TagType : uint8_t {
    EndOfContent = 0,
    Bool,
    Int,
    BitString,
    OctetString,
    Null,
    ObjectIdentifier,
    ObjectDescriptor,
    External,
    Float,
    Enumerated,
    EmbeddedPDV,
    UTF8String,
    RelativeOID,
    Time,
    Reserved,
    Sequence,
    Set,
    NumericString,
    PrintableString,
    T61String,
    VideotexString,
    IA5String,
    UTCTime,
    GeneralizedTime,
    GraphicString,
    VisibleString,
    GeneralString,
    UniversalString,
    CharacterString,
    BMPString,
    LongForm,
    Date = LongForm,
    TimeOfDay,
    DateTime,
    Duration,
    OIDIRI,
    RelativeOIDIRI,
};
enum class TagClass : uint8_t {
    Universal = 0,
    Application,
    ContextSpecific,
    Private,
};
size_t ParseDERLength(const MathLib::Array<uint8_t>& certificate, size_t& i) {
    const uint8_t data = certificate.At(i++);
    const uint8_t size = data & 0b1111111;
    if (data & (1 << 7)) return size;
    if (!size) MathLib::Panic("Indefinite length");
    if (size == 127) MathLib::Panic("Reserved length");
    size_t ret = 0;
    for (uint8_t i = 0; i < size; i++) ret = (ret << 8) | certificate.At(i++);
    return ret;
}
void ParseDERInternal(const MathLib::Array<uint8_t>& certificate, size_t& i) {
    const uint8_t encoding = certificate.At(i++);
    if ((TagType)encoding == TagType::LongForm) MathLib::Panic("Long form");
    const TagType tagType = (TagType)(encoding & 0b11111);
    // const TagClass tagClass = (TagClass)(encoding >> 6);
    // const bool constructed = encoding & (1 << 5);
    switch (tagType) {
        default: MathLib::Panic("Unknown tag type 0x"_M + MathLib::ToString((uint8_t)tagType, 16, 2));
    }
}
void ParseDER(const MathLib::Array<uint8_t>& certificate) {
    size_t i = 0;
    ParseDERInternal(certificate, i);
}
void Main(int argc, char** argv, MathLib::FileSystem& fs) {
    if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <ASN.1 binary file>");
    const MathLib::Array<uint8_t> certificate = fs.Open(MathLib::String(argv[1]), MathLib::OpenMode::Read).ReadAll();
    std::cout << argv[1] << std::endl;
    ParseDER(certificate);
}