/*--------------------------------------------------------------------------*\
 |  file: Bike1D_Pars.hh                                                    |
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


#ifndef BIKE1DPARS_HH
#define BIKE1DPARS_HH

#define numBc                    2
#define numModelPars             9
#define numConstraint1D          0
#define numConstraint2D          0
#define numConstraintU           2
#define numZvars                 2
#define numUvars                 2
#define numOMEGAvars             2
#define numQvars                 1
#define numPvars                 0
#define numPostProcess           5
#define numIntegratedPostProcess 1

// Zvars
#define iZ_v       0
#define iZ_lambda1 1


// Uvars
#define iU_mur     0
#define iU_muf     1


// Qvars
#define iQ_zeta    0


// Pvars


// ModelPars Maps
#define iM_Pmax    0
#define iM_g       1
#define iM_m       2
#define iM_muf_min 3
#define iM_mur_max 4
#define iM_mur_min 5
#define iM_v_f     6
#define iM_v_i     7
#define iM_vmax    8


#endif
