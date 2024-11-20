
# Assignment 3: Shared Memory Programming with Pthreads and OpenMP

## Overview

In this assignment, you will practice parallel programming for shared memory systems. The assignment has two parts:
1. Parallelizing the trapezoidal rule using Pthreads.
2. Parallelizing a histogram equalization algorithm using OpenMP.

---

## Part 1: Shared Memory Parallelism with Pthreads
### Task
- Parallelize the trapezoidal rule implementation in `trap.c` using Pthreads.

### File Provided
- `trap.c` (Serial implementation of the trapezoidal rule)

---

## Part 2: Shared Memory Parallelism with OpenMP
### Task
- Parallelize a histogram equalization algorithm provided in `histogram_equalization.c` using OpenMP.

### Files and Resources
- `histogram_equalization.tgz`: Contains:
  - Serial implementation (`histogram_equalization.c`)
  - Example images for testing.

### Steps
1. **Unpack the Archive**  
   Compile and run the serial program to observe its behavior.
   ```bash
   cc -DDEBUG histogram_equalization.c -lm -o heq
   ./heq bridge.png
   ```
   Output will be written to `output.png`.

2. **Understand the Code**  
   Review the serial implementation to understand its structure.

3. **Parallelize with OpenMP**  
   - Add OpenMP pragmas and functions.
   - Avoid race conditions.
   - Ensure correctness.

4. **Run on DARWIN Cluster**  
   - Start an interactive session with 4 cores.
   - Run your program, log output with `script output.txt`, and retrieve the log file.

5. **Analyze Performance**  
   Discuss reasons for sub-optimal speedup and suggest improvements for better parallel efficiency.

---

## Submission Instructions
- Submit the following files on Gradescope:
  - `pth_trap.c` (Pthreads implementation)
  - `omp_histogram_equalization.c` (OpenMP implementation)
  - `output.txt` (DARWIN log file)
  - Performance analysis discussion document.

### Notes
- You do not need to submit header files or output images.
- The TA will run your code to verify correctness.

---

## Grading

### Pthreads Program
- **Compilation**: 10 points
- **Parallel Execution**: 20 points
- **Efficiency > 0.6 with up to 4 threads**: 15 points

### OpenMP Program
- **Compilation**: 10 points
- **Parallel Execution**: 20 points
- **Performance Analysis**: 15 points
- **DARWIN Output**: 10 points

---

## Tips and Reminders
- Use `scp` or `rsync` to retrieve files from UNIX systems.
- Visit office hours for assistance if needed.

Good luck!
