#include <RadonFramework/precompiled.hpp>
#include <RadonFramework/Radon.hpp>
#include <RadonFramework/System/Environment.hpp>

class BasicWindow: public RF_Form::Form
{
public:
    BasicWindow()
    :m_Canvas(this) 
    {
        // get current user language
        RF_Type::String iso = RF_SysEnv::ActiveLanguage();
        RF_Type::String language = RF_SysEnv::ActiveLanguageName();
        RF_Type::String nativeLanguage = RF_SysEnv::ActiveNativeLanguageName();
        RF_Type::String location = RF_SysEnv::ActiveLanguageLocation();
        RF_Type::String nativeLocation = RF_SysEnv::ActiveNativeLanguageLocation();
        // show the user the the parameter used to choose the right font
//         Label4.Text=RF_Type::String::Format("Language=%s(%s) Location=%s(%s)",
//             language, nativeLanguage, location, nativeLocation);
        
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
    }

    virtual void Idle() override
    {
        m_Canvas.Clear();
        //m_Canvas.DrawText("Hello World!", fonts(0), 0, 0);
        m_Canvas.SwapBuffer();
    }
//     RF_Form::Label Label1;
//     RF_Form::Label Label2;
//     RF_Form::Label Label3;
//     RF_Form::Label Label4;
protected:
    RF_Draw::Canvas3D m_Canvas;
};

void main()
{ 
    RadonFramework::Radon radon;

    auto& screens = RF_Form::Screen::AllScreens();
    RF_Collect::Array<BasicWindow> windows;

    auto* app = RF_Form::WindowServiceLocator::Default().Application();
    app->ShowConsole(false);

    windows.Resize(screens.Count());

    for(RF_Type::Size i = 0; i < screens.Count(); i++)
    {
        windows(i).Position(screens[i].Position());
//        windows(i).Label1.Text = screens[i].DeviceName();
        auto& resolution = screens[i].CurrentResolution();
//         windows(i).Label2.Text = RF_Type::String::Format("Width=%d Height=%d",resolution.Width, resolution.Height);
//         windows(i).Label3.Text = RF_Type::String::Format("X=%ddpi Y=%ddpi", resolution.DPIX, resolution.DPIY);
    }

    app->Run(&windows(0));
    
    return;
}