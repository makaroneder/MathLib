#include "X86State.hpp"

X86State::X86State(X86Flags flags, Register ip) : flags(flags), ip(ip), a(0), b(0), c(0), d(0), si(0), di(0), sp(0), bp(0), r8(0), r9(0), r10(0), r11(0), r12(0), r13(0), r14(0), r15(0) {}
String X86State::ToString(const String& padding) const {
    String ret = flags.carry ? "CF" : "";
    ret += flags.parity ? (String(ret.IsEmpty() ? "" : ", ") + "PF") : "";
    ret += flags.auxiliaryCarry ? (String(ret.IsEmpty() ? "" : ", ") + "AF") : "";
    ret += flags.zero ? (String(ret.IsEmpty() ? "" : ", ") + "ZF") : "";
    ret += flags.sign ? (String(ret.IsEmpty() ? "" : ", ") + "SF") : "";
    ret += flags.trap ? (String(ret.IsEmpty() ? "" : ", ") + "TF") : "";
    ret += flags.interruptEnable ? (String(ret.IsEmpty() ? "" : ", ") + "IF") : "";
    ret += flags.direction ? (String(ret.IsEmpty() ? "" : ", ") + "DF") : "";
    ret += flags.overflow ? (String(ret.IsEmpty() ? "" : ", ") + "OF") : "";
    ret += flags.resume ? (String(ret.IsEmpty() ? "" : ", ") + "RF") : "";
    ret += flags.virtual8086 ? (String(ret.IsEmpty() ? "" : ", ") + "VM") : "";
    ret += flags.alignmentCheck ? (String(ret.IsEmpty() ? "" : ", ") + "AC") : "";
    ret += flags.virtualInterrupt ? (String(ret.IsEmpty() ? "" : ", ") + "VIF") : "";
    ret += flags.virtualInterruptPending ? (String(ret.IsEmpty() ? "" : ", ") + "VIP") : "";
    ret += flags.id ? (String(ret.IsEmpty() ? "" : ", ") + "ID") : "";
    ret = padding + "{\n" + padding + "\tFlags: " + (ret.IsEmpty() ? "none" : ret) + '\n';
    ret += padding + "\tIP = " + ip.ToString() + '\n';
    ret += padding + "\tCS = " + cs.ToString() + '\n';
    ret += padding + "\tDS = " + ds.ToString() + '\n';
    ret += padding + "\tES = " + es.ToString() + '\n';
    ret += padding + "\tFS = " + fs.ToString() + '\n';
    ret += padding + "\tGS = " + gs.ToString() + '\n';
    ret += padding + "\tSS = " + ss.ToString() + '\n';
    ret += padding + "\tSP = " + sp.ToString() + '\n';
    ret += padding + "\tBP = " + bp.ToString() + '\n';
    ret += padding + "\tA = " + a.ToString() + '\n';
    ret += padding + "\tB = " + b.ToString() + '\n';
    ret += padding + "\tC = " + c.ToString() + '\n';
    ret += padding + "\tD = " + d.ToString() + '\n';
    ret += padding + "\tSI = " + si.ToString() + '\n';
    ret += padding + "\tDI = " + di.ToString() + '\n';
    ret += padding + "\tR8 = " + r8.ToString() + '\n';
    ret += padding + "\tR9 = " + r9.ToString() + '\n';
    ret += padding + "\tR10 = " + r10.ToString() + '\n';
    ret += padding + "\tR11 = " + r11.ToString() + '\n';
    ret += padding + "\tR12 = " + r12.ToString() + '\n';
    ret += padding + "\tR13 = " + r13.ToString() + '\n';
    ret += padding + "\tR14 = " + r14.ToString() + '\n';
    ret += padding + "\tR15 = " + r15.ToString() + '\n';
    return ret + padding + '}';
}