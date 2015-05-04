/*--------------------------------------------------------------------------*\
 |  file: BikeSteering.cc                                                   |
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


#include "BikeSteering.hh"
#include "BikeSteering_Pars.hh"

namespace BikeSteeringDefine {

  char const *namesZvars[numZvars+1] = {
    "omega",
    "phi",
    "TimeSize",
    "lambda1",
    "lambda2",
    "lambda3",
    NULL
  } ;

  char const *namesUvars[numUvars+1] = {
    "Fy",
    NULL
  } ;

  char const *namesQvars[numQvars+1] = {
    "zeta",
    NULL
  } ;

  char const *namesPvars[numPvars+1] = {
    NULL
  } ;

  char const *namesOMEGAvars[numOMEGAvars+1] = {
    "omega1",
    "omega2",
    "omega3",
    "omega4",
    NULL
  } ;

  char const *namesPostProcess[numPostProcess+1] = {
    "lagrange_target",
    "penalties",
    "FyControl",
    "minimumTimeSize",
    "minTime",
    "Talpha",
    NULL
  } ;

  char const *namesIntegratedPostProcess[numIntegratedPostProcess+1] = {
    NULL
  } ;

  char const *namesModelPars[numModelPars+1] = {
    "Fmax",
    "Ix",
    "g",
    "h",
    "m",
    "omega_f",
    "omega_i",
    "phi_f",
    "phi_i",
    NULL
  } ;

  char const *namesConstraint1D[numConstraint1D+1] = {
    "minimumTimeSize",
    NULL
  } ;

  char const *namesConstraint2D[numConstraint2D+1] = {
    NULL
  } ;

  char const *namesConstraintU[numConstraintU+1] = {
    "FyControl",
    NULL
  } ;

  char const *namesBc[numBc+1] = {
    "initial_omega",
    "initial_phi",
    "final_omega",
    "final_phi",
    NULL
  } ;

  // --------------------------------------------------------------------------
  BikeSteering::BikeSteering( string const             & name,
                              std::basic_ostream<char> * _pCout,
                              indexType                  _infoLevel )
  : BVNLFD_Model(name,_pCout,_infoLevel)
  // Controls
  , FyControl("FyControl")
  // Constraints 1D
  , minimumTimeSize("minimumTimeSize")
  // Constraints 2D
  // User classes
  , system("BikeSteeringSystem")
  , solver("BikeSteeringSolver",_pCout,_infoLevel)
  {
    // Initialize to NaN all the ModelPars
    std::fill( ModelPars, ModelPars + numModelPars, NaN ) ;

    // Initialize string of names
    setupNames( numZvars,                 namesZvars,
                numUvars,                 namesUvars,
                numQvars,                 namesQvars,
                numPvars,                 namesPvars,
                numPostProcess,           namesPostProcess,
                numIntegratedPostProcess, namesIntegratedPostProcess,
                numBc,                    namesBc ) ;
  }

  BikeSteering::~BikeSteering() {
  }

  // --------------------------------------------------------------------------
  // initialize parameters using associative array

  void
  BikeSteering::setupParameters( GC::GenericContainer const & gc ) {
    bool allfound = true ;
    for ( indexType i = 0 ; i < numModelPars ; ++i ) {
      bool ok = gc.exists( namesModelPars[i] ) ;
      if ( !ok ) {
        cerr << "Missing parameter: '" << namesModelPars[i] << "'\n" ;
      } else {
        ModelPars[i] = gc(namesModelPars[i]).get_number() ;
      }
      allfound = allfound && ok ;
    }
    ASSERT( allfound, "in BikeSteering::setup not all parameters are set!") ;
  }

  // --------------------------------------------------------------------------
  void
  BikeSteering::setupClasses( GC::GenericContainer const & gc ) {

    // Initialize Constraints 1D
    ASSERT( gc.exists("minimumTimeSize"), "in BikeSteering::setupClasses(gc) missing key: ``minimumTimeSize''" ) ;
    minimumTimeSize.setup( gc("minimumTimeSize") ) ;

  }

  // --------------------------------------------------------------------------
  void
  BikeSteering::setupUserClasses( GC::GenericContainer const & gc ) {
  }

  // --------------------------------------------------------------------------
  void
  BikeSteering::setupMappedObjects( GC::GenericContainer const & gc ) {
  }

  // --------------------------------------------------------------------------
  void
  BikeSteering::setupControls( GC::GenericContainer const & gc ) {
    // initialize Controls

    { GC::GenericContainer const & gc1 = gc("FyControl") ;
      FyControl.setup( gc1("type").get_int()
                     , gc1("epsilon").get_number()
                     , gc1("tolerance").get_number() ) ; }

  }

  // --------------------------------------------------------------------------
  void
  BikeSteering::setupPointers( GC::GenericContainer const & gc ) {

    // Initialize user classes

    ASSERT( gc.exists("pMesh"), "in BikeSteering::setupPointers(gc) cant find key `pMesh' in gc" ) ;
    pMesh = gc("pMesh").get_pointer<MeshStd*>() ;
  }

  // --------------------------------------------------------------------------
  // initialize parameters using structures
  void
  BikeSteering::setupBC( BikeSteering_BC_params_struct const & params ) {
    // Bc on/off
    bool active[] = {
      (bool)params.initial_omega,
      (bool)params.initial_phi,
      (bool)params.final_omega,
      (bool)params.final_phi,
      false
    } ;
    BVNLFD_Model::setupBC( active ) ; // Setup active boundary conditions
  }

  // --------------------------------------------------------------------------
  void
  BikeSteering::setupParameters( BikeSteering_model_params_struct const & params ) {
    ModelPars[ iM_Fmax    ] = params.Fmax ;
    ModelPars[ iM_Ix      ] = params.Ix ;
    ModelPars[ iM_g       ] = params.g ;
    ModelPars[ iM_h       ] = params.h ;
    ModelPars[ iM_m       ] = params.m ;
    ModelPars[ iM_omega_f ] = params.omega_f ;
    ModelPars[ iM_omega_i ] = params.omega_i ;
    ModelPars[ iM_phi_f   ] = params.phi_f ;
    ModelPars[ iM_phi_i   ] = params.phi_i ;
  }

  // --------------------------------------------------------------------------
  void
  BikeSteering::setupParameters( valueConstPointer Pars ) {
    std::copy( Pars, Pars + numModelPars, ModelPars ) ;
  }

  // --------------------------------------------------------------------------
  void
  BikeSteering::setupClasses( BikeSteering_contraints_struct const & params ) {
    // Initialize Constraints 1D
    minimumTimeSize.setup( params.minimumTimeSize.subType
                         , params.minimumTimeSize.epsilon
                         , params.minimumTimeSize.tolerance ) ;


    // 2D constraints (DA FARE!!!!)

    // Controls
    FyControl.setup( params.FyControl.type
                   , params.FyControl.epsilon
                   , params.FyControl.tolerance ) ;
  }

  // --------------------------------------------------------------------------
  void
  BikeSteering::infoClasses( basic_ostream<char> & stream ) const {

    stream << "\nControls\n" ;
    FyControl . info(stream) ;

    stream << "\nConstraints 1D\n" ;
    minimumTimeSize . info(stream) ;

    stream << "\nUser class (pointer)\n" ;
    stream << "User function `pMesh`: " ;
    pMesh -> info( stream ) ;

    stream << "\nModel Parameters\n" ;
    for ( indexType i = 0 ; i < numModelPars ; ++i )
      stream << setw(40) << setfill('.') << namesModelPars[i]
             << " = " << ModelPars[i] << setfill(' ') << '\n' ;


  }

  // --------------------------------------------------------------------------
  void
  BikeSteering::setup( GC::GenericContainer const & gc ) {

    ASSERT( gc.exists("Parameters"),         "BikeSteering::setup: Missing key `Parameters` in data" ) ;
    ASSERT( gc.exists("Constraints"),        "BikeSteering::setup: Missing key `Constraints` in data" ) ;
    ASSERT( gc.exists("Controls"),           "BikeSteering::setup: Missing key `Controls` in data" ) ;
    ASSERT( gc.exists("BoundaryConditions"), "BikeSteering::setup: Missing key `BoundaryConditions` in data" ) ;

    ASSERT( gc.exists("MappedObjects"), "BikeSteering::setup: Missing key `MappedObjects` in data" ) ;

    ASSERT( gc.exists("Pointers"), "BikeSteering::setup: Missing key `Pointers` in data" ) ;

    this->setupParameters( gc("Parameters") ) ;
    this->setupClasses( gc("Constraints") ) ;
    this->setupMappedObjects( gc("MappedObjects") ) ;
    this->setupControls( gc("Controls") ) ;
    this->setupUserClasses( gc ) ;
    this->setupPointers( gc("Pointers") ) ;
    this->setupBC( gc("BoundaryConditions") ) ;

    // setup model
    if ( gc.exists("InfoLevel") ) changeInfoLevel(gc("InfoLevel").get_int()) ;
    if ( BVNLFD_Model::infoLevel >= 2 ) this->infoBC( *BVNLFD_Model::pCout ) ;
    if ( BVNLFD_Model::infoLevel >= 3 ) this->infoClasses( *BVNLFD_Model::pCout ) ;

    // setup nonlinear system with object handling mesh domain    
    system.setup( pMesh, this );
    if ( gc.exists("LU_method") ) system.setup( pMesh, this, gc("LU_method").get_int() );
    else                          system.setup( pMesh, this );

    if ( BVNLFD_Model::infoLevel >= 2 ) system.info( *BVNLFD_Model::pCout ) ;
  }

  // --------------------------------------------------------------------------
  bool
  BikeSteering::solve( GC::GenericContainer const & gc,
                       GC::GenericContainer       & gc_sol ) {
    bool ok = true ;
    gc_sol["converged"]  = false ;
    gc_sol["iterations"] = 0 ;

    try {

      // setup solver
      solver.setup( &system, BVNLFD_Model::pCout, BVNLFD_Model::infoLevel ) ;
      solver.setSolverParameters( gc("Solver") ) ;

      // compute solution
      if (gc.exists("Warm") && gc("Warm").get_bool()) solver.setNextComputationWarm();
      solver.computeSolution() ;
      ok = solver.isConverged() ;

      if ( BVNLFD_Model::infoLevel >= 1 ) solver.statistic(*BVNLFD_Model::pCout) ;

      // Save solution in generic object (even if not converged)
      system.saveSolution( solver.solution(), gc_sol ) ;
      
      // add some informations
      gc_sol("converged")  = ok ;
      gc_sol("iterations") = solver.iterations() ;
      if ( !ok ) {
        gc_sol["Error"] = solver.whatHappened() ;
      }
      bool do_diagnosis = false ;
      if ( gc.exists("Doctor") ) do_diagnosis = gc("Doctor").get_bool() ;
      if ( do_diagnosis ) system.diagnosis( solver.solution(), gc_sol["diagnosis"] ) ;
    }
    catch ( exception const & exc ) {
      gc_sol["Error"] = exc.what() ;
      ok = false ;
    }
    catch ( char const exc[] ) {
      gc_sol["Error"] = exc ;
      ok = false ;
    }
    catch (...) {
      gc_sol["Error"] = "BikeSteering::solver, fatal: unknown error" ;
      ok = false ;
    }
    return ok ;
  }
}
