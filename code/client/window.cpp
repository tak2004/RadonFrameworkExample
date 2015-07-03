#include <RadonFramework/precompiled.hpp>
#include <RadonFramework/Radon.hpp>

class BasicWindow: public RF_Form::Form
{
public:
    BasicWindow()
    :m_Canvas(this) 
    {
        RF_Draw::FontServiceLocator::Default().Update();
        auto& fonts = RF_Draw::FontServiceLocator::Default().Fonts();
    }

    virtual void Idle() override
    {
        m_Canvas.Clear();
        //m_Canvas.DrawText("Hello World!", fonts(0), 0, 0);
        m_Canvas.SwapBuffer();
        m_first = false;
    }
protected:
    RF_Draw::Canvas3D m_Canvas;
};

void main()
{ 
    RadonFramework::Radon radon;

    auto& screens = RF_Form::Screen::AllScreens();
    BasicWindow window;

    auto* app = RF_Form::WindowServiceLocator::Default().Application();
    app->ShowConsole(false);
    app->Run(&window);
    
    return;
}