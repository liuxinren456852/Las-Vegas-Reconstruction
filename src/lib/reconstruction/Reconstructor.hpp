/*
 * Reconstructor.h
 *
 *  Created on: 16.02.2011
 *      Author: Thomas Wiemann
 */

#ifndef RECONSTRUCTOR_H_
#define RECONSTRUCTOR_H_

#include "../geometry/BaseMesh.hpp"
#include "../reconstruction/PointCloudManager.hpp"

namespace lssr
{

/**
 * @brief Interface class for surface reconstruction algorithms
 *        that generate triangle meshes.
 */
template<typename VertexT, typename NormalT>
class Reconstructor
{
public:

    /**
     * @brief Constructs a Reconstructor object using the given point
     *        cloud handler
     */
    Reconstructor(PointCloudManager<VertexT, NormalT> &manager) : m_manager(manager) {}

    /**
     * @brief Generated a triangle mesh representation of the current
     *        point set.
     *
     * @param mesh      A surface representation of the current point
     *                  set.
     */
    virtual void getMesh(BaseMesh<VertexT, NormalT>& mesh) = 0;

protected:

    /// The point cloud manager that handles the loaded point cloud data.
    PointCloudManager<VertexT, NormalT>&      m_manager;
};

} //namespace lssr

#endif /* RECONSTRUCTOR_H_ */