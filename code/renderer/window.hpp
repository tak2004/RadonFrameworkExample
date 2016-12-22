#ifndef RE_WINDOW_HPP
#define RE_WINDOW_HPP

#include <RadonFramework/Drawing/Forms/Form.hpp>
#include <RadonFramework/Drawing/Canvas3D.hpp>
#include <RadonFramework/Drawing/AbstractRenderer.hpp>
#include <RadonFramework/Drawing/Forms/GridLayouter.hpp>
#include <RadonFramework/Drawing/Forms/GPUInfoGraph.hpp>

namespace RadonExample {

class BasicWindow: public RF_Form::Form
{
public:
    BasicWindow();
    virtual void RebuildVisuals() override;
protected:
    RF_Draw::Canvas3D m_Canvas;
    RF_Form::GridLayouter m_Layouter;
    RF_Form::GPUInfoGraph m_FanRPM;
};

}

#endif // RE_WINDOW_HPP