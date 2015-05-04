/*--------------------------------------------------------------------------*\
 |  file: Bike1D_Methods.cc                                                 |
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


#include "Bike1D.hh"
#include "Bike1D_Pars.hh"

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

namespace Bike1DDefine {
  /////////////////////////////////////////////////

 
  // user defined functions which has a body defined in MAPLE
  valueType
  Bike1D::Tmax_normalized( valueType _v ) const {
    
    return ModelPars[iM_Pmax] / ModelPars[iM_m] / ModelPars[iM_g] * (1 - _v / ModelPars[iM_vmax]);

  }


  valueType
  Bike1D::Tmax_normalized_D( valueType _v ) const {
    
    return -ModelPars[iM_Pmax] / ModelPars[iM_m] / ModelPars[iM_g] / ModelPars[iM_vmax];

  }


  valueType
  Bike1D::Tmax_normalized_DD( valueType _v ) const {
    
    return 0;

  }

 

  /////////////////////////////////////////////////

  valueType
  Bike1D::penalties_eval( indexType const i_segment,
                          Z_const_pointer_type _Z,
                          U_const_pointer_type _U,
                          Q_const_pointer_type _Q,
                          P_const_pointer_type _P ) const {

    valueConstPointer __Z = _Z.pointer() ;
    valueConstPointer __U = _U.pointer() ;
    valueConstPointer __Q = _Q.pointer() ;
    valueConstPointer __P = _P.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;
    
    valueType t1   =  __Z[0];
    valueType t2   =  1.0 / t1;
    valueType t5   =  Tmax_normalized(t1);
    valueType t7   =  clip(t5, 0, ModelPars[iM_mur_max]);
    valueType t8   =  murControl(__U[0], ModelPars[iM_mur_min], t7);
    valueType t12  =  mufControl(__U[1], ModelPars[iM_muf_min], 0);
    return t2 * t12 + t2 * t8;

  }

  /////////////////////////////////////////////////

  valueType
  Bike1D::lagrange_target( indexType const i_segment,
                           Z_const_pointer_type _Z,
                           U_const_pointer_type _U,
                           Q_const_pointer_type _Q,
                           P_const_pointer_type _P ) const {

    valueConstPointer __Z = _Z.pointer() ;
    valueConstPointer __U = _U.pointer() ;
    valueConstPointer __Q = _Q.pointer() ;
    valueConstPointer __P = _P.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;
    
    return 1.0 / __Z[0];

  }

  /////////////////////////////////////////////////

  valueType
  Bike1D::mayer_target( indexType const      i_segment_left,
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

    
    return 0;

  }

  /////////////////////////////////////////////////

  indexType Bike1D::a_numEqns() const { return 2 ; }

  void
  Bike1D::a_eval( indexType const i_segment,

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
    
    valueType t1   =  __ZM[0];
    valueType t8   =  __QR[0] - __QL[0];
    valueType t9   =  t1 * t1;
    valueType t10  =  1.0 / t9;
    valueType t11  =  __UM[0];
    valueType t12  =  ModelPars[iM_mur_min];
    valueType t13  =  Tmax_normalized(t1);
    valueType t14  =  ModelPars[iM_mur_max];
    valueType t15  =  clip(t13, 0, t14);
    valueType t16  =  murControl(t11, t12, t15);
    valueType t19  =  murControl.D_3(t11, t12, t15);
    valueType t21  =  clip.D_1(t13, 0, t14);
    valueType t22  =  Tmax_normalized_D(t1);
    valueType t25  =  __UM[1];
    valueType t27  =  mufControl(t25, ModelPars[iM_muf_min], 0);
    __result[ 0   ] = -t1 * (__ZR[1] - __ZL[1]) - t8 * (-t10 - t10 * t16 + 1.0 / t1 * t19 * t21 * t22 - t10 * t27);
    __result[ 1   ] = t1 * (__ZR[0] - __ZL[0]) - t8 * ModelPars[iM_g] * (t11 + t25);

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"a_eval",2);
    #endif
  }

  /////////////////////////////////////////////////

  indexType Bike1D::BC_numEqns() const { return 4 ; }

  void
  Bike1D::BC_eval( indexType const          i_segment_left,
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

    
    valueType t1   =  __ZL[0];
    __result[ 0   ] = t1 - ModelPars[iM_v_i];
    valueType t3   =  __ZR[0];
    __result[ 1   ] = t3 - ModelPars[iM_v_f];
    __result[ 2   ] = t1 * __ZL[1] + __OMEGA[0];
    __result[ 3   ] = -t3 * __ZR[1] + __OMEGA[1];

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"BC_eval",4);
    #endif
  }

  /////////////////////////////////////////////////

  indexType Bike1D::post_numEqns() const { return 5 ; }

  void
  Bike1D::post_eval( indexType const      i_segment,
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

    
    valueType t1   =  __Z[0];
    __result[ 0   ] = 1.0 / t1;
    valueType t4   =  Tmax_normalized(t1);
    valueType t6   =  clip(t4, 0, ModelPars[iM_mur_max]);
    valueType t7   =  murControl(__U[0], ModelPars[iM_mur_min], t6);
    valueType t11  =  mufControl(__U[1], ModelPars[iM_muf_min], 0);
    __result[ 1   ] = __result[0] * t11 + __result[0] * t7;
    __result[ 2   ] = t7;
    __result[ 3   ] = t11;
    __result[ 4   ] = t4;

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"post_eval",5);
    #endif
  }

  /////////////////////////////////////////////////

  indexType Bike1D::integrated_post_numEqns() const { return 1 ; }

  void
  Bike1D::integrated_post_eval( indexType const      i_segment,
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
    
    
    __result[ 0   ] = 1.0 / __Z[0];

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"integrated_post_eval",1);
    #endif
  }

  /////////////////////////////////////////////////

  indexType Bike1D::jump_numEqns() const { return 2 ; }

  void
  Bike1D::jump_eval( indexType const      i_segment_left,
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
    
    
    valueType t1   =  __ZR[0];
    valueType t2   =  __ZL[0];
    __result[ 0   ] = t1 - t2;
    __result[ 1   ] = t1 * __ZR[1] - t2 * __ZL[1];

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"jump_eval",2);
    #endif
  }

  /////////////////////////////////////////////////

  indexType Bike1D::u_numEqns() const { return 2 ; }

  void
  Bike1D::u_eval( indexType const      i_segment,
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

    
    valueType t4   =  __Z[0];
    valueType t5   =  __Z[1] * ModelPars[iM_g] * t4;
    valueType t7   =  Tmax_normalized(t4);
    valueType t9   =  clip(t7, 0, ModelPars[iM_mur_max]);
    __U[ iU_mur ] = murControl.solve(-t5, ModelPars[iM_mur_min], t9);
    __U[ iU_muf ] = mufControl.solve(-t5, ModelPars[iM_muf_min], 0);

    #else
      u_standard_eval( i_segment, _Z, _P, _Q, _U ) ;
    #endif
    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__U,"u_eval",2);
    #endif
  }

  /////////////////////////////////////////////////

  void
  Bike1D::um_eval( indexType const      i_segment,
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

    
    valueType t4   =  __Z[0];
    valueType t5   =  __Z[1] * ModelPars[iM_g] * t4;
    valueType t7   =  Tmax_normalized(t4);
    valueType t9   =  clip(t7, 0, ModelPars[iM_mur_max]);
    __U[ iU_mur ] = murControl.solve(-t5, ModelPars[iM_mur_min], t9);
    __U[ iU_muf ] = mufControl.solve(-t5, ModelPars[iM_muf_min], 0);

    #endif
    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__U,"um_eval",2);
    #endif
  }

  /////////////////////////////////////////////////

  indexType Bike1D::u_guess_numEqns() const { return 2 ; }

  void
  Bike1D::u_guess_eval( indexType const      i_segment,
                        Z_const_pointer_type _Z,
                        P_const_pointer_type _P,
                        Q_const_pointer_type _Q,
                        U_pointer_type       _U ) const {
    valueConstPointer __Z = _Z.pointer() ;
    valueConstPointer __P = _P.pointer() ; 
    valueConstPointer __Q = _Q.pointer() ;
    valuePointer      __U = _U.pointer() ;

    MeshStd::SegmentClass const & segment = pMesh->getSegmentByIndex(i_segment) ;
    
    std::fill( __U, __U+2, 0 ) ;
    
    __U[ iU_mur ] = 0;
    __U[ iU_muf ] = 0;

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__U,"u_guess_eval",2);
    #endif
  }

  /////////////////////////////////////////////////

  void
  Bike1D::u_check_if_admissible( indexType const      i_segment,
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
        
        mufControl.check_range(__U[1], ModelPars[iM_muf_min], 0);
        valueType t6   =  Tmax_normalized(__Z[0]);
        valueType t8   =  clip(t6, 0, ModelPars[iM_mur_max]);
        murControl.check_range(__U[0], ModelPars[iM_mur_min], t8);

    } catch ( exception const & exc ) {
      DO_ERROR("Bike1D::u_check_if_admissible: " << exc.what()) ;
    }
  }

  /////////////////////////////////////////////////

  indexType Bike1D::g_numEqns() const { return 2 ; }

  void
  Bike1D::g_eval( indexType const      i_segment,
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
    
    
    valueType t1   =  __Z[0];
    valueType t2   =  1.0 / t1;
    valueType t5   =  Tmax_normalized(t1);
    valueType t7   =  clip(t5, 0, ModelPars[iM_mur_max]);
    valueType t8   =  murControl.D_1(__U[0], ModelPars[iM_mur_min], t7);
    valueType t12  =  __Z[1] * ModelPars[iM_g];
    __result[ 0   ] = t2 * t8 + t12;
    valueType t15  =  mufControl.D_1(__U[1], ModelPars[iM_muf_min], 0);
    __result[ 1   ] = t2 * t15 + t12;

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"g_eval",2);
    #endif
  }

  /////////////////////////////////////////////////

  indexType Bike1D::q_numEqns() const { return 1 ; }

  void
  Bike1D::q_eval( indexType           i_node,
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
  indexType Bike1D::DaDz_numRows() const { return 2 ; }
  indexType Bike1D::DaDz_numCols() const { return 4 ; }
  indexType Bike1D::DaDz_nnz()     const { return 8 ; }

  void
  Bike1D::DaDz_pattern( indexType iIndex[], indexType jIndex[] ) const {
    iIndex[ 0  ] = 0    ; jIndex[ 0  ] = 0    ;
    iIndex[ 1  ] = 0    ; jIndex[ 1  ] = 1    ;
    iIndex[ 2  ] = 0    ; jIndex[ 2  ] = 2    ;
    iIndex[ 3  ] = 0    ; jIndex[ 3  ] = 3    ;
    iIndex[ 4  ] = 1    ; jIndex[ 4  ] = 0    ;
    iIndex[ 5  ] = 1    ; jIndex[ 5  ] = 1    ;
    iIndex[ 6  ] = 1    ; jIndex[ 6  ] = 2    ;
    iIndex[ 7  ] = 1    ; jIndex[ 7  ] = 3    ;

  }

  void
  Bike1D::DaDz_sparse( indexType const      i_segment,

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
    
    valueType t5   =  __QR[0] - __QL[0];
    valueType t6   =  __ZM[0];
    valueType t7   =  t6 * t6;
    valueType t9   =  1.0 / t7 / t6;
    valueType t11  =  __UM[0];
    valueType t12  =  ModelPars[iM_mur_min];
    valueType t13  =  Tmax_normalized(t6);
    valueType t14  =  ModelPars[iM_mur_max];
    valueType t15  =  clip(t13, 0, t14);
    valueType t16  =  murControl(t11, t12, t15);
    valueType t19  =  1.0 / t7;
    valueType t20  =  murControl.D_3(t11, t12, t15);
    valueType t22  =  clip.D_1(t13, 0, t14);
    valueType t23  =  Tmax_normalized_D(t6);
    valueType t24  =  t22 * t23;
    valueType t27  =  1.0 / t6;
    valueType t28  =  murControl.D_3_3(t11, t12, t15);
    valueType t30  =  t22 * t22;
    valueType t31  =  t23 * t23;
    valueType t34  =  t27 * t20;
    valueType t35  =  clip.D_1_1(t13, 0, t14);
    valueType t38  =  Tmax_normalized_DD(t6);
    valueType t41  =  __UM[1];
    valueType t42  =  ModelPars[iM_muf_min];
    valueType t43  =  mufControl(t41, t42, 0);
    valueType t48  =  murControl.D_1(t11, t12, t15);
    valueType t50  =  murControl.D_1_3(t11, t12, t15);
    valueType t53  =  t27 * t50 * t24 - t19 * t48;
    valueType t54  =  t5 * t53;
    valueType t55  =  invDgDuM_mat[0][0];
    valueType t57  =  invDgDuM_mat[0][1];
    valueType t59  =  mufControl.D_1(t41, t42, 0);
    valueType t61  =  -t57 * t19 * t59 + t55 * t53;
    valueType t63  =  t5 * t19;
    valueType t65  =  invDgDuM_mat[1][1];
    valueType t68  =  -t65 * t19 * t59 + t57 * t53;
    __result[ 0   ] = -__ZR[1] / 2 + __ZL[1] / 2 - t5 * (t27 * t28 * t30 * t31 - 2 * t19 * t20 * t24 + t34 * t22 * t38 + t34 * t35 * t31 + 2 * t9 * t16 + 2 * t9 * t43 + 2 * t9) / 2 + t54 * t61 / 2 - t63 * t59 * t68 / 2;
    valueType t72  =  ModelPars[iM_g];
    valueType t74  =  t72 * t57;
    valueType t75  =  t72 * t55 + t74;
    valueType t77  =  t54 * t75 / 2;
    valueType t79  =  t72 * t65 + t74;
    valueType t82  =  t63 * t59 * t79 / 2;
    __result[ 1   ] = t6 + t77 - t82;
    __result[ 2   ] = __result[0];
    __result[ 3   ] = -t6 + t77 - t82;
    valueType t84  =  __ZR[0] / 2;
    valueType t86  =  __ZL[0] / 2;
    valueType t87  =  t5 * t72;
    valueType t89  =  t87 * t61 / 2;
    valueType t91  =  t87 * t68 / 2;
    __result[ 4   ] = -t6 + t84 - t86 + t89 + t91;
    __result[ 5   ] = t87 * t75 / 2 + t87 * t79 / 2;
    __result[ 6   ] = t6 + t84 - t86 + t89 + t91;
    __result[ 7   ] = __result[5];

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"DaDz_sparse",8);
    #endif
  }

  /////////////////////////////////////////////////
  indexType Bike1D::DaDp_numRows() const { return 2 ; }
  indexType Bike1D::DaDp_numCols() const { return 0 ; }
  indexType Bike1D::DaDp_nnz()     const { return 0 ; }

  void
  Bike1D::DaDp_pattern( indexType iIndex[], indexType jIndex[] ) const {

  }

  void
  Bike1D::DaDp_sparse( indexType const i_segment,

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
  indexType Bike1D::DgDu_numRows() const { return 2 ; }
  indexType Bike1D::DgDu_numCols() const { return 2 ; }
  indexType Bike1D::DgDu_nnz()     const { return 2 ; }

  void
  Bike1D::DgDu_pattern( indexType iIndex[], indexType jIndex[] ) const {
    iIndex[ 0  ] = 0    ; jIndex[ 0  ] = 0    ;
    iIndex[ 1  ] = 1    ; jIndex[ 1  ] = 1    ;

  }

  void
  Bike1D::DgDu_sparse( indexType const      i_segment,
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

    
    valueType t1   =  __Z[0];
    valueType t2   =  1.0 / t1;
    valueType t5   =  Tmax_normalized(t1);
    valueType t7   =  clip(t5, 0, ModelPars[iM_mur_max]);
    valueType t8   =  murControl.D_1_1(__U[0], ModelPars[iM_mur_min], t7);
    __result[ 0   ] = t2 * t8;
    valueType t11  =  mufControl.D_1_1(__U[1], ModelPars[iM_muf_min], 0);
    __result[ 1   ] = t2 * t11;

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"DgDu_sparse",2);
    #endif
  }

  /////////////////////////////////////////////////
  indexType Bike1D::DgDp_numRows() const { return 2 ; }
  indexType Bike1D::DgDp_numCols() const { return 0 ; }
  indexType Bike1D::DgDp_nnz()     const { return 0 ; }

  void
  Bike1D::DgDp_pattern( indexType iIndex[], indexType jIndex[] ) const {

  }

  void
  Bike1D::DgDp_sparse( indexType const      i_segment,
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
  indexType Bike1D::DBCDz_numRows() const { return 4 ; }
  indexType Bike1D::DBCDz_numCols() const { return 4 ; }
  indexType Bike1D::DBCDz_nnz()     const { return 6 ; }

  void
  Bike1D::DBCDz_pattern( indexType iIndex[], indexType jIndex[] ) const {
    iIndex[ 0  ] = 0    ; jIndex[ 0  ] = 0    ;
    iIndex[ 1  ] = 1    ; jIndex[ 1  ] = 2    ;
    iIndex[ 2  ] = 2    ; jIndex[ 2  ] = 0    ;
    iIndex[ 3  ] = 2    ; jIndex[ 3  ] = 1    ;
    iIndex[ 4  ] = 3    ; jIndex[ 4  ] = 2    ;
    iIndex[ 5  ] = 3    ; jIndex[ 5  ] = 3    ;

  }

  void
  Bike1D::DBCDz_sparse( indexType const          i_segment_left,
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
    __result[ 2   ] = __ZL[1];
    __result[ 3   ] = __ZL[0];
    __result[ 4   ] = -__ZR[1];
    __result[ 5   ] = -__ZR[0];

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"DBCDz_sparse",6);
    #endif
  }

  /////////////////////////////////////////////////
  indexType Bike1D::DBCDpomega_numRows() const { return 4 ; }
  indexType Bike1D::DBCDpomega_numCols() const { return 2 ; }
  indexType Bike1D::DBCDpomega_nnz()     const { return 2 ; }

  void
  Bike1D::DBCDpomega_pattern( indexType iIndex[], indexType jIndex[] ) const {
    iIndex[ 0  ] = 2    ; jIndex[ 0  ] = 0    ;
    iIndex[ 1  ] = 3    ; jIndex[ 1  ] = 1    ;

  }

  void
  Bike1D::DBCDpomega_sparse( indexType const          i_segment_left,
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

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"DBCDpomega_sparse",2);
    #endif
  }

  /////////////////////////////////////////////////
  indexType Bike1D::DjumpDz_numRows() const { return 2 ; }
  indexType Bike1D::DjumpDz_numCols() const { return 4 ; }
  indexType Bike1D::DjumpDz_nnz()     const { return 6 ; }

  void
  Bike1D::DjumpDz_pattern( indexType iIndex[], indexType jIndex[] ) const {
    iIndex[ 0  ] = 0    ; jIndex[ 0  ] = 0    ;
    iIndex[ 1  ] = 0    ; jIndex[ 1  ] = 2    ;
    iIndex[ 2  ] = 1    ; jIndex[ 2  ] = 0    ;
    iIndex[ 3  ] = 1    ; jIndex[ 3  ] = 1    ;
    iIndex[ 4  ] = 1    ; jIndex[ 4  ] = 2    ;
    iIndex[ 5  ] = 1    ; jIndex[ 5  ] = 3    ;

  }

  void
  Bike1D::DjumpDz_sparse( indexType const      i_segment_left,
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
    __result[ 2   ] = -__ZL[1];
    __result[ 3   ] = -__ZL[0];
    __result[ 4   ] = __ZR[1];
    __result[ 5   ] = __ZR[0];

    #ifdef MECHATRONIX_DEBUG
    CHECK_NAN(__result,"DjumpDz_sparse",6);
    #endif
  }

  /////////////////////////////////////////////////
  indexType Bike1D::DjumpDp_numRows() const { return 2 ; }
  indexType Bike1D::DjumpDp_numCols() const { return 0 ; }
  indexType Bike1D::DjumpDp_nnz()     const { return 0 ; }

  void
  Bike1D::DjumpDp_pattern( indexType iIndex[], indexType jIndex[] ) const {

  }

  void
  Bike1D::DjumpDp_sparse( indexType const      i_segment_left,
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

// EOF: Bike1D_Methods.cc 
