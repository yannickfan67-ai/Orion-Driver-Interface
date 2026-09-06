#ifndef ORION_DRIVER_PACKAGE_H
#define ORION_DRIVER_PACKAGE_H

#include <stdint.h>

#define ODRV_MAGIC 0x5652444Fu /* 'ODRV' little-endian */
#define ODRV_FORMAT_VERSION 1

/*
 * ODRV architecture IDs are package-format IDs and remain stable forever.
 * They are intentionally independent from ODI_ARCH_* runtime IDs.
 */
#define ODRV_ARCH_X86_64 1
#define ODRV_ARCH_AARCH64 2
#define ODRV_ARCH_I686 3
#define ODRV_ARCH_RISCV64 4

#define ODRV_PAYLOAD_ELF64 1
#define ODRV_PAYLOAD_ELF32 2

#pragma pack(push,1)
typedef struct {
    uint32_t magic;
    uint16_t format_version;
    uint16_t header_size;
    uint16_t odi_abi_major;
    uint16_t odi_abi_minor;
    uint16_t arch;
    uint16_t payload_kind;
    uint32_t flags;
    uint32_t manifest_offset;
    uint32_t manifest_size;
    uint32_t payload_offset;
    uint32_t payload_size;
    uint32_t resources_offset;
    uint32_t resources_size;
    uint32_t crc32;
    uint8_t reserved[80];
} odrv_header_v1;
#pragma pack(pop)

_Static_assert(sizeof(odrv_header_v1) == 128, "ODRV v1 header must be 128 bytes");

#endif
