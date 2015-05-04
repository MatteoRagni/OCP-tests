/*--------------------------------------------------------------------------*\
 |  file: BikeSteering_Pars.hh                                              |
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


#ifndef BIKESTEERINGPARS_HH
#define BIKESTEERINGPARS_HH

#define numBc                    4
#define numModelPars             9
#define numConstraint1D          1
#define numConstraint2D          0
#define numConstraintU           1
#define numZvars                 6
#define numUvars                 1
#define numOMEGAvars             4
#define numQvars                 1
#define numPvars                 0
#define numPostProcess           6
#define numIntegratedPostProcess 0

// Zvars
#define iZ_omega    0
#define iZ_phi      1
#define iZ_TimeSize 2
#define iZ_lambda1  3
#define iZ_lambda2  4
#define iZ_lambda3  5


// Uvars
#define iU_Fy       0


// Qvars
#define iQ_zeta     0


// Pvars


// ModelPars Maps
#define iM_Fmax     0
#define iM_Ix       1
#define iM_g        2
#define iM_h        3
#define iM_m        4
#define iM_omega_f  5
#define iM_omega_i  6
#define iM_phi_f    7
#define iM_phi_i    8


#endif
