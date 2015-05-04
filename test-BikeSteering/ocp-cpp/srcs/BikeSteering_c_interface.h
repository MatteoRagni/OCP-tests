/*--------------------------------------------------------------------------*\
 |  file: BikeSteering_c_interface.h                                        |
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


#ifndef BIKESTEERING_C_INTERFACE_STRUCTS_HH
#define BIKESTEERING_C_INTERFACE_STRUCTS_HH

#include <stdint.h>
#include <MechatronixCore/MechatronixCore.hh>
#include <MechatronixSolver/MechatronixSolver.hh>

using namespace std ;
using namespace MechatronixCore ;
using namespace MechatronixSolver ;

#ifdef __cplusplus
extern "C" {
#endif

typedef double   data_t ;
typedef uint32_t index_t ;

typedef struct {
  index_t max_iter ;
  data_t  tolerance ;
} BikeSteering_solver_params_struct ;

/*! Model parameters */
typedef struct {
  data_t Fmax ;
  data_t Ix ;
  data_t g ;
  data_t h ;
  data_t m ;
  data_t omega_f ;
  data_t omega_i ;
  data_t phi_f ;
  data_t phi_i ;
} BikeSteering_model_params_struct ;

/*! BC parameters */
typedef struct {
  uint8_t initial_omega ;
  uint8_t initial_phi ;
  uint8_t final_omega ;
  uint8_t final_phi ;
} BikeSteering_BC_params_struct ;

/*! Constraints parameters */
typedef struct {
  index_t type ;
  data_t  epsilon ;
  data_t  tolerance ;
} Controls_struct ;

typedef struct {
  index_t subType ;
  data_t  epsilon ;
  data_t  tolerance ;
} Constraints_struct ;

typedef struct {
  index_t subType ;
  data_t  epsilon ;
  data_t  tolerance ;
  data_t  min ;
  data_t  max ;
} Constraint_interval ;

typedef struct {
  data_t  epsilon ;
  data_t  exponent ;
} Constraint_bipower ;

typedef struct {
  data_t  exponent ;
} Constraint_power ;

typedef struct {
  data_t  exponent ;
} Constraint_2D ;

typedef struct {
  // 1D constraints
  Constraints_struct minimumTimeSize ;

  // 2D constraints

  // Controls
  Controls_struct FyControl ;


} BikeSteering_contraints_struct ;

// prototype for filling parameter from FFI structures
void
BIKESTEERING_API_DLL
BikeSteering_setup_model( BikeSteering_BC_params_struct    const * bc_params,  
                          BikeSteering_model_params_struct const * model_params,
                          BikeSteering_contraints_struct   const * contraints_params ) ;

extern "C" void BIKESTEERING_API_DLL BikeSteering_setup_solver( BikeSteering_solver_params_struct const * solver_params ) ;
extern "C" void BIKESTEERING_API_DLL BikeSteering_write_solution_to_file( char const fname[] ) ;
extern "C" int  BIKESTEERING_API_DLL BikeSteering_printout_enabled() ;
extern "C" void BIKESTEERING_API_DLL BikeSteering_enable_printout() ;
extern "C" void BIKESTEERING_API_DLL BikeSteering_disable_printout() ;
extern "C" void BIKESTEERING_API_DLL BikeSteering_reset_multiplier() ;
extern "C" void BIKESTEERING_API_DLL BikeSteering_reset_BC_multiplier() ;
extern "C" void BIKESTEERING_API_DLL BikeSteering_set_internal_guess() ;

#ifdef __cplusplus
}
#endif

#endif

// EOF: BikeSteering_c_interface_structs.h

