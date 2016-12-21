#include <RadonFramework/precompiled.hpp>
#include <RadonFramework/Radon.hpp>
#include "window.hpp"

#include "WindowsWindowService.hpp"
#include "WDMOpenGLCanvas3DService.hpp"

void main()
{ 
    RadonFramework::Radon radon;

    RF_Mem::AutoPointer<RF_Diag::Appender> debuggerOutput(new RF_IO::LogDebuggerOutput);
    RF_IO::Log::AddAppender(debuggerOutput);
    
    RF_Form::WindowsWindowService::Register();
    RF_Draw::WDMOpenGLCanvas3DService::Register();

    RadonExample::BasicWindow window;

    auto* app = RF_Form::WindowServiceLocator::Default().Application();
    if (app != nullptr)
    {
        app->ShowConsole(false);
        app->Run(&window);
    }
    
    return;
}