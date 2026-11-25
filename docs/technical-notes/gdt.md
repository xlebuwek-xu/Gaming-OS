# Global Descriptor Table (GDT)

## 1. Overview

The Global Descriptor Table (GDT) is a data structure used by x86 processors to define segment descriptors.

In x86-64 long mode, segmentation is mostly disabled or limited, but the GDT remains important for defining code and data segment descriptors and configuring the processor's segment registers.

## 2. GDT Structure

The Gaming OS GDT currently contains three descriptors:

| Index | Descriptor | Selector | Purpose |
|---|---|---|---|
| 0 | Null Descriptor | 0x00 | Required null entry |
| 1 | Kernel Code Segment | 0x08 | Defines the 64-bit kernel code segment |
| 2 | Kernel Data Segment | 0x10 | Defines the kernel data segment |

Each descriptor occupies 8 bytes, so the complete GDT occupies 24 bytes.

## 3. GDT Registers

The GDTR register stores:

- Base address of the GDT.
- Limit of the GDT (table size minus one).

The `LGDT` instruction loads GDTR with the address and limit of the new table.

The `SGDT` instruction is used to read the current GDTR value for verification.

## 4. Implementation

The GDT implementation consists of:

- `kernel/include/gdt.h` — GDT interface and diagnostic state structure.
- `kernel/arch/x86_64/gdt.c` — GDT descriptors, GDTR pointer, initialization and state-reading functions.
- `kernel/arch/x86_64/gdt_load.S` — Assembly routine that loads the GDT and reloads segment registers.
- `kernel/core/kernel.c` — Calls GDT initialization and prints diagnostic information through the serial port.

The GDT contains:

- Null descriptor: `0x0000000000000000`
- 64-bit kernel code descriptor: `0x00AF9A000000FFFF`
- Kernel data descriptor: `0x00CF92000000FFFF`

The code selector is `0x08`, and the data selector is `0x10`.

After loading GDTR, the assembly routine reloads DS, ES and SS. It also reloads CS using a far return.

## 5. Testing

The implementation was built using the cross-compiler and tested by booting the kernel in QEMU.

The kernel successfully loaded the custom GDT and continued execution.

### QEMU verification results

| Parameter | Observed value |
|---|---|
| GDTR Base | `0xFFFFFFFF80002000` |
| GDTR Limit | `0x0000000000000017` |
| CS | `0x0000000000000008` |
| DS | `0x0000000000000010` |
| ES | `0x0000000000000010` |
| SS | `0x0000000000000010` |

The GDTR limit of `0x17` corresponds to a 24-byte GDT (24 - 1).

The segment selectors match the expected code and data descriptor indexes.

The final kernel message confirms that execution continued after GDT initialization:

`Gaming OS: first boot milestone reached!`

## 6. Current Limitations

- The GDT currently contains only basic kernel code and data descriptors.
- User-mode segments and privilege-level transitions are not implemented.
- Task State Segment (TSS) support is not implemented.

## 7. Status

**Status: Implemented, loaded and successfully tested in QEMU.**
