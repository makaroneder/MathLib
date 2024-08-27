#!/bin/bash

if [ "$#" -le 3 ]; then
    echo "Usage: $0 <output> <symbol file> <breakpoint> <emulator command>"
    exit 1
fi
cat > $1 << EOF
set disassembly-flavor intel
symbol-file $2
break *$3
target remote | $4 -S -gdb stdio
continue
EOF