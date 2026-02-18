DefinitionBlock("", "DSDT", 1, "OEMID ", "TABLEID ", 0x00000000) {
    Scope(_SB) {
        Device(PCI0) {
            Name(_HID, EisaId("PNP0A03"))
        }
        Device(RTC) {
            Name(_HID, EisaId("PNP0B00"))
            Name(_CRS, ResourceTemplate() {
                IO(Decode16, 0x0070, 0x0070, 0x01, 0x08)
                IRQNoFlags() { 8 }
            })
            OperationRegion(CMS0, SystemCMOS, Zero, 0x40)
            Field(CMS0, ByteAcc, NoLock, Preserve) {
                RTSE, 8,
                Offset(0x02),
                RTMN, 8,
                Offset(0x04),
                RTHR, 8,
                Offset(0x06),
                RTDY, 8,
                RTDE, 8,
            }
        }
        Device(PS2K) {
            Name(_HID, EisaId("PNP0303"))
            // Method(_STA, 0, NotSerialized) {
            //     Return(0x0f)
            // }
            Name(_CRS, ResourceTemplate() {
                IO(Decode16, 0x0060, 0x0060, 0x01, 0x01)
                IO(Decode16, 0x0064, 0x0064, 0x01, 0x01)
                IRQ(Edge, ActiveHigh, Exclusive) { 1 }
            })
            Name(_PRS, ResourceTemplate() {
                StartDependentFn(0x00, 0x00) {
                    FixedIO(0x0060, 0x01)
                    FixedIO(0x0064, 0x01)
                    IRQNoFlags() { 1 }
                }
                EndDependentFn()
            })
            Name(_PRW, Package(0x02) {
                0x18,
                0x03,
            })
        }
    }
}