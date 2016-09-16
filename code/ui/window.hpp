#ifndef RE_WINDOW_HPP
#define RE_WINDOW_HPP

#include <RadonFramework/Drawing/AbstractRenderer.hpp>

namespace RadonExample {

class OpenGLRenderer: public RF_Draw::AbstractRenderer
{
public:
    virtual void StartFrame() override;
    virtual void EndFrame() override;
    virtual void Process(const RF_Draw::Path2D& Path) override;
    virtual void Draw(const RF_Draw::Path2D& Path) override;
};

class BasicWindow: public RF_Form::Form
{
public:
    BasicWindow();

    RF_Form::Label& GetLabel1();
protected:
    RF_Draw::Canvas3D m_Canvas;
    RF_Form::Label m_Label1;
    OpenGLRenderer m_OpenGLRenderer;
};

}

#endif // RE_WINDOW_HPP