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
    virtual void Idle() override;
protected: 
    RF_Draw::Canvas3D m_Canvas;
    RF_Form::GridLayouter m_Layouter;
    RF_Form::GPUInfoGraph m_FanRPM;
    struct Particle
    {
        RF_Geo::Vec3f Position;
        RF_Geo::Vec3f Force;
    };
    RF_Collect::Array<Particle> m_Particles;
    RF_Type::UInt64 m_NextPhysicStep;
    RF_Type::UInt64 m_PhysicStep;
    RF_Type::UInt32 m_BufferId;
    RF_Type::UInt32 m_ProgramId;
    RF_Type::UInt32 m_MaterialId;
    RF_Type::UInt32 m_ObjectId;
    RF_Collect::CommandBucket<>* m_RendererQueue;
};

}

#endif // RE_WINDOW_HPP