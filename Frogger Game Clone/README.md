# 🐸 Frogger Game Clone 

##  Project Overview
This project is a faithful reproduction of the classic 1981 arcade game **Frogger**, developed entirely in **C** for Linux environments.

Beyond recreating the gameplay, the primary goal of this project was to explore and implement advanced **System Programming** concepts. The game engine was developed in **two distinct versions** to compare different concurrency models:
1.  **Multi-Process Version:** Based on `fork()` and IPC via Pipes.
2.  **Multi-Threaded Version:** Based on `pthread`, Shared Memory, and Mutex synchronization.

## Technical Architecture

### 1. The Multi-Process Approach (`fork` & `pipe`)
* **Architecture:** Every game entity (Frog, Vehicles, Logs, Bullets) is a separate process spawned via `fork()`.
* **Communication (IPC):** The game engine (Parent Process) communicates with entities (Child Processes) using **unnamed pipes**.
* **Non-Blocking I/O:** Pipes are configured as `O_NONBLOCK` to ensure the rendering engine runs smoothly without waiting for slower processes.
* **Signal Handling:** Proper handling of `kill` signals to prevent zombie processes upon exit.

### 2. The Multi-Threaded Approach (`pthreads`)
* **Architecture:** Entities run as lightweight threads within the same address space.
* **Synchronization:** Implemented **Fine-Grained Locking** using `pthread_mutex` arrays (one mutex per entity) to prevent race conditions while maximizing parallelism.
* **Resource Management:** Dynamic memory allocation for shared structures and thread arguments.

### 3. Graphics & Audio
* **Graphics:** Uses the **ncurses** library for efficient TUI (Text User Interface) rendering, window management, and color handling.
* **Audio:** Integrated **SDL2_mixer** to handle background music and sound effects (jump, collision, time-out) asynchronously.

## 🛠️Installation & Dependencies

To run this project, you need a Linux environment with the GCC compiler and the required libraries.

##Credits:
Developed by <br>
**Francesco Pili** - <br>
**Nicola Floris** - <br>
for *Operating Systems and Network Programming* in the **University of Cagliari**.

