# ODI ABI stabilization roadmap

This repository is a technical specification. It does not contain product UI or hypervisor code.

## ABI 1.x work

- define driver probe/bind/unbind lifecycle precisely
- standard error/status code namespace
- device IDs and bus-match records
- IRQ registration, masking, acknowledgement and teardown rules
- MMIO/PIO mapping lifetime and memory-order requirements
- DMA allocation/mapping, address-width and cache-coherency rules
- device power/reset/suspend/resume lifecycle
- concurrency/locking requirements for driver callbacks
- capability negotiation between kernel and driver
- architecture-neutral scalar/alignment rules for x86_64 and AArch64

## `.odrv` package work

- manifest schema and canonical encoding
- dependency and minimum-kernel fields
- payload/section alignment
- cryptographic signatures and trust chain metadata
- reproducible package hash rules
- resource/firmware payloads
- forward-compatible optional sections

## Compatibility policy

- ABI major changes may break binary compatibility
- ABI minor changes append capabilities/callbacks only
- unknown optional fields must be ignored where safe
- kernel must reject packages requiring unsupported ABI majors or mandatory capabilities
- every normative structure gets compile-time size/alignment tests
