#if defined(_MSC_VER)
	#pragma warning( disable : 4244 )
#endif

#include <tuttle/common/utils/global.hpp>
#include <tuttle/plugin/ImageGilProcessor.hpp>
#include <tuttle/plugin/PluginException.hpp>
#include <tuttle/common/image/gilGlobals.hpp>

#include <cmath>
#include <vector>
#include <ofxsImageEffect.h>
#include <ofxsMultiThread.h>
#include <boost/gil/gil_all.hpp>
#include <boost/filesystem/fstream.hpp>

#include "lutEngine/lut_reader.h"
#include "lutEngine/hd3d_lut.h"
#include "lutEngine/hd3d_tetra_interpolator.h"

#include "LutProcess.hpp"
#include "LutDefinitions.hpp"

namespace tuttle {
namespace plugin {
namespace lut {

using namespace boost::gil;
using namespace boost::filesystem;

template<class View>
LutProcess<View>::LutProcess( LutPlugin& instance )
	: ImageGilFilterProcessor<View>( instance ),
	_plugin( instance )
{
    _lut3D = &_plugin.lut3D();
}

/**
 * @brief Function called by rendering thread each time
 *        a process must be done.
 *
 * @param[in] procWindow  Processing window
 */
template<class View>
void LutProcess<View>::multiThreadProcessImages( const OfxRectI& procWindow )
{
	applyLut( this->_dstView, this->_srcView, procWindow );
}

template<class View>
void LutProcess<View>::applyLut( View& dst, View& src, const OfxRectI& procWindow )
{
	typedef typename View::x_iterator vIterator;
	typedef typename channel_type<View>::type Pixel;

	for( int y = procWindow.y1; y < procWindow.y2; ++y )
	{
		vIterator sit = src.row_begin( y );
		vIterator dit = dst.row_begin( y );
		for( int x = procWindow.x1; x < procWindow.x2; ++x )
		{
			Color col = _lut3D->getColor( ( *sit )[0], ( *sit )[1], ( *sit )[2] );
			( *dit )[0] = static_cast<Pixel>(col.x);
			( *dit )[1] = static_cast<Pixel>(col.y);
			( *dit )[2] = static_cast<Pixel>(col.z);
			if( dst.num_channels() > 3 )
				( *dit )[3] = channel_traits< typename channel_type< View >::type >::max_value();
			++sit;
			++dit;
		}
		if( this->progressForward() )
			return;
	}
}

}
}
}
