// $Id: SiTrackerBarrel_geo.cpp 1360 2014-10-27 16:32:06Z Nikiforos.Nikiforou@cern.ch $
//====================================================================
//  AIDA Detector description implementation for LCD
//--------------------------------------------------------------------
//
//  Author     : M.Frank
//
//====================================================================
// Framework include files
#define DD4HEP_USE_SURFACEINSTALL_HELPER DD4hep_SiTrackerBarrelSurfacePlugin
#include "DD4hep/SurfaceInstaller.h"

/// Install measurement surfaces
template <typename UserData> 
void Installer<UserData>::install(DetElement component, PlacedVolume pv)   {
  Volume comp_vol = pv.volume();
  if ( comp_vol.isSensitive() )  {  
    Volume mod_vol  = parentVolume(component);
    DD4hep::Geometry::Box mod_shape(mod_vol.solid()), comp_shape(comp_vol.solid());

    if ( !comp_shape.isValid() || !mod_shape.isValid() )   {
      invalidInstaller("Components and/or modules are not boxes -- invalid shapes");
    }
    else if ( !handleUsingCache(component,comp_vol) )  {
      const double* trans = placementTranslation(component);
      double half_module_thickness = mod_shape->GetDZ();
      double sensitive_z_position  = trans[2];
      double outer_thickness = half_module_thickness + sensitive_z_position;
      double inner_thickness = half_module_thickness - sensitive_z_position;
      Vector3D u(1.,0.,0.), v(0.,1.,0.), n(0.,0.,1.), o(0.,0.,sensitive_z_position);

      VolPlane surf(comp_vol,Type(Type::Sensitive,Type::Measurement1D),
		    inner_thickness, outer_thickness, u, v, n, o);
      addSurface(component,surf);
    }
  }
}