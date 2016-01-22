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
    gloperate::InputSlot<gloperate::ResourceManager*> resourceManager;
    gloperate::InputSlot<std::string> modelFilename;

    gloperate::Data<gloperate::PolygonalDrawable *> model;

protected:
    virtual void process() override;
};
