#include <RadonFramework/precompiled.hpp>
#include <RadonFramework/Radon.hpp>
#include <RadonFramework/System/Environment.hpp>
#include "window.hpp"

#include <windows.h>
#undef DrawText

namespace RadonExample {

BasicWindow::BasicWindow()
:m_Canvas(this) 
{
    // get current user language
    RF_Type::String iso = RF_SysEnv::ActiveLanguage();
    RF_Type::String language = RF_SysEnv::ActiveLanguageName();
    RF_Type::String nativeLanguage = RF_SysEnv::ActiveNativeLanguageName();
    RF_Type::String location = RF_SysEnv::ActiveLanguageLocation();
    RF_Type::String nativeLocation = RF_SysEnv::ActiveNativeLanguageLocation();
    // show the user the the parameter used to choose the right font
    Label4.SetText(RF_Type::String::Format("Language=%s(%s) Location=%s(%s)",
        language, nativeLanguage, location, nativeLocation));
        
    RF_Collect::Array<RF_Text::UnicodeRangeIdentifier> ranges;
    RF_Draw::FontStyle styleFilter = RF_Draw::FontStyle::Standard | RF_Draw::FontStyle::Regular;
        
    auto& fontService = RF_Draw::FontServiceLocator::Default();
    fontService.GetUnicodeCharRanges(iso, ranges);
    fontService.EnableStyleFilter(styleFilter);
    fontService.EnableOrientationFilter(true);
    fontService.EnableCharRangeFilter(ranges);
    fontService.Update();

    auto& fonts = fontService.Fonts();
    //auto* font = fontService.FindFontByName("Arial");

    RF_Draw::Path2D& path = m_Canvas.Draw2D.BeginPath();
    path.FillProperties().Color = RF_Draw::Color4f(0.11f,0.11f,0.11f,1.0f);
    path.AddRectangle(RF_Geo::Point2Df(0, 0), RF_Geo::Size2Df(128,32));
    m_Canvas.Draw2D.EndPath(path);
    RF_Draw::Text2D& hwText = m_Canvas.Draw2D.Text(fonts(0), "Hello World");
    
    m_Text = &hwText;
    m_Path = &path;
}

void BasicWindow::Idle()
{
    m_Canvas.Clear();
    m_Canvas.Draw2D.DrawPath(*m_Path, RF_Geo::Point2Df(10, 10));
    m_Canvas.Draw2D.DrawText(*m_Text, RF_Geo::Point2Df(10, 10));
    m_Canvas.SwapBuffer();
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
        windows(i).Title(RF_Type::String::Format("X=%ddpi Y=%ddpi", dpiX, dpiY));
        windows(i).Label1.SetText(screens[i].DeviceName());
        auto& resolution = screens[i].CurrentResolution();

        windows(i).Label2.SetText(RF_Type::String::Format("Width=%d Height=%d",resolution.Width, resolution.Height));
        windows(i).Label3.SetText(RF_Type::String::Format("X=%ddpi Y=%ddpi", dpiX, dpiY));
    }

    app->Run(&windows(0));
    
    return;
}