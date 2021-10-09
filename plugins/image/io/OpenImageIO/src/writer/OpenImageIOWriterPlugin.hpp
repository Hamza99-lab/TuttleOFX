#ifndef _TUTTLE_PLUGIN_OPENIMAGEIO_WRITER_PLUGIN_HPP_
#define _TUTTLE_PLUGIN_OPENIMAGEIO_WRITER_PLUGIN_HPP_

#include "OpenImageIOWriterDefinitions.hpp"

#include <tuttle/ioplugin/context/WriterPlugin.hpp>

#include <OpenImageIO/typedesc.h>

namespace tuttle
{
namespace plugin
{
namespace openImageIO
{
namespace writer
{

struct OpenImageIOWriterProcessParams
{
    std::string _filepath;               ///< filepath
    std::string _project;  ///< project metadata
    std::string _copyright;  ///< copyright metadata
    ETuttlePluginComponents _components; ///< Force RGB
    ETuttlePluginBitDepth _bitDepth;     ///< Output bit depth (real bit depth, not the buffer passed to OpenImageIO)
    ETuttlePluginSubsampling _subsampling; ///< Output subsampling
    ETuttlePluginEndianness _endianness; ///< Output endianness

    bool _premultiply; ///< Output premultiply
    int _quality;      ///< Output quality
    int _orientation;  ///< Output orientation
};

/**
 * @brief OpenImageIO writer
 */
class OpenImageIOWriterPlugin : public WriterPlugin
{
public:
    OpenImageIOWriterPlugin(OfxImageEffectHandle handle);

public:
    OpenImageIOWriterProcessParams getProcessParams(const OfxTime time);
    void render(const OFX::RenderArguments& args);

public:
    OFX::ChoiceParam* _components; ///< Choose components RGBA/RGB
    OFX::IntParam* _quality;
    OFX::ChoiceParam* _paramSubsampling;
    OFX::ChoiceParam* _orientation;
    OFX::StringParam* _project;
    OFX::StringParam* _copyright;
    OFX::ChoiceParam* _endianness;
};
}
}
}
}

#endif
