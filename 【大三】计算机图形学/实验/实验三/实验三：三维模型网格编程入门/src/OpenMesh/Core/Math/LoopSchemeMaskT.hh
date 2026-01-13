/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *      Copyright (C) 2001-2005 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openmesh.org                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 *                                                                           *
 *                                License                                    *
 *                                                                           *
 *  This library is free software; you can redistribute it and/or modify it  *
 *  under the terms of the GNU Library General Public License as published   *
 *  by the Free Software Foundation, version 2.                              *
 *                                                                           *
 *  This library is distributed in the hope that it will be useful, but      *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of               *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU        *
 *  Library General Public License for more details.                         *
 *                                                                           *
 *  You should have received a copy of the GNU Library General Public        *
 *  License along with this library; if not, write to the Free Software      *
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                *
 *                                                                           *
\*===========================================================================*/

#ifndef LOOPSCHEMEMASKT_HH
#define LOOPSCHEMEMASKT_HH

#include <math.h>
#include <vector>

#include <OpenMesh/Core/Utils/SingletonT.hh>

namespace OpenMesh
{

/** implements cache for the weights of the original Loop scheme
    supported:
      - vertex projection rule on the next level
      - vertex projection rule on the limit surface
      - vertex projection rule on the k-th (level) step (Barthe, Kobbelt'2003)
      - vertex tangents on the limit surface
*/

template <class T_, unsigned int cache_size_ = 100>
class LoopSchemeMaskT
{
public:
  enum { cache_size = cache_size_ };
  typedef T_                                Scalar;
  typedef unsigned int                      uint;

protected:

  Scalar                                    proj_weights_[cache_size];
  Scalar                                    limit_weights_[cache_size];
  Scalar                                    step_weights_[cache_size];
  std::vector<Scalar>                       tang0_weights_[cache_size];
  std::vector<Scalar>                       tang1_weights_[cache_size];

protected:

  inline static Scalar                      compute_proj_weight(uint _valence)
  {
    //return pow(3.0 / 2.0 + cos(2.0 * M_PI / _valence), 2) / 2.0 - 1.0;
    double denom = (3.0 + 2.0*cos(2.0*M_PI/(double)_valence));
    double weight = (64.0*_valence)/(40.0 - denom*denom) - _valence;
    return (Scalar) weight;
  }

  inline static Scalar                      compute_limit_weight(uint _valence)
  {
    double proj_weight = compute_proj_weight(_valence);
    proj_weight = proj_weight/(proj_weight + _valence);//normalize the proj_weight
    double weight = (3.0/8.0)/(1.0 - proj_weight + (3.0/8.0));
    return (Scalar)weight;
  }

  inline static Scalar                      compute_step_weight(uint _valence)
  {
    double proj_weight = compute_proj_weight(_valence);
    proj_weight = proj_weight/(proj_weight + _valence);//normalize the proj_weight
    double weight = proj_weight - (3.0/8.0);
    return (Scalar)weight;
  }

  inline static Scalar                      compute_tang0_weight(uint _valence, uint _ver_id)
  {
    return (Scalar)cos(2.0*M_PI*(double)_ver_id/(double)_valence);
  }

  inline static Scalar                      compute_tang1_weight(uint _valence, uint _ver_id)
  {
    return (Scalar)sin(2.0*M_PI*(double)_ver_id/(double)_valence);
  }

  void                                      cache_weights()
  {
    proj_weights_[0] = 1;
    for (uint k = 1; k < cache_size; ++k)
    {
      proj_weights_[k] = compute_proj_weight(k);
      limit_weights_[k] = compute_limit_weight(k);
      step_weights_[k] = compute_step_weight(k);
      tang0_weights_[k].resize(k);
      tang1_weights_[k].resize(k);
      for (uint i = 0; i < k; ++i)
      {
        tang0_weights_[k][i] = compute_tang0_weight(k,i);
        tang1_weights_[k][i] = compute_tang1_weight(k,i);
      }
    }
  }

public:

  LoopSchemeMaskT()
  {
    cache_weights();
  }

  inline Scalar                             proj_weight(uint _valence) const
  {
    assert(_valence < cache_size );
    return proj_weights_[_valence];
  }

  inline Scalar                             limit_weight(uint _valence) const
  {
    assert(_valence < cache_size );
    return limit_weights_[_valence];
  }

  inline Scalar                             step_weight(uint _valence, uint _step) const
  {
    assert(_valence < cache_size);
    return pow(step_weights_[_valence], _step);//can be precomputed
  }

  inline Scalar                             tang0_weight(uint _valence, uint _ver_id) const
  {
    assert(_valence < cache_size );
    assert(_ver_id < _valence);
    return tang0_weights_[_valence][_ver_id];
  }

  inline Scalar                             tang1_weight(uint _valence, uint _ver_id) const
  {
    assert(_valence < cache_size );
    assert(_ver_id < _valence);
    return tang1_weights_[_valence][_ver_id];
  }

  void                                      dump(uint _max_valency = cache_size - 1) const
  {
    assert(_max_valency <= cache_size - 1);
    //CConsole::printf("(k : pw_k, lw_k): ");
    for (uint i = 0;  i <= _max_valency; ++i)
    {
      //CConsole::stream() << "(" << i << " : " << proj_weight(i) << ", " << limit_weight(i) << ", " << step_weight(i,1) << "), ";
    }
    //CConsole::printf("\n");
  }
};

typedef LoopSchemeMaskT<double, 100>        LoopSchemeMaskDouble;
typedef SingletonT<LoopSchemeMaskDouble>    LoopSchemeMaskDoubleSingleton;

};//namespace OpenMesh

#endif//LOOPSCHEMEMASKT_HH

