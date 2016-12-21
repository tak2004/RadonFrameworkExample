#ifndef RE_WINDOW_HPP
#define RE_WINDOW_HPP

#include <RadonFramework/Drawing/Forms/Form.hpp>
#include <RadonFramework/Drawing/Canvas3D.hpp>
#include <RadonFramework/Drawing/AbstractRenderer.hpp>

namespace RadonExample {

class BasicWindow: public RF_Form::Form
{
public:
    BasicWindow();
protected:
    RF_Draw::Canvas3D m_Canvas;
    RF_Draw::AbstractRenderer* m_Renderer;
};

}

#endif // RE_WINDOW_HPP