class COMMON_Build
  DATA = {

    #--------------------------
    :Xvars => [ "v" ],
    :Lvars => [ "lambda1" ],
    :Zvars => [ "v", "lambda1" ],
    :Uvars => [ "mur", "muf" ],
    :Pvars => [],
    :OMEGAvars => [ "omega1", "omega2" ],
    :POSTnames => [ "lagrange_target", "penalties", "murControl", "mufControl", "Tmax_norma" ],
    :INTPOSTnames => [ "time" ],
    :Qvars => [ "zeta" ],

    #--------------------------
    :bvp_parameters => [ "g", "muf_min", "mur_max", "mur_min" ],
    :guess_parameters => [],
    :bc_parameters => [ "v_f", "v_i" ],
    :post_processing_parameters => [],
    :user_function_parameters => [ "Pmax", "m", "vmax" ],
    :continuation_parameters => [],
    :constraint_parameters => [],

    #--------------------------
    :ModelParameters => [
      {
        :name    => "Pmax",
        :C_value => "50000",
        :value   => "50000",
      },
      {
        :name    => "g",
        :C_value => "0.981e1",
        :value   => "9.81",
      },
      {
        :name    => "m",
        :C_value => "275",
        :value   => "275",
      },
      {
        :name    => "muf_min",
        :C_value => "-1",
        :value   => "-1",
      },
      {
        :name    => "mur_max",
        :C_value => "1",
        :value   => "1",
      },
      {
        :name    => "mur_min",
        :C_value => "-1",
        :value   => "-1",
      },
      {
        :name    => "v_f",
        :C_value => "10",
        :value   => "10",
      },
      {
        :name    => "v_i",
        :C_value => "10",
        :value   => "10",
      },
      {
        :name    => "vmax",
        :C_value => "100",
        :value   => "100",
      },
    ],

    #--------------------------
    :AuxiliaryParameters => [
      {
        :name    => "muf_min",
        :C_value => "-1",
        :value   => "-1",
      },
      {
        :name    => "mur_min",
        :C_value => "-1",
        :value   => "-1",
      },
      {
        :name    => "mur_max",
        :C_value => "1",
        :value   => "1",
      },
    ],

    #--------------------------
    :UserFunctions => [
      {
        :func => "Tmax_normalized",
        :derivatives => "2",
        :args => [ "_v" ],
        :body => "2",
      },
    ],
    :UserMapFunctions => [
      {
        :par_delta => "0",
        :func => "clip",
        :par_h => "0.01",
        :namepars => [ "delta", "h" ],
        :args => [ "x", "a", "b" ],
        :class => "ClipIntervalWithSinAtan",
      },
    ],
    :UserFunctionsClassInstances => [
      {
        :setup => {
          :s0 => "0",
          :segments => [
            {
              :length => "1000",
              :n      => "1000",
            },
          ],
        },
        :namespace => "MechatronixCore",
        :is_mesh_object => "true",
        :instance => "*pMesh",
        :header => "#include <MechatronixCore/MechatronixCore.hh>",
        :mapped => [],
        :class => "Mechatronix#MeshStd",
      },
    ],

    #--------------------------
    :Constraint1D => [],
    :Constraint2D => [],
    :ConstraintU  => [
      {
        :type      => "U_LOGARITHMIC",
        :name      => "murControl",
        :scale     => "1/v(zeta)",
        :epsilon   => "0.001",
        :tolerance => "0.001",
        :min       => "mur_min",
        :max       => "clip(Tmax_normalized(v(zeta)),0,mur_max)",
        :u         => "mur(zeta)",
        :class     => "PenaltyU",
      },
      {
        :type      => "U_LOGARITHMIC",
        :name      => "mufControl",
        :scale     => "1/v(zeta)",
        :epsilon   => "0.001",
        :tolerance => "0.001",
        :min       => "muf_min",
        :max       => "0",
        :u         => "muf(zeta)",
        :class     => "PenaltyU",
      },
    ],
    :Bc => [
      {
        :name  => "initial_v",
        :name1 => "initial_v",
        :value => "1",
      },
      {
        :name  => "final_v",
        :name1 => "final_v",
        :value => "1",
      },
    ],
    :q_n_eqns => 1,
    :a_n_eqns => 2,
    :u_n_eqns => 2,
    :p_guess_n_eqns => 0,
    :jump_n_eqns => 2,
    :BC_n_eqns => 4,
    :post_n_eqns => 5,
    :integrated_post_n_eqns => 1,
    :g_n_eqns => 2,

    :DaDz => {
      :n_rows  => 2,
      :n_cols  => 4,
      :nnz     => 8,
      :pattern => [
        [  0,   0], [  0,   1], [  0,   2], [  0,   3], [  1,   0], [  1,   1],
        [  1,   2], [  1,   3],
      ],
    },


    :DaDp => {
      :n_rows  => 2,
      :n_cols  => 0,
      :nnz     => 0,
      :pattern => [],
    },


    :DgDu => {
      :n_rows  => 2,
      :n_cols  => 2,
      :nnz     => 2,
      :pattern => [
        [  0,   0], [  1,   1],
      ],
    },


    :DgDp => {
      :n_rows  => 2,
      :n_cols  => 0,
      :nnz     => 0,
      :pattern => [],
    },


    :DjumpDz => {
      :n_rows  => 2,
      :n_cols  => 4,
      :nnz     => 6,
      :pattern => [
        [  0,   0], [  0,   2], [  1,   0], [  1,   1], [  1,   2], [  1,   3],
      ],
    },


    :DjumpDp => {
      :n_rows  => 2,
      :n_cols  => 0,
      :nnz     => 0,
      :pattern => [],
    },


    :DBCDz => {
      :n_rows  => 4,
      :n_cols  => 4,
      :nnz     => 6,
      :pattern => [
        [  0,   0], [  1,   2], [  2,   0], [  2,   1], [  3,   2], [  3,   3],
      ],
    },


    :DBCDpomega => {
      :n_rows  => 4,
      :n_cols  => 2,
      :nnz     => 2,
      :pattern => [
        [  2,   0], [  3,   1],
      ],
    },

    :z_guess_n_eqns => 2,
    :z_check_n_eqns => 0,
    :u_guess_n_eqns => 2,
    :u_check_n_eqns => 2,

    :NumContinuationStep => 0

  }
end
# end of data file