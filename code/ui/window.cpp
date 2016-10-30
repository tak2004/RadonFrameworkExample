#include <RadonFramework/precompiled.hpp>
#include <RadonFramework/Radon.hpp>
#include <RadonFramework/System/Environment.hpp>
#include "window.hpp"
#include <RadonFramework/System/Drawing/OpenGL.hpp>
#include <RadonFramework/Diagnostics/Profiling/GPUServiceLocator.hpp>
#include "../shared/AMDGPUService.hpp"
#include "../shared/NvidiaGPUService.hpp"

namespace RadonExample {

BasicWindow::BasicWindow()
:Form()
,m_Canvas(*this)
,m_Layouter(&m_Canvas, 3, 3)
{
    m_Layouter.AddChild(m_VLayouter, 0,2);
    m_VLayouter.AddChild(m_Label1);
    m_VLayouter.AddChild(m_FanRPM);
    m_Label1.SetText(":)"_rfs);
    m_FanRPM.SetInfo(RF_Prof::GPUService::GPUActivity);
    m_OpenGLRenderer.SetCanvas(m_Canvas);
    m_Canvas.SetRenderer(m_OpenGLRenderer);
}

RF_Form::Label& BasicWindow::GetLabel1()
{
    return m_Label1;
}

}

void main()
{ 
    RadonFramework::Radon radon;

    auto& screens = RF_Form::Screen::AllScreens();
    RF_Collect::Array<RadonExample::BasicWindow> windows;

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

    auto* app = RF_Form::WindowServiceLocator::Default().Application();
    app->ShowConsole(false);

    windows.Resize(screens.Count());

    for(RF_Type::Size i = 0; i < screens.Count(); i++)
    {
        windows(i).SetWindowPosition(screens[i].Position());
        RF_Type::UInt32 dpiX = screens[i].DPIX();
        RF_Type::UInt32 dpiY = screens[i].DPIY();
        RF_Geo::Size2D<> windowSize((windows(i).Width()*dpiX)/96,(windows(i).Height()*dpiY)/96);
        RF_Geo::Point2D<> dpi;
        dpi.X = dpiX;
        dpi.Y = dpiY;
        windows(i).DPIChanged(dpi);
        windows(i).SetWindowSize(windowSize);
        windows(i).Title(RF_Type::String::Format("X=%ddpi Y=%ddpi"_rfs, dpiX, dpiY));
    }

    app->Run(&windows(0));
    
    return;
}