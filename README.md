# IBI
Interactive Brainfuck Interpreter

# Implementation details

  Current implementation is based on the "simple brainfuck interpreter" by Daniel B. Christofani
  http://www.hevanet.com/cristofd/brainfuck/sbi.c 

  - Tape size 65536 cells
  - Stepping off the cell bounds causes undefined behavior
  - Cell size 2 bytes (16 bits)
  - Cell value is wrapping around
  - Stack based loops (to speed up runtime execution)
  - EOF leaves the cell value unchanged
  - New line on I/O is 10 ('\n')
  - extra "#" command prints debug info

# Usage

  - linux: ./ibi or ./ibi [filename]
  - windows: ibi.exe or ibi.exe [filename]

# Build

  - linux: gcc ibi.c -o ibi
  - windows: gcc.exe ibi.c -o ibi

