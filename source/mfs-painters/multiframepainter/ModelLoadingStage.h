#pragma once

#include <vector>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>

#include "TypeDefinitions.h"

namespace gloperate
{
    class PolygonalDrawable;
    class ResourceManager;
}

class ModelLoadingStage : public gloperate::AbstractStage
{
public:
    ModelLoadingStage();

    gloperate::InputSlot<gloperate::ResourceManager*> resourceManager;
    gloperate::InputSlot<std::string> modelFilename;

    gloperate::Data<PolygonalDrawables> drawables;

protected:
    virtual void process() override;
};
