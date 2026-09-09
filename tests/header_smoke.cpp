#include <cstddef>
#include <cstdint>
#include "odi.h"
#include "odi_services.h"
#include "odrv.h"

static_assert(sizeof(odrv_header_v1)==128,"ODRV ABI size");
static_assert(ODI_ABI_MAJOR==1,"ODI ABI major");
static_assert(ODI_SERVICE_ABI_VERSION==(1u<<16),"service ABI version");

int main(){
    odi_driver_descriptor d{};
    odi_kernel_api api{};
    odi_net_service net{};
    const odi_driver_descriptor *(*entry)(void)=&odi_driver_entry;
    (void)d;(void)api;(void)net;(void)entry;
    return 0;
}
