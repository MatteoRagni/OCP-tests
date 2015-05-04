/*--------------------------------------------------------------------------*\
 |  file: BikeSteering_Guess.cc                                             |
 |                                                                          |
 |  version: 1.0   date 27/4/2015                                           |
 |                                                                          |
 |  Copyright (C) 2015                                                      |
 |                                                                          |
 |      Enrico Bertolazzi and Francesco Biral and Paolo Bosetti             |
 |      Dipartimento di Ingegneria Industriale                              |
 |      Universita` degli Studi di Trento                                   |
 |      Via Mesiano 77, I-38050 Trento, Italy                               |
 |      email: enrico.bertolazzi@unitn.it                                   |
 |             francesco.biral@unitn.it                                     |
 |             paolo.bosetti@unitn.it                                       |
\*--------------------------------------------------------------------------*/


#include "BikeSteering.hh"
#include "BikeSteering_Pars.hh"

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-variable"
#elif defined(__llvm__) || defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#elif defined(_MSC_VER)
#pragma warning( disable : 4100 )
#endif

using namespace MechatronixCore ;
using namespace MechatronixSolver ;

namespace BikeSteeringDefine {

  void
  BikeSteering::guess_eval( indexType            numNodes,
                            Q_const_pointer_type _q,
                            Z_pointer_type       _z,
                            P_pointer_type       _p,
                            OMEGA_pointer_type   _omega ) const {
    valueConstPointer q = _q.pointer() ;
    valuePointer      z = _z.pointer() ;
    valuePointer      p = _p.pointer() ;
    valuePointer  omega = _omega.pointer() ;
    // initialize parameters (if any)
        

    // initialize states and multiplier
    std::fill( z, z+numNodes*numZvars, 0 ) ;
    for ( indexType i = 0 ; i < numNodes ; ++i ) {
      valuePointer      __Z = z + i * numZvars ;
      valueConstPointer __Q = q + i * numQvars ;
      // modify the following code if necessary
      indexType i_segment = pMesh->nodeToSegment(i) ;
      MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;
      
      __Z[ iZ_TimeSize ] = 1;

    }
    // initialize boundary multiplier
    for ( indexType i = 0 ; i < numOMEGAvars ; ++i ) omega[i] = 0 ;
  }

  #define Xoptima__check__lt(A,B) ASSERT( (A) <  (B), "check_if_admissible failed <  check!" )
  #define Xoptima__check__le(A,B) ASSERT( (A) <= (B), "check_if_admissible failed <= check!" )

  void
  BikeSteering::check_if_admissible( indexType                numNodes,
                                     Q_const_pointer_type     _q,
                                     Z_const_pointer_type     _z,
                                     P_const_pointer_type     _p,
                                     OMEGA_const_pointer_type _omega ) const {
  }
}
