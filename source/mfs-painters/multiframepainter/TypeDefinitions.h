#pragma once

#include <vector>
#include <memory>

namespace gloperate
{
    class PolygonalDrawable;
}

using PolygonalDrawables = std::vector<std::unique_ptr<gloperate::PolygonalDrawable>>;
