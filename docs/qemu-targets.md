# QEMU reference hardware for ODI

These devices are the initial ODI reference targets.

## Tier 0 — driver bring-up

### QEMU EDU (`1234:11e8`)

Command line: `-device edu`

Why it is useful:
- 1 MiB PCI MMIO BAR
- simple read/write liveness register
- factorial operation
- INTx/MSI interrupt path
- DMA controller

ODI reference tests should at minimum:
1. enumerate the PCI function
2. map BAR0
3. read the ID register at `0x00`
4. write a value to `0x04` and verify that the device returns its bitwise inverse
5. optionally exercise factorial, IRQ and DMA

Important EDU registers used by the reference driver:
- `0x00` identification
- `0x04` liveness inversion
- `0x08` factorial data
- `0x20` status
- `0x24` interrupt status
- `0x60` interrupt raise
- `0x64` interrupt acknowledge
- `0x80/0x88/0x90/0x98` DMA source/destination/count/command

### QEMU fw_cfg

On x86/x86_64 the historical PIO interface uses:
- selector: `0x510`
- data: `0x511`
- DMA address: `0x514`

Selecting key `0x0000` and reading four data bytes must return `QEMU`. This is the first platform/PIO reference driver because it can be tested without PCI or a complex device model.

## Tier 1 — common virtual hardware

### VirtIO PCI

VirtIO PCI uses vendor ID `0x1AF4`. ODI should implement the PCI transport once and reuse it across devices.

Priority device IDs:
- 1 network
- 2 block
- 3 console
- 4 entropy/RNG
- 16 GPU
- 18 input
- 25 sound
- 26 filesystem

Initial implementation order: transport -> RNG -> block -> net -> input -> GPU.

## Tier 2 — compatibility hardware

Useful QEMU compatibility targets after the ODI core is stable:
- RTL8139 (existing in-tree network driver)
- Intel e1000/e1000e
- AHCI/SATA
- NVMe
- standard VGA / Bochs VBE
- ICH9 HDA
- USB HID

These are intentionally behind VirtIO because each requires a separate hardware-specific driver rather than sharing a common transport.

## Source references

Normative external references:
- QEMU EDU device documentation: https://www.qemu.org/docs/master/specs/edu.html
- QEMU fw_cfg documentation: https://www.qemu.org/docs/master/specs/fw_cfg.html
- OASIS VirtIO 1.3 specification: https://docs.oasis-open.org/virtio/virtio/v1.3/virtio-v1.3.html
