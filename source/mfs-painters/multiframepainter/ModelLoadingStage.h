#pragma once

#include <vector>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>

#include "TypeDefinitions.h"
#include "Preset.h"
#include "Material.h"

namespace globjects
{
    class Texture;
}

namespace gloperate
{
    class PolygonalDrawable;
    class ResourceManager;
}

class aiMaterial;

class ModelLoadingStage : public gloperate::AbstractStage
{
public:
    ModelLoadingStage();

    gloperate::InputSlot<gloperate::ResourceManager*> resourceManager;
    gloperate::InputSlot<Preset> preset;

    gloperate::Data<PresetInformation> presetInformation;
    gloperate::Data<IdDrawablesMap> drawablesMap;
    gloperate::Data<IdMaterialMap> materialMap;

protected:
    float m_maxAnisotropy;

    virtual void process() override;

    globjects::ref_ptr<globjects::Texture> loadTexture(const std::string& filename) const;
    Material loadMaterial(aiMaterial* mat, const std::string& directory) const;

    static PresetInformation getPresetInformation(Preset preset);
    static std::string getFilename(Preset preset);
};
