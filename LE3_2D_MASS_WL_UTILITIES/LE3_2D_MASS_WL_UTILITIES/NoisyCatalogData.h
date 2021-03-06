/**
 * @file LE3_2D_MASS_WL_UTILITIES/NoisyCatalogData.h
 * @date 10/13/20
 * @author user
 *
 * @copyright (C) 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#ifndef _NOISYCATALOGDATA_H
#define _NOISYCATALOGDATA_H

#include "ElementsKernel/Logging.h"
#include <vector>
#include <cstdlib>
#include <iostream>

namespace Euclid {
 namespace WeakLensing {
  namespace TwoDMass {

/**
 * @class NoisyCatalogData
 * @brief
 *
 */
class NoisyCatalogData {

public:

  /**
   * @brief Constructor
   */
  NoisyCatalogData();

  /**
   * @brief Destructor
   */
  virtual ~NoisyCatalogData() = default;

 /**
  * @brief         method to create noisy catalog data
  * @param[in]     <inputData> Catalog Data
  * @return        Catalog Data with noise included in it
 */
 std::vector<std::vector<double> > create_noisy_data(std::vector<std::vector<double> > inputData);

private:

};  // End of NoisyCatalogData class

} /* namespace TwoDMass */
} /* namespace WeakLensing */
} /* namespace Euclid */
#endif
