#include <RadonFramework/precompiled.hpp>
#include "window.hpp"

namespace RadonExample {

BasicWindow::BasicWindow()
:Form()
,m_Canvas(*this)
,m_Layouter(&m_Canvas, 3, 3)
{
    m_Layouter.AddChild(m_FanRPM, 0, 2);
    m_FanRPM.SetInfo(RF_Prof::GPUService::GPUActivity);
}

void BasicWindow::RebuildVisuals()
{
    Form::RebuildVisuals();

    RF_Geo::Point2Df stardata[5] = {{0.0f, 300.0f},       // 0: x,y,z,r,g,b
    {-100.0f, 0.0f},       // 1:
    {160, 190},       // 2:
    {-160, 190},       // 3:
    {100, 0.0f}};     // 4:
    RF_Draw::Fill red;
    RF_Draw::Stroke stroke;
    red.Color = RF_Draw::Color4f::White();
    stroke.Color = RF_Draw::Color4f::Black();
    stroke.Width = 5;
    m_Path.SetStroke(stroke);
    m_Path.Clear();
    m_Path.SetFill(red);
    m_Path.LineTo(stardata[0]);
    m_Path.LineTo(stardata[1]);
    m_Path.LineTo(stardata[2]);
    m_Path.LineTo(stardata[3]);
    m_Path.LineTo(stardata[4]);
    m_Path.Close();
    m_Path.Finalize();

}

}