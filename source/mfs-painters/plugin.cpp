
#include <gloperate/gloperate-version.h>
#include <gloperate/plugin/plugin_api.h>

#include "multiframepainter/MultiFramePainter.h"
#include "singleframepainter/SingleFramePainter.h"


GLOPERATE_PLUGIN_LIBRARY

    GLOPERATE_PAINTER_PLUGIN(MultiFramePainter
    , "MultiFramePainter"
    , "Moep"
    , GLOPERATE_AUTHOR_ORGANIZATION
    , "v0.0.0" )

    GLOPERATE_PAINTER_PLUGIN(SingleFramePainter
    , "SingleFramePainter"
    , "Moep"
    , GLOPERATE_AUTHOR_ORGANIZATION
    , "v0.0.0" )

GLOPERATE_PLUGIN_LIBRARY_END
