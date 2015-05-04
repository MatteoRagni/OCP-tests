--[[
/*--------------------------------------------------------------------------*\
 |  file: Bike1D_Data.lua                                                   |
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

--]]

-- Auxiliary values
mur_max = 1
muf_min = -1
mur_min = -1


model = {

  -- Level of message
  data.LU_method = LU_automatic,

  -- Enable doctor
  data.Doctor = false,

  -- Level of message
  InfoLevel = 4,

  -- setup solver
  Solver = {
    max_iter  = 300,  
    tolerance = 1e-9,
  },

  -- Boundary Conditions
  initial_v = 1,
  final_v   = 1,

  -- Model Parameters
  g       = 9.81,
  muf_min = -1,
  mur_max = 1,
  mur_min = -1,

  -- Boundary Conditions
  v_f = 10,
  v_i = 10,

  -- Post Processing Parameters

  -- User Function Parameters
  Pmax = 50000,
  m    = 275,
  vmax = 100,

  -- Continuation Parameters

  -- Constraints Parameters

  -- functions mapped objects
  MappedObjects = {
  -- ClipIntervalWithSinAtan
    cliph => 0.01,
    clipdelta => 0,
  }

  -- Controls
  -- Penalty type controls: U_QUADRATIC, U_QUADRATIC2, U_PARABOLA, U_CUBIC
  -- Barrier type controls: U_LOGARITHMIC, U_COS_LOGARITHMIC, U_HYPERBOLIC

  murControlType      = U_LOGARITHMIC,
  murControlEpsilon   = 0.001,
  murControlTolerance = 0.001,
  mufControlType      = U_LOGARITHMIC,
  mufControlEpsilon   = 0.001,
  mufControlTolerance = 0.001,

  Constraints = {
  -- Constraint1D: none defined
  -- Constraint2D: none defined
  },

  -- User defined classes initialization
  data.Mesh = {:s0=>"0", :segments=>[{:length=>"1000", :n=>"1000"}]};


-- user function classes initializations
-- EOF
