#include <RadonFramework/precompiled.hpp>
#include "window.hpp"
#include "RadonFramework/Collections/Algorithm/ForEach.hpp"
#include "RadonFramework/Collections/Algorithm/ForEachBulk.hpp"
#include "RadonFramework/Math/Random.hpp"
#include "RadonFramework/Drawing/BasicRenderFunction.hpp"
#include "Color3D.glsl"

namespace RadonExample {

BasicWindow::BasicWindow()
:Form()
,m_Canvas(*this)
,m_Layouter(&m_Canvas, 3, 3)
,m_Particles(5000)
,m_PhysicStep(RF_Time::TimeSpan::TicksPerSecond / 60)
,m_BufferId(0)
{
    // assign dispatch functions of the active renderer
    RF_Draw::GenerateBuffer::DispatchFunction = m_Canvas.GetRenderer()->GetGeneralPurposeDispatcher(RF_Draw::BasicRenderFunctionType::GenerateBuffer);
    RF_Draw::DestroyBuffer::DispatchFunction = m_Canvas.GetRenderer()->GetGeneralPurposeDispatcher(RF_Draw::BasicRenderFunctionType::DestroyBuffer);
    RF_Draw::UpdateBuffer::DispatchFunction = m_Canvas.GetRenderer()->GetGeneralPurposeDispatcher(RF_Draw::BasicRenderFunctionType::UpdateBuffer);
    RF_Draw::RenderObject::DispatchFunction = m_Canvas.GetRenderer()->GetGeneralPurposeDispatcher(RF_Draw::BasicRenderFunctionType::RenderObject);
    RF_Draw::GenerateProgram::DispatchFunction = m_Canvas.GetRenderer()->GetGeneralPurposeDispatcher(RF_Draw::BasicRenderFunctionType::GenerateProgram);
    RF_Draw::DestroyProgram::DispatchFunction = m_Canvas.GetRenderer()->GetGeneralPurposeDispatcher(RF_Draw::BasicRenderFunctionType::DestroyProgram);
    RF_Draw::GenerateMaterial::DispatchFunction = m_Canvas.GetRenderer()->GetGeneralPurposeDispatcher(RF_Draw::BasicRenderFunctionType::GenerateMaterial);

    m_Canvas.GetRenderer()->ResizeQueueCount(1);
    m_RendererQueue = m_Canvas.GetRenderer()->GetQueue(0);
    m_NextPhysicStep = m_Now + m_PhysicStep;
    m_Layouter.AddChild(m_FanRPM, 0, 2);
    m_FanRPM.SetInfo(RF_Prof::GPUService::GPUActivity);
    // initialize particles    
    RF_Algo::ForEachBulk(m_Particles, [](RF_Collect::Array<Particle>::EnumeratorType& Particle, RF_Type::UInt32 Elements) {
        RF_Math::Random rng;
        for (auto i = 0; i < Elements; ++i, ++Particle)
        {
            Particle->Force = {0.1f,-0.2f,0.3f};
            Particle->Position = {rng.NextFloat()*100.0f - 50.0f,rng.NextFloat()*100.0f,rng.NextFloat()*100.0f - 50.0f};
        }
    });
    // generate a buffer with the data to render
    RF_Draw::GenerateBuffer* genBuffer = m_RendererQueue->AddCommand<RF_Draw::GenerateBuffer>(0, sizeof(RF_Geo::Vec3f)*m_Particles.Count());
    genBuffer->Buffer = &m_BufferId;
    genBuffer->ByteSize = m_RendererQueue->GetCommandMemorySize(*genBuffer);
    genBuffer->Data = m_RendererQueue->GetCommandMemory(*genBuffer);
    RF_SysMem::Copy(genBuffer->Data, &m_Particles(0), genBuffer->ByteSize);
    // generate a shader program which can process the buffer data
    RF_Draw::GenerateProgram* genProgram = m_RendererQueue->AddCommand<RF_Draw::GenerateProgram>(0, sizeof(Color3D_vert) + sizeof(Color3D_frag));
    genProgram->Program = &m_ProgramId;
    genProgram->VertexData = Color3D_vert;
    genProgram->FragmentData = Color3D_frag;
    genProgram->GeometryData = nullptr;
    genProgram->VertexByteSize = sizeof(Color3D_vert);
    genProgram->FragmentByteSize = sizeof(Color3D_frag);
    genProgram->GeometryByteSize = 0;
    // generate a material which orchestrate the rendering
    RF_Draw::GenerateMaterial* genMaterial = m_RendererQueue->AddCommand<RF_Draw::GenerateMaterial>(0);
    genMaterial->Material = &m_MaterialId;
    genMaterial->Program = &m_ProgramId;
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

void BasicWindow::Idle()
{
    RF_Draw::UpdateBuffer* update = nullptr;
    // multi threaded simulation
    if(m_NextPhysicStep < m_Now)
    {
        RF_Algo::ForEach(m_Particles, [](RF_Collect::Array<Particle>::EnumeratorType& Particle){
            if(Particle->Position[1] > 0)
            {
                Particle->Position += Particle->Force;
            }
            else
            {
                Particle->Position -= Particle->Force * 100.0f;
            }        
        });
        m_NextPhysicStep += m_PhysicStep;
        // update gpu memory
        update = m_RendererQueue->AddCommand<RF_Draw::UpdateBuffer>(1, sizeof(RF_Geo::Vec3f)*m_Particles.Count());
        update->Buffer = &m_BufferId;
        update->Data = m_RendererQueue->GetCommandMemory(*update);
        update->ByteSize = m_RendererQueue->GetCommandMemorySize(*update);
        RF_SysMem::Copy(update->Data, &m_Particles(0), update->ByteSize);
    }
    // render particles
    RF_Draw::RenderObject* render = nullptr;
    if (update == nullptr)
    {
        render = m_RendererQueue->AddCommand<RF_Draw::RenderObject>(1);
    }
    else
    {
        render = m_RendererQueue->AppendCommand<RF_Draw::RenderObject>(*update);
    }
    render->Buffer = &m_BufferId;
    render->Elements = m_Particles.Count();
    render->ElementType = RF_Draw::RenderObject::PointSprites;
    render->Material = &m_MaterialId;
    // take care of ui rendering and Canvas3D will be process the commands
    Form::Idle();
}

}