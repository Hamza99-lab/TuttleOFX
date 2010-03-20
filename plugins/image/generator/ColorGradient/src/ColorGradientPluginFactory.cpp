#include "ColorGradientPlugin.hpp"
#include "ColorGradientDefinitions.hpp"

#include <tuttle/plugin/ImageGilProcessor.hpp>
#include <tuttle/plugin/Progress.hpp>
#include <tuttle/plugin/PluginException.hpp>

#include <string>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ofxsImageEffect.h>
#include <ofxsMultiThread.h>
#include <boost/gil/gil_all.hpp>
#include <boost/scoped_ptr.hpp>

namespace tuttle {
namespace plugin {
namespace colorGradient {

static const bool   kSupportTiles = true;

mDeclarePluginFactory( ColorGradientPluginFactory, { }, { } );


/**
 * @brief Function called to describe the plugin main features.
 * @param[in, out]   desc     Effect descriptor
 */
void ColorGradientPluginFactory::describe( OFX::ImageEffectDescriptor &desc )
{
	desc.setLabel( "ColorGradient" );
	desc.setPluginGrouping( "tuttle" );

	// add the supported contexts
	desc.addSupportedContext( OFX::eContextGeneral );
	desc.addSupportedContext( OFX::eContextFilter );

	// add supported pixel depths
	desc.addSupportedBitDepth( OFX::eBitDepthUByte );
	desc.addSupportedBitDepth( OFX::eBitDepthUShort );
	desc.addSupportedBitDepth( OFX::eBitDepthFloat );

	// plugin flags
	desc.setSupportsTiles( kSupportTiles );
}

/**
 * @brief Function called to describe the plugin controls and features.
 * @param[in, out]   desc       Effect descriptor
 * @param[in]        context    Application context
 */
void ColorGradientPluginFactory::describeInContext( OFX::ImageEffectDescriptor &desc,
                                                    OFX::ContextEnum context )
{
	OFX::ClipDescriptor *srcClip = desc.defineClip( kOfxImageEffectSimpleSourceClipName );
	srcClip->addSupportedComponent( OFX::ePixelComponentRGBA );
	srcClip->addSupportedComponent( OFX::ePixelComponentAlpha );
	srcClip->setSupportsTiles( kSupportTiles );

	// Create the mandated output clip
	OFX::ClipDescriptor *dstClip = desc.defineClip( kOfxImageEffectOutputClipName );
	dstClip->addSupportedComponent( OFX::ePixelComponentRGBA );
	dstClip->addSupportedComponent( OFX::ePixelComponentAlpha );
	dstClip->setSupportsTiles( kSupportTiles );

	OFX::ChoiceParamDescriptor* gradientType = desc.defineChoiceParam( kGradientType );
	gradientType->appendOption( kGradientType1DLinear );
	gradientType->appendOption( kGradientType1DRadial );
	gradientType->appendOption( kGradientType2D );
	gradientType->setDefault( 0 );

	OFX::IntParamDescriptor* nbPoint = desc.defineIntParam( kNbPoints );
	nbPoint->setDefault( 2 );
	nbPoint->setRange( 2, kMaxNbPoints );
	nbPoint->setDisplayRange( 2, kMaxNbPoints );

	for( unsigned int i = 0; i < kMaxNbPoints; ++i)
	{
		OFX::Double2DParamDescriptor* point = desc.defineDouble2DParam( getPointParamName(i) );
		point->setLabel( getPointParamName(i) );
//		point->setIsSecret( true );
		point->setDoubleType( OFX::eDoubleTypeNormalisedXYAbsolute );
		OFX::RGBAParamDescriptor* color = desc.defineRGBAParam( getColorParamName(i) );
		color->setLabel( getColorParamName(i) );
//		color->setIsSecret( true );
	}
}

/**
 * @brief Function called to create a plugin effect instance
 * @param[in] handle  effect handle
 * @param[in] context    Application context
 * @return  plugin instance
 */
OFX::ImageEffect* ColorGradientPluginFactory::createInstance( OfxImageEffectHandle handle,
                                                            OFX::ContextEnum context )
{
	return new ColorGradientPlugin(handle);
}

}
}
}

namespace OFX {
namespace Plugin {

void getPluginIDs(OFX::PluginFactoryArray &ids)
{
	static tuttle::plugin::colorGradient::ColorGradientPluginFactory p( "fr.tuttle.colorgradient", 1, 0 );
	ids.push_back( &p );
}

}
}
