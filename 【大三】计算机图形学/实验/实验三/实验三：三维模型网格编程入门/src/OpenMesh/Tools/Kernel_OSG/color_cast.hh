#ifndef OPENMESH_KERNEL_OSG_COLOR_CAST_HH
#define OPENMESH_KERNEL_OSG_COLOR_CAST_HH

#include <algorithm>
#include <OpenMesh/Core/Utils/color_cast.hh>
#include <OpenSG/OSGGeometry.h>

namespace OpenMesh {

/// Helper struct
/// \internal
template <>
struct color_caster<osg::Color3ub,osg::Color3f>
{
  typedef osg::Color3ub return_type;
  typedef unsigned char ub;

  inline static return_type cast(const osg::Color3f& _src)
  {
    return return_type( (ub)std::min((_src[0]* 255.0f + 0.5f),255.0f),
                        (ub)std::min((_src[1]* 255.0f + 0.5f),255.0f),
                        (ub)std::min((_src[2]* 255.0f + 0.5f),255.0f) );
  }
};

/// Helper struct
/// \internal
template <>
struct color_caster<osg::Color3f,osg::Color3ub>
{
  typedef osg::Color3f return_type;

  inline static return_type cast(const osg::Color3ub& _src)
  {
    return return_type( (float)(_src[0] / 255.0f ),
                        (float)(_src[1] / 255.0f ),
                        (float)(_src[2] / 255.0f ) );
  }
};

} // namespace OpenMesh

#endif // OPENMESH_KERNEL_OSG_COLOR_CAST_HH 
