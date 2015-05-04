/*--------------------------------------------------------------------------*\
 |  file: Bike1D.cc                                                         |
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


#include "Bike1D.hh"
#include "Bike1D_Pars.hh"

namespace Bike1DDefine {

  char const *namesZvars[numZvars+1] = {
    "v",
    "lambda1",
    NULL
  } ;

  char const *namesUvars[numUvars+1] = {
    "mur",
    "muf",
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
    NULL
  } ;

  char const *namesPostProcess[numPostProcess+1] = {
    "lagrange_target",
    "penalties",
    "murControl",
    "mufControl",
    "Tmax_norma",
    NULL
  } ;

  char const *namesIntegratedPostProcess[numIntegratedPostProcess+1] = {
    "time",
    NULL
  } ;

  char const *namesModelPars[numModelPars+1] = {
    "Pmax",
    "g",
    "m",
    "muf_min",
    "mur_max",
    "mur_min",
    "v_f",
    "v_i",
    "vmax",
    NULL
  } ;

  char const *namesConstraint1D[numConstraint1D+1] = {
    NULL
  } ;

  char const *namesConstraint2D[numConstraint2D+1] = {
    NULL
  } ;

  char const *namesConstraintU[numConstraintU+1] = {
    "murControl",
    "mufControl",
    NULL
  } ;

  char const *namesBc[numBc+1] = {
    "initial_v",
    "final_v",
    NULL
  } ;

  // --------------------------------------------------------------------------
  Bike1D::Bike1D( string const             & name,
                  std::basic_ostream<char> * _pCout,
                  indexType                  _infoLevel )
  : BVNLFD_Model(name,_pCout,_infoLevel)
  // Controls
  , murControl("murControl")
  , mufControl("mufControl")
  // Constraints 1D
  // Constraints 2D
  // User classes
  , system("Bike1DSystem")
  , solver("Bike1DSolver",_pCout,_infoLevel)
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

  Bike1D::~Bike1D() {
  }

  // --------------------------------------------------------------------------
  // initialize parameters using associative array

  void
  Bike1D::setupParameters( GC::GenericContainer const & gc ) {
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
    ASSERT( allfound, "in Bike1D::setup not all parameters are set!") ;
  }

  // --------------------------------------------------------------------------
  void
  Bike1D::setupClasses( GC::GenericContainer const & gc ) {
  }

  // --------------------------------------------------------------------------
  void
  Bike1D::setupUserClasses( GC::GenericContainer const & gc ) {
  }

  // --------------------------------------------------------------------------
  void
  Bike1D::setupMappedObjects( GC::GenericContainer const & gc ) {

    // Initialize user mapped functions

    ASSERT( gc.exists("clip"), "in Bike1D::setupMappedObjects(gc) missing key: ``clip''");
    clip.setup( gc("clip") ) ;
  }

  // --------------------------------------------------------------------------
  void
  Bike1D::setupControls( GC::GenericContainer const & gc ) {
    // initialize Controls

    { GC::GenericContainer const & gc1 = gc("murControl") ;
      murControl.setup( gc1("type").get_int()
                      , gc1("epsilon").get_number()
                      , gc1("tolerance").get_number() ) ; }

    { GC::GenericContainer const & gc1 = gc("mufControl") ;
      mufControl.setup( gc1("type").get_int()
                      , gc1("epsilon").get_number()
                      , gc1("tolerance").get_number() ) ; }

  }

  // --------------------------------------------------------------------------
  void
  Bike1D::setupPointers( GC::GenericContainer const & gc ) {

    // Initialize user classes

    ASSERT( gc.exists("pMesh"), "in Bike1D::setupPointers(gc) cant find key `pMesh' in gc" ) ;
    pMesh = gc("pMesh").get_pointer<MeshStd*>() ;
  }

  // --------------------------------------------------------------------------
  // initialize parameters using structures
  void
  Bike1D::setupBC( Bike1D_BC_params_struct const & params ) {
    // Bc on/off
    bool active[] = {
      (bool)params.initial_v,
      (bool)params.final_v,
      false
    } ;
    BVNLFD_Model::setupBC( active ) ; // Setup active boundary conditions
  }

  // --------------------------------------------------------------------------
  void
  Bike1D::setupParameters( Bike1D_model_params_struct const & params ) {
    ModelPars[ iM_Pmax    ] = params.Pmax ;
    ModelPars[ iM_g       ] = params.g ;
    ModelPars[ iM_m       ] = params.m ;
    ModelPars[ iM_muf_min ] = params.muf_min ;
    ModelPars[ iM_mur_max ] = params.mur_max ;
    ModelPars[ iM_mur_min ] = params.mur_min ;
    ModelPars[ iM_v_f     ] = params.v_f ;
    ModelPars[ iM_v_i     ] = params.v_i ;
    ModelPars[ iM_vmax    ] = params.vmax ;
  }

  // --------------------------------------------------------------------------
  void
  Bike1D::setupParameters( valueConstPointer Pars ) {
    std::copy( Pars, Pars + numModelPars, ModelPars ) ;
  }

  // --------------------------------------------------------------------------
  void
  Bike1D::setupClasses( Bike1D_contraints_struct const & params ) {
    // Initialize Constraints 1D

    // 2D constraints (DA FARE!!!!)

    // Controls
    murControl.setup( params.murControl.type
                    , params.murControl.epsilon
                    , params.murControl.tolerance ) ;
    mufControl.setup( params.mufControl.type
                    , params.mufControl.epsilon
                    , params.mufControl.tolerance ) ;
  }

  // --------------------------------------------------------------------------
  void
  Bike1D::infoClasses( basic_ostream<char> & stream ) const {

    stream << "\nControls\n" ;
    murControl . info(stream) ;
    mufControl . info(stream) ;

    stream << "\nUser class (pointer)\n" ;
    stream << "User function `pMesh`: " ;
    pMesh -> info( stream ) ;

    stream << "\nUser mapped functions\n" ;
    clip . info( stream ) ;

    stream << "\nModel Parameters\n" ;
    for ( indexType i = 0 ; i < numModelPars ; ++i )
      stream << setw(40) << setfill('.') << namesModelPars[i]
             << " = " << ModelPars[i] << setfill(' ') << '\n' ;


  }

  // --------------------------------------------------------------------------
  void
  Bike1D::setup( GC::GenericContainer const & gc ) {

    ASSERT( gc.exists("Parameters"),         "Bike1D::setup: Missing key `Parameters` in data" ) ;
    ASSERT( gc.exists("Constraints"),        "Bike1D::setup: Missing key `Constraints` in data" ) ;
    ASSERT( gc.exists("Controls"),           "Bike1D::setup: Missing key `Controls` in data" ) ;
    ASSERT( gc.exists("BoundaryConditions"), "Bike1D::setup: Missing key `BoundaryConditions` in data" ) ;

    ASSERT( gc.exists("MappedObjects"), "Bike1D::setup: Missing key `MappedObjects` in data" ) ;

    ASSERT( gc.exists("Pointers"), "Bike1D::setup: Missing key `Pointers` in data" ) ;

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
  Bike1D::solve( GC::GenericContainer const & gc,
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
      gc_sol["Error"] = "Bike1D::solver, fatal: unknown error" ;
      ok = false ;
    }
    return ok ;
  }
}
