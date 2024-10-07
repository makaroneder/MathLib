#include "Dialog.hpp"
#include <String.hpp>

Dialog::Dialog(const String& text, const Array<DialogOption>& options) : text(Split(text, "\n", false)), options(options) {}