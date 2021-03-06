/**
 * @file src/lib/GetSphericalMCMaps.cpp
 * @date 10/13/20
 * @author vkansal
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

#include "LE3_2D_MASS_WL_SPHERICAL/GetSphericalMCMaps.h"
#include "LE3_2D_MASS_WL_UTILITIES/NoisyCatalogData.h"
#include "ElementsKernel/Temporary.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace Euclid::WeakLensing::TwoDMass;
using namespace Euclid::WeakLensing::TwoDMass::Spherical;
using Euclid::WeakLensing::TwoDMass::Spherical::SphericalIO;

// handle on created path names
boost::filesystem::path temp_path;
static Elements::Logging logger = Elements::Logging::getLogger("SphericalMC");

namespace LE3_2D_MASS_WL_SPHERICAL {

 GetSphericalMCMaps::GetSphericalMCMaps(std::vector<std::vector<double> >& inputData): m_inData(inputData) {}

 GetSphericalMCMaps::GetSphericalMCMaps(std::vector<std::vector<double> >& inputData,
                     LE3_2D_MASS_WL_SPHERICAL::SphericalParam &SphParam): m_inData(inputData),
                     m_sphericalParam(SphParam)
{ }

 std::pair<Healpix_Map<double>, Healpix_Map<double> > GetSphericalMCMaps::getDeNoisedShearMap() {

  // Object to perform extraction
   Sph_map_maker mapMaker(m_sphericalParam);
   //std::pair<Healpix_Map<double>, Healpix_Map<double> > shearPair =
   auto [ Shear1, Shear2, GalCount ] =
                              mapMaker.create_ShearMap(m_inData);
   Sph_mass_mapping mapping;
   std::pair<Healpix_Map<double>, Healpix_Map<double> > kappaPair =
                      mapping.create_SheartoConvMap(Shear1, Shear2);

   int nside = kappaPair.first.Nside();

   Healpix_Map<double> mapE;
   mapE.SetNside(nside, RING);

   Healpix_Map<double> mapB;
   mapB.SetNside(nside, RING);

   mapE = kappaPair.first;
   mapB = kappaPair.second;
   //Apply Filter
   if (fabs(m_sphericalParam.getSigmaGauss())>0.001){
     applyGaussianFilter_hp(mapE, m_sphericalParam.getSigmaGauss());
     //applyGaussianFilter_hp(mapB, m_sphericalParam.getSigmaGauss());
   }

   // Perform inverse mass mapping
   //std::pair<Healpix_Map<double>, Healpix_Map<double> > k2shearPair =
   //                     mapping.create_ConvtoShearMap(kappaPair.first, kappaPair.second);
   std::pair<Healpix_Map<double>, Healpix_Map<double> > k2shearPair =
                        mapping.create_ConvtoShearMap(mapE, mapB);
   std::pair<Healpix_Map<double>, Healpix_Map<double> > convergencePair = std::make_pair(mapE, mapB);
   mapping.computeReducedShear_hp(k2shearPair, convergencePair);

   return k2shearPair;
 }

  std::vector<std::pair<Healpix_Map<double>, Healpix_Map<double> > >
                           GetSphericalMCMaps::getNoisedShearMaps() {
    int Niter = m_sphericalParam.getNResamples();
    logger.info() << "Number of Nresamples: " << Niter;
    std::vector<std::pair<Healpix_Map<double>, Healpix_Map<double> > > ShearMapList;
    if (Niter <= 0) {
      Niter = 1;
    }
    for (int i = 0; i !=Niter; ++i) {
      std::vector<std::vector<double> > RanData;
      NoisyCatalogData randomise;
      RanData = randomise.create_noisy_data(m_inData);
      Sph_map_maker mapMaker(m_sphericalParam);

      auto [ Shear1, Shear2, GalCount ] = mapMaker.create_ShearMap(RanData);
      std::pair<Healpix_Map<double>, Healpix_Map<double> > shearPair = std::make_pair(Shear1, Shear2);
      ShearMapList.push_back(shearPair);

    }

    return ShearMapList;
  }

  bool GetSphericalMCMaps::performAddition(std::pair<Healpix_Map<double>, Healpix_Map<double> >& DenoisedShearMap,
               std::pair<Healpix_Map<double>, Healpix_Map<double> >& NoisedShearMap, const std::string& ShearMap) {
    Healpix_Base hb;
    unsigned int npix = (DenoisedShearMap.first).Npix();
    logger.info()<<"npix: "<<npix;
    int order = hb.nside2order((DenoisedShearMap.first).Nside());
    Healpix_Map<double> g1_hmap(order, RING);
    Healpix_Map<double> g2_hmap(order, RING);
    for (unsigned int id_pix=0; id_pix<npix; id_pix++){
       //g1_hmap[id_pix] = (DenoisedShearMap.first)[id_pix] + (NoisedShearMap.first)[id_pix];
       double r1 = (1. + (((DenoisedShearMap.first)[id_pix] * (NoisedShearMap.first)[id_pix]) - 
                   ((-1. *(DenoisedShearMap.second)[id_pix]) * (NoisedShearMap.second)[id_pix])));
       double i1 = ((DenoisedShearMap.first)[id_pix] * (NoisedShearMap.second)[id_pix]) + 
                   ((-1. * (DenoisedShearMap.second)[id_pix]) * (NoisedShearMap.first)[id_pix]);
       double r2 = (NoisedShearMap.first)[id_pix] + (DenoisedShearMap.first)[id_pix];
       double i2 = (NoisedShearMap.second)[id_pix] + (DenoisedShearMap.second)[id_pix];
       double valE = ((r2*r1) + (i2*i1))/(pow(r1, 2) + pow(i1, 2));
       double valB = ((i2*r1) - (i1*r2))/(pow(r1, 2) + pow(i1, 2));
       g1_hmap[id_pix] = valE;
       g2_hmap[id_pix] = valB;
    }
    SphericalIO SphericalIO(m_sphericalParam);
    SphericalIO.write_Map(ShearMap, g1_hmap, "GAMMA1");
    SphericalIO.write_Map(ShearMap, g2_hmap, "GAMMA2");
    return true;
  }

}  // namespace LE3_2D_MASS_WL_SPHERICAL
