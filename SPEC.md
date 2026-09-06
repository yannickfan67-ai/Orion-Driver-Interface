# Orion Driver Interface 0.2

Status: draft, ABI **1.1**.

## 1. Goals

ODI separates hardware drivers from the UN_Orion kernel core. A driver must not directly depend on private kernel symbols. It receives an `odi_kernel_api` table and exposes one `odi_driver_descriptor`.

ODI is designed to survive both hardware expansion and architecture ports. A driver source may target legacy PC hardware in 86Box, modern PCI/VirtIO devices in QEMU, and platform devices on future ARM64/RISC-V Orion ports without inventing a second driver ABI.

## 2. Version negotiation

`ODI_ABI_MAJOR` is breaking. `ODI_ABI_MINOR` is additive.

Compatibility rules:
- major versions must match;
- a kernel minor must be >= the driver's required minor;
- structures are append-only within a major ABI;
- both sides must honor `struct_size` before reading appended fields;
- optional functionality is advertised with capability bits;
- unknown capability bits must be ignored;
- a driver missing an optional capability must fail gracefully with `ODI_ENOTSUP`, not touch unavailable hardware services.

`ODI_API_HAS()` and `ODI_DRIVER_HAS()` are the canonical prefix-size probes.

## 3. Architecture registry

Runtime architecture IDs:
- `ODI_ARCH_I686`
- `ODI_ARCH_X86_64`
- `ODI_ARCH_AARCH64`
- `ODI_ARCH_RISCV64`

`odi_kernel_api.architecture` identifies the running kernel. A 1.1 driver may declare `supported_architectures` as a bit mask. Portable source should isolate architecture-specific barriers, endian helpers and interrupt primitives behind small local helpers.

The `.odrv` package has its own stable architecture registry. Existing v1 values are never renumbered: x86_64=1 and AArch64=2 remain fixed; i686=3 and RISC-V64=4 are additive.

## 4. Driver lifecycle

1. package validation
2. ABI validation
3. architecture/capability validation
4. match/probe
5. attach
6. start
7. normal I/O / IRQ callbacks
8. stop
9. detach

`probe` must not permanently change hardware state. `attach` may allocate resources. `start` enables normal device operation.

## 5. Device identity and buses

ODI 1.1 buses include:
- PCI
- PCIe
- platform
- VirtIO PCI
- ISA
- USB
- ACPI-described devices

PCI records expose BDF, vendor/device ID, class/subclass/prog-if, revision and BARs.

ISA records expose I/O base/size, IRQ, 8/16-bit DMA channels and optional PnP identity. This is specifically intended to make legacy-PC coverage testable under 86Box without hard-coding addresses inside the Orion kernel.

## 6. Kernel services

Drivers request services only through `odi_kernel_api`:
- logging
- monotonic time
- physical page allocation/free
- MMIO map/unmap
- PIO read/write
- PCI config read/write
- IRQ registration/mask/unmask
- DMA allocation and address translation

The 1.1 capability word advertises whether PIO/MMIO/PCI/IRQ/DMA/ISA/USB/ACPI/MSI/DMA64 services exist. No driver may assume identity-mapped physical memory or availability of x86-only PIO on non-x86 systems.

## 7. Interrupt model

Drivers register an IRQ callback through the kernel API. The callback returns whether the interrupt was handled. The kernel owns interrupt-controller acknowledgement; device-specific acknowledgement remains the driver's responsibility.

The base ABI deliberately does not expose PIC/APIC/GIC details. Those stay behind the kernel IRQ service so the same device driver can survive interrupt-controller changes.

## 8. DMA model

DMA memory must be allocated through ODI. Drivers declare an address mask/alignment and receive both CPU virtual and device-visible addresses. This prevents drivers from assuming arbitrary pointers are DMA-capable and leaves room for IOMMU support.

## 9. Driver classes

Base class IDs include bus, network, block, display, input, audio, RNG, platform, USB and firmware. Class-specific higher-level service interfaces are versioned separately from the base lifecycle ABI.

## 10. `.odrv` package

`.odrv` is the distributable Orion driver package. Version 1 keeps a fixed **128-byte header**, followed by a UTF-8 manifest, native payload and optional resources.

Required properties:
- magic `ODRV`
- format version
- required ODI ABI
- package architecture
- payload kind
- offsets/sizes for manifest, payload and resources
- CRC32 corruption check

CRC32 is not a security signature. Signing can be added using reserved/header extensions without changing the v1 fixed prefix.

Payload kinds now reserve ELF64 and ELF32, allowing i686 packaging while retaining x86_64/AArch64/RISC-V64 ELF64.

## 11. Native loading status

ODK can already validate/package native driver payloads. Dynamic `.odrv` loading is not yet complete in UN_Orion 0.0.5; reference drivers can therefore be built in-tree for kernel testing while retaining the same ODI entry descriptor.

## 12. Compatibility target policy

Reference testing is split into two hardware groups:

- **modern virtual hardware**: QEMU EDU, fw_cfg, VirtIO, RTL8139 and later NVMe/AHCI;
- **PC compatibility hardware**: 86Box ISA/PCI systems covering NE2000-class NICs, legacy IDE/ATAPI, VGA/SVGA, PS/2/serial and Sound Blaster-class audio.

The test emulator is not part of the driver ABI. A driver passes because it communicates with the documented/emulated device through ODI, not because it calls emulator-specific APIs.
