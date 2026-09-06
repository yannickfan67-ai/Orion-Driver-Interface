#ifndef ORION_DRIVER_INTERFACE_H
#define ORION_DRIVER_INTERFACE_H

#include <stdint.h>
#include <stddef.h>

#define ODI_ABI_MAJOR 1
#define ODI_ABI_MINOR 0

#define ODI_OK 0
#define ODI_ENODEV (-1)
#define ODI_EINVAL (-2)
#define ODI_ENOMEM (-3)
#define ODI_EIO (-4)
#define ODI_EBUSY (-5)
#define ODI_ENOTSUP (-6)

typedef enum {
    ODI_BUS_NONE = 0,
    ODI_BUS_PCI = 1,
    ODI_BUS_PLATFORM = 2,
    ODI_BUS_VIRTIO_PCI = 3
} odi_bus_type;

typedef enum {
    ODI_CLASS_UNKNOWN = 0,
    ODI_CLASS_BUS = 1,
    ODI_CLASS_NETWORK = 2,
    ODI_CLASS_BLOCK = 3,
    ODI_CLASS_DISPLAY = 4,
    ODI_CLASS_INPUT = 5,
    ODI_CLASS_AUDIO = 6,
    ODI_CLASS_RNG = 7,
    ODI_CLASS_PLATFORM = 8
} odi_driver_class;

typedef struct {
    uint64_t base;
    uint64_t size;
    uint32_t flags;
    uint32_t reserved;
} odi_bar;

#define ODI_BAR_IO       (1u << 0)
#define ODI_BAR_MMIO     (1u << 1)
#define ODI_BAR_PREFETCH (1u << 2)
#define ODI_BAR_64BIT    (1u << 3)

typedef struct {
    uint8_t bus;
    uint8_t device;
    uint8_t function;
    uint8_t revision;
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t class_code;
    uint8_t subclass;
    uint8_t prog_if;
    uint8_t irq_line;
    odi_bar bars[6];
} odi_pci_identity;

typedef struct {
    uint32_t bus_type;
    uint32_t reserved;
    union {
        odi_pci_identity pci;
        struct {
            const char *name;
            uint64_t resource_base;
            uint64_t resource_size;
        } platform;
    } id;
} odi_device;

typedef struct {
    void *cpu_addr;
    uint64_t device_addr;
    uint64_t size;
} odi_dma_buffer;

typedef int (*odi_irq_handler)(void *context);

typedef struct odi_kernel_api {
    uint16_t abi_major;
    uint16_t abi_minor;
    uint32_t struct_size;

    void (*log)(uint32_t level, const char *message);
    uint64_t (*time_ns)(void);

    int (*alloc_pages)(uint64_t page_count, uint64_t max_phys, void **cpu_addr, uint64_t *phys_addr);
    void (*free_pages)(void *cpu_addr, uint64_t page_count);

    void *(*map_mmio)(uint64_t phys_addr, uint64_t size, uint32_t flags);
    void (*unmap_mmio)(void *virt_addr, uint64_t size);

    uint8_t  (*pio_read8)(uint16_t port);
    uint16_t (*pio_read16)(uint16_t port);
    uint32_t (*pio_read32)(uint16_t port);
    void (*pio_write8)(uint16_t port, uint8_t value);
    void (*pio_write16)(uint16_t port, uint16_t value);
    void (*pio_write32)(uint16_t port, uint32_t value);

    uint32_t (*pci_read32)(uint8_t bus, uint8_t dev, uint8_t fn, uint8_t offset);
    void (*pci_write32)(uint8_t bus, uint8_t dev, uint8_t fn, uint8_t offset, uint32_t value);

    int (*irq_register)(uint32_t irq, odi_irq_handler handler, void *context);
    void (*irq_unregister)(uint32_t irq, odi_irq_handler handler, void *context);
    void (*irq_mask)(uint32_t irq);
    void (*irq_unmask)(uint32_t irq);

    int (*dma_alloc)(uint64_t size, uint64_t align, uint64_t dma_mask, odi_dma_buffer *out);
    void (*dma_free)(odi_dma_buffer *buffer);
} odi_kernel_api;

typedef struct odi_driver_descriptor {
    uint32_t struct_size;
    uint16_t abi_major;
    uint16_t abi_minor;
    uint32_t driver_class;
    uint32_t flags;
    const char *name;
    const char *version;

    int (*probe)(const odi_kernel_api *api, const odi_device *device);
    int (*attach)(const odi_kernel_api *api, const odi_device *device, void **driver_context);
    int (*start)(const odi_kernel_api *api, void *driver_context);
    void (*stop)(const odi_kernel_api *api, void *driver_context);
    void (*detach)(const odi_kernel_api *api, void *driver_context);
} odi_driver_descriptor;

#define ODI_DRIVER_ENTRY_SYMBOL odi_driver_entry
const odi_driver_descriptor *odi_driver_entry(void);

#endif
