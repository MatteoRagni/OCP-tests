/*--------------------------------------------------------------------------*\
 |  file: BikeSteering_Main.cc                                              |
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


#define MECHATRONIX_USE_RUBY
//#define MECHATRONIX_USE_LUA

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

using namespace BikeSteeringLoad ;

int
main() {
  BikeSteering         model("BikeSteering",&cout,1) ;
  GC::GenericContainer gc_data, gc_solution ;

  // user defined Object instances (external)
  MeshStd              mesh( "mesh" ) ;

  try {

    // read Ruby script
    #ifdef MECHATRONIX_USE_RUBY
      MrubyInterpreter mrb ;
      mrb.load( "./data/BikeSteering_Data.rb" ) ;
      mrb.expr_to_GC( "Mechatronix.content", gc_data ) ;
    #endif

    #ifdef MECHATRONIX_USE_LUA
      LuaInterpreter lua ;
      lua.load( "data/BikeSteering_Data.lua" ) ;
      lua.global_to_GC( "content", gc_data ) ;
    #endif

    // alias for user object classes passed as pointers
    GC::GenericContainer & ptrs = gc_data["Pointers"] ;
    // setup user object classes
    ASSERT(gc_data.exists("Mesh"), "missing key: ``Mesh'' in gc_data") ;
    mesh.setup(gc_data("Mesh")) ;
    ptrs[ "pMesh" ] = &mesh ;

    // setup model
    model.setup( gc_data ) ;

    bool ok = model.solve( gc_data, gc_solution ) ;
    if ( ok ) {
      // dump solution to file
      ofstream file( "data/BikeSteering_out.txt" ) ;
      file.precision(18) ;
      gc_solution.writeFormattedData(file) ;
      file.close() ;
    
      cout.precision(18) ;
      cout << "Lagrange target = " << gc_solution("lagrange").get_number() << '\n' ;
      cout << "Mayer    target = " << gc_solution("mayer").get_number()    << '\n' ;
      cout << "Penalties       = " << gc_solution("penalty").get_number()  << '\n' ;
    } else {
      cout << gc_solution("Error").get_string() << '\n' ;
      // dump solution to file
      ofstream file( "data/BikeSteering_not_converged.txt" ) ;
      file.precision(18) ;
      gc_solution.writeFormattedData(file) ;
      file.close() ;
    }
    if ( gc_solution.exists("diagnosis") ) gc_solution("diagnosis").print(cout) ;
  }
  catch ( exception const & exc ) {
    cerr << exc.what() << '\n' ;
    ALL_DONE_FOLKS ;
    exit(0) ;
  }
  catch ( char const exc[] ) {
    cerr << exc << '\n' ;
    ALL_DONE_FOLKS ;
    exit(0) ;
  }
  catch (...) {
    cout << "ERRORE SCONOSCIUTO\n" ;
    ALL_DONE_FOLKS ;
    exit(0) ;
  }
  
  ALL_DONE_FOLKS ;
}
