/*--------------------------------------------------------------------------*\
 |  file: Bike1D_c_interface.cc                                             |
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
#include "Bike1D_c_interface.h"

using namespace std ;
// Mechatronix namespaces
using namespace MechatronixCore ;
using namespace MechatronixAstro ;
using namespace MechatronixSolver ;
using namespace MechatronixInterfaceMruby ;
using namespace MechatronixInterfaceLua ;

// user namespaces
using namespace MechatronixCore ;

using namespace Bike1DLoad ;

namespace Bike1D_namespace {
  
  Bike1D               model("Bike1D",&cout,1) ;
  BVNLFD_System        system("theSystem") ;
  Hyness               solver("theSolver",&cout,1) ;
  bool                 printoutEnabled = true ;
  
  
  // user defined Object instances (external)
  MeshStd              mesh( "mesh" ) ;
  
  extern "C"
  BIKE1D_API_DLL
  void
  Bike1D_setup_model( Bike1D_BC_params_struct    const * bc_params,  
                      Bike1D_model_params_struct const * model_params,
                      Bike1D_contraints_struct   const * contraints_params ) {
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
  BIKE1D_API_DLL
  void
  Bike1D_setup_solver( Bike1D_solver_params_struct const * solver_params ) {
    solver.setMaxIter( solver_params -> max_iter ) ;
    solver.setTolerance( solver_params -> tolerance ) ;
    solver.setParams( 1E-12, 2 ) ;
    //solver.setup( &system, &cout ) ;
  }
  
  extern "C"
  BIKE1D_API_DLL
  void
  Bike1D_write_solution_to_file( char const fname[] ) {
    ofstream file( fname ) ;
    if ( file.good() )
      system.dumpSolutionToStream ( solver.solution(), file ) ;
    else
      cerr << "Cannot save to file: " << fname << '\n' ;
    file.close() ;
  }
  
  extern "C"
  BIKE1D_API_DLL
  int
  Bike1D_printout_enabled() { return printoutEnabled ? 1 : 0 ; }
  
  extern "C"
  BIKE1D_API_DLL
  void
  Bike1D_enable_printout() { printoutEnabled = true ; }
  
  extern "C"
  BIKE1D_API_DLL
  void
  Bike1D_disable_printout() { printoutEnabled = false ; }
  
  extern "C"
  BIKE1D_API_DLL
  void
  Bike1D_reset_multiplier() {
    system.resetMultipliers((double*)solver.solution()) ;
  }
  
  extern "C"
  BIKE1D_API_DLL
  void
  Bike1D_reset_BC_multiplier() {
    system.resetBCmultipliers((double*)solver.solution()) ;
  }
  
  extern "C"
  BIKE1D_API_DLL
  void
  Bike1D_set_internal_guess() {
    solver.setInternalGuess() ;
  }
  
}

// EOF: Bike1D_c_interface.cc
