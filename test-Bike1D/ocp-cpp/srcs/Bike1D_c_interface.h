/*--------------------------------------------------------------------------*\
 |  file: Bike1D_c_interface.h                                              |
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


#ifndef BIKE1D_C_INTERFACE_STRUCTS_HH
#define BIKE1D_C_INTERFACE_STRUCTS_HH

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
} Bike1D_solver_params_struct ;

/*! Model parameters */
typedef struct {
  data_t Pmax ;
  data_t g ;
  data_t m ;
  data_t muf_min ;
  data_t mur_max ;
  data_t mur_min ;
  data_t v_f ;
  data_t v_i ;
  data_t vmax ;
} Bike1D_model_params_struct ;

/*! BC parameters */
typedef struct {
  uint8_t initial_v ;
  uint8_t final_v ;
} Bike1D_BC_params_struct ;

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

  // 2D constraints

  // Controls
  Controls_struct murControl ;
  Controls_struct mufControl ;


} Bike1D_contraints_struct ;

// prototype for filling parameter from FFI structures
void
BIKE1D_API_DLL
Bike1D_setup_model( Bike1D_BC_params_struct    const * bc_params,  
                    Bike1D_model_params_struct const * model_params,
                    Bike1D_contraints_struct   const * contraints_params ) ;

extern "C" void BIKE1D_API_DLL Bike1D_setup_solver( Bike1D_solver_params_struct const * solver_params ) ;
extern "C" void BIKE1D_API_DLL Bike1D_write_solution_to_file( char const fname[] ) ;
extern "C" int  BIKE1D_API_DLL Bike1D_printout_enabled() ;
extern "C" void BIKE1D_API_DLL Bike1D_enable_printout() ;
extern "C" void BIKE1D_API_DLL Bike1D_disable_printout() ;
extern "C" void BIKE1D_API_DLL Bike1D_reset_multiplier() ;
extern "C" void BIKE1D_API_DLL Bike1D_reset_BC_multiplier() ;
extern "C" void BIKE1D_API_DLL Bike1D_set_internal_guess() ;

#ifdef __cplusplus
}
#endif

#endif

// EOF: Bike1D_c_interface_structs.h

