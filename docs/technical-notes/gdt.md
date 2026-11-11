# Global Descriptor Table (GDT)

## 1. Overview

The Global Descriptor Table (GDT) is a data structure used by x86 processors to define segment descriptors.

In x86-64 long mode, segmentation is mostly disabled or limited, but the GDT remains important for defining code and data segment descriptors and configuring the processor's segment registers.

## 2. Main Components

The initial Gaming OS GDT will contain three descriptors:

| Index | Descriptor | Purpose |
|---|---|---|
| 0 | Null Descriptor | Required null entry |
| 1 | Kernel Code Segment | Defines the kernel code segment |
| 2 | Kernel Data Segment | Defines the kernel data segment |

## 3. GDT Registers

The GDTR register stores:

- Base address of the GDT.
- Limit of the GDT (table size minus one).

The `LGDT` instruction loads the GDT register with a pointer to the new table.

## 4. Initial GDT Design

The planned initial GDT contains:

- Null descriptor.
- 64-bit kernel code descriptor.
- Kernel data descriptor.

The code and data descriptors will use a flat memory model.

## 5. Implementation Plan

1. Define the GDT entry structure.
2. Create the initial GDT table.
3. Define the GDTR pointer structure.
4. Implement the GDT loading routine.
5. Reload the required segment registers.
6. Verify the configuration in the QEMU environment.

## 6. Testing

The implementation will be tested after the kernel build and boot process are available.

Testing will verify that:

- The GDT is loaded successfully.
- The expected segment selectors are configured.
- Kernel execution continues after GDT initialization.

**Status:** Research and initial design completed. Implementation pending.