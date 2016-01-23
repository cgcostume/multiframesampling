
#include "MultiFramePainter.h"

#include <random>

#include <cpplocate/ModuleInfo.h>

#include <glm/gtc/constants.hpp>

#include <iozeug/FilePath.h>

#include <gloperate/resources/ResourceManager.h>
#include <gloperate/base/registernamedstrings.h>
#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/VirtualTimeCapability.h>
#include <gloperate/painter/TargetFramebufferCapability.h>
#include <gloperate/painter/PerspectiveProjectionCapability.h>
#include <gloperate/painter/CameraCapability.h>
#include <gloperate/painter/VirtualTimeCapability.h>


using namespace reflectionzeug;
using namespace globjects;
using namespace gloperate;


MultiFramePainter::MultiFramePainter(ResourceManager & resourceManager, const cpplocate::ModuleInfo & moduleInfo)
: PipelinePainter("MultiFramePainter", resourceManager, moduleInfo, m_pipeline)
{
    // Get data path
    std::string dataPath = moduleInfo.value("dataPath");
    dataPath = iozeug::FilePath(dataPath).path();
    if (dataPath.size() > 0)
    {
        dataPath = dataPath + "/";
    }
    else
    {
        dataPath = "data/";
    }

    m_pipeline.resourceManager.setData(&resourceManager);

    // Setup painter
    m_targetFramebufferCapability = addCapability(new gloperate::TargetFramebufferCapability());
    m_viewportCapability = addCapability(new gloperate::ViewportCapability());
    m_projectionCapability = addCapability(new gloperate::PerspectiveProjectionCapability(m_viewportCapability));
    m_cameraCapability = addCapability(new gloperate::CameraCapability());

    m_pipeline.viewport.setData(m_viewportCapability);
    m_pipeline.projection.setData(m_projectionCapability);
    m_pipeline.camera.setData(m_cameraCapability);
}

MultiFramePainter::~MultiFramePainter()
{
}

void MultiFramePainter::onInitialize()
{
    PipelinePainter::onInitialize();
}

void MultiFramePainter::onPaint()
{
    PipelinePainter::onPaint();
}
