class COMMON_Build
  DATA = {

    #--------------------------
    :Xvars => [ "omega", "phi", "TimeSize" ],
    :Lvars => [ "lambda1", "lambda2", "lambda3" ],
    :Zvars => [ "omega", "phi", "TimeSize", "lambda1", "lambda2", "lambda3" ],
    :Uvars => [ "Fy" ],
    :Pvars => [],
    :OMEGAvars => [ "omega1", "omega2", "omega3", "omega4" ],
    :POSTnames => [
      "lagrange_target",
      "penalties",
      "FyControl",
      "minimumTimeSize",
      "minTime",
      "Talpha",
    ],
    :INTPOSTnames => [],
    :Qvars => [ "zeta" ],

    #--------------------------
    :bvp_parameters => [ "Fmax", "Ix", "g", "h", "m" ],
    :guess_parameters => [],
    :bc_parameters => [ "omega_f", "omega_i", "phi_f", "phi_i" ],
    :post_processing_parameters => [],
    :user_function_parameters => [],
    :continuation_parameters => [],
    :constraint_parameters => [],

    #--------------------------
    :ModelParameters => [
      {
        :name    => "Fmax",
        :value   => "m*g*mu",
        :C_value => "m * g * mu",
      },
      {
        :name    => "Ix",
        :value   => "40",
        :C_value => "40",
      },
      {
        :name    => "g",
        :value   => "9.81",
        :C_value => "0.981e1",
      },
      {
        :name    => "h",
        :value   => "0.6",
        :C_value => "0.6e0",
      },
      {
        :name    => "m",
        :value   => "250",
        :C_value => "250",
      },
      {
        :name    => "omega_f",
        :value   => "0",
        :C_value => "0",
      },
      {
        :name    => "omega_i",
        :value   => "0",
        :C_value => "0",
      },
      {
        :name    => "phi_f",
        :value   => "0.174533",
        :C_value => "0.1745329252e0",
      },
      {
        :name    => "phi_i",
        :value   => "0",
        :C_value => "0",
      },
    ],

    #--------------------------
    :AuxiliaryParameters => [
      {
        :name    => "g",
        :value   => "9.81",
        :C_value => "0.981e1",
      },
      {
        :name    => "mu",
        :value   => "1",
        :C_value => "1",
      },
      {
        :name    => "m",
        :value   => "250",
        :C_value => "250",
      },
      {
        :name    => "Fmax",
        :value   => "m*g*mu",
        :C_value => "m * g * mu",
      },
    ],

    #--------------------------
    :UserFunctions => [],
    :UserMapFunctions => [],
    :UserFunctionsClassInstances => [
      {
        :namespace => "MechatronixCore",
        :is_mesh_object => "true",
        :header => "#include <MechatronixCore/MechatronixCore.hh>",
        :setup => {
          :s0 => "0",
          :segments => [
            {
              :length => "0.1",
              :n      => "10",
            },
            {
              :length => "0.4",
              :n      => "40",
            },
            {
              :length => "0.4",
              :n      => "40",
            },
            {
              :length => "0.1",
              :n      => "10",
            },
          ],
        },
        :mapped => [],
        :class => "Mechatronix#MeshStd",
        :instance => "*pMesh",
      },
    ],

    #--------------------------
    :Constraint1D => [
      {
        :name      => "minimumTimeSize",
        :epsilon   => "0.001",
        :arg       => "TimeSize(zeta)",
        :subtype   => "PENALTY_REGULAR",
        :class     => "Penalty1DGreaterThan",
        :scale     => "1",
        :active    => "true",
        :tolerance => "0.001",
      },
    ],
    :Constraint2D => [],
    :ConstraintU  => [
      {
        :name      => "FyControl",
        :epsilon   => "0.001",
        :u         => "Fy(zeta)",
        :min       => "-Fmax",
        :max       => "Fmax",
        :class     => "PenaltyU",
        :scale     => "TimeSize(zeta)",
        :tolerance => "0.001",
        :type      => "U_COS_LOGARITHMIC",
      },
    ],
    :Bc => [
      {
        :name  => "initial_omega",
        :value => "1",
        :name1 => "initial_omega",
      },
      {
        :name  => "initial_phi",
        :value => "1",
        :name1 => "initial_phi",
      },
      {
        :name  => "final_omega",
        :value => "1",
        :name1 => "final_omega",
      },
      {
        :name  => "final_phi",
        :value => "1",
        :name1 => "final_phi",
      },
    ],
    :q_n_eqns => 1,
    :a_n_eqns => 6,
    :u_n_eqns => 1,
    :p_guess_n_eqns => 0,
    :jump_n_eqns => 6,
    :BC_n_eqns => 10,
    :post_n_eqns => 6,
    :integrated_post_n_eqns => 0,
    :g_n_eqns => 1,

    :DaDz => {
      :n_rows  => 6,
      :n_cols  => 12,
      :nnz     => 40,
      :pattern => [
        [  0,   2], [  0,   3], [  0,   4], [  0,   8], [  0,   9], [  0,  10],
        [  1,   2], [  1,   3], [  1,   4], [  1,   8], [  1,   9], [  1,  10],
        [  2,   0], [  2,   1], [  2,   2], [  2,   3], [  2,   4], [  2,   5],
        [  2,   6], [  2,   7], [  2,   8], [  2,   9], [  2,  10], [  2,  11],
        [  3,   0], [  3,   1], [  3,   2], [  3,   6], [  3,   7], [  3,   8],
        [  4,   0], [  4,   1], [  4,   2], [  4,   4], [  4,   6], [  4,   7],
        [  4,   8], [  4,  10], [  5,   2], [  5,   8],
      ],
    },


    :DaDp => {
      :n_rows  => 6,
      :n_cols  => 0,
      :nnz     => 0,
      :pattern => [],
    },


    :DgDu => {
      :n_rows  => 1,
      :n_cols  => 1,
      :nnz     => 1,
      :pattern => [ [0, 0] ],
    },


    :DgDp => {
      :n_rows  => 1,
      :n_cols  => 0,
      :nnz     => 0,
      :pattern => [],
    },


    :DjumpDz => {
      :n_rows  => 6,
      :n_cols  => 12,
      :nnz     => 12,
      :pattern => [
        [  0,   0], [  0,   6], [  1,   1], [  1,   7], [  2,   2], [  2,   8],
        [  3,   4], [  3,  10], [  4,   3], [  4,   9], [  5,   5], [  5,  11],
      ],
    },


    :DjumpDp => {
      :n_rows  => 6,
      :n_cols  => 0,
      :nnz     => 0,
      :pattern => [],
    },


    :DBCDz => {
      :n_rows  => 10,
      :n_cols  => 12,
      :nnz     => 10,
      :pattern => [
        [  0,   0], [  1,   1], [  2,   6], [  3,   7], [  4,   4], [  5,   3],
        [  6,   5], [  7,  10], [  8,   9], [  9,  11],
      ],
    },


    :DBCDpomega => {
      :n_rows  => 10,
      :n_cols  => 4,
      :nnz     => 4,
      :pattern => [
        [  4,   0], [  5,   1], [  7,   2], [  8,   3],
      ],
    },

    :z_guess_n_eqns => 6,
    :z_check_n_eqns => 0,
    :u_guess_n_eqns => 1,
    :u_check_n_eqns => 1,

    :NumContinuationStep => 0

  }
end
# end of data file