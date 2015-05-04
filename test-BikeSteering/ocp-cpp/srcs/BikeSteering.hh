/*--------------------------------------------------------------------------*\
 |  file: BikeSteering.hh                                                   |
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


#ifndef BIKESTEERING_HH
#define BIKESTEERING_HH

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
  #ifndef BIKESTEERING_API_DLL
    #ifdef BIKESTEERING_EXPORT
      #define BIKESTEERING_API_DLL __declspec(dllexport) 
    #elif defined(BIKESTEERING_IMPORT)
      #define BIKESTEERING_API_DLL __declspec(dllimport) 
    #else
      #define BIKESTEERING_API_DLL
    #endif
  #endif
#else
  #define BIKESTEERING_API_DLL
#endif

#include "BikeSteering_c_interface.h"

namespace BikeSteeringDefine {

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

  class BIKESTEERING_API_DLL BikeSteering : public BVNLFD_Model {
    
    // Model Paramaters
    valueType ModelPars[9] ;

    // Controls
    PenaltyU        FyControl ;

    // Constraints 1D
    Penalty1DGreaterThan minimumTimeSize ;

    // Constraints 2D

    // User mapped functions

    // User classes (internal)

    // User classes (external)
    MeshStd         * pMesh ;

    // block copy constructor
    BikeSteering( BikeSteering const & ) ;
    BikeSteering const & operator = ( BikeSteering const & ) ;

    
    // solver classes
    BVNLFD_System system ;
    Hyness        solver ;

  public:

    explicit
    BikeSteering( string const             & name,
                  std::basic_ostream<char> * _pCout,
                  indexType                  _infoLevel ) ;

    virtual ~BikeSteering() ;

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
    void setupParameters( BikeSteering_model_params_struct const & params ) ;
    void setupClasses( BikeSteering_contraints_struct const & params ) ;
    void setupBC( BikeSteering_BC_params_struct const & params ) ;

    // user functions prototype (with derivative)

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

namespace BikeSteeringLoad {
  using BikeSteeringDefine::BikeSteering ;
  using namespace MechatronixCore ;
}

#endif
