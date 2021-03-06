/**
 * @file LE3_2D_MASS_WL_UTILITIES/Utils.h
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

#ifndef _UTILS_H
#define _UTILS_H

#include "ElementsKernel/Logging.h"
#include  "ElementsKernel/Project.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <math.h>
#include <map>
#include "ST_DM_SchemaApi/SchemaApi.h"

#include "EL_FitsFile/MefFile.h"
//Cfitsio & Healpix Library
#include "fitsio.h"

#include "healpix_cxx/fitshandle.h"
#include "healpix_cxx/pointing.h"
#include "healpix_cxx/healpix_map_fitsio.h"
#include "healpix_cxx/healpix_base.h"
#include "healpix_cxx/healpix_map.h"
#include "healpix_cxx/healpix_data_io.h"
#include "healpix_cxx/healpix_map_fitsio.h"

#include <healpix_cxx/arr.h>

#include <healpix_cxx/alm.h>
#include <healpix_cxx/alm_fitsio.h>
#include <healpix_cxx/alm_healpix_tools.h>
#include <healpix_cxx/alm_powspec_tools.h>

#include <healpix_cxx/powspec.h>
#include <healpix_cxx/powspec_fitsio.h>

using namespace Euclid;
using namespace FitsIO;
namespace fs = boost::filesystem;

namespace Euclid {
 namespace WeakLensing {
  namespace TwoDMass {

 /**
  * @brief        Check the object named fileName to identify the file type
  * @param[in]    <fileName>, <path> name of the file to check
  * @param[in]    <magic>, file Signature to detect file type
  * @return       <bool> file match the signature
 */
 bool checkFileType(fs::path fileName, const std::vector<char>& sign);

 /**
  * @brief    get the current time string
 */
 std::string getDateTimeString();

 /**
  * @brief         check if key exists in file
  * @param[in]     <boost::filesystem::path> filename
  * @param[in]     < string > string of text that need to be search in file
  * @return        return true if input key exists in input file, otherwise false
 */
 bool fileHasField(fs::path fileName, const std::string& key);

 /**
  * @brief         splits string
  * @param[in]     <string> input string
  * @param[out]    < vector <string> > vector of splitted text
  * @return        return vector of splitted text
 */
 std::vector<std::string> split_fits_id(const std::string& text);

 /**
  * @brief         method to return the list of filenames from json file
  * @param[in]     <boost::filesystem::path> workdir
  * @param[in]     <boost::filesystem::path> name of input json file
  * @param[out]    < vector <boost::filesystem::path> > vector of filenames
  * @return        return vector of filenames
 */
 std::vector<fs::path> read_filenames(fs::path workdir, fs::path inputJsonFile);

 /**
  * @brief         method to give the Minimum and maximum Value from one vector
  * @param[in]     <Vector> Vector to find the min & max values
  * @param[out]    <vecMin> Minimum Values that exists inside the vector
  * @param[out]    <vecMax> Maximum Values that exists inside the vector
  * @return        true, if it processed correctly
 */
  bool vecMinMax(std::vector <double>& vector, double *vecMin, double *vecMax);

 /**
  * @brief         method to check if input catalog is according to Data Model
  * @param[in]     <string> input filename
  * @param[in]     <Vector> Vector of column names according to Data Model
  * @return        true, if input catalog is according to Data Model
 */
  //bool checkFileIsEuclidize(const std::string& filename, std::vector<std::string>& colname); 

 /**
  * @brief         method to get column names based on shear method & according to Data Model
  * @param[in]     <string> Type of shear method
  * @param[out]    <Vector> Vector of column names according to Data Model
  * @return        column names based on shear method & according to Data Model
 */
  std::vector<std::string> getcolumnNames (const std::string& catType);

 /**
  * @brief         method to get column name index
  * @param[in]     <Vector> Vector of column names according to Data Model
  * @param[in]     <string> string tobe find in Vector of column names
  * @param[out]    <int> column name index
  * @return        column name index
 */
  int getIndexCol(std::vector<std::string>& colname, std::string string);

 /**
  * @brief         method to get column name after comparing sub-string from column
  * @param[in]     <Vector> Vector of column names according to Data Model
  * @param[in]     <string> Substring1 tobe find in Vector of column names
  * @param[in]     <string> Substring2 tobe find in Vector of column names
  * @param[out]    <string> name of required column
  * @return        required column name
 */
  std::string getColName(std::vector<std::string>& colname, std::string substring1, std::string substring2);
 /**
  * @brief         method to get column name after comparing sub-string from column
  * @param[in]     <Vector> Vector of column names according to Data Model
  * @param[in]     <string> Substring tobe find in Vector of column names
  * @param[out]    <string> name of required column
  * @return        required column name
 */
  std::string getColName(std::vector<std::string>& colname, std::string substring);

  /**
  @brief  This method read input fitsfile that contain Healpix Map information
  @param  <Map> name of the input fits file
  @retun  <mapE, mapB> map in healpix format
  */
  std::pair<Healpix_Map<double>, Healpix_Map<double> > readHealpixMap(const std::string &Map);

  /**
  @brief  This method returns Sky coordinates (Ra, Dec) using Pixel index of healpix map
  @param  <Map> input Healpix Map
  @param  <index> Pixel index of healpix map
  @retun  <double, double> Sky coordinates (Ra, Dec)
  */
  std::pair<double, double> getIndex2RaDec(Healpix_Map<double>& map, int index);

 /**
  * @brief    get the keyword file including path
 */
 //static boost::filesystem::path getKeywordFile(const std::string& fitsId);

 /**
  *@brief  XML File signature to detect file type
  *@detail source: https://en.wikipedia.org/wiki/List_of_file_signatures
 */
 const std::vector<char> signXML = {0x3c, 0x3f, 0x78, 0x6d, 0x6c, 0x20};

 /**
  *@brief  FITS file signature to detect file type
  *@detail source: https://en.wikipedia.org/wiki/List_of_file_signatures
 */
 const std::vector<char> signFITS = {0x53, 0x49, 0x4D, 0x50, 0x4C, 0x45, 0x20, 0x20,
                                   0x3D, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                   0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                   0x20, 0x20, 0x20, 0x20, 0x20, 0x54};
 /**
  *@brief  normalisation for different scale map
 */
 const std::vector<double> norm = {0.8811, 0.1988, 0.0846, 0.0408, 0.0202, 0.01014, 0.0051, 0.00382, 0., 0.};

 /**
  *@brief  normalisation for different Spherical scale map
 */
 const std::vector<double> SphericalNorm = {0.8980439031689823, 0.1794034833116207, 0.08969876606779066,
                0.04487843144318756, 0.022418383118745656, 0.011220793531292756, 0.005608036372105656,
                0.0028126266820498905, 0.0013715941244536187, 0.0006964483614277237, 0.0003751679040680689,
                0.0001904098857186993, 0.00012508689538739463};

 /**
  *@brief  normalisation for different scale map
 */
 const std::vector<double> NormB3Spline = {0.889434,  0.200105, 0.0857724, 0.0413447, 0.0202689,
                                                       0.00995628, 0.00513504, 0., 0., 0.};

  /**
   * @enum      mapType
   * @brief     Type of maps
   */
 enum class mapType {shearMap,/**< shear map */
                     convMap/**< convergence map */
                    };

  /**
   * @enum      catalogType
   * @brief     Type of maps
   */
 enum class catalogType {KSBCatalog,/**< shearCatalog (KSBCatalog) */
                         LensMCCatalog,/**< shearCatalog (LensMCCatalog) */
                         MomentsMLCatalog,/**< shearCatalog (MomentsMLCatalog) */
                         RegaussCatalog,/**< shearCatalog (RegaussCatalog) */
                         LE2Catalog,/**< LE2Catalog (LE2Catalog for all WL PFs) */
                         clusterCatalog,/**< clusterCatalog (clusterCatalog) */
                         VisibilityMask/**< Visibility Mask (VMPZ_ID) */
                        };

 /**
 * @brief This Enum defines the types of outputs created by this Software as required by PF
 *
 */
 enum class outputType {NoisedPatch,/**< Noisy convergence patch */
                        DenoisedPatch,/**< Denoisy convergence patch */
                        SNRPatch,/**< SNR convergence patch */
                        NoisedSphere,/**< Noisy Sphere convergence */
                        DenoisedSphere,/**< Denoisy Sphere convergence */
                        SNRSphere,/**< SNR Sphere convergence */
                        MCSphere,/**< Monte Carlo convergence */
                        GalCountSphere,/**< Galaxy Count Sphere convergence */
                        ClusterCatalog, /**< convergence maps (tar of single clusters) for all clusters */
                        SingleClusterCatalog,/**< convergence patch based on single cluster*/
                        PeakCatalog,/**< Peak count catalog */
                        NoisedPatchesSphere,/**< Noisy Patches to Sphere convergence */
                        ProjCenterPos,/**< Centers position for patches to sphere convergence */
                        DenoisedPatchesSphere,/**< Denoisy Patches to Sphere convergence */
                        SNRPatchesSphere,/**< SNR Patches to Sphere convergence */
                        GalCountPatchesSphere,/**< Galaxy Count for Patches Sphere convergence */
                        MCPatchesSphere/**< Monte Carlo convergence patches */
                       };

 /**
  * @brief Generate a the column with data / without unit.
  */
 template <typename T>
 VecColumn<T> generateColumn(const std::string &name, std::vector<double> data) {
   return VecColumn<T>({ name, "", 1 }, data);
 }

 /**
  * @brief    FLOAT_EPSILON a constant value
 */
 constexpr double FLOAT_EPSILON = 5.96047e-08;

 /**
  * @brief    deg2rad (degree to radian conversion parameter) a constant value
 */
 static const constexpr double deg2rad = M_PI/double(180);
 /**
  * @brief    rotPhi a constant value
 */
 static const constexpr double rotPhi = double(0);

 /**
  * @brief    rotPhi a constant value
 */
 static const constexpr double SIGMA2FWHM = sqrt(8. * log(2));

 /**
  * @brief    It defines the version of the software
 */
 const std::string SWVersion = Elements::Project::versionString();

} /* namespace TwoDMass */
} /* namespace WeakLensing */
} /* namespace Euclid */

#endif
