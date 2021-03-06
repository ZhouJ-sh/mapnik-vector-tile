#ifndef __MAPNIK_VECTOR_TILE_GEOMETRY_DECODER_H__
#define __MAPNIK_VECTOR_TILE_GEOMETRY_DECODER_H__

// mapnik-vector-tile
#include "vector_tile_config.hpp"

//protozero
#include <protozero/pbf_reader.hpp>

//mapnik
#include <mapnik/box2d.hpp>
#include <mapnik/geometry.hpp>
#if defined(DEBUG)
#include <mapnik/debug.hpp>
#endif

//std
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace mapnik 
{ 
    
namespace vector_tile_impl 
{

// NOTE: this object is for one-time use.  Once you've progressed to the end
//       by calling next(), to re-iterate, you must construct a new object
class GeometryPBF 
{
public:
    using value_type = std::int64_t;
    using pbf_itr = std::pair<protozero::pbf_reader::const_uint32_iterator, protozero::pbf_reader::const_uint32_iterator >;
    
    explicit GeometryPBF(pbf_itr const& geo_iterator);

    enum command : uint8_t
    {
        end = 0,
        move_to = 1,
        line_to = 2,
        close = 7
    };

    uint32_t get_length() const
    {
        return length;
    }

    command point_next(value_type & rx, value_type & ry);
    command line_next(value_type & rx, value_type & ry, bool skip_lineto_zero);
    command ring_next(value_type & rx, value_type & ry, bool skip_lineto_zero);

private:
    std::pair< protozero::pbf_reader::const_uint32_iterator, protozero::pbf_reader::const_uint32_iterator > geo_iterator_;
    value_type x, y;
    value_type ox, oy;
    uint32_t length;
    uint8_t cmd;
    #if defined(DEBUG)
public:
    bool already_had_error;
    #endif
};

template <typename value_type>
MAPNIK_VECTOR_INLINE mapnik::geometry::geometry<value_type> decode_geometry(GeometryPBF & paths, 
                                                                            std::int32_t geom_type, 
                                                                            unsigned version,
                                                                            value_type tile_x,
                                                                            value_type tile_y,
                                                                            double scale_x,
                                                                            double scale_y,
                                                                            mapnik::box2d<double> const& bbox);

template <typename value_type>
MAPNIK_VECTOR_INLINE mapnik::geometry::geometry<value_type> decode_geometry(GeometryPBF & paths, 
                                                                            std::int32_t geom_type, 
                                                                            unsigned version,
                                                                            value_type tile_x,
                                                                            value_type tile_y,
                                                                            double scale_x,
                                                                            double scale_y);

} // end ns vector_tile_impl

} // end ns mapnik

#if !defined(MAPNIK_VECTOR_TILE_LIBRARY)
#include "vector_tile_geometry_decoder.ipp"
#endif

#endif // __MAPNIK_VECTOR_TILE_GEOMETRY_DECODER_H__
