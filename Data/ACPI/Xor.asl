DefinitionBlock("", "DSDT", 1, "OEMID ", "TABLEID ", 0x00000000) {
    Scope(_SB) {
        Device(DEV1) {
            Name(_HID, EisaId("PNP0A00"))
            Name(MASK, 0)
            Method(FUN1, 1, NotSerialized) {
                Local0 = Arg0
                Local0 ^= 0xff
                Return(Local0 ^ ^MASK)
            }
            Method(FUN2, 1, NotSerialized) {
                Return(Arg0 ^ 0xff ^ ^MASK)
            }
        }
    }
}