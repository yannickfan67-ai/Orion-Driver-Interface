# Orion Driver Interface 0.1

Status: draft, ABI major 1.

## 1. Goals

ODI separates hardware drivers from the UN_Orion kernel core. A driver must not directly depend on private kernel symbols. It receives an `odi_kernel_api` table and exposes one `odi_driver_descriptor`.

## 2. Version negotiation

`ODI_ABI_MAJOR` is breaking. `ODI_ABI_MINOR` is additive. A kernel may load a driver when major versions match and the kernel minor version is greater than or equal to the driver's required minor version.

## 3. Driver lifecycle

1. package validation
2. ABI validation
3. match/probe
4. attach
5. start
6. normal I/O / IRQ callbacks
7. stop
8. detach

`probe` must not permanently change hardware state. `attach` may allocate resources. `start` enables normal device operation.

## 4. Device identity

ODI buses initially include:
- PCI
- platform
- VirtIO PCI

A PCI device record exposes BDF, vendor/device ID, class/subclass/prog-if, revision and up to six BAR descriptors.

## 5. Kernel services

Drivers request services only through `odi_kernel_api`:
- logging
- monotonic time
- physical page allocation/free
- MMIO map/unmap
- PIO read/write
- PCI config read/write
- IRQ registration/mask/unmask
- DMA allocation and address translation

No driver may assume identity-mapped physical memory.

## 6. Interrupt model

Drivers register an IRQ callback through the kernel API. The callback returns whether the interrupt was handled. The kernel owns interrupt-controller acknowledgement; a device-specific acknowledge operation remains the driver's responsibility.

## 7. DMA model

DMA memory must be allocated through ODI. Drivers declare an address mask/alignment and receive both CPU virtual and device-visible physical addresses. This prevents drivers from assuming that arbitrary kernel pointers are DMA-capable.

## 8. Driver classes

Initial class IDs:
- `ODI_CLASS_BUS`
- `ODI_CLASS_NETWORK`
- `ODI_CLASS_BLOCK`
- `ODI_CLASS_DISPLAY`
- `ODI_CLASS_INPUT`
- `ODI_CLASS_AUDIO`
- `ODI_CLASS_RNG`
- `ODI_CLASS_PLATFORM`

Class-specific higher-level interfaces are versioned separately from the base driver ABI.

## 9. `.odrv` package

`.odrv` is the distributable Orion driver package. Version 1 contains a fixed header followed by a UTF-8 manifest, native payload and optional resources.

Required properties:
- magic `ODRV`
- format version
- ODI ABI requirement
- architecture
- payload kind
- offsets/sizes for manifest, payload and resources
- CRC32 for accidental corruption detection

CRC32 is not a security signature. A later signing extension will add cryptographic authenticity without changing the v1 fixed prefix.

## 10. Native payload

The first native target is x86_64. The planned loader accepts a position-independent ELF64 driver image and resolves only the ODI entry symbol. Dynamic loading is not yet implemented in UN_Orion 0.0.5, so ODK currently validates and packages drivers while reference drivers may also be built in-tree for kernel testing.

## 11. Stability

ODI implementation code may evolve rapidly before 1.0, but any ABI-major change must increment `ODI_ABI_MAJOR`. Driver packages must never depend on struct padding not explicitly defined by the standard.
