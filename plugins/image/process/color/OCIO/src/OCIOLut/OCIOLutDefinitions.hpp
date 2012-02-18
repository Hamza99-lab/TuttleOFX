#ifndef _TUTTLE_PLUGIN_OCIOLUTDEFINITIONS_HPP_
#define _TUTTLE_PLUGIN_OCIOLUTDEFINITIONS_HPP_

#include <tuttle/plugin/global.hpp>

namespace tuttle {
namespace plugin {
namespace ocio{
namespace lut {

static const std::string kParamInputFilename      = "filename";
static const std::string kParamInterpolationType   = "interpolation";

static const std::string kParamInterpolationNearest   = "nearest";
static const std::string kParamInterpolationLinear = "linear";
static const std::string kParamInterpolationTetrahedral = "tetrahedral";

static const std::string kOCIOInputspace = "RawInput";
static const std::string kOCIOOutputspace = "ProcessedOutput";

enum EInterpolationType
{
	eInterpolationTypeNearest = 0,
	eInterpolationTypeLinear = 1,
	eInterpolationTypeTetrahedral = 2,

};
}
}
}
}

#endif
