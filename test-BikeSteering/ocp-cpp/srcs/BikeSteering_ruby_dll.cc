/*--------------------------------------------------------------------------*\
 |  file: BikeSteering_Main.cc                                              |
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


#include <MechatronixInterfaceMruby/mruby/mruby.h>
#include <MechatronixInterfaceMruby/mruby/mruby/string.h>
#include <MechatronixInterfaceMruby/mruby/mruby/variable.h>
#include <MechatronixInterfaceMruby/mruby/mruby/hash.h>
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

#define EXTERN_C extern "C"

#include <sstream>

#ifndef MECHATRONIX_OS_WINDOWS
  #include <signal.h>
  #include <execinfo.h>
#endif

class BIKESTEERING_API_DLL BikeSteering_Problem {
  #ifndef MECHATRONIX_OS_WINDOWS
  struct sigaction struct_sigaction       ; // for managing sigaction
  struct sigaction struct_sigaction_ctrlC ; // for managing sigaction

  static
  void
  segfault_sigaction(int signal, siginfo_t *si, void *arg) {
    std::stringstream msg ;
    msg << "Caught segfault at address: " << si->si_addr << '\n' ;
    void* callstack[128];
    int frames = backtrace(callstack, 128);
    char** strs = backtrace_symbols(callstack, frames);
    for ( int i = 0; i < frames; ++i) msg << strs[i] << '\n' ;
    free(strs);
    ASSERT(false,msg.str()) ;
  }

  static
  void
  segfault_sigaction_ctrlC(int signal, siginfo_t *si, void *arg) {
    cerr << "Caught segfault at address: " << si->si_addr << '\n' ;
    void* callstack[128];
    int frames = backtrace(callstack, 128);
    char** strs = backtrace_symbols(callstack, frames);
    for ( int i = 0; i < frames; ++i) cerr << strs[i] << '\n' ;
    free(strs);
    exit(1) ;
  }
  #endif

  BikeSteering model ;

  // user defined Object instances (external)
  MeshStd    mesh ;
  
  // block copy constructor
  BikeSteering_Problem( BikeSteering_Problem const & ) ;
  BikeSteering_Problem const & operator = ( BikeSteering_Problem const & ) ;

public:

  BikeSteering_Problem( std::basic_ostream<char> * pCout, indexType infoLevel )
  : model("BikeSteering",pCout,infoLevel)
  , mesh( "mesh" )
  {
    #ifndef MECHATRONIX_OS_WINDOWS
    memset(&struct_sigaction, 0, sizeof(struct sigaction));
    sigemptyset(&struct_sigaction.sa_mask);
    struct_sigaction.sa_sigaction = segfault_sigaction;
    struct_sigaction.sa_flags     = SA_SIGINFO;
    sigaction(SIGSEGV, &struct_sigaction, NULL);

    memset(&struct_sigaction_ctrlC, 0, sizeof(struct sigaction));
    sigemptyset(&struct_sigaction_ctrlC.sa_mask);
    struct_sigaction_ctrlC.sa_sigaction = segfault_sigaction_ctrlC;
    struct_sigaction_ctrlC.sa_flags     = SA_SIGINFO;
    sigaction(SIGINT, &struct_sigaction_ctrlC, NULL);
    #endif
  }

  ~BikeSteering_Problem() {}

  bool
  setup( GC::GenericContainer & gc_data ) {
    bool ok = false ;
    try {

      // alias for user object classes passed as pointers
      GC::GenericContainer & ptrs = gc_data["Pointers"] ;
      // setup user object classes
      mesh.setup(gc_data("Mesh")) ;
      ptrs[ "pMesh" ] = &mesh ;
      // setup model
      stringstream ss_msg;
      model.changeStream(&ss_msg) ;
      model.setup(gc_data);
      model.changeStream(&cout) ;
      gc_data["output_msg"] = ss_msg.str();
      ok = true ;
    }
    catch ( exception const & exc ) {
      gc_data["Error"] = exc.what() ;
    }
    catch (...) {
      gc_data["Error"] = "BikeSteering_ocp_setup, fatal: unksnown error\n" ;
    }
    return ok ;
  }

  bool
  solve( GC::GenericContainer & gc_data,
         GC::GenericContainer & gc_solution ) {
    bool ok = false ;
    try {
      ok = model.solve( gc_data, gc_solution ) ;
    }
    catch ( exception const & exc ) {
      gc_solution["Error"] = exc.what() ;
    }
    catch (...) {
      gc_solution["Error"] = "BikeSteering_ocp_solve, fatal: unknown error" ;
    }
    return ok ;
  }

  void
  dumpSolutionToFile( char const fname[] ) const {
    ofstream file( fname ) ;
    if ( file.good() ) {
      GC::GenericContainer gc ;
      model.save( gc ) ;
      gc.writeFormattedData(file) ;
    } else {
      cerr << "Cannot save to file: " << fname << "\n" ;
    }
    file.close() ;
  }

} ;

static map< string, BikeSteering_Problem * > problems ;

/*            
::  ____        _             _____ _____ ___ 
:: |  _ \ _   _| |__  _   _  |  ___|  ___|_ _|
:: | |_) | | | | '_ \| | | | | |_  | |_   | | 
:: |  _ <| |_| | |_) | |_| | |  _| |  _|  | | 
:: |_| \_\\__,_|_.__/ \__, | |_|   |_|   |___|
::                    |___/                   
::  ___       _             __                
:: |_ _|_ __ | |_ ___ _ __ / _| __ _  ___ ___ 
::  | || '_ \| __/ _ \ '__| |_ / _` |/ __/ _ \
::  | || | | | ||  __/ |  |  _| (_| | (_|  __/
:: |___|_| |_|\__\___|_|  |_|  \__,_|\___\___|
*/

EXTERN_C
BIKESTEERING_API_DLL 
bool
BikeSteering_ocp_setup( char const id[], GC::GenericContainer & gc_data  ) {
  map< string, BikeSteering_Problem * >::iterator it = problems.find(id) ;
  if ( it == problems.end() ) {
    indexType infoLevel = 1 ;
    if ( gc_data.exists("InfoLevel") ) infoLevel = gc_data("InfoLevel").get_int() ;
    problems[id] = new BikeSteering_Problem(&cout,infoLevel) ;
    return problems[id]->setup(gc_data) ;
  } else {
    return it->second->setup(gc_data) ;
  }
}

EXTERN_C
BIKESTEERING_API_DLL 
bool
BikeSteering_ocp_solve( char const             id[],
                        GC::GenericContainer & gc_data,
                        GC::GenericContainer & gc_solution ) {
  map< string, BikeSteering_Problem * >::iterator it = problems.find(id) ;
  if ( it == problems.end() ) {
    gc_solution["Error"] = "BikeSteering_ocp_solve, you must do setup first!\n" ;
    return false ;
  } else {
    return it->second->solve(gc_data,gc_solution) ;
  }
}

EXTERN_C
BIKESTEERING_API_DLL
void
BikeSteering_write_ocp_solution( char const id[], char const fname[] ) {
  map< string, BikeSteering_Problem * >::const_iterator it = problems.find(id) ;
  if ( it == problems.end() ) {
    cerr << "BikeSteering_write_ocp_solution, no solution found!\n" ;
  } else {
    it -> second -> dumpSolutionToFile( fname ) ;
  }
}

/*                       _             _       _             __                
::  _ __ ___  _ __ _   _| |__  _   _  (_)_ __ | |_ ___ _ __ / _| __ _  ___ ___ 
:: | '_ ` _ \| '__| | | | '_ \| | | | | | '_ \| __/ _ \ '__| |_ / _` |/ __/ _ \
:: | | | | | | |  | |_| | |_) | |_| | | | | | | ||  __/ |  |  _| (_| | (_|  __/
:: |_| |_| |_|_|   \__,_|_.__/ \__, | |_|_| |_|\__\___|_|  |_|  \__,_|\___\___|
::                             |___/                                           
*/

void
mrb_to_GC( mrb_state            * mrb,
           mrb_value            & data,
           GC::GenericContainer & gc,
           string const         & trace );

void
mrb_from_GC( mrb_state                  * mrb,
             GC::GenericContainer const & gc,
             mrb_value                  & data,
             std::string const          & trace );


EXTERN_C
BIKESTEERING_API_DLL
mrb_value
mrb_BikeSteering_ocp_setup(mrb_state *mrb, mrb_value self) {
  // ruby parameters
  mrb_value id_value   = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, "@id"   ));
  mrb_value data_value = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, "@data" ));
  
  // to C++ parameters
  GC::GenericContainer gc_data;
  string id = mrb_str_to_cstr(mrb, id_value);
  mrb_to_GC( mrb, data_value, gc_data, "mrb_to_GC:" );
  
  // setup the solver
  //bool ok = ocp.setup( id.c_str(), ocp.gc_data ) ;
  bool ok;
  map< string, BikeSteering_Problem * >::iterator it = problems.find(id.c_str());
  if ( it == problems.end() ) {
    indexType infoLevel = 1 ;
    if ( gc_data.exists("InfoLevel") ) infoLevel = gc_data("InfoLevel").get_int();
    problems[id] = new BikeSteering_Problem(&cout,infoLevel);
    ok = problems[id]->setup(gc_data);
  } else {
    ok = it->second->setup(gc_data);
  }

  // return values
  if ( ok ) {
    return mrb_str_new_cstr(mrb, gc_data("output_msg").get_string().c_str());
  } else {
    mrb_raise(mrb, mrb_class_get(mrb, "RuntimeError"), gc_data("Error").get_string().c_str());
    return mrb_false_value();
  }
}


EXTERN_C
BIKESTEERING_API_DLL 
mrb_value
mrb_BikeSteering_ocp_solve(mrb_state *mrb, mrb_value self) {
  // ruby parameters
  mrb_value id_value   = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, "@id"   ));
  mrb_value data_value = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, "@data" ));
  
  // to C++ parameters
  GC::GenericContainer gc_data, gc_solution;
  string id = mrb_str_to_cstr(mrb, id_value);
  mrb_to_GC( mrb, data_value, gc_data, "mrb_to_GC:" );
  
  // launch the solver
  map< string, BikeSteering_Problem * >::const_iterator it = problems.find(id);
  if ( it == problems.end() ) {
    cerr << "BikeSteering_ocp_solve, you must do setup first!\n";
  } else {
    it->second->solve(gc_data, gc_solution);
  }
  
  // return values
  mrb_value res = mrb_hash_new(mrb) ;
  mrb_from_GC(mrb, gc_solution, res, "mrb_from_GC:");
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@ocp_solution"), res);
  return res;
}


EXTERN_C
BIKESTEERING_API_DLL 
mrb_value
mrb_BikeSteering_write_ocp_solution(mrb_state *mrb, mrb_value self) {
  // ruby parameters
  mrb_value a_string;
  mrb_get_args(mrb, "S", &a_string);  
  mrb_value id_value   = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, "@id"   )) ;
  
  // to C++ parameters
  string id = mrb_str_to_cstr(mrb, id_value) ;
  
  map< string, BikeSteering_Problem * >::const_iterator it = problems.find(id) ;
  if ( it == problems.end() ) {
    cerr << "BikeSteering_ocp_write_to_file, no solution found!\n" ;
  } else {
    it -> second -> dumpSolutionToFile( mrb_string_value_cstr(mrb, &a_string) ) ;
  }
  
  return mrb_nil_value();
}


EXTERN_C
BIKESTEERING_API_DLL
void
mrb_libBikeSteering_gem_init(mrb_state* mrb)
{
  struct RClass *ocp_class;
  struct RClass *mechatronix;
  
  mechatronix = mrb_module_get(mrb, "Mechatronix");
  ocp_class = mrb_class_get_under(mrb, mechatronix, "OCPSolver");
  
  mrb_define_const(mrb, ocp_class,  "NAME",  mrb_str_new_cstr(mrb, "BikeSteering"));
  mrb_define_method(mrb, ocp_class, "setup", mrb_BikeSteering_ocp_setup, MRB_ARGS_NONE());
  mrb_define_method(mrb, ocp_class, "solve", mrb_BikeSteering_ocp_solve, MRB_ARGS_NONE());
  mrb_define_method(mrb, ocp_class, "write", mrb_BikeSteering_write_ocp_solution, MRB_ARGS_REQ(1));
}


EXTERN_C
BIKESTEERING_API_DLL
void
mrb_libBikeSteering_gem_final(mrb_state* mrb)
{
}

// EOF: BikeSteering_ruby_dll.cc
