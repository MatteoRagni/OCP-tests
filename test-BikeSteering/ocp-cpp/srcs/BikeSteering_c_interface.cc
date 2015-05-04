/*--------------------------------------------------------------------------*\
 |  file: BikeSteering_c_interface.cc                                       |
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
#include "BikeSteering_c_interface.h"

using namespace std ;
// Mechatronix namespaces
using namespace MechatronixCore ;
using namespace MechatronixAstro ;
using namespace MechatronixSolver ;
using namespace MechatronixInterfaceMruby ;
using namespace MechatronixInterfaceLua ;

// user namespaces
using namespace MechatronixCore ;

using namespace BikeSteeringLoad ;

namespace BikeSteering_namespace {
  
  BikeSteering         model("BikeSteering",&cout,1) ;
  BVNLFD_System        system("theSystem") ;
  Hyness               solver("theSolver",&cout,1) ;
  bool                 printoutEnabled = true ;
  
  
  // user defined Object instances (external)
  MeshStd              mesh( "mesh" ) ;
  
  extern "C"
  BIKESTEERING_API_DLL
  void
  BikeSteering_setup_model( BikeSteering_BC_params_struct    const * bc_params,  
                            BikeSteering_model_params_struct const * model_params,
                            BikeSteering_contraints_struct   const * contraints_params ) {
    model.setupBC( *bc_params ) ;
    model.setupParameters( *model_params ) ;
    model.setupClasses( *contraints_params ) ;
    //model.setupUserClasses() ) ; DA DEFINIRE!!!!!!!
    if ( printoutEnabled ) {
      model.infoBC( cout ) ;
      model.infoClasses( cout ) ;
    }
  }
  
  extern "C"
  BIKESTEERING_API_DLL
  void
  BikeSteering_setup_solver( BikeSteering_solver_params_struct const * solver_params ) {
    solver.setMaxIter( solver_params -> max_iter ) ;
    solver.setTolerance( solver_params -> tolerance ) ;
    solver.setParams( 1E-12, 2 ) ;
    //solver.setup( &system, &cout ) ;
  }
  
  extern "C"
  BIKESTEERING_API_DLL
  void
  BikeSteering_write_solution_to_file( char const fname[] ) {
    ofstream file( fname ) ;
    if ( file.good() )
      system.dumpSolutionToStream ( solver.solution(), file ) ;
    else
      cerr << "Cannot save to file: " << fname << '\n' ;
    file.close() ;
  }
  
  extern "C"
  BIKESTEERING_API_DLL
  int
  BikeSteering_printout_enabled() { return printoutEnabled ? 1 : 0 ; }
  
  extern "C"
  BIKESTEERING_API_DLL
  void
  BikeSteering_enable_printout() { printoutEnabled = true ; }
  
  extern "C"
  BIKESTEERING_API_DLL
  void
  BikeSteering_disable_printout() { printoutEnabled = false ; }
  
  extern "C"
  BIKESTEERING_API_DLL
  void
  BikeSteering_reset_multiplier() {
    system.resetMultipliers((double*)solver.solution()) ;
  }
  
  extern "C"
  BIKESTEERING_API_DLL
  void
  BikeSteering_reset_BC_multiplier() {
    system.resetBCmultipliers((double*)solver.solution()) ;
  }
  
  extern "C"
  BIKESTEERING_API_DLL
  void
  BikeSteering_set_internal_guess() {
    solver.setInternalGuess() ;
  }
  
}

// EOF: BikeSteering_c_interface.cc
