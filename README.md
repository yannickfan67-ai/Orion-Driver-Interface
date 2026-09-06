# Orion Driver Interface (ODI)

ODI is the stable driver contract for UN_Orion.

Current specification: **ODI 0.1 / ABI 1**.

The project defines:
- driver lifecycle and kernel/driver ABI negotiation
- PCI/platform/virtio device matching
- driver classes (network, block, display, input, audio, bus, RNG, platform)
- MMIO/PIO/DMA/IRQ service contracts
- the `.odrv` package container
- compatibility and versioning rules

The first reference targets are QEMU EDU, QEMU fw_cfg and VirtIO PCI. These targets are intentionally chosen because they cover MMIO, PIO, IRQ, DMA and standardized virtqueues without depending on undocumented hardware behavior.

This repository is the canonical specification. Implementations in `UN_Orion` should consume ODI rather than inventing driver-private kernel APIs.

## Repository layout

- `SPEC.md` — normative ODI ABI specification
- `include/odi.h` — C ABI declarations
- `include/odrv.h` — `.odrv` binary container definitions
- `docs/qemu-targets.md` — emulated hardware selected for reference drivers

Tooling, templates and example driver projects live in the separate `Orion-Driver-Kit` repository.
