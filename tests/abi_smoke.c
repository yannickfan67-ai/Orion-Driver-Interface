#include <stddef.h>
#include <stdint.h>
#include "odi.h"
#include "odi_services.h"
#include "odrv.h"

typedef struct {
    uint16_t abi_major,abi_minor;
    uint32_t struct_size;
    void (*log)(uint32_t,const char*);
} odi_kernel_api_v1_prefix;

static int check_struct_size_guards(void){
    odi_kernel_api api={0};
    odi_driver_descriptor d={0};
    api.struct_size=(uint32_t)offsetof(odi_kernel_api,capabilities);
    d.struct_size=(uint32_t)offsetof(odi_driver_descriptor,required_kernel_capabilities);
    if(ODI_API_HAS(&api,capabilities))return 1;
    if(ODI_DRIVER_HAS(&d,required_kernel_capabilities))return 2;
    api.struct_size=(uint32_t)sizeof(api);
    d.struct_size=(uint32_t)sizeof(d);
    if(!ODI_API_HAS(&api,capabilities)||!ODI_API_HAS(&api,service_publish))return 3;
    if(!ODI_DRIVER_HAS(&d,required_kernel_capabilities)||!ODI_DRIVER_HAS(&d,supported_architectures))return 4;
    return 0;
}

int main(void){
    if(sizeof(odrv_header_v1)!=128)return 1;
    if(ODRV_MAGIC!=0x5652444Fu||ODRV_FORMAT_VERSION!=1)return 2;
    if(ODRV_ARCH_X86_64!=1||ODRV_ARCH_AARCH64!=2||ODRV_ARCH_I686!=3||ODRV_ARCH_RISCV64!=4)return 3;
    if(ODI_ARCH_I686!=1||ODI_ARCH_X86_64!=2||ODI_ARCH_AARCH64!=3||ODI_ARCH_RISCV64!=4)return 4;
    if(ODI_ARCH_X86_64==ODRV_ARCH_X86_64)return 5; /* namespaces are intentionally independent */
    if(offsetof(odi_kernel_api,log)!=offsetof(odi_kernel_api_v1_prefix,log))return 6;
    if(ODI_SERVICE_ABI_MAJOR!=1u||ODI_SERVICE_ABI_MINOR!=0u)return 7;
    if(sizeof(odi_service_header)!=16)return 8;
    if(sizeof(odi_display_mode)!=16)return 9;
    if(check_struct_size_guards())return 10;
    return 0;
}
