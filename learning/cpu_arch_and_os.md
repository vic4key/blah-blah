# OPERATING SYSTEM & CPU ARCHITECTURES



## CPU Instruction Sets

All CPU instruction sets are the following:
  - AMD Instruction Sets:
    - AMD64 (64-bit): A 64-bit extension of the x86 instruction set. Introduced in 2003.
  - Intel & AMD Instruction Sets:
    - x86 (32-bit): Intel x86 (i386) instruction set. Introduced in 1978.
    - x86-64 (64-bit): Intel x64 or IA-32e. A 64-bit extension of the x86 instruction set. Introduced in 2005.
    - IA-32: Also known as Intel x86 (i386). An older 32-bit instruction set used in older CPUs.
    - IA-64: Also known as Intel Itanium. A 64-bit extension of the x86 instruction set.
  - ARM (Advanced RISC Machine) Instruction Sets:
    - ARM (32-bit): A 32-bit ARM instruction set.
    - ARM64 (64-bit): A 64-bit extension of the ARM instruction set (from ARMv8).
  - MIPS: The MIPS instruction set.
  - PowerPC: The PowerPC instruction set.



## CPU Architectures

All CPU architectures are the following:
  - x86 (32-bit): Commonly used in desktops and laptops.
  - x86-64/AMD64 (64-bit): Introduced in 2003 (from Intel's Pentium 4). Extends x86 instruction set. Commonly used in desktops and laptops.
  - ARM   (32-bit): Commonly used in embedded systems, mobile devices, etc.
  - ARM64 (64-bit): Introduced in 2011 (from ARMv8). Commonly used in embedded systems, mobile devices, etc.
  - MIPS  (32-bit and 64-bit): Commonly used in older router devices and some embedded systems.
  - PowerPC (32-bit and 64-bit): Commonly used in older Apple computers and some non-Apple systems.
  - RISC-V  (32-bit and 64-bit): Relatively new open standard that has gained popularity for use in embedded systems and some servers.
  - Apple CPU architectures:
    - Apple M1    (ARM64-based): Introduced in 2020 (from macOS Big Sur). Commonly used in Mac and iPad.
    - Apple A4-14 (ARM32-based): Commonly used in iPhone.
    - Apple A15   (ARM64-based): Introduced in 2020 (from iPad mini & iPad Air 4). Commonly used in iPad.

*Note: Linux kernel supported all these CPU architectures.



## Operating System

Windows (Acer, Dell, HP, etc)
  - x86   (32-bit)
  - x86-64/AMD64 (64-bit): Since 2003 (from Intel's Pentium 4)
  - ARM   (from Windows RT)
  - ARM64 (from Windows 10 on ARM64-based)

Android (Samsung, LG, etc)
  - ARM   (Primarily)
  - ARM64 (Primarily, from ARMv8)
  - MIPS  (Some old phone. Less common today. Almost No)
  - x86   (Less common today. Eg. Nokia, etc)

iOS (Unix-based - Apple: iPhone, iPad, iPod)
  - ARM64 (Transition from ARM32 to ARM64 from 2013+ that iPhone 5s or later)

MacOS (Unix-based - Apple: Mac)
  - PowerPC       (32-bit): Before to 2006
  - x86-64/AMD64  (64-bit): Since 2006~2020
  - Apple M1      (ARM64-based): Since 2020 and later (from macOS Big Sur)

Unix
  - x86 (32-bit)
  - x86-64/AMD64 (64-bit)
  - ARM   (32-bit)
  - ARM64 (64-bit)
  - PowerPC   (32-bit)
  - PowerPC64 (64-bit PowerPC)
  - SPARC
  - MIPS
  - PA-RISC
  - DEC Alpha
  - Motorola 68000
  - HP PA-RISC
  - IA-64
  - RISC-V



## Executable File Format

PE file format supports the following CPU architectures:
  - x86 (32-bit)
  - x86-64/AMD64 (64-bit)
  - IA-64 (Itanium)
  - ARM   (32-bit)
  - ARM64 (64-bit)
  - MIPS
  - Alpha
  - PowerPC

ELF file format supports the following CPU architectures:
  - ARM   (32-bit)
  - ARM64 (64-bit)
  - x86   (32-bit)
  - x86-64/AMD64 (64-bit)
  - MIPS
  - RISC-V

Mach-O file format supports the following CPU architectures:
  - ARM   (32-bit)
  - ARM64 (64-bit)
  - x86   (32-bit)
  - x86-64/AMD64 (64-bit)
  - PowerPC   (32-bit)
  - PowerPC64 (64-bit PowerPC)



## Others

The terms "CPU types" and "CPU architectures" are often used interchangeably, but they do have slightly different meanings.
  - CPU types also known as instruction set architectures (ISAs), are the specific set of instructions and operations that a CPU can understand and execute. For example, Intel and AMD processors have different ISAs.
  - CPU architectures refer to the overall design of a CPU, including the organization of its components and how they work together. This includes details such as the number of cores, cache memory, and how data is processed and moved between components.

The x86_64 & AMD64 are not the same one:
  - AMD64 was implemented by AMD and Introduced in 2003.
  - x86_64 was implemented by Intel and Introduced in 2005.
  - It's worth noting that AMD's and Intel's implementation of the x86-64 architecture are mostly compatible, allowing programs and operating systems designed for one to run on the other. However, there are some differences in the way the two architectures handle certain aspects, such as hardware virtualization and cache coherency.
