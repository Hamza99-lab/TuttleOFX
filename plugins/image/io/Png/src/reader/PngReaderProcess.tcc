#include "PngReaderDefinitions.hpp"
#include "PngReaderProcess.hpp"
#include "PngReaderPlugin.hpp"

#include <terry/globals.hpp>
#include <tuttle/plugin/exceptions.hpp>

#include <boost/gil.hpp>
#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <boost/filesystem/fstream.hpp>

#include <boost/scoped_ptr.hpp>
#include <boost/assert.hpp>

namespace tuttle
{
namespace plugin
{
namespace png
{
namespace reader
{

using namespace boost::gil;
namespace bfs = boost::filesystem;

typedef any_image<boost::mpl::vector<gray8_image_t, gray16_image_t, gray32f_image_t, rgba8_image_t, rgba16_image_t,
                                     rgba32f_image_t, rgb8_image_t, rgb16_image_t, rgb32f_image_t> > any_image_t;
typedef any_image_t::view_t any_view_t;

template <class View>
PngReaderProcess<View>::PngReaderProcess(PngReaderPlugin& instance)
    : ImageGilProcessor<View>(instance, eImageOrientationFromTopToBottom)
    , _plugin(instance)
{
    this->setNoMultiThreading();
}

template <class View>
void PngReaderProcess<View>::setup(const OFX::RenderArguments& args)
{
    ImageGilProcessor<View>::setup(args);

    _params = _plugin.getProcessParams(args.time);
}

/**
 * @brief Function called by rendering thread each time a process must be done.
 * @param[in] procWindowRoW  Processing window in RoW
 */
template <class View>
void PngReaderProcess<View>::multiThreadProcessImages(const OfxRectI& procWindowRoW)
{
    // no tiles and no multithreading supported
    BOOST_ASSERT(procWindowRoW == this->_dstPixelRod);
    readImage(this->_dstView);
}

/**
 */
template <class View>
View& PngReaderProcess<View>::readImage(View& dst)
{
    any_image_t anyImg;

    try
    {
        png_read_image(_params._filepath, anyImg);
        any_view_t srcView = view(anyImg);
        srcView = subimage_view(srcView, 0, 0, dst.width(), dst.height());
        copy_and_convert_pixels(srcView, dst);
    }
    catch(boost::exception& e)
    {
        e << exception::user("Png: Unable to read information in file.");
        e << exception::filename(_params._filepath);
        throw;
    }
    catch(...)
    {
        BOOST_THROW_EXCEPTION(exception::File() << exception::user("Png: Unable to read information in file.")
                                                << exception::dev(boost::current_exception_diagnostic_information())
                                                << exception::filename(_params._filepath));
    }
    return dst;
}
}
}
}
}
