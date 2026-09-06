#ifndef ORION_DRIVER_SERVICES_H
#define ORION_DRIVER_SERVICES_H

#include <stdint.h>
#include <stddef.h>

#define ODI_SERVICE_ABI_MAJOR 1u
#define ODI_SERVICE_ABI_MINOR 0u
#define ODI_SERVICE_ABI_VERSION ((ODI_SERVICE_ABI_MAJOR << 16) | ODI_SERVICE_ABI_MINOR)

/* Every published service begins with this prefix. */
typedef struct odi_service_header {
    uint32_t abi_version;
    uint32_t struct_size;
    uint64_t capabilities;
} odi_service_header;

/* Network */
typedef void (*odi_net_rx_handler)(void *context,const void *frame,size_t length);
typedef struct odi_net_service {
    odi_service_header header;
    int (*get_mac)(void *context,uint8_t mac[6]);
    uint32_t (*get_mtu)(void *context);
    int (*transmit)(void *context,const void *frame,size_t length);
    int (*set_rx_handler)(void *context,odi_net_rx_handler handler,void *handler_context);
    void *reserved[4];
} odi_net_service;

/* Block storage */
typedef struct odi_block_service {
    odi_service_header header;
    uint32_t (*sector_size)(void *context);
    uint64_t (*sector_count)(void *context);
    int (*read)(void *context,uint64_t lba,uint32_t sectors,void *buffer,size_t buffer_size);
    int (*write)(void *context,uint64_t lba,uint32_t sectors,const void *buffer,size_t buffer_size);
    int (*flush)(void *context);
    void *reserved[4];
} odi_block_service;

/* Display */
#define ODI_PIXEL_XRGB8888 1u
#define ODI_PIXEL_BGRX8888 2u
#define ODI_PIXEL_RGB565   3u
typedef struct odi_display_mode {
    uint32_t width,height,stride_pixels,pixel_format;
} odi_display_mode;
typedef struct odi_display_service {
    odi_service_header header;
    int (*get_mode)(void *context,odi_display_mode *mode);
    int (*set_mode)(void *context,uint32_t width,uint32_t height,uint32_t preferred_format);
    void *(*map_framebuffer)(void *context,size_t *byte_size);
    void (*unmap_framebuffer)(void *context,void *address,size_t byte_size);
    int (*present)(void *context,uint32_t x,uint32_t y,uint32_t width,uint32_t height);
    void *reserved[4];
} odi_display_service;

/* Input */
typedef enum {
    ODI_INPUT_KEY=1,
    ODI_INPUT_POINTER=2,
    ODI_INPUT_WHEEL=3,
    ODI_INPUT_TOUCH=4
} odi_input_event_type;
typedef struct odi_input_event {
    uint32_t type;
    uint32_t code;
    int32_t value0,value1,value2;
    uint64_t timestamp_ns;
} odi_input_event;
typedef void (*odi_input_handler)(void *context,const odi_input_event *event);
typedef struct odi_input_service {
    odi_service_header header;
    int (*set_handler)(void *context,odi_input_handler handler,void *handler_context);
    void *reserved[6];
} odi_input_service;

/* Audio: interleaved little-endian PCM is the ABI-1 baseline. */
typedef struct odi_audio_format {
    uint32_t sample_rate;
    uint16_t channels;
    uint16_t bits_per_sample;
} odi_audio_format;
typedef struct odi_audio_service {
    odi_service_header header;
    int (*set_format)(void *context,const odi_audio_format *format);
    int (*write_pcm)(void *context,const void *samples,size_t byte_count);
    uint32_t (*buffered_frames)(void *context);
    void *reserved[5];
} odi_audio_service;

#endif
