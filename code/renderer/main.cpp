#include <RadonFramework/precompiled.hpp>
#include <RadonFramework/Radon.hpp>
#include "window.hpp"
#include <RadonFramework/Diagnostics/Profiling/GPUServiceLocator.hpp>
#include "../shared/AMDGPUService.hpp"
#include "../shared/NvidiaGPUService.hpp"

void main()
{ 
    RadonFramework::Radon radon;

    RF_Mem::AutoPointer<RF_Diag::Appender> debuggerOutput(new RF_IO::LogDebuggerOutput);
    RF_IO::Log::AddAppender(debuggerOutput);
    
    if(RF_SysProf::AMDGPUService::IsAvailable())
    {
        RF_Mem::AutoPointer<RF_Prof::GPUService> gpuService(new RF_SysProf::AMDGPUService("AMD"_rfs));
        gpuService->ObtainGPUs();
        RF_Prof::GPUServiceLocator::Register(gpuService);
    }

    if(RF_SysProf::NvidiaGPUService::IsAvailable())
    {
        RF_Mem::AutoPointer<RF_Prof::GPUService> gpuService(new RF_SysProf::NvidiaGPUService("Nvidia"_rfs));
        gpuService->ObtainGPUs();
        RF_Prof::GPUServiceLocator::Register(gpuService);
    }

    RadonExample::BasicWindow window;

    auto* app = RF_Form::WindowServiceLocator::Default().Application();
    if (app != nullptr)
    {
        app->ShowConsole(false);
        app->Run(&window);
    }
    
    return;
}