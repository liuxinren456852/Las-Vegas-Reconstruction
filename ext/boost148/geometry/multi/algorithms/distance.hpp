// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2011 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2011 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2011 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_MULTI_ALGORITHMS_DISTANCE_HPP
#define BOOST_GEOMETRY_MULTI_ALGORITHMS_DISTANCE_HPP


#include <boost/numeric/conversion/bounds.hpp>
#include <boost/range.hpp>

#include <boost148/geometry/multi/core/tags.hpp>
#include <boost148/geometry/multi/core/geometry_id.hpp>
#include <boost148/geometry/multi/core/point_type.hpp>

#include <boost148/geometry/algorithms/distance.hpp>
#include <boost148/geometry/util/select_coordinate_type.hpp>


namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace distance
{


template<typename Geometry, typename MultiGeometry, typename Strategy>
struct distance_single_to_multi
{
    typedef typename strategy::distance::services::return_type<Strategy>::type return_type;

    static inline return_type apply(Geometry const& geometry,
                MultiGeometry const& multi,
                Strategy const& strategy)
    {
        bool first = true;
        return_type mindist;

        for(typename range_iterator<MultiGeometry const>::type it = boost::begin(multi);
                it != boost::end(multi);
                ++it)
        {
            return_type dist = geometry::distance(geometry, *it);
            if (first || dist < mindist)
            {
                mindist = dist;
            }
            first = false;
        }

        return mindist;
    }
};

template<typename Multi1, typename Multi2, typename Strategy>
struct distance_multi_to_multi
{
    typedef typename strategy::distance::services::return_type<Strategy>::type return_type;

    static inline return_type apply(Multi1 const& multi1,
                Multi2 const& multi2, Strategy const& strategy)
    {
        bool first = true;
        return_type mindist;

        for(typename range_iterator<Multi1 const>::type it = boost::begin(multi1);
                it != boost::end(multi1);
                ++it)
        {
            return_type dist = distance_single_to_multi
                <
                    typename range_value<Multi1>::type,
                    Multi2,
                    Strategy
                >::apply(*it, multi2, strategy);
            if (first || dist < mindist)
            {
                mindist = dist;
            }
            first = false;
        }

        return mindist;
    }
};


}} // namespace detail::distance
#endif


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template
<
    typename SingleGeometryTag,
    typename G1,
    typename G2,
    typename Strategy
>
struct distance<SingleGeometryTag, multi_tag, G1, G2, strategy_tag_distance_point_point, Strategy>
    : detail::distance::distance_single_to_multi<G1, G2, Strategy>
{};

template <typename G1, typename G2, typename Strategy>
struct distance<multi_tag, multi_tag, G1, G2, strategy_tag_distance_point_point, Strategy>
    : detail::distance::distance_multi_to_multi<G1, G2, Strategy>
{};

} // namespace dispatch
#endif



}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_MULTI_ALGORITHMS_DISTANCE_HPP
