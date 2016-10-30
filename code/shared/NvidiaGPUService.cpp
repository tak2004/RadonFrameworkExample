#include "RadonFramework/precompiled.hpp"
#include "RadonFramework/Collections/BitArray.hpp"
#include "NvidiaGPUService.hpp"
#include "nvapi.h"

namespace RadonFramework { namespace System { namespace Diagnostics { namespace Profiling { 

class NvidiaGPUService::GPUInfo
{
public:
    RF_Collect::Array<RF_Type::Float32> m_Values;
    RF_Collect::Array<RF_Type::Float32> m_MaxValues;
    RF_Collect::Array<RF_Type::Float32> m_MinValues;
    RF_Collect::Array<RF_Type::Bool> m_AvailableValues;
    NvPhysicalGpuHandle m_AdapterId;
};

NvidiaGPUService::NvidiaGPUService(const RF_Type::String& Name)
:GPUService(Name)
{
}

NvidiaGPUService::~NvidiaGPUService()
{
//    NvAPI_Unload();
}

void NvidiaGPUService::ObtainGPUs()
{
    NvPhysicalGpuHandle hPhysicalGpu[NVAPI_MAX_PHYSICAL_GPUS];
    NvU32 physicalGpuCount = 0;
    NvAPI_Status ret = NvAPI_EnumPhysicalGPUs(hPhysicalGpu, &physicalGpuCount);
    m_GPUs.New(physicalGpuCount);
    for (NvU32 i = 0; i < physicalGpuCount; ++i)
    {
        m_GPUs[i].m_AvailableValues.Resize(9);
        for(auto j = 0; j < m_GPUs[i].m_AvailableValues.Count(); ++j)
        {
            m_GPUs[i].m_AvailableValues(j) = false;
        }
        m_GPUs[i].m_MaxValues.Resize(9);
        m_GPUs[i].m_MinValues.Resize(9);
        m_GPUs[i].m_Values.Resize(9);
        m_GPUs[i].m_AdapterId = hPhysicalGpu[i];        

        m_GPUs[i].m_AvailableValues(GPUService::FanSpeedPercentage) = false;
        m_GPUs[i].m_MaxValues(GPUService::FanSpeedPercentage) = 1;
        m_GPUs[i].m_MinValues(GPUService::FanSpeedPercentage) = 0;
                    
        NvU32 tach = 0;
        if(NvAPI_GPU_GetTachReading(m_GPUs[i].m_AdapterId, &tach) == NVAPI_OK)
        {
            m_GPUs[i].m_AvailableValues(GPUService::FanSpeedRPM) = true;
            m_GPUs[i].m_MaxValues(GPUService::FanSpeedRPM) = 1;
            m_GPUs[i].m_MinValues(GPUService::FanSpeedRPM) = 0;
        }

        NV_GPU_THERMAL_SETTINGS thermal = {0};
        thermal.version = NV_GPU_THERMAL_SETTINGS_VER;
        thermal.sensor[0].target = NVAPI_THERMAL_TARGET_GPU;
        thermal.sensor[0].controller = NVAPI_THERMAL_CONTROLLER_GPU_INTERNAL;
        if(NvAPI_GPU_GetThermalSettings(m_GPUs[i].m_AdapterId, NVAPI_THERMAL_TARGET_NONE, &thermal) == NVAPI_OK)
        {
            m_GPUs[i].m_AvailableValues(GPUService::GPUTemperature) = true;
            m_GPUs[i].m_MaxValues(GPUService::GPUTemperature) = thermal.sensor[0].defaultMaxTemp;
            m_GPUs[i].m_MinValues(GPUService::GPUTemperature) = thermal.sensor[0].defaultMinTemp;
        }

        NvU32 lanes;
        if(NvAPI_GPU_GetCurrentPCIEDownstreamWidth(m_GPUs[i].m_AdapterId, &lanes) == NVAPI_OK)
        {
            m_GPUs[i].m_AvailableValues(GPUService::Lanes) = true;
            m_GPUs[i].m_MaxValues(GPUService::Lanes) = 16;
            m_GPUs[i].m_MinValues(GPUService::Lanes) = 0;
            m_GPUs[i].m_Values(GPUService::Lanes) = lanes;
        }
        
        NV_GPU_CLOCK_FREQUENCIES_V2 clkFreq = {0};
        clkFreq.ClockType = NV_GPU_CLOCK_FREQUENCIES_CURRENT_FREQ;
        clkFreq.version = NV_GPU_CLOCK_FREQUENCIES_VER;
        if(NvAPI_GPU_GetAllClockFrequencies(m_GPUs[i].m_AdapterId, &clkFreq) == NVAPI_OK)
        {
            m_GPUs[i].m_AvailableValues(GPUService::MemoryClock) = clkFreq.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].bIsPresent;
            m_GPUs[i].m_MaxValues(GPUService::MemoryClock) = 1;
            m_GPUs[i].m_MinValues(GPUService::MemoryClock) = 0;

            m_GPUs[i].m_AvailableValues(GPUService::CoreClock) = clkFreq.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].bIsPresent;
            m_GPUs[i].m_MaxValues(GPUService::CoreClock) = 1;
            m_GPUs[i].m_MinValues(GPUService::CoreClock) = 0;
        }

        NV_GPU_DYNAMIC_PSTATES_INFO_EX states = {0};
        states.version = NV_GPU_DYNAMIC_PSTATES_INFO_EX_VER;
        if(NvAPI_GPU_GetDynamicPstatesInfoEx(m_GPUs[i].m_AdapterId, &states) == NVAPI_OK)
        {
            m_GPUs[i].m_AvailableValues(GPUService::GPUActivity) = states.utilization[0].bIsPresent;
            m_GPUs[i].m_MaxValues(GPUService::GPUActivity) = 1;
            m_GPUs[i].m_MinValues(GPUService::GPUActivity) = 0;
        }

        m_GPUs[i].m_AvailableValues(GPUService::BusSpeed) = false;
        m_GPUs[i].m_MaxValues(GPUService::BusSpeed) = 1;
        m_GPUs[i].m_MinValues(GPUService::BusSpeed) = 0;
    }
}

RF_Type::Size NvidiaGPUService::GPUCount() const
{
    return m_GPUs.Count();
}

const RF_Collect::Array<RF_Type::Float32>& NvidiaGPUService::CurrentValues(const RF_Type::Size GPUIndex) const
{
    if(GPUIndex < m_GPUs.Count() && m_GPUs.Count() > 0)
    {
        return m_GPUs[GPUIndex].m_Values;
    }
    else
    {
        return m_EmptyValues;
    }
}

const RF_Collect::Array<RF_Type::Float32>& NvidiaGPUService::MaxValues(const RF_Type::Size GPUIndex) const
{
    if(GPUIndex < m_GPUs.Count() && m_GPUs.Count() > 0)
    {
        return m_GPUs[GPUIndex].m_MaxValues;
    }
    else
    {
        return m_EmptyValues;
    }
}

const RF_Collect::Array<RF_Type::Float32>& NvidiaGPUService::MinValues(const RF_Type::Size GPUIndex) const
{
    if(GPUIndex < m_GPUs.Count() && m_GPUs.Count() > 0)
    {
        return m_GPUs[GPUIndex].m_MinValues;
    }
    else
    {
        return m_EmptyValues;
    }
}

const RF_Collect::Array<RF_Type::Bool>& NvidiaGPUService::AvailableValues(const RF_Type::Size GPUIndex) const
{
    if(m_GPUs.Count() > 0 && GPUIndex < m_GPUs.Count())
    {
        return m_GPUs[GPUIndex].m_AvailableValues;
    }
    else
    {
        return m_EmptyAvailableValues;
    }
}

void NvidiaGPUService::Update(const RF_Type::Size GPUIndex)
{
    if(m_GPUs.Count() > 0 && GPUIndex < m_GPUs.Count())
    {
        if(m_GPUs[GPUIndex].m_AvailableValues(GPUService::FanSpeedRPM))
        {
            NvU32 tach = 0;
            if(NvAPI_GPU_GetTachReading(m_GPUs[GPUIndex].m_AdapterId, &tach) == NVAPI_OK)
            {
                m_GPUs[GPUIndex].m_Values(GPUService::FanSpeedRPM) = tach;
            }
        }

        if(m_GPUs[GPUIndex].m_AvailableValues(GPUService::GPUTemperature))
        {
            NV_GPU_THERMAL_SETTINGS thermal = {0};
            thermal.version = NV_GPU_THERMAL_SETTINGS_VER;
            thermal.sensor[0].target = NVAPI_THERMAL_TARGET_GPU;
            thermal.sensor[0].controller = NVAPI_THERMAL_CONTROLLER_GPU_INTERNAL;
            if(NvAPI_GPU_GetThermalSettings(m_GPUs[GPUIndex].m_AdapterId, NVAPI_THERMAL_TARGET_NONE, &thermal) == NVAPI_OK)
            {
                m_GPUs[GPUIndex].m_Values(GPUService::GPUTemperature) = thermal.sensor[0].currentTemp;
            }
        }

        if(m_GPUs[GPUIndex].m_AvailableValues(GPUService::Lanes))
        {
            NvU32 lanes;
            if(NvAPI_GPU_GetCurrentPCIEDownstreamWidth(m_GPUs[GPUIndex].m_AdapterId, &lanes) == NVAPI_OK)
            {
                m_GPUs[GPUIndex].m_Values(GPUService::Lanes) = lanes;
            }
        }

        if(m_GPUs[GPUIndex].m_AvailableValues(GPUService::MemoryClock))
        {
            NV_GPU_CLOCK_FREQUENCIES_V2 clkFreq = {0};
            clkFreq.ClockType = NV_GPU_CLOCK_FREQUENCIES_CURRENT_FREQ;
            clkFreq.version = NV_GPU_CLOCK_FREQUENCIES_VER;
            if(NvAPI_GPU_GetAllClockFrequencies(m_GPUs[GPUIndex].m_AdapterId, &clkFreq) == NVAPI_OK)
            {
                m_GPUs[GPUIndex].m_Values(GPUService::MemoryClock) = clkFreq.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency / 1000;
            }
        }

        if(m_GPUs[GPUIndex].m_AvailableValues(GPUService::CoreClock))
        {
            NV_GPU_CLOCK_FREQUENCIES_V2 clkFreq = {0};
            clkFreq.ClockType = NV_GPU_CLOCK_FREQUENCIES_CURRENT_FREQ;
            clkFreq.version = NV_GPU_CLOCK_FREQUENCIES_VER;
            if(NvAPI_GPU_GetAllClockFrequencies(m_GPUs[GPUIndex].m_AdapterId, &clkFreq) == NVAPI_OK)
            {
                m_GPUs[GPUIndex].m_Values(GPUService::CoreClock) = clkFreq.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency/1000;
            }
        }

        if(m_GPUs[GPUIndex].m_AvailableValues(GPUService::GPUActivity))
        {
            NV_GPU_DYNAMIC_PSTATES_INFO_EX states = {0};
            states.version = NV_GPU_DYNAMIC_PSTATES_INFO_EX_VER;
            if(NvAPI_GPU_GetDynamicPstatesInfoEx(m_GPUs[GPUIndex].m_AdapterId, &states) == NVAPI_OK)
            {
                m_GPUs[GPUIndex].m_Values(GPUService::GPUActivity) = states.utilization[0].percentage / 100.0f;
            }
        }
    }
}

RF_Type::Bool NvidiaGPUService::IsAvailable()
{   
    NvAPI_Status ret = NVAPI_OK;
    ret = NvAPI_Initialize();
    return ret == NVAPI_OK;
}

} } } }