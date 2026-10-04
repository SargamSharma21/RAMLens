# RAMLens

RAMLens is a **C-based Memory Management Simulator** that demonstrates how dynamic memory allocation strategies work in an operating-system environment.

The simulator represents memory as a linked list of blocks and supports multiple allocation strategies, memory deallocation, coalescing of free blocks, memory statistics, predefined workloads, and performance benchmarking.

## Features

- Dynamic memory block simulation
- Memory allocation and deallocation
- Four allocation strategies:
  - First Fit
  - Best Fit
  - Worst Fit
  - Next Fit
- Memory map visualization
- Memory statistics
- External fragmentation calculation
- Automatic coalescing of adjacent free blocks
- Duplicate process detection
- Predefined workloads:
  - Basic
  - Fragmentation
  - Stress
- Allocation strategy benchmarking
- CLI-based interaction
- Memory reset functionality

## Project Structure

```text
RAMLens/
│
├── include/
│   ├── allocator.h
│   ├── benchmark.h
│   ├── cli.h
│   ├── memory.h
│   └── workload.h
│
├── src/
│   ├── allocator.c
│   ├── benchmark.c
│   ├── cli.c
│   ├── main.c
│   ├── memory.c
│   └── workload.c
│
├── .gitignore
├── Makefile
├── LICENSE
└── README.md
```

## Requirements

- GCC
- GNU Make
- C11-compatible compiler
- Linux/macOS environment

## Build

Clone the repository and navigate to the project directory.

Compile the project using:

```bash
make
```

This creates the executable:

```text
ramlens
```

To remove the compiled executable:

```bash
make clean
```

## Run

Start RAMLens with:

```bash
./ramlens
```

You will see:

```text
RAMLens - Memory Management Simulator

RAMLens >
```

## CLI Commands

### Allocate Memory

```text
alloc <process> <size>
```

Example:

```text
RAMLens > alloc Chrome 200
```

### Free Memory

```text
free <process>
```

Example:

```text
RAMLens > free Chrome
```

### View Memory Map

```text
map
```

Example output:

```text
========== MEMORY MAP ==========
Start: 0 KB | Size: 200 KB | USED | Process: Chrome
Start: 200 KB | Size: 824 KB | FREE
================================
```

### View Statistics

```text
stats
```

Displays:

- Total memory
- Used memory
- Free memory
- Allocated blocks
- Free blocks
- Largest free block
- Memory utilization
- External fragmentation

### Change Allocation Strategy

```text
strategy <name>
```

Available strategies:

```text
strategy first
strategy best
strategy worst
strategy next
```

### Run a Workload

```text
workload <type>
```

Available workloads:

```text
workload basic
workload fragmentation
workload stress
```

### Benchmark a Workload

```text
benchmark <type>
```

Examples:

```text
benchmark basic
benchmark fragmentation
benchmark stress
```

The benchmark compares the supported allocation strategies based on memory usage, free memory, largest free block, fragmentation, and execution time.

### Reset Memory

```text
reset
```

Restores the simulator to its initial memory state.

### Help

```text
help
```

Displays the available commands.

### Exit

```text
exit
```

Exits RAMLens.

## Allocation Strategies

### First Fit

Searches memory from the beginning and allocates the requested memory in the first suitable free block.

### Best Fit

Searches all available blocks and chooses the smallest block that can satisfy the allocation.

### Worst Fit

Searches all available blocks and chooses the largest suitable free block.

### Next Fit

Continues searching from the position of the previous allocation instead of starting from the beginning each time.

## Memory Management

RAMLens represents memory using a linked list of blocks.

Each block stores:

- Starting address
- Block size
- Free/used state
- Process name
- Pointer to the next block

When a free block is larger than the requested allocation, the block is split into:

```text
Allocated Block + Remaining Free Block
```

When adjacent free blocks are created through deallocation, RAMLens coalesces them into a single larger free block.

## Fragmentation

RAMLens calculates external fragmentation using:

```text
External Fragmentation =
(1 - Largest Free Block / Total Free Memory) × 100
```

This helps demonstrate how memory can become fragmented even when sufficient total free memory exists.

## Example

```text
RAMLens > alloc A 100

Allocation successful!

RAMLens > alloc B 200

Allocation successful!

RAMLens > free A

Deallocation successful!

RAMLens > stats

========== MEMORY STATISTICS ==========
Total Memory           : 1024 KB
Used Memory            : 200 KB
Free Memory            : 824 KB
Allocated Blocks       : 1
Free Blocks            : 2
Largest Free Block     : 724 KB
Utilization            : 19.53%
External Fragmentation : 12.14%
========================================
```

## Educational Purpose

RAMLens is designed to provide a practical demonstration of **operating-system memory management concepts**, including:

- Contiguous memory allocation
- Allocation strategies
- Memory fragmentation
- Memory utilization
- Block splitting
- Block coalescing
- Allocation performance

The project provides a command-line environment where these concepts can be observed through actual allocation and deallocation operations.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.