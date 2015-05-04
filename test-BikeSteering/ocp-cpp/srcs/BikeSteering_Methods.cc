/*--------------------------------------------------------------------------*\
 |  file: BikeSteering_Methods.cc                                           |
 |                                                                          |
 |  version: 1.0   date 4/5/2015                                            |
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

using namespace std ;
// Mechatronix namespaces
using namespace MechatronixCore ;
using namespace MechatronixAstro ;
using namespace MechatronixSolver ;
using namespace MechatronixInterfaceMruby ;
using namespace MechatronixInterfaceLua ;

// user namespaces
using namespace MechatronixCore ;


#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-variable"
#elif defined(__llvm__) || defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#elif defined(_MSC_VER)
#pragma warning( disable : 4100 )
#endif

namespace BikeSteeringDefine {
  /////////////////////////////////////////////////

 

  /////////////////////////////////////////////////

  valueType
  BikeSteering::penalties_eval( indexType const i_segment,
                                Z_const_pointer_type _Z,
                                U_const_pointer_type _U,
                                Q_const_pointer_type _Q,
                                P_const_pointer_type _P ) const {

    valueConstPointer __Z = _Z.pointer() ;
    valueConstPointer __U = _U.pointer() ;
    valueConstPointer __Q = _Q.pointer() ;
    valueConstPointer __P = _P.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;
    
    valueType t1   =  __Z[2];
    valueType t2   =  minimumTimeSize(t1);
    valueType t4   =  ModelPars[iM_Fmax];
    valueType t5   =  FyControl(__U[0], -t4, t4);
    return t1 * t5 + t2;

  }

  /////////////////////////////////////////////////

  valueType
  BikeSteering::lagrange_target( indexType const i_segment,
                                 Z_const_pointer_type _Z,
                                 U_const_pointer_type _U,
                                 Q_const_pointer_type _Q,
                                 P_const_pointer_type _P ) const {

    valueConstPointer __Z = _Z.pointer() ;
    valueConstPointer __U = _U.pointer() ;
    valueConstPointer __Q = _Q.pointer() ;
    valueConstPointer __P = _P.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;
    
    return 0;

  }

  /////////////////////////////////////////////////

  valueType
  BikeSteering::mayer_target( indexType const      i_segment_left,
                              Z_const_pointer_type _ZL,
                              U_const_pointer_type _UL,
                              Q_const_pointer_type _QL,

                              indexType const      i_segment_right,
                              Z_const_pointer_type _ZR,
                              U_const_pointer_type _UR,
                              Q_const_pointer_type _QR,

                              P_const_pointer_type _P ) const {

    valueConstPointer __ZL = _ZL.pointer() ;
    valueConstPointer __UL = _UL.pointer() ;
    valueConstPointer __QL = _QL.pointer() ;
    valueConstPointer __ZR = _ZR.pointer() ;
    valueConstPointer __UR = _UR.pointer() ;
    valueConstPointer __QR = _QR.pointer() ;
    valueConstPointer __P  = _P.pointer() ;

    
    return __ZR[2];

  }

  /////////////////////////////////////////////////

  indexType BikeSteering::a_numEqns() const { return 6 ; }

  void
  BikeSteering::a_eval( indexType const i_segment,

                        Z_const_pointer_type _ZL,
                        Q_const_pointer_type _QL,

                        Z_const_pointer_type _ZR,
                        Q_const_pointer_type _QR,

                        P_const_pointer_type _P,
  
                        U_const_pointer_type _UL,
                        U_const_pointer_type _UM,
                        U_const_pointer_type _UR,
                        valueType            __result[] ) const {

    valueConstPointer __ZL = _ZL.pointer() ;
    valueConstPointer __QL = _QL.pointer() ;
    valueConstPointer __ZR = _ZR.pointer() ;
    valueConstPointer __QR = _QR.pointer() ;
    valueConstPointer __P  = _P.pointer() ;
    valueConstPointer __UL = _UL.pointer() ;
    valueConstPointer __UM = _UM.pointer() ;
    valueConstPointer __UR = _UR.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;
    valueType __ZM[numZvars], __QM[numQvars] ;
    for ( indexType i=0 ; i<numZvars ; ++i ) __ZM[i] = 0.5*(__ZL[i]+__ZR[i]) ;
    for ( indexType i=0 ; i<numQvars ; ++i ) __QM[i] = 0.5*(__QL[i]+__QR[i]) ;
    
    valueType t1   =  ModelPars[iM_Ix];
    valueType t8   =  __QR[0] - __QL[0];
    valueType t9   =  __ZM[3];
    valueType t11  =  __ZM[2];
    __result[ 0   ] = -t1 * (__ZR[4] - __ZL[4]) - t8 * t9 * t11;
    valueType t15  =  __ZM[4];
    valueType t18  =  ModelPars[iM_m];
    valueType t19  =  ModelPars[iM_g];
    valueType t20  =  t18 * t19;
    valueType t21  =  ModelPars[iM_h];
    __result[ 1   ] = -t11 * t15 * t20 * t21 * t8 + __ZL[3] - __ZR[3];
    valueType t26  =  minimumTimeSize.D(t11);
    valueType t27  =  __UM[0];
    valueType t28  =  ModelPars[iM_Fmax];
    valueType t29  =  FyControl(t27, -t28, t28);
    valueType t30  =  __ZM[0];
    valueType t32  =  __ZM[1];
    __result[ 2   ] = -__ZR[5] + __ZL[5] - t8 * (t26 + t29 + t9 * t30 + t15 * (t20 * t21 * t32 - t21 * t27));
    __result[ 3   ] = -t11 * t30 * t8 - __ZL[1] + __ZR[1];
    __result[ 4   ] = t1 * (__ZR[0] - __ZL[0]) - t8 * (t11 * t18 * t19 * t21 * t32 - t11 * t21 * t27);
    __result[ 5   ] = __ZR[2] - __ZL[2];

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"a_eval",6);
    #endif
  }

  /////////////////////////////////////////////////

  indexType BikeSteering::BC_numEqns() const { return 10 ; }

  void
  BikeSteering::BC_eval( indexType const          i_segment_left,
                         Z_const_pointer_type     _ZL,
                         Q_const_pointer_type     _QL,

                         indexType const          i_segment_right,
                         Z_const_pointer_type     _ZR,
                         Q_const_pointer_type     _QR,

                         P_const_pointer_type     _P,
                         OMEGA_const_pointer_type _OMEGA,
                         valueType                __result[] ) const {

    valueConstPointer __ZL    = _ZL.pointer() ;
    valueConstPointer __QL    = _QL.pointer() ;
    valueConstPointer __ZR    = _ZR.pointer() ;
    valueConstPointer __QR    = _QR.pointer() ;
    valueConstPointer __P     = _P.pointer() ;
    valueConstPointer __OMEGA = _OMEGA.pointer() ;

    MeshStd::SegmentClass const & segmentLeft  = pMesh->getSegmentByIndex(i_segment_left) ;
    MeshStd::SegmentClass const & segmentRight = pMesh->getSegmentByIndex(i_segment_right) ;

    
    __result[ 0   ] = __ZL[0] - ModelPars[iM_omega_i];
    __result[ 1   ] = __ZL[1] - ModelPars[iM_phi_i];
    __result[ 2   ] = __ZR[0] - ModelPars[iM_omega_f];
    __result[ 3   ] = __ZR[1] - ModelPars[iM_phi_f];
    valueType t9   =  ModelPars[iM_Ix];
    __result[ 4   ] = t9 * __ZL[4] + __OMEGA[0];
    __result[ 5   ] = __OMEGA[1] + __ZL[3];
    __result[ 6   ] = __ZL[5];
    __result[ 7   ] = -t9 * __ZR[4] + __OMEGA[2];
    __result[ 8   ] = __OMEGA[3] - __ZR[3];
    __result[ 9   ] = 1 - __ZR[5];

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"BC_eval",10);
    #endif
  }

  /////////////////////////////////////////////////

  indexType BikeSteering::post_numEqns() const { return 6 ; }

  void
  BikeSteering::post_eval( indexType const      i_segment,
                           Z_const_pointer_type _Z,
                           U_const_pointer_type _U,
                           P_const_pointer_type _P,
                           Q_const_pointer_type _Q,
                           valueType            __result[] ) const {

    valueConstPointer __Z = _Z.pointer() ;
    valueConstPointer __U = _U.pointer() ;
    valueConstPointer __P = _P.pointer() ;
    valueConstPointer __Q = _Q.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;

    
    __result[ 0   ] = 0;
    valueType t1   =  __Z[2];
    valueType t2   =  minimumTimeSize(t1);
    valueType t4   =  ModelPars[iM_Fmax];
    valueType t5   =  FyControl(__U[0], -t4, t4);
    __result[ 1   ] = t1 * t5 + t2;
    __result[ 2   ] = t5;
    __result[ 3   ] = t2;
    __result[ 4   ] = t1;
    valueType t15  =  sqrt(ModelPars[iM_m] * ModelPars[iM_g] * ModelPars[iM_h] / ModelPars[iM_Ix]);
    __result[ 5   ] = __result[4] * t15;

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"post_eval",6);
    #endif
  }

  /////////////////////////////////////////////////

  indexType BikeSteering::integrated_post_numEqns() const { return 0 ; }

  void
  BikeSteering::integrated_post_eval( indexType const      i_segment,
                                      Z_const_pointer_type _Z,
                                      U_const_pointer_type _U,
                                      P_const_pointer_type _P,
                                      Q_const_pointer_type _Q,
                                      valueType            __result[] ) const {

    valueConstPointer __Z = _Z.pointer() ;
    valueConstPointer __U = _U.pointer() ;
    valueConstPointer __P = _P.pointer() ;
    valueConstPointer __Q = _Q.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;
    
    

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"integrated_post_eval",0);
    #endif
  }

  /////////////////////////////////////////////////

  indexType BikeSteering::jump_numEqns() const { return 6 ; }

  void
  BikeSteering::jump_eval( indexType const      i_segment_left,
                           Z_const_pointer_type _ZL,
                           Q_const_pointer_type _QL,
  
                           indexType const      i_segment_right,
                           Z_const_pointer_type _ZR,
                           Q_const_pointer_type _QR,
  
                           P_const_pointer_type _P,

                           U_const_pointer_type _UL,
                           U_const_pointer_type _UR,
                           valueType            __result[] ) const {

    valueConstPointer __ZL = _ZL.pointer() ;
    valueConstPointer __QL = _QL.pointer() ;
    valueConstPointer __ZR = _ZR.pointer() ;
    valueConstPointer __QR = _QR.pointer() ;
    valueConstPointer __P  = _P.pointer() ;
    valueConstPointer __UL = _UL.pointer() ;
    valueConstPointer __UR = _UR.pointer() ;

    MeshStd::SegmentClass const & segmentLeft  = pMesh->getSegmentByIndex(i_segment_left) ;
    MeshStd::SegmentClass const & segmentRight = pMesh->getSegmentByIndex(i_segment_right) ;
    
    
    __result[ 0   ] = __ZR[0] - __ZL[0];
    __result[ 1   ] = __ZR[1] - __ZL[1];
    __result[ 2   ] = __ZR[2] - __ZL[2];
    valueType t7   =  ModelPars[iM_Ix];
    __result[ 3   ] = -t7 * __ZL[4] + t7 * __ZR[4];
    __result[ 4   ] = __ZR[3] - __ZL[3];
    __result[ 5   ] = __ZR[5] - __ZL[5];

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"jump_eval",6);
    #endif
  }

  /////////////////////////////////////////////////

  indexType BikeSteering::u_numEqns() const { return 1 ; }

  void
  BikeSteering::u_eval( indexType const      i_segment,
                        Z_const_pointer_type _Z,
                        P_const_pointer_type _P,
                        Q_const_pointer_type _Q,
                        U_pointer_type       _U ) const {
    valuePointer      __U = _U.pointer() ;
    #ifndef MECHATRONIX_U_ITERATIVE
    valueConstPointer __Z = _Z.pointer() ;
    valueConstPointer __P = _P.pointer() ; 
    valueConstPointer __Q = _Q.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;

    
    valueType t4   =  ModelPars[iM_Fmax];
    __U[ iU_Fy ] = FyControl.solve(__Z[4] * ModelPars[iM_h], -t4, t4);

    #else
      u_standard_eval( i_segment, _Z, _P, _Q, _U ) ;
    #endif
    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__U,"u_eval",1);
    #endif
  }

  /////////////////////////////////////////////////

  void
  BikeSteering::um_eval( indexType const      i_segment,
                         Z_const_pointer_type _ZL,
                         Q_const_pointer_type _QL,
                         Z_const_pointer_type _ZR,
                         Q_const_pointer_type _QR,
                         P_const_pointer_type _P,
                         U_pointer_type       _U ) const {
    valueConstPointer __ZL = _ZL.pointer() ;
    valueConstPointer __QL = _QL.pointer() ;
    valueConstPointer __ZR = _ZR.pointer() ;
    valueConstPointer __QR = _QR.pointer() ;
    valueConstPointer __P  = _P.pointer() ; 
    valuePointer      __U  = _U.pointer() ;

    valueType __Z[numZvars], __Q[numQvars] ;
    for ( indexType i=0 ; i<numZvars ; ++i ) __Z[i] = 0.5*(__ZL[i]+__ZR[i]) ;
    for ( indexType i=0 ; i<numQvars ; ++i ) __Q[i] = 0.5*(__QL[i]+__QR[i]) ;
    #ifdef MECHATRONIX_U_ITERATIVE
      Z_const_pointer_type __ZZ(__Z) ;
      Q_const_pointer_type __QQ(__Q) ;
      u_standard_eval( i_segment, __ZZ, _P, __QQ, _U ) ;
    #else

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;

    
    valueType t4   =  ModelPars[iM_Fmax];
    __U[ iU_Fy ] = FyControl.solve(__Z[4] * ModelPars[iM_h], -t4, t4);

    #endif
    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__U,"um_eval",1);
    #endif
  }

  /////////////////////////////////////////////////

  indexType BikeSteering::u_guess_numEqns() const { return 1 ; }

  void
  BikeSteering::u_guess_eval( indexType const      i_segment,
                              Z_const_pointer_type _Z,
                              P_const_pointer_type _P,
                              Q_const_pointer_type _Q,
                              U_pointer_type       _U ) const {
    valueConstPointer __Z = _Z.pointer() ;
    valueConstPointer __P = _P.pointer() ; 
    valueConstPointer __Q = _Q.pointer() ;
    valuePointer      __U = _U.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;
    
    std::fill( __U, __U+1, 0 ) ;
    
    __U[ iU_Fy ] = 0;

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__U,"u_guess_eval",1);
    #endif
  }

  /////////////////////////////////////////////////

  void
  BikeSteering::u_check_if_admissible( indexType const      i_segment,
                                       U_const_pointer_type _U,
                                       Z_const_pointer_type _Z,
                                       P_const_pointer_type _P,
                                       Q_const_pointer_type _Q ) const {
    valueConstPointer __U = _U.pointer() ;
    valueConstPointer __Z = _Z.pointer() ;
    valueConstPointer __P = _P.pointer() ; 
    valueConstPointer __Q = _Q.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;

    try {
        
        valueType t2   =  ModelPars[iM_Fmax];
        FyControl.check_range(__U[0], -t2, t2);

    } catch ( exception const & exc ) {
      DO_ERROR("BikeSteering::u_check_if_admissible: " << exc.what()) ;
    }
  }

  /////////////////////////////////////////////////

  indexType BikeSteering::g_numEqns() const { return 1 ; }

  void
  BikeSteering::g_eval( indexType const      i_segment,
                        U_const_pointer_type _U,
                        Z_const_pointer_type _Z,
                        P_const_pointer_type _P,
                        Q_const_pointer_type _Q,
                        valueType            __result[] ) const {

    valueConstPointer __U = _U.pointer() ;
    valueConstPointer __Z = _Z.pointer() ;
    valueConstPointer __P = _P.pointer() ; 
    valueConstPointer __Q = _Q.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;
    
    
    valueType t1   =  __Z[2];
    valueType t3   =  ModelPars[iM_Fmax];
    valueType t4   =  FyControl.D_1(__U[0], -t3, t3);
    __result[ 0   ] = -__Z[4] * t1 * ModelPars[iM_h] + t1 * t4;

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"g_eval",1);
    #endif
  }

  /////////////////////////////////////////////////

  indexType BikeSteering::q_numEqns() const { return 1 ; }

  void
  BikeSteering::q_eval( indexType           i_node,
                        indexType           i_segment,
                        valueConstReference s,
                        Q_pointer_type      _result ) const {
    valuePointer __result = _result.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;
    
    
    __result[ 0   ] = s;

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"q_eval",1);
    #endif
  }

  /////////////////////////////////////////////////
  indexType BikeSteering::DaDz_numRows() const { return 6 ; }
  indexType BikeSteering::DaDz_numCols() const { return 12 ; }
  indexType BikeSteering::DaDz_nnz()     const { return 40 ; }

  void
  BikeSteering::DaDz_pattern( indexType iIndex[], indexType jIndex[] ) const {
    iIndex[ 0  ] = 0    ; jIndex[ 0  ] = 2    ;
    iIndex[ 1  ] = 0    ; jIndex[ 1  ] = 3    ;
    iIndex[ 2  ] = 0    ; jIndex[ 2  ] = 4    ;
    iIndex[ 3  ] = 0    ; jIndex[ 3  ] = 8    ;
    iIndex[ 4  ] = 0    ; jIndex[ 4  ] = 9    ;
    iIndex[ 5  ] = 0    ; jIndex[ 5  ] = 10   ;
    iIndex[ 6  ] = 1    ; jIndex[ 6  ] = 2    ;
    iIndex[ 7  ] = 1    ; jIndex[ 7  ] = 3    ;
    iIndex[ 8  ] = 1    ; jIndex[ 8  ] = 4    ;
    iIndex[ 9  ] = 1    ; jIndex[ 9  ] = 8    ;
    iIndex[ 10 ] = 1    ; jIndex[ 10 ] = 9    ;
    iIndex[ 11 ] = 1    ; jIndex[ 11 ] = 10   ;
    iIndex[ 12 ] = 2    ; jIndex[ 12 ] = 0    ;
    iIndex[ 13 ] = 2    ; jIndex[ 13 ] = 1    ;
    iIndex[ 14 ] = 2    ; jIndex[ 14 ] = 2    ;
    iIndex[ 15 ] = 2    ; jIndex[ 15 ] = 3    ;
    iIndex[ 16 ] = 2    ; jIndex[ 16 ] = 4    ;
    iIndex[ 17 ] = 2    ; jIndex[ 17 ] = 5    ;
    iIndex[ 18 ] = 2    ; jIndex[ 18 ] = 6    ;
    iIndex[ 19 ] = 2    ; jIndex[ 19 ] = 7    ;
    iIndex[ 20 ] = 2    ; jIndex[ 20 ] = 8    ;
    iIndex[ 21 ] = 2    ; jIndex[ 21 ] = 9    ;
    iIndex[ 22 ] = 2    ; jIndex[ 22 ] = 10   ;
    iIndex[ 23 ] = 2    ; jIndex[ 23 ] = 11   ;
    iIndex[ 24 ] = 3    ; jIndex[ 24 ] = 0    ;
    iIndex[ 25 ] = 3    ; jIndex[ 25 ] = 1    ;
    iIndex[ 26 ] = 3    ; jIndex[ 26 ] = 2    ;
    iIndex[ 27 ] = 3    ; jIndex[ 27 ] = 6    ;
    iIndex[ 28 ] = 3    ; jIndex[ 28 ] = 7    ;
    iIndex[ 29 ] = 3    ; jIndex[ 29 ] = 8    ;
    iIndex[ 30 ] = 4    ; jIndex[ 30 ] = 0    ;
    iIndex[ 31 ] = 4    ; jIndex[ 31 ] = 1    ;
    iIndex[ 32 ] = 4    ; jIndex[ 32 ] = 2    ;
    iIndex[ 33 ] = 4    ; jIndex[ 33 ] = 4    ;
    iIndex[ 34 ] = 4    ; jIndex[ 34 ] = 6    ;
    iIndex[ 35 ] = 4    ; jIndex[ 35 ] = 7    ;
    iIndex[ 36 ] = 4    ; jIndex[ 36 ] = 8    ;
    iIndex[ 37 ] = 4    ; jIndex[ 37 ] = 10   ;
    iIndex[ 38 ] = 5    ; jIndex[ 38 ] = 2    ;
    iIndex[ 39 ] = 5    ; jIndex[ 39 ] = 8    ;

  }

  void
  BikeSteering::DaDz_sparse( indexType const      i_segment,

                             Z_const_pointer_type _ZL,
                             Q_const_pointer_type _QL,

                             Z_const_pointer_type _ZR,
                             Q_const_pointer_type _QR,

                             P_const_pointer_type _P,

                             U_const_pointer_type _UL,
                             U_const_pointer_type _UM,
                             U_const_pointer_type _UR,
                             valueType            __result[] ) const {
    valueConstPointer __ZL = _ZL.pointer() ;
    valueConstPointer __QL = _QL.pointer() ;
    valueConstPointer __ZR = _ZR.pointer() ;
    valueConstPointer __QR = _QR.pointer() ;
    valueConstPointer __P  = _P.pointer() ;
    valueConstPointer __UL = _UL.pointer() ;
    valueConstPointer __UM = _UM.pointer() ;
    valueConstPointer __UR = _UR.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;

    valueType __ZM[numZvars], __QM[numQvars] ;
    valueType invDgDuL_mat[numUvars][numUvars] ;
    valueType invDgDuR_mat[numUvars][numUvars] ;
    valueType invDgDuM_mat[numUvars][numUvars] ;
    for ( indexType i=0 ; i<numZvars ; ++i ) __ZM[i] = 0.5*(__ZL[i]+__ZR[i]) ;
    for ( indexType i=0 ; i<numQvars ; ++i ) __QM[i] = 0.5*(__QL[i]+__QR[i]) ;
    Z_const_pointer_type ___ZM(__ZM) ;
    Q_const_pointer_type ___QM(__QM) ;
    U_const_pointer_type ___UM(__UM) ;
    invDgDu_eval(i_segment,_ZL,_P,_QL,_UL,(valuePointer)invDgDuL_mat) ;
    invDgDu_eval(i_segment,_ZR,_P,_QR,_UR,(valuePointer)invDgDuR_mat) ;
    invDgDu_eval(i_segment,___ZM,_P,___QM,___UM,(valuePointer)invDgDuM_mat) ;
    
    valueType t3   =  __QR[0] - __QL[0];
    __result[ 0   ] = -t3 * __ZM[3] / 2;
    valueType t7   =  __ZM[2];
    valueType t8   =  t3 * t7;
    __result[ 1   ] = -t8 / 2;
    __result[ 2   ] = ModelPars[iM_Ix];
    __result[ 3   ] = __result[0];
    __result[ 4   ] = __result[1];
    __result[ 5   ] = -__result[2];
    valueType t10  =  __ZM[4];
    valueType t14  =  ModelPars[iM_m] * ModelPars[iM_g];
    valueType t15  =  ModelPars[iM_h];
    valueType t16  =  t14 * t15;
    __result[ 6   ] = -t3 * t10 * t16 / 2;
    __result[ 7   ] = 1;
    __result[ 8   ] = -t8 * t16 / 2;
    __result[ 9   ] = __result[6];
    __result[ 10  ] = -1;
    __result[ 11  ] = __result[8];
    __result[ 12  ] = __result[3];
    __result[ 13  ] = __result[9];
    valueType t21  =  minimumTimeSize.DD(t7);
    valueType t23  =  __UM[0];
    valueType t24  =  ModelPars[iM_Fmax];
    valueType t25  =  FyControl.D_1(t23, -t24, t24);
    valueType t27  =  -t10 * t15 + t25;
    valueType t28  =  t27 * t27;
    valueType t30  =  invDgDuM_mat[0][0];
    __result[ 14  ] = t3 * t28 * t30 / 2 - t3 * t21 / 2;
    __result[ 15  ] = -t3 * __ZM[0] / 2;
    __result[ 16  ] = -t3 * (t14 * t15 * __ZM[1] - t15 * t23) / 2 - t3 * t27 * t30 * t7 * t15 / 2;
    __result[ 17  ] = 1;
    __result[ 18  ] = __result[12];
    __result[ 19  ] = __result[13];
    __result[ 20  ] = __result[14];
    __result[ 21  ] = __result[15];
    __result[ 22  ] = __result[16];
    __result[ 23  ] = -1;
    __result[ 24  ] = __result[4];
    __result[ 25  ] = -1;
    __result[ 26  ] = __result[21];
    __result[ 27  ] = __result[24];
    __result[ 28  ] = 1;
    __result[ 29  ] = __result[26];
    __result[ 30  ] = __result[5];
    __result[ 31  ] = __result[11];
    __result[ 32  ] = __result[22];
    valueType t47  =  t7 * t7;
    valueType t49  =  t15 * t15;
    __result[ 33  ] = t3 * t47 * t49 * t30 / 2;
    __result[ 34  ] = __result[2];
    __result[ 35  ] = __result[31];
    __result[ 36  ] = __result[32];
    __result[ 37  ] = __result[33];
    __result[ 38  ] = -1;
    __result[ 39  ] = 1;

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"DaDz_sparse",40);
    #endif
  }

  /////////////////////////////////////////////////
  indexType BikeSteering::DaDp_numRows() const { return 6 ; }
  indexType BikeSteering::DaDp_numCols() const { return 0 ; }
  indexType BikeSteering::DaDp_nnz()     const { return 0 ; }

  void
  BikeSteering::DaDp_pattern( indexType iIndex[], indexType jIndex[] ) const {

  }

  void
  BikeSteering::DaDp_sparse( indexType const i_segment,

                             Z_const_pointer_type _ZL,
                             Q_const_pointer_type _QL,

                             Z_const_pointer_type _ZR,
                             Q_const_pointer_type _QR,

                             P_const_pointer_type _P,

                             U_const_pointer_type _UL,
                             U_const_pointer_type _UM,
                             U_const_pointer_type _UR,
                             valueType            __result[] ) const {

    valueConstPointer __ZL = _ZL.pointer() ;
    valueConstPointer __QL = _QL.pointer() ;
    valueConstPointer __ZR = _ZR.pointer() ;
    valueConstPointer __QR = _QR.pointer() ;
    valueConstPointer __P  = _P.pointer() ;
    valueConstPointer __UL = _UL.pointer() ;
    valueConstPointer __UM = _UM.pointer() ;
    valueConstPointer __UR = _UR.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;

    valueType __ZM[numZvars], __QM[numQvars] ;
    valueType invDgDuL_mat[numUvars][numUvars] ;
    valueType invDgDuR_mat[numUvars][numUvars] ;
    valueType invDgDuM_mat[numUvars][numUvars] ;
    for ( indexType i=0 ; i<numZvars ; ++i ) __ZM[i] = 0.5*(__ZL[i]+__ZR[i]) ;
    for ( indexType i=0 ; i<numQvars ; ++i ) __QM[i] = 0.5*(__QL[i]+__QR[i]) ;
    Z_const_pointer_type ___ZM(__ZM) ;
    Q_const_pointer_type ___QM(__QM) ;
    U_const_pointer_type ___UM(__UM) ;
    invDgDu_eval(i_segment,_ZL,_P,_QL,_UL,(valuePointer)invDgDuL_mat) ;
    invDgDu_eval(i_segment,_ZR,_P,_QR,_UR,(valuePointer)invDgDuR_mat) ;
    invDgDu_eval(i_segment,___ZM,_P,___QM,___UM,(valuePointer)invDgDuM_mat) ;
    

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"DaDp_sparse",0);
    #endif
  }

  /////////////////////////////////////////////////
  indexType BikeSteering::DgDu_numRows() const { return 1 ; }
  indexType BikeSteering::DgDu_numCols() const { return 1 ; }
  indexType BikeSteering::DgDu_nnz()     const { return 1 ; }

  void
  BikeSteering::DgDu_pattern( indexType iIndex[], indexType jIndex[] ) const {
    iIndex[ 0  ] = 0    ; jIndex[ 0  ] = 0    ;

  }

  void
  BikeSteering::DgDu_sparse( indexType const      i_segment,
                             U_const_pointer_type _U,
                             Z_const_pointer_type _Z,
                             P_const_pointer_type _P,
                             Q_const_pointer_type _Q,
                             valueType            __result[] ) const {

    valueConstPointer __U = _U.pointer() ;
    valueConstPointer __Z = _Z.pointer() ;
    valueConstPointer __P = _P.pointer() ;
    valueConstPointer __Q = _Q.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;

    
    valueType t3   =  ModelPars[iM_Fmax];
    valueType t4   =  FyControl.D_1_1(__U[0], -t3, t3);
    __result[ 0   ] = __Z[2] * t4;

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"DgDu_sparse",1);
    #endif
  }

  /////////////////////////////////////////////////
  indexType BikeSteering::DgDp_numRows() const { return 1 ; }
  indexType BikeSteering::DgDp_numCols() const { return 0 ; }
  indexType BikeSteering::DgDp_nnz()     const { return 0 ; }

  void
  BikeSteering::DgDp_pattern( indexType iIndex[], indexType jIndex[] ) const {

  }

  void
  BikeSteering::DgDp_sparse( indexType const      i_segment,
                             U_const_pointer_type _U,
                             Z_const_pointer_type _Z,
                             P_const_pointer_type _P,
                             Q_const_pointer_type _Q,
                             valueType            __result[] ) const {

    valueConstPointer __U = _U.pointer() ;
    valueConstPointer __Z = _Z.pointer() ;
    valueConstPointer __P = _P.pointer() ;
    valueConstPointer __Q = _Q.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;

    

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"DgDp_sparse",0);
    #endif
  }

  /////////////////////////////////////////////////
  indexType BikeSteering::DBCDz_numRows() const { return 10 ; }
  indexType BikeSteering::DBCDz_numCols() const { return 12 ; }
  indexType BikeSteering::DBCDz_nnz()     const { return 10 ; }

  void
  BikeSteering::DBCDz_pattern( indexType iIndex[], indexType jIndex[] ) const {
    iIndex[ 0  ] = 0    ; jIndex[ 0  ] = 0    ;
    iIndex[ 1  ] = 1    ; jIndex[ 1  ] = 1    ;
    iIndex[ 2  ] = 2    ; jIndex[ 2  ] = 6    ;
    iIndex[ 3  ] = 3    ; jIndex[ 3  ] = 7    ;
    iIndex[ 4  ] = 4    ; jIndex[ 4  ] = 4    ;
    iIndex[ 5  ] = 5    ; jIndex[ 5  ] = 3    ;
    iIndex[ 6  ] = 6    ; jIndex[ 6  ] = 5    ;
    iIndex[ 7  ] = 7    ; jIndex[ 7  ] = 10   ;
    iIndex[ 8  ] = 8    ; jIndex[ 8  ] = 9    ;
    iIndex[ 9  ] = 9    ; jIndex[ 9  ] = 11   ;

  }

  void
  BikeSteering::DBCDz_sparse( indexType const          i_segment_left,
                              Z_const_pointer_type     _ZL,
                              Q_const_pointer_type     _QL,
                              indexType const          i_segment_right,
                              Z_const_pointer_type     _ZR,
                              Q_const_pointer_type     _QR,
                              P_const_pointer_type     _P,
                              OMEGA_const_pointer_type _OMEGA,
                              valueType                __result[] ) const {

    valueConstPointer __ZL    = _ZL.pointer() ;
    valueConstPointer __QL    = _QL.pointer() ;
    valueConstPointer __ZR    = _ZR.pointer() ;
    valueConstPointer __QR    = _QR.pointer() ;
    valueConstPointer __P     = _P.pointer() ;
    valueConstPointer __OMEGA = _OMEGA.pointer() ;

    MeshStd::SegmentClass const & segmentLeft  = pMesh->getSegmentByIndex(i_segment_left) ;
    MeshStd::SegmentClass const & segmentRight = pMesh->getSegmentByIndex(i_segment_right) ;

    
    __result[ 0   ] = 1;
    __result[ 1   ] = 1;
    __result[ 2   ] = 1;
    __result[ 3   ] = 1;
    __result[ 4   ] = ModelPars[iM_Ix];
    __result[ 5   ] = 1;
    __result[ 6   ] = 1;
    __result[ 7   ] = -__result[4];
    __result[ 8   ] = -1;
    __result[ 9   ] = -1;

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"DBCDz_sparse",10);
    #endif
  }

  /////////////////////////////////////////////////
  indexType BikeSteering::DBCDpomega_numRows() const { return 10 ; }
  indexType BikeSteering::DBCDpomega_numCols() const { return 4 ; }
  indexType BikeSteering::DBCDpomega_nnz()     const { return 4 ; }

  void
  BikeSteering::DBCDpomega_pattern( indexType iIndex[], indexType jIndex[] ) const {
    iIndex[ 0  ] = 4    ; jIndex[ 0  ] = 0    ;
    iIndex[ 1  ] = 5    ; jIndex[ 1  ] = 1    ;
    iIndex[ 2  ] = 7    ; jIndex[ 2  ] = 2    ;
    iIndex[ 3  ] = 8    ; jIndex[ 3  ] = 3    ;

  }

  void
  BikeSteering::DBCDpomega_sparse( indexType const          i_segment_left,
                                   Z_const_pointer_type     _ZL,
                                   Q_const_pointer_type     _QL,
                                   indexType const          i_segment_right,
                                   Z_const_pointer_type     _ZR,
                                   Q_const_pointer_type     _QR,
                                   P_const_pointer_type     _P,
                                   OMEGA_const_pointer_type _OMEGA,
                                   valueType                __result[] ) const {

    valueConstPointer __ZL    = _ZL.pointer() ;
    valueConstPointer __QL    = _QL.pointer() ;
    valueConstPointer __ZR    = _ZR.pointer() ;
    valueConstPointer __QR    = _QR.pointer() ;
    valueConstPointer __P     = _P.pointer() ;
    valueConstPointer __OMEGA = _OMEGA.pointer() ;

    MeshStd::SegmentClass const & segmentLeft  = pMesh->getSegmentByIndex(i_segment_left) ;
    MeshStd::SegmentClass const & segmentRight = pMesh->getSegmentByIndex(i_segment_right) ;

    
    __result[ 0   ] = 1;
    __result[ 1   ] = 1;
    __result[ 2   ] = 1;
    __result[ 3   ] = 1;

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"DBCDpomega_sparse",4);
    #endif
  }

  /////////////////////////////////////////////////
  indexType BikeSteering::DjumpDz_numRows() const { return 6 ; }
  indexType BikeSteering::DjumpDz_numCols() const { return 12 ; }
  indexType BikeSteering::DjumpDz_nnz()     const { return 12 ; }

  void
  BikeSteering::DjumpDz_pattern( indexType iIndex[], indexType jIndex[] ) const {
    iIndex[ 0  ] = 0    ; jIndex[ 0  ] = 0    ;
    iIndex[ 1  ] = 0    ; jIndex[ 1  ] = 6    ;
    iIndex[ 2  ] = 1    ; jIndex[ 2  ] = 1    ;
    iIndex[ 3  ] = 1    ; jIndex[ 3  ] = 7    ;
    iIndex[ 4  ] = 2    ; jIndex[ 4  ] = 2    ;
    iIndex[ 5  ] = 2    ; jIndex[ 5  ] = 8    ;
    iIndex[ 6  ] = 3    ; jIndex[ 6  ] = 4    ;
    iIndex[ 7  ] = 3    ; jIndex[ 7  ] = 10   ;
    iIndex[ 8  ] = 4    ; jIndex[ 8  ] = 3    ;
    iIndex[ 9  ] = 4    ; jIndex[ 9  ] = 9    ;
    iIndex[ 10 ] = 5    ; jIndex[ 10 ] = 5    ;
    iIndex[ 11 ] = 5    ; jIndex[ 11 ] = 11   ;

  }

  void
  BikeSteering::DjumpDz_sparse( indexType const      i_segment_left,
                                Z_const_pointer_type _ZL,
                                Q_const_pointer_type _QL,

                                indexType const      i_segment_right,
                                Z_const_pointer_type _ZR,
                                Q_const_pointer_type _QR,

                                P_const_pointer_type _P,

                                U_const_pointer_type _UL,
                                U_const_pointer_type _UR,
                                valueType            __result[] ) const {

    valueConstPointer __ZL = _ZL.pointer() ;
    valueConstPointer __QL = _QL.pointer() ;
    valueConstPointer __ZR = _ZR.pointer() ;
    valueConstPointer __QR = _QR.pointer() ;
    valueConstPointer __P  = _P.pointer() ;
    valueConstPointer __UL = _UL.pointer() ;
    valueConstPointer __UR = _UR.pointer() ;

    MeshStd::SegmentClass const & segmentLeft  = pMesh->getSegmentByIndex(i_segment_left) ;
    MeshStd::SegmentClass const & segmentRight = pMesh->getSegmentByIndex(i_segment_right) ;

    
    __result[ 0   ] = -1;
    __result[ 1   ] = 1;
    __result[ 2   ] = -1;
    __result[ 3   ] = 1;
    __result[ 4   ] = -1;
    __result[ 5   ] = 1;
    valueType t1   =  ModelPars[iM_Ix];
    __result[ 6   ] = -t1;
    __result[ 7   ] = t1;
    __result[ 8   ] = -1;
    __result[ 9   ] = 1;
    __result[ 10  ] = -1;
    __result[ 11  ] = 1;

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"DjumpDz_sparse",12);
    #endif
  }

  /////////////////////////////////////////////////
  indexType BikeSteering::DjumpDp_numRows() const { return 6 ; }
  indexType BikeSteering::DjumpDp_numCols() const { return 0 ; }
  indexType BikeSteering::DjumpDp_nnz()     const { return 0 ; }

  void
  BikeSteering::DjumpDp_pattern( indexType iIndex[], indexType jIndex[] ) const {

  }

  void
  BikeSteering::DjumpDp_sparse( indexType const      i_segment_left,
                                Z_const_pointer_type _ZL,
                                Q_const_pointer_type _QL,

                                indexType const      i_segment_right,
                                Z_const_pointer_type _ZR,
                                Q_const_pointer_type _QR,

                                P_const_pointer_type _P,

                                U_const_pointer_type _UL,
                                U_const_pointer_type _UR,
                                valueType            __result[] ) const {

    valueConstPointer __ZL = _ZL.pointer() ;
    valueConstPointer __QL = _QL.pointer() ;
    valueConstPointer __ZR = _ZR.pointer() ;
    valueConstPointer __QR = _QR.pointer() ;
    valueConstPointer __P  = _P.pointer() ;
    valueConstPointer __UL = _UL.pointer() ;
    valueConstPointer __UR = _UR.pointer() ;

    MeshStd::SegmentClass const & segmentLeft  = pMesh->getSegmentByIndex(i_segment_left) ;
    MeshStd::SegmentClass const & segmentRight = pMesh->getSegmentByIndex(i_segment_right) ;

    

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"DjumpDp_sparse",0);
    #endif
  }

}

// EOF: BikeSteering_Methods.cc 
