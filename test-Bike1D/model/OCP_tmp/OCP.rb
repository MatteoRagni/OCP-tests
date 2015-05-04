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
        :value   => "50000",
        :name    => "Pmax",
        :C_value => "50000",
      },
      {
        :value   => "9.81",
        :name    => "g",
        :C_value => "0.981e1",
      },
      {
        :value   => "275",
        :name    => "m",
        :C_value => "275",
      },
      {
        :value   => "-1",
        :name    => "muf_min",
        :C_value => "-1",
      },
      {
        :value   => "1",
        :name    => "mur_max",
        :C_value => "1",
      },
      {
        :value   => "-1",
        :name    => "mur_min",
        :C_value => "-1",
      },
      {
        :value   => "10",
        :name    => "v_f",
        :C_value => "10",
      },
      {
        :value   => "10",
        :name    => "v_i",
        :C_value => "10",
      },
      {
        :value   => "100",
        :name    => "vmax",
        :C_value => "100",
      },
    ],

    #--------------------------
    :AuxiliaryParameters => [
      {
        :value   => "1",
        :name    => "mur_max",
        :C_value => "1",
      },
      {
        :value   => "-1",
        :name    => "muf_min",
        :C_value => "-1",
      },
      {
        :value   => "-1",
        :name    => "mur_min",
        :C_value => "-1",
      },
    ],

    #--------------------------
    :UserFunctions => [
      {
        :args => [ "_v" ],
        :body => "2",
        :derivatives => "2",
        :func => "Tmax_normalized",
      },
    ],
    :UserMapFunctions => [
      {
        :namepars => [ "h", "delta" ],
        :args => [ "x", "a", "b" ],
        :class => "ClipIntervalWithSinAtan",
        :par_delta => "0",
        :func => "clip",
        :par_h => "0.01",
      },
    ],
    :UserFunctionsClassInstances => [
      {
        :mapped => [],
        :header => "#include <MechatronixCore/MechatronixCore.hh>",
        :class => "Mechatronix#MeshStd",
        :setup => {
          :s0 => "0",
          :segments => [
            {
              :length => "1000",
              :n      => "1000",
            },
          ],
        },
        :instance => "*pMesh",
        :is_mesh_object => "true",
        :namespace => "MechatronixCore",
      },
    ],

    #--------------------------
    :Constraint1D => [],
    :Constraint2D => [],
    :ConstraintU  => [
      {
        :max       => "clip(Tmax_normalized(v(zeta)),0,mur_max)",
        :class     => "PenaltyU",
        :min       => "mur_min",
        :u         => "mur(zeta)",
        :scale     => "1/v(zeta)",
        :name      => "murControl",
        :epsilon   => "0.001",
        :type      => "U_LOGARITHMIC",
        :tolerance => "0.001",
      },
      {
        :max       => "0",
        :class     => "PenaltyU",
        :min       => "muf_min",
        :u         => "muf(zeta)",
        :scale     => "1/v(zeta)",
        :name      => "mufControl",
        :epsilon   => "0.001",
        :type      => "U_LOGARITHMIC",
        :tolerance => "0.001",
      },
    ],
    :Bc => [
      {
        :value => "1",
        :name  => "initial_v",
        :name1 => "initial_v",
      },
      {
        :value => "1",
        :name  => "final_v",
        :name1 => "final_v",
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