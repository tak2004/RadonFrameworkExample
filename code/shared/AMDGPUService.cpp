#include "RadonFramework/precompiled.hpp"
#include "RadonFramework/Collections/BitArray.hpp"
#include "AMDGPUService.hpp"
#include "adl_sdk.h"

#define AMDVENDORID             (1002)
typedef int(*ADL_MAIN_CONTROL_CREATE)(ADL_MAIN_MALLOC_CALLBACK, int);
typedef int(*ADL_MAIN_CONTROL_DESTROY)();
typedef int(*ADL_ADAPTER_NUMBEROFADAPTERS_GET) (int*);
typedef int(*ADL_ADAPTER_ADAPTERINFO_GET) (LPAdapterInfo, int);
typedef int(*ADL_ADAPTER_ACTIVE_GET) (int, int*);
typedef int(*ADL_OVERDRIVE_CAPS) (int iAdapterIndex, int *iSupported, int *iEnabled, int *iVersion);
//Overdrive5
typedef int(*ADL_OVERDRIVE5_THERMALDEVICES_ENUM) (int iAdapterIndex, int iThermalControllerIndex, ADLThermalControllerInfo *lpThermalControllerInfo);
typedef int(*ADL_OVERDRIVE5_ODPARAMETERS_GET) (int  iAdapterIndex, ADLODParameters *  lpOdParameters);
typedef int(*ADL_OVERDRIVE5_TEMPERATURE_GET) (int iAdapterIndex, int iThermalControllerIndex, ADLTemperature *lpTemperature);
typedef int(*ADL_OVERDRIVE5_FANSPEED_GET) (int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedValue *lpFanSpeedValue);
typedef int(*ADL_OVERDRIVE5_FANSPEEDINFO_GET) (int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedInfo *lpFanSpeedInfo);
typedef int(*ADL_OVERDRIVE5_ODPERFORMANCELEVELS_GET) (int iAdapterIndex, int iDefault, ADLODPerformanceLevels *lpOdPerformanceLevels);
typedef int(*ADL_OVERDRIVE5_ODPARAMETERS_GET) (int iAdapterIndex, ADLODParameters *lpOdParameters);
typedef int(*ADL_OVERDRIVE5_CURRENTACTIVITY_GET) (int iAdapterIndex, ADLPMActivity *lpActivity);
typedef int(*ADL_OVERDRIVE5_POWERCONTROL_CAPS)(int iAdapterIndex, int *lpSupported);
typedef int(*ADL_OVERDRIVE5_POWERCONTROLINFO_GET)(int iAdapterIndex, ADLPowerControlInfo *lpPowerControlInfo);
typedef int(*ADL_OVERDRIVE5_POWERCONTROL_GET)(int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);
//Overdrive6
typedef int(*ADL_OVERDRIVE6_FANSPEED_GET)(int iAdapterIndex, ADLOD6FanSpeedInfo *lpFanSpeedInfo);
typedef int(*ADL_OVERDRIVE6_THERMALCONTROLLER_CAPS)(int iAdapterIndex, ADLOD6ThermalControllerCaps *lpThermalControllerCaps);
typedef int(*ADL_OVERDRIVE6_TEMPERATURE_GET)(int iAdapterIndex, int *lpTemperature);
typedef int(*ADL_OVERDRIVE6_CAPABILITIES_GET) (int iAdapterIndex, ADLOD6Capabilities *lpODCapabilities);
typedef int(*ADL_OVERDRIVE6_STATEINFO_GET)(int iAdapterIndex, int iStateType, ADLOD6StateInfo *lpStateInfo);
typedef int(*ADL_OVERDRIVE6_CURRENTSTATUS_GET)(int iAdapterIndex, ADLOD6CurrentStatus *lpCurrentStatus);
typedef int(*ADL_OVERDRIVE6_POWERCONTROL_CAPS) (int iAdapterIndex, int *lpSupported);
typedef int(*ADL_OVERDRIVE6_POWERCONTROLINFO_GET)(int iAdapterIndex, ADLOD6PowerControlInfo *lpPowerControlInfo);
typedef int(*ADL_OVERDRIVE6_POWERCONTROL_GET)(int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);

namespace RadonFramework { namespace System { namespace Diagnostics { namespace Profiling { 

ADL_MAIN_CONTROL_CREATE					ADL_Main_Control_Create;
ADL_MAIN_CONTROL_DESTROY				ADL_Main_Control_Destroy;
ADL_ADAPTER_NUMBEROFADAPTERS_GET		ADL_Adapter_NumberOfAdapters_Get;
ADL_ADAPTER_ADAPTERINFO_GET				ADL_Adapter_AdapterInfo_Get;
ADL_ADAPTER_ACTIVE_GET					ADL_Adapter_Active_Get;
ADL_OVERDRIVE_CAPS						ADL_Overdrive_Caps;
//Overdrive5
ADL_OVERDRIVE5_THERMALDEVICES_ENUM		ADL_Overdrive5_ThermalDevices_Enum;
ADL_OVERDRIVE5_ODPARAMETERS_GET			ADL_Overdrive5_ODParameters_Get;
ADL_OVERDRIVE5_TEMPERATURE_GET			ADL_Overdrive5_Temperature_Get;
ADL_OVERDRIVE5_FANSPEED_GET				ADL_Overdrive5_FanSpeed_Get;
ADL_OVERDRIVE5_FANSPEEDINFO_GET			ADL_Overdrive5_FanSpeedInfo_Get;
ADL_OVERDRIVE5_ODPERFORMANCELEVELS_GET	ADL_Overdrive5_ODPerformanceLevels_Get;
ADL_OVERDRIVE5_CURRENTACTIVITY_GET		ADL_Overdrive5_CurrentActivity_Get;
ADL_OVERDRIVE5_POWERCONTROL_CAPS		ADL_Overdrive5_PowerControl_Caps;
ADL_OVERDRIVE5_POWERCONTROLINFO_GET		ADL_Overdrive5_PowerControlInfo_Get;
ADL_OVERDRIVE5_POWERCONTROL_GET			ADL_Overdrive5_PowerControl_Get;
//Overdrive6
ADL_OVERDRIVE6_FANSPEED_GET ADL_Overdrive6_FanSpeed_Get;
ADL_OVERDRIVE6_THERMALCONTROLLER_CAPS ADL_Overdrive6_ThermalController_Caps;
ADL_OVERDRIVE6_TEMPERATURE_GET ADL_Overdrive6_Temperature_Get;
ADL_OVERDRIVE6_CAPABILITIES_GET ADL_Overdrive6_Capabilities_Get;
ADL_OVERDRIVE6_STATEINFO_GET ADL_Overdrive6_StateInfo_Get;
ADL_OVERDRIVE6_CURRENTSTATUS_GET ADL_Overdrive6_CurrentStatus_Get;
ADL_OVERDRIVE6_POWERCONTROL_CAPS ADL_Overdrive6_PowerControl_Caps;
ADL_OVERDRIVE6_POWERCONTROLINFO_GET ADL_Overdrive6_PowerControlInfo_Get;
ADL_OVERDRIVE6_POWERCONTROL_GET ADL_Overdrive6_PowerControl_Get;

class AMDGPUService::GPUInfo
{
public:
    RF_Collect::Array<RF_Type::Float32> m_Values;
    RF_Collect::Array<RF_Type::Float32> m_MaxValues;
    RF_Collect::Array<RF_Type::Float32> m_MinValues;
    RF_Collect::Array<RF_Type::Bool> m_AvailableValues;
    RF_Type::Int32 m_AdapterIndex;
    RF_Type::Bool m_IsOverdrive6;
};

// Memory allocation function
void* __stdcall ADL_Main_Memory_Alloc(int iSize)
{
    void* lpBuffer = malloc(iSize);
    return lpBuffer;
}

// Optional Memory de-allocation function
void __stdcall ADL_Main_Memory_Free(void** lpBuffer)
{
    if(NULL != *lpBuffer)
    {
        free(*lpBuffer);
        *lpBuffer = NULL;
    }
}

AMDGPUService::AMDGPUService(const RF_Type::String& Name)
:GPUService(Name)
{
}

AMDGPUService::~AMDGPUService()
{
    if(ADL_Main_Control_Destroy)
    {
        ADL_Main_Control_Destroy();
    }
}

void AMDGPUService::ObtainGPUs()
{
    if (!m_Library)
    {
        m_Library = RF_Sys::DynamicLibrary::LoadSystemLibrary("atiadlxx.dll"_rfs);
        if(!m_Library)
        {
            m_Library = RF_Sys::DynamicLibrary::LoadSystemLibrary("atiadlxy.dll"_rfs);
        }        
        ADL_Main_Control_Create = (ADL_MAIN_CONTROL_CREATE)m_Library->GetFunctionAddress("ADL_Main_Control_Create"_rfs);
        ADL_Main_Control_Destroy = (ADL_MAIN_CONTROL_DESTROY)m_Library->GetFunctionAddress("ADL_Main_Control_Destroy"_rfs);
        ADL_Adapter_NumberOfAdapters_Get = (ADL_ADAPTER_NUMBEROFADAPTERS_GET)m_Library->GetFunctionAddress("ADL_Adapter_NumberOfAdapters_Get"_rfs);
        ADL_Adapter_AdapterInfo_Get = (ADL_ADAPTER_ADAPTERINFO_GET)m_Library->GetFunctionAddress("ADL_Adapter_AdapterInfo_Get"_rfs);
        ADL_Adapter_Active_Get = (ADL_ADAPTER_ACTIVE_GET)m_Library->GetFunctionAddress("ADL_Adapter_Active_Get"_rfs);
        ADL_Overdrive_Caps = (ADL_OVERDRIVE_CAPS)m_Library->GetFunctionAddress("ADL_Overdrive_Caps"_rfs);
    }

    if(ADL_OK != ADL_Main_Control_Create(ADL_Main_Memory_Alloc, 1))
    {
        return;
    }

    int  adapterCount = 0;
    if(ADL_OK != ADL_Adapter_NumberOfAdapters_Get(&adapterCount))
    {
        return;
    }

    if(0 == adapterCount)
    {
        return;
    }

    RF_Collect::BitArray<> useGPU(adapterCount);
    useGPU.Reset();    
    RF_Mem::AutoPointerArray<AdapterInfo> adapters(adapterCount);
    RF_SysMem::Set(adapters.Get(), 0, adapters.Size());
    ADL_Adapter_AdapterInfo_Get(adapters.Get(), adapters.Size());

    int  iOverdriveSupported = 0;
    int  iOverdriveEnabled = 0;
    int	 iOverdriveVersion = 0;
    for(auto i = 0; i < adapterCount; i++)
    {
        int adapterActive = 0;
        ADL_Adapter_Active_Get(adapters[i].iAdapterIndex, &adapterActive);
        iOverdriveSupported = 0;
        if(adapterActive && adapters[i].iVendorID == AMDVENDORID)
        {
            if(ADL_OK != ADL_Overdrive_Caps(adapters[i].iAdapterIndex,
                &iOverdriveSupported, &iOverdriveEnabled, &iOverdriveVersion))
            {
                continue;
            }

            if(iOverdriveSupported)
            {
                useGPU.Set(i);
            }
        }
    }

    auto usedAdapters = useGPU.GetSetIndices();
    m_GPUs.New(usedAdapters.Count());
    for(auto i = 0; i < usedAdapters.Count(); i++)
    {
        m_GPUs[i].m_AvailableValues.Resize(9);
        for(auto j = 0; j < m_GPUs[i].m_AvailableValues.Count(); ++j)
        {
            m_GPUs[i].m_AvailableValues(j) = false;
        }
        m_GPUs[i].m_MaxValues.Resize(9);
        m_GPUs[i].m_MinValues.Resize(9);
        m_GPUs[i].m_Values.Resize(9);
        m_GPUs[i].m_AdapterIndex = adapters[usedAdapters[i]].iAdapterIndex;

        int adapterActive = 0;
        ADL_Adapter_Active_Get(adapters[usedAdapters[i]].iAdapterIndex, &adapterActive);
        iOverdriveSupported = 0;
        if(adapterActive && adapters[usedAdapters[i]].iVendorID == AMDVENDORID)
        {
            if(ADL_OK != ADL_Overdrive_Caps(adapters[usedAdapters[i]].iAdapterIndex, 
                &iOverdriveSupported, &iOverdriveEnabled, &iOverdriveVersion))
            {
                continue;
            }

            if(iOverdriveSupported)
            {
                if(iOverdriveVersion == 5)
                {
                    m_GPUs[i].m_IsOverdrive6 = false;
                }
                else if(iOverdriveVersion == 6)
                {
                    ADL_Overdrive6_FanSpeed_Get = (ADL_OVERDRIVE6_FANSPEED_GET)m_Library->GetFunctionAddress("ADL_Overdrive6_FanSpeed_Get"_rfs);
                    ADL_Overdrive6_ThermalController_Caps = (ADL_OVERDRIVE6_THERMALCONTROLLER_CAPS)m_Library->GetFunctionAddress("ADL_Overdrive6_ThermalController_Caps"_rfs);
                    ADL_Overdrive6_Temperature_Get = (ADL_OVERDRIVE6_TEMPERATURE_GET)m_Library->GetFunctionAddress("ADL_Overdrive6_Temperature_Get"_rfs);
                    ADL_Overdrive6_Capabilities_Get = (ADL_OVERDRIVE6_CAPABILITIES_GET)m_Library->GetFunctionAddress("ADL_Overdrive6_Capabilities_Get"_rfs);
                    ADL_Overdrive6_StateInfo_Get = (ADL_OVERDRIVE6_STATEINFO_GET)m_Library->GetFunctionAddress("ADL_Overdrive6_StateInfo_Get"_rfs);
                    ADL_Overdrive6_CurrentStatus_Get = (ADL_OVERDRIVE6_CURRENTSTATUS_GET)m_Library->GetFunctionAddress("ADL_Overdrive6_CurrentStatus_Get"_rfs);
                    ADL_Overdrive6_PowerControl_Caps = (ADL_OVERDRIVE6_POWERCONTROL_CAPS)m_Library->GetFunctionAddress("ADL_Overdrive6_PowerControl_Caps"_rfs);
                    ADL_Overdrive6_PowerControlInfo_Get = (ADL_OVERDRIVE6_POWERCONTROLINFO_GET)m_Library->GetFunctionAddress("ADL_Overdrive6_PowerControlInfo_Get"_rfs);
                    ADL_Overdrive6_PowerControl_Get = (ADL_OVERDRIVE6_POWERCONTROL_GET)m_Library->GetFunctionAddress("ADL_Overdrive6_PowerControl_Get"_rfs);
                    m_GPUs[i].m_IsOverdrive6 = true;
                }
            }
        }

        if (iOverdriveSupported && m_GPUs[i].m_IsOverdrive6)
        {
            ADLOD6ThermalControllerCaps thermalControllerCaps = {0};
            if(ADL_OK == ADL_Overdrive6_ThermalController_Caps(adapters[usedAdapters[i]].iAdapterIndex, &thermalControllerCaps))
            {
                if(ADL_OD6_TCCAPS_FANSPEED_CONTROL == (thermalControllerCaps.iCapabilities & ADL_OD6_TCCAPS_FANSPEED_CONTROL))
                {
                    m_GPUs[i].m_AvailableValues(GPUService::FanSpeedPercentage) = 
                        ADL_OD6_TCCAPS_FANSPEED_PERCENT_READ == (thermalControllerCaps.iCapabilities & ADL_OD6_TCCAPS_FANSPEED_PERCENT_READ);
                    m_GPUs[i].m_MaxValues(GPUService::FanSpeedPercentage) = thermalControllerCaps.iFanMaxPercent;
                    m_GPUs[i].m_MinValues(GPUService::FanSpeedPercentage) = thermalControllerCaps.iFanMinPercent;
                    m_GPUs[i].m_AvailableValues(GPUService::FanSpeedRPM) =
                        ADL_OD6_TCCAPS_FANSPEED_RPM_READ == (thermalControllerCaps.iCapabilities & ADL_OD6_TCCAPS_FANSPEED_RPM_READ);
                    m_GPUs[i].m_MaxValues(GPUService::FanSpeedRPM) = thermalControllerCaps.iFanMaxRPM;
                    m_GPUs[i].m_MinValues(GPUService::FanSpeedRPM) = thermalControllerCaps.iFanMinRPM;
                }

                if(ADL_OD6_TCCAPS_THERMAL_CONTROLLER == (thermalControllerCaps.iCapabilities & ADL_OD6_TCCAPS_THERMAL_CONTROLLER))
                {
                    m_GPUs[i].m_AvailableValues(GPUService::Temperature) = true;
                    m_GPUs[i].m_MaxValues(GPUService::Temperature) = 130;
                    m_GPUs[i].m_MinValues(GPUService::Temperature) = 0;
                }
            }

            ADLOD6Capabilities od6Capabilities = {0};
            if(ADL_OK == ADL_Overdrive6_Capabilities_Get(adapters[usedAdapters[i]].iAdapterIndex, &od6Capabilities))
            {
                m_GPUs[i].m_AvailableValues(GPUService::Activity) = 
                    (od6Capabilities.iCapabilities & ADL_OD6_CAPABILITY_GPU_ACTIVITY_MONITOR) == ADL_OD6_CAPABILITY_GPU_ACTIVITY_MONITOR;
                m_GPUs[i].m_MaxValues(GPUService::Activity) = 100;
                m_GPUs[i].m_MinValues(GPUService::Activity) = 0;
            }

            m_GPUs[i].m_AvailableValues(GPUService::CoreClock) = true;
            m_GPUs[i].m_MaxValues(GPUService::CoreClock) = od6Capabilities.sEngineClockRange.iMax/100;
            m_GPUs[i].m_MinValues(GPUService::CoreClock) = od6Capabilities.sEngineClockRange.iMin/100;
            m_GPUs[i].m_AvailableValues(GPUService::MemoryClock) = true;
            m_GPUs[i].m_MaxValues(GPUService::MemoryClock) = od6Capabilities.sMemoryClockRange.iMax/100;
            m_GPUs[i].m_MinValues(GPUService::MemoryClock) = od6Capabilities.sMemoryClockRange.iMin/100;
            m_GPUs[i].m_AvailableValues(GPUService::Lanes) = true;
            m_GPUs[i].m_MaxValues(GPUService::Lanes) = 16;
            m_GPUs[i].m_MinValues(GPUService::Lanes) = 0;
            m_GPUs[i].m_AvailableValues(GPUService::BusSpeed) = true;
            m_GPUs[i].m_MaxValues(GPUService::BusSpeed) = 1;
            m_GPUs[i].m_MinValues(GPUService::BusSpeed) = 0;
        }
    }
}

RF_Type::Size AMDGPUService::GPUCount() const
{
    return m_GPUs.Count();
}

const RF_Collect::Array<RF_Type::Float32>& AMDGPUService::CurrentValues(const RF_Type::Size GPUIndex) const
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

const RF_Collect::Array<RF_Type::Float32>& AMDGPUService::MaxValues(const RF_Type::Size GPUIndex) const
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

const RF_Collect::Array<RF_Type::Float32>& AMDGPUService::MinValues(const RF_Type::Size GPUIndex) const
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

const RF_Collect::Array<RF_Type::Bool>& AMDGPUService::AvailableValues(const RF_Type::Size GPUIndex) const
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

void AMDGPUService::Update(const RF_Type::Size GPUIndex)
{
    if(m_GPUs.Count() > 0 && GPUIndex < m_GPUs.Count())
    {
        if (m_GPUs[GPUIndex].m_IsOverdrive6)
        {
            ADLOD6CurrentStatus currentStatus = {0};
            if(ADL_OK == ADL_Overdrive6_CurrentStatus_Get(m_GPUs[GPUIndex].m_AdapterIndex, &currentStatus))
            {
                m_GPUs[GPUIndex].m_Values(GPUService::CoreClock) = currentStatus.iEngineClock / 100;
                m_GPUs[GPUIndex].m_Values(GPUService::MemoryClock) = currentStatus.iMemoryClock / 100;
                m_GPUs[GPUIndex].m_Values(GPUService::Lanes) = currentStatus.iCurrentBusLanes;
                m_GPUs[GPUIndex].m_Values(GPUService::BusSpeed) = currentStatus.iCurrentBusSpeed;
                m_GPUs[GPUIndex].m_Values(GPUService::Activity) = currentStatus.iActivityPercent;
            }
            int temperature = 0;
            if(ADL_OK == ADL_Overdrive6_Temperature_Get(m_GPUs[GPUIndex].m_AdapterIndex, &temperature))
            {
                m_GPUs[GPUIndex].m_Values(GPUService::Activity) = temperature/1000;
            }
            ADLOD6FanSpeedInfo fanSpeedInfo = {0};
            if(ADL_OK == ADL_Overdrive6_FanSpeed_Get(m_GPUs[GPUIndex].m_AdapterIndex, &fanSpeedInfo))
            {
                m_GPUs[GPUIndex].m_Values(GPUService::FanSpeedPercentage) = fanSpeedInfo.iFanSpeedPercent;
                m_GPUs[GPUIndex].m_Values(GPUService::FanSpeedRPM) = fanSpeedInfo.iFanSpeedRPM;
            }
        }
    }
}

RF_Type::Bool AMDGPUService::IsAvailable()
{   
    RF_Mem::AutoPointer<RF_Sys::DynamicLibrary> atiadl = RF_Sys::DynamicLibrary::LoadSystemLibrary("atiadlxx.dll"_rfs);
    if(!atiadl)
    {
        atiadl = RF_Sys::DynamicLibrary::LoadSystemLibrary("atiadlxy.dll"_rfs);
    }

    if(atiadl)
    {        
        ADL_Main_Control_Create = (ADL_MAIN_CONTROL_CREATE)atiadl->GetFunctionAddress("ADL_Main_Control_Create"_rfs);
        ADL_Main_Control_Destroy = (ADL_MAIN_CONTROL_DESTROY)atiadl->GetFunctionAddress("ADL_Main_Control_Destroy"_rfs);
        ADL_Adapter_NumberOfAdapters_Get = (ADL_ADAPTER_NUMBEROFADAPTERS_GET)atiadl->GetFunctionAddress("ADL_Adapter_NumberOfAdapters_Get"_rfs);
        ADL_Adapter_AdapterInfo_Get = (ADL_ADAPTER_ADAPTERINFO_GET)atiadl->GetFunctionAddress("ADL_Adapter_AdapterInfo_Get"_rfs);
        ADL_Adapter_Active_Get = (ADL_ADAPTER_ACTIVE_GET)atiadl->GetFunctionAddress("ADL_Adapter_Active_Get"_rfs);
        ADL_Overdrive_Caps = (ADL_OVERDRIVE_CAPS)atiadl->GetFunctionAddress("ADL_Overdrive_Caps"_rfs);
    }
    return ADL_Main_Control_Destroy && ADL_Main_Control_Create && ADL_Adapter_NumberOfAdapters_Get &&
        ADL_Adapter_AdapterInfo_Get && ADL_Adapter_Active_Get && ADL_Overdrive_Caps;
}

} } } }