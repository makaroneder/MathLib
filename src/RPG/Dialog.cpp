#include "Dialog.hpp"
#include <String.hpp>

Dialog::Dialog(const MathLib::String& text, const MathLib::Array<DialogOption>& options) : text(Split(text, "\n", false)), options(options) {}