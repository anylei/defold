#include <stdint.h>
#include "sound.h"
#include "sound2.h"

namespace dmDeviceOpenNull
{
    dmSound::Result DeviceNullOpen(const dmSound::OpenDeviceParams* params, dmSound::HDevice* device)
    {
        return dmSound::RESULT_OK;
    }

    void DeviceNullClose(dmSound::HDevice device)
    {
    }

    dmSound::Result DeviceNullQueue(dmSound::HDevice device, const int16_t* samples, uint32_t sample_count)
    {
        return dmSound::RESULT_OK;
    }

    uint32_t DeviceNullFreeBufferSlots(dmSound::HDevice device)
    {
        return 0;
    }

    void DeviceNullDeviceInfo(dmSound::HDevice device, dmSound::DeviceInfo* info)
    {
    }

    DM_DECLARE_SOUND_DEVICE(NullSoundDevice, "null", DeviceNullOpen, DeviceNullClose, DeviceNullQueue, DeviceNullFreeBufferSlots, DeviceNullDeviceInfo);
}

