#!/usr/bin/env ruby
 #--------------------------------------------------------------------------#
 #  file: Bike1D_run_ffi.rb                                                 #
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


require './lib/Bike1D_ffi.rb'
require './data/Bike1D_Data.rb'

ocp = Bike1D::OCPSolver.new

begin
  res = ocp.solve
  if res[:converged] then
    ocp.write_ocp_solution("data/Bike1D_OCP_result.txt")
  else
    ocp.write_ocp_solution("data/Bike1D_OCP_result_NOT_CONVERGED.txt")     
  end
rescue RuntimeError => e
  puts e
  ocp.write_ocp_solution("data/Bike1D_OCP_result_NOT_CONVERGED.txt")     
rescue
  puts "Unknown error!"
end

if ARGV.include? "-g" then
  dg = Dgraph.new
  dg.set_fields %w(z x v)
  dg.plot "z", "x"
  dg.plot "z", "v"
  res[:data][0].each_with_index do |z,i|
    dg << {z:z, x:res[:data][1][i], v:res[:data][2][i]}
  end
  dg.close
end

puts "All Done Folks!"

# EOF
