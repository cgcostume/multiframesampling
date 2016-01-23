#pragma once

#include <string>

#include <globjects/base/ref_ptr.h>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>

namespace gloperate
{
    class PolygonalDrawable;
    class ResourceManager;
}

class ModelLoadingStage : public gloperate::AbstractStage
{
public:
    ModelLoadingStage();

    using PolygonalDrawables = std::vector<std::unique_ptr<gloperate::PolygonalDrawable>>;

    gloperate::InputSlot<gloperate::ResourceManager*> resourceManager;
    gloperate::InputSlot<std::string> modelFilename;

    gloperate::Data<PolygonalDrawables> drawables;

protected:
    virtual void process() override;
};
