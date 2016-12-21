#include <RadonFramework/precompiled.hpp>
#include <RadonFramework/Radon.hpp>
#include <RadonFramework/System/Environment.hpp>
#include "window.hpp"

namespace RadonExample {

BasicWindow::BasicWindow()
:m_Canvas(*this) 
{
    // get current user language
    RF_Type::String iso = RF_SysEnv::ActiveLanguage();
    RF_Type::String language = RF_SysEnv::ActiveLanguageName();
    RF_Type::String nativeLanguage = RF_SysEnv::ActiveNativeLanguageName();
    RF_Type::String location = RF_SysEnv::ActiveLanguageLocation();
    RF_Type::String nativeLocation = RF_SysEnv::ActiveNativeLanguageLocation();
    // show the user the the parameter used to choose the right font
    Label4.SetText(RF_Type::String::Format("Language=%s(%s) Location=%s(%s)"_rfs,
        language, nativeLanguage, location, nativeLocation));
        
    RF_Collect::Array<RF_Text::UnicodeRangeInfo> ranges;
    RF_Draw::FontStyle styleFilter = RF_Draw::FontStyle::Standard | RF_Draw::FontStyle::Regular;
        
    auto& fontService = RF_Draw::FontServiceLocator::Default();
    fontService.GetUnicodeCharRanges(iso, ranges);
    fontService.EnableStyleFilter(styleFilter);
    fontService.EnableOrientationFilter(true);
    fontService.EnableCharRangeFilter(ranges);
    fontService.Update();

    auto& fonts = fontService.Fonts();
    //auto* font = fontService.FindFontByName("Arial");
}

}

void main()
{ 
    RadonFramework::Radon radon;

    auto& screens = RF_Form::Screen::AllScreens();
    RF_Collect::Array<RadonExample::BasicWindow> windows;

    windows.Resize(screens.Count());

    for(RF_Type::Size i = 0; i < screens.Count(); i++)
    {
        windows(i).SetWindowPosition(screens[i].Position());
        RF_Type::UInt32 dpiX = screens[i].DPIX();
        RF_Type::UInt32 dpiY = screens[i].DPIY();
        RF_Geo::Size2D<> windowSize((windows(i).Width()*dpiX)/96,(windows(i).Height()*dpiY)/96);
        windows(i).SetWindowSize(windowSize);
        windows(i).Title(RF_Type::String::Format("X=%ddpi Y=%ddpi"_rfs, dpiX, dpiY));
        windows(i).Label1.SetText(screens[i].DeviceName());
        auto& resolution = screens[i].CurrentResolution();

        windows(i).Label2.SetText(RF_Type::String::Format("Width=%d Height=%d"_rfs,resolution.Width, resolution.Height));
        windows(i).Label3.SetText(RF_Type::String::Format("X=%ddpi Y=%ddpi"_rfs, dpiX, dpiY));
    }

    auto* app = RF_Form::WindowServiceLocator::Default().Application();
    if (app != nullptr)
    {
        app->ShowConsole(false);
        app->Run(&windows(0));
    }
    
    return;
}