# csce311p2

# RISC-V OS Project README

## Original Objectives

From the assignment:

* Create a working command-line operating system for a RISC‑V machine.
* Support loading separate programs.
* Support running multiple programs simultaneously.
* Add synchronization mechanisms.
* Include protection mechanisms.
* Optional/advanced: add a file system.
* Support creating/loading new programs.
* Run in QEMU using the `virt` platform.
* Document work thoroughly, including logging all AI chats.

## Chat Log

* User: "option A is good; but can you log our chat in the readme as we go? like update it each time?"
* Assistant: Created README canvas and confirmed logging.
* User: "update the readme and yes option A pls!"
* Assistant: Updated the canvas README and asked if repo files should be printed all in one message.
* User: "all files in one message"
* Assistant: Provided the full repository content in a single message.
* User: "can you resend the files with documentation explaining what each section does? update the log too. Also, can you include at the beginning of the readme what I said the original objectives of the command line are?"
* Assistant: Updated README with objectives and sent fully documented files.
* User: "i should make two files src and user at the same level as the makefile and readme, yes? update the ghat log"
* Assistant: Confirmed directory layout and added repo-structure instructions.
* User: "update log"
* Assistant: Updated the chat log.
* User: "can you explain the specific functions of each file and how they work tg? update log"
* Assistant: Provided a file-by-file explanation and described how the files interact to form the OS.
* User: "update log with that info as well"
* Assistant: Added detailed architecture explanation to the log.

---

## Overview

This repository contains a minimal educational RISC-V operating system designed to run on QEMU's `virt` machine. It includes:

* Boot code
* Console I/O (SBI)
* Cooperative threading scheduler
* Tiny in-RAM filesystem
* Simple shell
* Demo "user" programs (linked into kernel)

---

## Build Instructions

You need:

* `riscv64-unknown-elf-gcc` toolchain
* `qemu-system-riscv64`
* `make`

To build:

```
make
```

To run:

```
make run
```

or manually:

```
qemu-system-riscv64 -machine virt -nographic -m 256M -kernel build/kernel.elf
```

---

## Features Implemented

* Boot entry & stack setup
* SBI console
* Cooperative multitasking
* Context switching via assembly stub
* Tiny in-RAM filesystem parser
* Simple shell with primitive commands
* Demo user programs as additional threads

---

## Next Steps

* Preemptive scheduling via timer interrupt
* User-mode + virtual memory
* ELF loader for real user programs
* Better filesystem
* Protection, resource limits
