#pragma once

#include <vector>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>

#include "TypeDefinitions.h"
#include "Preset.h"

namespace globjects
{
    class Texture;
}

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
    gloperate::InputSlot<Preset> preset;

    gloperate::Data<PresetInformation> presetInformation;
    gloperate::Data<PolygonalDrawables> drawables;
    gloperate::Data<IdTextureMap> textureMap;

protected:
    virtual void process() override;

    static PresetInformation getPresetInformation(Preset preset);
    static std::string getFilename(Preset preset);
};
