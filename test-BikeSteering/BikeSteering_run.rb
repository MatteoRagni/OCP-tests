#!/usr/bin/env pins
 #--------------------------------------------------------------------------#
 #  file: BikeSteering_run.rb                                               #
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

require 'mechatronix'

# Set the model name
problem_name = "BikeSteering"

# Compile the scripts, unless the library already exists
# the command line flag '-f' forces recompilation
# the command line flag '-b' only performs compilation and skips solution
if ! File.exist?("lib/lib#{problem_name}.dylib") || ARGV.include?('-f') || ARGV.include?('-b') then
  require "build"
  compiler = MXBuilder.new(problem_name)
  compiler.build
  exit if ARGV.include?('-b')
end

require './data/BikeSteering_Data.rb'

# If requested, enable the Doctor
Mechatronix.content.Doctor = ARGV.include?('-d')

# Link the library
ocp = Mechatronix::OCPSolver.new "lib/lib#{problem_name}.#{Mechatronix::DYLIB_EXT}"

# Setup the solver
ocp.setup

# Calculate the OCP solution. Result is into ocp.ocp_solution
ocp.solve

# Write results
unless ocp.ocp_solution[:Error] then
  if ocp.ocp_solution[:converged] then
    ocp.write_ocp_solution("data/#{problem_name}_OCP_result.txt")
  else
    ocp.write_ocp_solution("data/#{problem_name}_OCP_result_NOT_CONVERGED.txt")
  end
else
  ocp.write_ocp_solution("data/#{problem_name}_OCP_result_NOT_CONVERGED.txt")
  puts ocp.ocp_solution[:Error]
end

# If requested, print out Doctor's diagnosis
if ARGV.include?('-d') or Mechatronix.content.Doctor then
  d = ocp.ocp_solution[:diagnosis]
  puts "\nDOCTOR'S DIAGNOSIS\n#{'=' * 79}"
  puts "- INVERSE OF CONDITION NUMBER (the smaller, the more ill-conditioned):"
  puts "  Norm 1        = #{d[:inverse_of_conditon_number][:norm_1]}"
  puts "  Norm Infinity = #{d[:inverse_of_conditon_number][:norm_inf]}"
  #ctrls = d[:controls]
  #if ctrls && ctrls.length > 0 then
  #  ncrtl = ctrls.length
  #  ncell = ctrls[0].length
  #  (0..ncell-1).each { |nc|
  #    puts "  cell[#{nc}] (rcond) = #{ctrls[ncrtl-1][nc]/ctrls[0][nc]}"
  #  }
  #end
  #cells  = d[:cells]
  #ncells = cells.length
  #cells.each_with_index do |c,idx|
  #  puts "  cell[#{idx}] (X-rcond) = #{c[:X_singular_values][-1]/c[:X_singular_values][0]}"
  #  puts "  cell[#{idx}] (L-rcond) = #{c[:LAMBDA_singular_values][-1]/c[:LAMBDA_singular_values][0]}"
  #  puts "  cell[#{idx}] (rcond)   = #{c[:ratio_singular_value]}"
  #end
  puts "  bc = #{d[:bc]}"
  puts "#{'=' * 79}"
end

figlet "All Done Folks!"

# EOF
