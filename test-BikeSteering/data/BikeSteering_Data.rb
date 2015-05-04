 #--------------------------------------------------------------------------#
 #  file: BikeSteering_Data.rb                                              #
 #                                                                          #
 #  version: 1.0   date 27/4/2015                                           #
 #                                                                          #
 #  Copyright (C) 2015                                                      #
 #                                                                          #
 #      Enrico Bertolazzi and Francesco Biral and Paolo Bosetti             #
 #      Dipartimento di Ingegneria Industriale                              #
 #      Universita` degli Studi di Trento                                   #
 #      Via Mesiano 77, I-38050 Trento, Italy                               #
 #      email: enrico.bertolazzi@unitn.it                                   #
 #             francesco.biral@unitn.it                                     #
 #             paolo.bosetti@unitn.it                                       #
 #--------------------------------------------------------------------------#


include Mechatronix

# Auxiliary values
mu   = 1
m    = 250
g    = 9.81
Fmax = m*g*mu


mechatronix do |data|

  # Level of message
  data.LU_method = LU_automatic

  # Enable doctor
  data.Doctor = false

  # Level of message
  data.InfoLevel = 4
  
  # setup solver
  data.Solver = {
    :max_iter  => 300,  
    :tolerance => 1e-9,
  }
  
  # Boundary Conditions
  data.BoundaryConditions = {
    :initial_omega => SET,
    :initial_phi   => SET,
    :final_omega   => SET,
    :final_phi     => SET,
  }

  data.Parameters = {

    # Model Parameters
    :Fmax => m*g*mu,
    :Ix   => 40,
    :g    => 9.81,
    :h    => 0.6,
    :m    => 250,

    # Guess Parameters

    # Boundary Conditions
    :omega_f => 0,
    :omega_i => 0,
    :phi_f   => 0.174533,
    :phi_i   => 0,

    # Post Processing Parameters

    # User Function Parameters

    # Continuation Parameters

    # Constraints Parameters
  }

  # functions mapped on objects
  data.MappedObjects = {}

  # Controls
  # Penalty type controls: U_QUADRATIC, U_QUADRATIC2, U_PARABOLA, U_CUBIC
  # Barrier type controls: U_LOGARITHMIC, U_COS_LOGARITHMIC, U_HYPERBOLIC

  data.Controls = {}
  data.Controls[:FyControl] = {
    :type      => U_COS_LOGARITHMIC,
    :epsilon   => 0.001,
    :tolerance => 0.001
  }


  data.Constraints = {}
  # Constraint1D
  # Penalty subtype: PENALTY_REGULAR, PENALTY_SMOOTH, PENALTY_PIECEWISE
  # Barrier subtype: BARRIER_LOG, BARRIER_LOG_EXP, BARRIER_LOG0
  # Penalty1DGreaterThan
  data.Constraints[:minimumTimeSize] = {
    :subType   => PENALTY_REGULAR,
    :epsilon   => 0.001,
    :tolerance => 0.001,
    :active    => true
  }
  # Constraint2D: none defined

  # User defined classes initialization
  data.Mesh = 
  {
    :s0       => 0,
    :segments => [
      
      {
        :n      => 10,
        :length => 0.1,
      },
      
      {
        :n      => 40,
        :length => 0.4,
      },
      
      {
        :n      => 40,
        :length => 0.4,
      },
      
      {
        :n      => 10,
        :length => 0.1,
      },
    ],
  } ;


end

# user function classes initializations
# EOF
