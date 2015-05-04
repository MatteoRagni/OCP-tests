/*--------------------------------------------------------------------------*\
 |  file: Bike1D.hh                                                         |
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


#ifndef BIKE1D_HH
#define BIKE1D_HH

// Macro definition to use Mechatronix Classes:
// - MechatronixRoad namespace
// - MechatronixVehicle namespace: it includes Tyre Class and Engine Class
// User has to uncomment each macro definition to use appropriate class

//#define MECHATRONIX_U_ITERATIVE
// Mechatronix Headers
#include <MechatronixCore/MechatronixCore.hh>
#include <MechatronixAstro/MechatronixAstro.hh>
#include <MechatronixSolver/MechatronixSolver.hh>
#include <MechatronixInterfaceMruby/MechatronixInterfaceMruby.hh>
#include <MechatronixInterfaceLua/MechatronixInterfaceLua.hh>

// user headers
#include <MechatronixCore/MechatronixCore.hh>


#ifdef MECHATRONIX_OS_WINDOWS
  #ifndef BIKE1D_API_DLL
    #ifdef BIKE1D_EXPORT
      #define BIKE1D_API_DLL __declspec(dllexport) 
    #elif defined(BIKE1D_IMPORT)
      #define BIKE1D_API_DLL __declspec(dllimport) 
    #else
      #define BIKE1D_API_DLL
    #endif
  #endif
#else
  #define BIKE1D_API_DLL
#endif

#include "Bike1D_c_interface.h"

namespace Bike1DDefine {

  using namespace std ;
  // Mechatronix namespaces
  using namespace MechatronixCore ;
  using namespace MechatronixAstro ;
  using namespace MechatronixSolver ;
  using namespace MechatronixInterfaceMruby ;
  using namespace MechatronixInterfaceLua ;

  // user namespaces
  using namespace MechatronixCore ;


  extern char const *namesBc[] ;
  extern char const *namesZvars[] ;
  extern char const *namesUvars[] ;
  extern char const *namesQvars[] ;
  extern char const *namesPvars[] ;
  extern char const *namesOMEGAvars[] ;

  extern char const *namesModelPars[] ;

  extern char const *namesPostProcess[] ;
  extern char const *namesInteratedPostProcess[] ;
  extern char const *namesConstraint1D[] ;
  extern char const *namesConstraint2D[] ;
  extern char const *namesConstraintU[] ;

  using BVNLFDDefine::Z_pointer_type ;
  using BVNLFDDefine::U_pointer_type ;
  using BVNLFDDefine::Q_pointer_type ;
  using BVNLFDDefine::P_pointer_type ;
  using BVNLFDDefine::OMEGA_pointer_type ;

  using BVNLFDDefine::Z_const_pointer_type ;
  using BVNLFDDefine::U_const_pointer_type ;
  using BVNLFDDefine::Q_const_pointer_type ;
  using BVNLFDDefine::P_const_pointer_type ;
  using BVNLFDDefine::OMEGA_const_pointer_type ;

  class BIKE1D_API_DLL Bike1D : public BVNLFD_Model {
    
    // Model Paramaters
    valueType ModelPars[9] ;

    // Controls
    PenaltyU   murControl ;
    PenaltyU   mufControl ;

    // Constraints 1D

    // Constraints 2D

    // User mapped functions
    ClipIntervalWithSinAtan clip ;

    // User classes (internal)

    // User classes (external)
    MeshStd    * pMesh ;

    // block copy constructor
    Bike1D( Bike1D const & ) ;
    Bike1D const & operator = ( Bike1D const & ) ;

    
    // solver classes
    BVNLFD_System system ;
    Hyness        solver ;

  public:

    explicit
    Bike1D( string const             & name,
            std::basic_ostream<char> * _pCout,
            indexType                  _infoLevel ) ;

    virtual ~Bike1D() ;

    void
    changeInfoLevel( indexType info ) {
      BVNLFD_Model::infoLevel = info ;
      solver.changeInfoLevel(info) ;
    }

    void
    changeStream( std::basic_ostream<char> * _pCout ) {
      BVNLFD_Model::pCout = _pCout ;
      solver.changeStream(_pCout) ;
    }

    void infoClasses( basic_ostream<char> & stream ) const ;

    // C++ initializer (raccolti in setup( gc ))
    void setupParameters( GC::GenericContainer const & gc ) ;
    void setupParameters( valueConstPointer Pars ) ;
    void updateParameter( valueType val, indexType idx ) { ModelPars[idx] = val ; }
    void setupClasses( GC::GenericContainer const & gc ) ;
    void setupUserClasses( GC::GenericContainer const & gc ) ;
    void setupMappedObjects( GC::GenericContainer const & gc ) ;
    void setupControls( GC::GenericContainer const & gc ) ;
    void setupPointers( GC::GenericContainer const & gc ) ;

    void setupBC( GC::GenericContainer const & bc ) { BVNLFD_Model::setupBC( bc ) ; } 
    void setupBC( bool isActive[] )                 { BVNLFD_Model::setupBC( isActive ) ; }

    // C interface initializer
    void setupParameters( Bike1D_model_params_struct const & params ) ;
    void setupClasses( Bike1D_contraints_struct const & params ) ;
    void setupBC( Bike1D_BC_params_struct const & params ) ;

    // user functions prototype (with derivative)
    valueType Tmax_normalized      ( valueType _v ) const ;
    valueType Tmax_normalized_D    ( valueType _v ) const ;
    valueType Tmax_normalized_DD   ( valueType _v ) const ;

    #include <MechatronixSolver/BVNLFD_Model.hxx>

    // C++ initializer (all in one)
    void setup( GC::GenericContainer const & gc ) ;
    bool solve( GC::GenericContainer const & in, GC::GenericContainer & out ) ;

    // Save solution in generic object (even if not converged)
    void
    save( GC::GenericContainer & gc_sol ) const 
    { system.saveSolution( solver.solution(), gc_sol ) ; }

  } ;
}

namespace Bike1DLoad {
  using Bike1DDefine::Bike1D ;
  using namespace MechatronixCore ;
}

#endif
