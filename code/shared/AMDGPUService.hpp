#ifndef RF_SYSTEM_DIAGNOSTICS_PROFILING_AMDGPUSERVICE_HPP
#define RF_SYSTEM_DIAGNOSTICS_PROFILING_AMDGPUSERVICE_HPP
#if _MSC_VER > 1000
#pragma once
#endif

#include <RadonFramework/Diagnostics/Profiling/GPUService.hpp>
#include <RadonFramework/System/DynamicLibrary.hpp>

namespace RadonFramework { namespace System { namespace Diagnostics { namespace Profiling {

class AMDGPUService: public RF_Prof::GPUService
{
public:
    AMDGPUService(const RF_Type::String& Name);
    virtual ~AMDGPUService()override;
    virtual void ObtainGPUs()override;
    virtual RF_Type::Size GPUCount() const override;
    virtual const RF_Collect::Array<RF_Type::Float32>& CurrentValues(const RF_Type::Size GPUIndex) const override;
    virtual const RF_Collect::Array<RF_Type::Float32>& MaxValues(const RF_Type::Size GPUIndex) const override;
    virtual const RF_Collect::Array<RF_Type::Float32>& MinValues(const RF_Type::Size GPUIndex) const override;
    virtual const RF_Collect::Array<RF_Type::Bool>& AvailableValues(const RF_Type::Size GPUIndex) const override;
    virtual void Update(const RF_Type::Size GPUIndex) override;
    static RF_Type::Bool IsAvailable();
private:
    class GPUInfo;
    RF_Mem::AutoPointerArray<GPUInfo> m_GPUs;
    RF_Collect::Array<RF_Type::Float32> m_EmptyValues;
    RF_Collect::Array<RF_Type::Bool> m_EmptyAvailableValues;
    RF_Mem::AutoPointer<RF_Sys::DynamicLibrary> m_Library;
};

} } } }

#ifndef RF_SHORTHAND_NAMESPACE_SYSPROF
#define RF_SHORTHAND_NAMESPACE_SYSPROF
namespace RF_SysProf = RadonFramework::System::Diagnostics::Profiling;
#endif

#endif //!RF_SYSTEM_DIAGNOSTICS_PROFILING_AMDGPUSERVICE_HPP