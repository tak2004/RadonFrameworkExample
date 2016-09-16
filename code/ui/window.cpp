#include <RadonFramework/precompiled.hpp>
#include <RadonFramework/Radon.hpp>
#include <RadonFramework/System/Environment.hpp>
#include "window.hpp"

namespace RadonExample {

BasicWindow::BasicWindow()
:m_Canvas(*this)
,m_Label1(&m_Canvas)
{
    m_Label1.SetText(":)"_rfs);
    m_Canvas.SetRenderer(m_OpenGLRenderer);
}

RF_Form::Label& BasicWindow::GetLabel1()
{
    return m_Label1;
}

void OpenGLRenderer::StartFrame()
{
    
}

void OpenGLRenderer::EndFrame()
{
    
}

void OpenGLRenderer::Process(const RF_Draw::Path2D& Path)
{
    
}

void OpenGLRenderer::Draw(const RF_Draw::Path2D& Path)
{
    
}

}

void main()
{ 
    RadonFramework::Radon radon;

    auto& screens = RF_Form::Screen::AllScreens();
    RF_Collect::Array<RadonExample::BasicWindow> windows;

    auto* app = RF_Form::WindowServiceLocator::Default().Application();
    app->ShowConsole(false);

    windows.Resize(screens.Count());

    for(RF_Type::Size i = 0; i < screens.Count(); i++)
    {
        windows(i).Position(screens[i].Position());
        RF_Type::UInt32 dpiX = screens[i].DPIX();
        RF_Type::UInt32 dpiY = screens[i].DPIY();
        RF_Geo::Size2D<RF_Type::Int32> windowSize((windows(i).Width()*dpiX)/96,(windows(i).Height()*dpiY)/96);
        windows(i).Resize(windowSize);
        windows(i).Size(windowSize);
        windows(i).Title(RF_Type::String::Format("X=%ddpi Y=%ddpi"_rfs, dpiX, dpiY));
    }

    app->Run(&windows(0));
    
    return;
}