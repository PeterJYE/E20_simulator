# E20 Cache Simulator

This project is a C++ simulator for the E20 processor with cache simulation support. It loads an E20 machine code file, executes the program, and prints cache access information for `lw` and `sw` instructions.

## Features

- Simulates E20 machine code execution
- Supports basic E20 instructions such as:
  - `add`
  - `sub`
  - `or`
  - `and`
  - `slt`
  - `jr`
  - `slti`
  - `lw`
  - `sw`
  - `addi`
  - `jal`
  - `jeq`
- Supports one-level cache simulation
- Supports two-level cache simulation
- Tracks cache hits and misses for load instructions
- Tracks store operations
- Uses LRU-style replacement inside each cache row

## File Structure

```text
E20_simulator.cpp
