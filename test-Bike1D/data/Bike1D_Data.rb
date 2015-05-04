 #--------------------------------------------------------------------------#
 #  file: Bike1D_Data.rb                                                    #
 #                                                                          #
 #  version: 1.0   date 4/5/2015                                            #
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
muf_min = -1
mur_min = -1
mur_max = 1


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
    :initial_v => SET,
    :final_v   => SET,
  }

  data.Parameters = {

    # Model Parameters
    :g       => 9.81,
    :muf_min => -1,
    :mur_max => 1,
    :mur_min => -1,

    # Guess Parameters

    # Boundary Conditions
    :v_f => 10,
    :v_i => 10,

    # Post Processing Parameters

    # User Function Parameters
    :Pmax => 50000,
    :m    => 275,
    :vmax => 100,

    # Continuation Parameters

    # Constraints Parameters
  }

  # functions mapped on objects
  data.MappedObjects = {}

  # ClipIntervalWithSinAtan
  data.MappedObjects[:clip] = { :delta => 0, :h => 0.01 }

  # Controls
  # Penalty type controls: U_QUADRATIC, U_QUADRATIC2, U_PARABOLA, U_CUBIC
  # Barrier type controls: U_LOGARITHMIC, U_COS_LOGARITHMIC, U_HYPERBOLIC

  data.Controls = {}
  data.Controls[:murControl] = {
    :type      => U_LOGARITHMIC,
    :epsilon   => 0.001,
    :tolerance => 0.001
  }

  data.Controls[:mufControl] = {
    :type      => U_LOGARITHMIC,
    :epsilon   => 0.001,
    :tolerance => 0.001
  }


  data.Constraints = {}
  # Constraint1D: none defined
  # Constraint2D: none defined

  # User defined classes initialization
  data.Mesh = 
  {
    :s0       => 0,
    :segments => [
      
      {
        :length => 1000,
        :n      => 1000,
      },
    ],
  } ;


end

# user function classes initializations
# EOF
