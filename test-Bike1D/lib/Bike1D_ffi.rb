 #--------------------------------------------------------------------------#
 #  file: Bike1D_run.rb                                                     #
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


require 'ffi'

#class FFI::Pointer
#  def read_array_of_string
#    elements = []
#    loc = self
#    until ((element = loc.read_pointer).null?)
#      elements << element.read_string
#      loc += FFI::Type::POINTER.size
#    end
#    return elements
#  end
#end

module Mechatronix
  SET               = true
  FREE              = false
  
  LU_automatic      = 0
  LU_amodio         = 1
  LU_block          = 2
  LU_arceco         = 3
  LU_superLU        = 4
  
  U_QUADRATIC       = 0
  U_QUADRATIC2      = 1
  U_CUBIC           = 2
  U_QUARTIC         = 3
  U_PARABOLA        = 4
  U_LOGARITHMIC     = 5
  U_COS_LOGARITHMIC = 6
  U_TAN2            = 7
  U_HYPERBOLIC      = 8
  U_BIPOWER         = 9
  
  PENALTY_REGULAR   = 0
  PENALTY_SMOOTH    = 1
  PENALTY_PIECEWISE = 2
  
  BARRIER_LOG       = 0
  BARRIER_LOG_EXP   = 1
  
  PENALTY2D_RHOMB      = 0
  PENALTY2D_ELLIPSE    = 1
  PENALTY2D_FOURCLOVER = 2
  
  GREATER_THAN = 0
  LESS_THAN    = 1
  INTERVAL     = 2
  POWER        = 3
  BIPOWER      = 4
end

module Bike1D

    
  extend FFI::Library
  
  #base_path = File.dirname(__FILE__)
  base_path = File.expand_path('../', __FILE__)

  case RUBY_PLATFORM
  when /darwin/
    LIB_PATH = base_path+"/libBike1D.dylib"
  when /linux/
    LIB_PATH = base_path+"/libBike1D.so"
  when /mingw/
    LIB_PATH = base_path+"/libBike1D.dll"
  else
    raise RuntimeError, "Unsupported platform: #{RUBY_PLATFORM}"
  end

  raise RuntimeError, "Missing library #{LIB_PATH}" unless File.exist? LIB_PATH
  begin
    ffi_lib LIB_PATH
  rescue
    warn "remember to run export DYLD_LIBRARY_PATH=#{LIB_PATH}"
    exit
  end

  # GENERIC_CONTAINER_OK = 0
  # GENERIC_CONTAINER_BAD_TYPE
  # GENERIC_CONTAINER_NO_DATA
  # GENERIC_CONTAINER_NOT_EMPTY
  # GENERIC_CONTAINER_BAD_HEAD

  attach_function :GC_select,                     [ :string ], :int
  attach_function :GC_delete,                     [ :string ], :int
  attach_function :GC_fill_for_test,              [ :string ], :int
  
  attach_function :GC_get_type,                   [], :int
  attach_function :GC_get_type_name,              [], :string
  attach_function :GC_print,                      [], :int
  attach_function :GC_mem_ptr,                    [ :string ], :pointer

  # head movement
  attach_function :GC_pop_head,                   [], :int
  attach_function :GC_reset_head,                 [], :int

  # set
  attach_function :GC_set_bool,                   [ :int ], :int
  attach_function :GC_set_int,                    [ :int ], :int
  attach_function :GC_set_real,                   [ :double ], :int
  attach_function :GC_set_complex2,               [ :double, :double ], :int
  attach_function :GC_set_string,                 [ :string ], :int

  # get
  attach_function :GC_get_bool,                   [], :int
  attach_function :GC_get_int,                    [], :int
  attach_function :GC_get_long,                   [], :long
  attach_function :GC_get_real,                   [], :double
  attach_function :GC_get_complex_re,             [], :double
  attach_function :GC_get_complex_im,             [], :double
  attach_function :GC_get_string,                 [], :string
  
  # push
  attach_function :GC_push_bool,                  [ :int ], :int
  attach_function :GC_push_int,                   [ :int ], :int
  attach_function :GC_push_real,                  [ :double ], :int
  attach_function :GC_push_complex2,              [ :double, :double ], :int
  attach_function :GC_push_string,                [ :string ], :int

  # get with position
  attach_function :GC_get_bool_at_pos,            [ :int ], :int
  attach_function :GC_get_int_at_pos,             [ :int ], :int
  attach_function :GC_get_real_at_pos,            [ :int ], :double
  attach_function :GC_get_complex_real_at_pos,    [ :int ], :double
  attach_function :GC_get_complex_imag_at_pos,    [ :int ], :double
  attach_function :GC_get_string_at_pos,          [ :int ], :string

  attach_function :GC_get_matrix_num_rows,        [], :int
  attach_function :GC_get_matrix_num_cols,        [], :int

  attach_function :GC_get_real_at_coor,           [ :int, :int ], :double
  attach_function :GC_get_complex_real_at_coor,   [ :int, :int ], :double
  attach_function :GC_get_complex_imag_at_coor,   [ :int, :int ], :double

  # empty vectors
  attach_function :GC_set_empty_vector_of_bool,   [], :int
  attach_function :GC_set_empty_vector_of_int,    [], :int
  attach_function :GC_set_empty_vector_of_real,   [], :int
  attach_function :GC_set_empty_vector_of_complex,[], :int
  attach_function :GC_set_empty_vector_of_string, [], :int

  # vectors not used
  #attach_function :GC_set_vector_of_bool,         [ :pointer, :int ], :int
  #attach_function :GC_set_vector_of_int,          [ :pointer, :int ], :int
  #attach_function :GC_set_vector_of_real,         [ :pointer, :int ], :int
  #attach_function :GC_set_vector_of_complex,      [ :pointer, :int ], :int
  #attach_function :GC_set_vector_of_string,       [ :pointer, :int ], :int

  # generic vector
  attach_function :GC_set_vector,                 [ :int ], :int
  attach_function :GC_set_empty_vector,           [], :int
  attach_function :GC_push_vector_position,       [ :int ], :int
  attach_function :GC_get_vector_size,            [], :int

  # generic map
  attach_function :GC_set_map,                    [], :int
  attach_function :GC_init_map_key,               [], :int
  attach_function :GC_get_next_key,               [], :string
  attach_function :GC_push_map_position,          [ :string ], :int

  #############################################################################

  attach_function :OCP_setup,
                  :Bike1D_ocp_setup,
                  [ :string, :buffer_in ], :bool

  attach_function :OCP_solve,
                  :Bike1D_ocp_solve,
                  [ :string, :buffer_in, :buffer_out ], :bool

  attach_function :OCP_write_ocp_solution,
                  :Bike1D_write_ocp_solution,
                  [ :string, :string ], :void

  attach_function :SPLINE_new,           [ :string, :string ], :int
  attach_function :SPLINE_select,        [ :string ], :int
  attach_function :SPLINE_delete,        [ :string ], :int
  attach_function :SPLINE_print,         [], :int
  attach_function :SPLINE_get_type_name, [], :string
  attach_function :SPLINE_mem_ptr,       [ :string ], :pointer
  attach_function :SPLINE_init,          [], :int
  attach_function :SPLINE_push,          [ :double, :double ], :int
  attach_function :SPLINE_build,         [], :int
  attach_function :SPLINE_build2,        [ :buffer_in, :buffer_in, :int ], :int

  attach_function :SPLINE_eval,          [ :double ], :double
  attach_function :SPLINE_eval_D,        [ :double ], :double
  attach_function :SPLINE_eval_DD,       [ :double ], :double
  attach_function :SPLINE_eval_DDD,      [ :double ], :double

  #   ____        _ _            
  #  / ___| _ __ | (_)_ __   ___ 
  #  \___ \| '_ \| | | '_ \ / _ \
  #   ___) | |_) | | | | | |  __/
  #  |____/| .__/|_|_|_| |_|\___|
  #        |_|                   
  class Spline
    attr_reader :id

    def initialize(type="pchip")
      @id = self.__id__.to_s
      Bike1D.SPLINE_new @id, type
      return @id
    end

    def clear
      Bike1D.SPLINE_select @id
      Bike1D.SPLINE_init
    end

    def build
      Bike1D.SPLINE_select @id
      Bike1D.SPLINE_build
    end

    def type
      Bike1D.SPLINE_select @id
      Bike1D.SPLINE_get_type_name
    end

    def push_back( x, y )
      Bike1D.SPLINE_select @id
      Bike1D.SPLINE_push x, y
    end

    def value( x )
      Bike1D.SPLINE_select @id
      return Bike1D.SPLINE_eval(x)
    end

    def D( x )
      Bike1D.SPLINE_select @id
      return Bike1D.SPLINE_eval_D(x)
    end

    def DD( x )
      Bike1D.SPLINE_select @id
      return Bike1D.SPLINE_eval_DD(x)
    end

    def DDD( x )
      Bike1D.SPLINE_select @id
      return Bike1D.SPLINE_eval_DDD(x)
    end

  end

  #
  #   ____  ____    _           _               _     
  #  / ___|/ ___|  | |_ ___    | |__   __ _ ___| |__  
  # | |  _| |      | __/ _ \   | '_ \ / _` / __| '_ \ 
  # | |_| | |___   | || (_) |  | | | | (_| \__ \ | | |
  #  \____|\____|___\__\___/___|_| |_|\__,_|___/_| |_|
  #            |_____|    |_____|
  #
  def self.toBoolVector
    nelem = self.GC_get_vector_size
    res = []
    (0..nelem-1).each { |i| res << self.GC_get_bool_at_pos(i) }
    return res
  end

  def self.toIntVector
    nelem = self.GC_get_vector_size
    res = []
    (0..nelem-1).each { |i| res << self.GC_get_int_at_pos(i) }
    return res
  end

  def self.toRealVector
    nelem = self.GC_get_vector_size
    res = []
    (0..nelem-1).each { |i| res << self.GC_get_real_at_pos(i) }
    return res
  end

  def self.toComplexVector
    nelem = self.GC_get_vector_size
    res = []
    (0..nelem-1).each { |i| res << [ self.GC_get_complex_real_at_pos(i), self.GC_get_complex_imag_at_pos(i) ] }
    return res
  end

  def self.toStringVector
    nelem = self.GC_get_vector_size
    res = []
    (0..nelem-1).each { |i| res << self.GC_get_string_at_pos(i) }
    return res
  end

  def self.toVector
    nelem = self.GC_get_vector_size
    res = []
    (0..nelem-1).each do |i|
      self.GC_push_vector_position i
      res << self.GC_to_hash
      self.GC_pop_head
    end
    return res
  end

  def self.toRealMatrix
    puts "in toRealMatrix"
    nr  = self.GC_get_matrix_num_rows
    nc  = self.GC_get_matrix_num_cols
    res = []
    (0...nr).each do |i|
      row = []
      (0...nc).each do |j|
        row << self.GC_get_real_at_coor(i,j) 
      end
      res << row
    end
    puts "out toRealMatrix"
    return res
  end

  def self.toComplexMatrix
    puts "in toComplexMatrix"
    nr  = self.GC_get_matrix_num_rows
    nc  = self.GC_get_matrix_num_cols
    res = []
    (0...nr).each do |i|
      row = []
      (0...nc).each do |j|
        row << [ self.GC_get_complex_real_at_coor(i,j), self.GC_get_complex_imag_at_coor(i,j) ]
      end
      res << row
    end
    puts "out toComplexMatrix"
    return res
  end


  def self.toMixedHash
    #self.GC_get_map
    keys = []
    self.GC_init_map_key
    while (key = self.GC_get_next_key) do keys << key end
    res = {}
    keys.each do |key|
      self.GC_push_map_position key
      res[key.to_sym] = self.GC_to_hash
      self.GC_pop_head
    end
    return res
  end

  def self.GC_to_hash
    type = self.GC_get_type_name
    case type
    #when /NOTYPE/
    #when /pointer/
    when /^bool_type$/
      res = self.GC_get_bool
    when /^int_type$/
      res = self.GC_get_int
    when /^long_type$/
      res = self.GC_get_long
    when /^real_type$/
      res = self.GC_get_real
    when /^string_type$/
      res = self.GC_get_string
    when /^vec_bool_type$/
      res = self.toBoolVector
    when /^vec_int_type$/
      res = self.toIntVector
    when /^vec_real_type$/
      res = self.toRealVector
    when /^vec_string_type$/
      res = self.toStringVector
    when /^vec_complex_type$/
      res = self.toComplexVector
    when /^mat_real_type$/
      res = self.toRealMatrix
    when /^mat_complex_type$/
      res = self.toComplexMatrix
    when /^vector_type$/
      res = self.toVector
    when /^map_type$/
      res = self.toMixedHash
    else
      puts "GC_to_hash #{self.GC_get_type_name} not managed"
      res = nil
    end
    return res
  end
  #
  #    ____  ____      __                         _               _     
  #   / ___|/ ___|    / _|_ __ ___  _ __ ___     | |__   __ _ ___| |__  
  #  | |  _| |       | |_| '__/ _ \| '_ ` _ \    | '_ \ / _` / __| '_ \ 
  #  | |_| | |___    |  _| | | (_) | | | | | |   | | | | (_| \__ \ | | |
  #   \____|\____|___|_| |_|  \___/|_| |_| |_|___|_| |_|\__,_|___/_| |_|
  #             |_____|                     |_____|                     
  #
  def self.addToVector(vec)
    # empty vectors
    if vec[0].kind_of?(TrueClass) or vec[0].kind_of?(FalseClass) then
      self.GC_set_empty_vector_of_bool
    elsif vec[0].kind_of?(Fixnum) then
      self.GC_set_empty_vector_of_int
    elsif vec[0].kind_of?(Float) then
      self.GC_set_empty_vector_of_real
    elsif vec[0].kind_of?(String) then
      self.GC_set_empty_vector_of_string
    else
      self.fromVector vec
      return ;
    end
    # add elements
    vec.each { |var|
      if var.kind_of?(TrueClass) then
        self.GC_push_bool 1
      elsif var.kind_of?(FalseClass) then
        self.GC_push_bool 0
      elsif var.kind_of?(Fixnum) then
        self.GC_push_int var
      elsif var.kind_of?(Float) then
        self.GC_push_real var
      elsif var.kind_of?(String) then
        self.GC_push_string var
      else
        self.fromVector vec
        return ;
      end
    }
  end

  def self.fromVector(var)
    nelem = var.length
    self.GC_set_vector nelem
    (0..nelem-1).each do |i|
      self.GC_push_vector_position i
      self.GC_from_hash var[i]
      self.GC_pop_head
    end
  end

  def self.fromMixedHash(var)
    self.GC_set_map
    var.each do |k,v|
      self.GC_push_map_position k.to_s
      self.GC_from_hash v
      self.GC_pop_head
    end
  end

  def self.GC_from_hash(var)
    if var.kind_of?(TrueClass) then
      self.GC_set_bool 1
    elsif var.kind_of?(FalseClass) then
      self.GC_set_bool 0
    elsif var.kind_of?(Fixnum) then
      self.GC_set_int var
    elsif var.kind_of?(Float) then
      self.GC_set_real var
    elsif var.kind_of?(String) then
      self.GC_set_string var
    elsif var.kind_of?(Array) then
      if var.length > 0 then
        self.addToVector(var)
      else
        self.GC_set_empty_vector_of_bool
      end
    elsif var.kind_of?(Hash) then
      self.fromMixedHash var
    elsif var.respond_to? :to_hash then
      self.fromMixedHash var.to_hash
    elsif var.respond_to? :[] then
      self.fromMixedHash var
    else
      puts "Found MRB_TT_OBJECT: " ;
    end
  end

  class OCPSolver
    attr_accessor :data
    attr_reader   :id
   
    def initialize(data=nil)
      @id       = self.__id__.to_s
      self.data = (data || Mechatronix.content)
      Bike1D.GC_select @id+"_result"
      Bike1D.GC_select @id
      ObjectSpace.define_finalizer(self, proc { Bike1D.GC_delete @id })
      return @id
    end

    def solve( convert = true )
      Bike1D.GC_select @id
      Bike1D.GC_from_hash @data
      ok  = Bike1D.OCP_setup( @id, Bike1D.GC_mem_ptr(@id) )
      out = Bike1D.GC_to_hash
      raise RuntimeError, "Failed to do setup: #{out[:Error]}\n" unless ok
      
      ok = Bike1D.OCP_solve( @id, Bike1D.GC_mem_ptr(@id), Bike1D.GC_mem_ptr(@id+"_result"))
      Bike1D.GC_select @id+"_result"
      out = Bike1D.GC_to_hash
      raise RuntimeError, "Failed to do solve: #{out[:Error]}\n" unless ok

      if convert then
        out = Bike1D.GC_to_hash
        if out[:Error] then
          raise RuntimeError, "Failed to compute: #{out[:Error]}\n"
        else
          idx  = out[:idx]
          data = out[:data]
          tmp  = {}
          out[:headers].each { |h| tmp[h.to_sym] = data[idx[h.to_sym]].dup }
          out.delete(:idx) ;
          out.delete(:data) ;
          out[:data] = tmp ;
        end
        return out
      else
        return Bike1D.GC_to_hash
      end
    end

    def write_ocp_solution( fname ) 
      Bike1D.OCP_write_ocp_solution( @id, fname )
    end

  end

end
 
module Mechatronix
  @@content = nil
  def self.content=(v); @@content = v; end
  def self.content;     @@content;     end
  
  class Container
    attr_accessor :name, :match, :content
    attr_reader   :key_converter
  
    def self.[](hsh)
      raise ArgumentError, "Need a Hash" unless hsh.respond_to? :to_hash
      m = self.new
      hsh.to_hash.each do |k,v|
        m.send("#{k}=".to_sym, v)
      end
      return m
    end
    
    def initialize(name="My model")
      @name = name
      @match = /^[A-Z]/
      @content= Hash.new
      @key_converter = :to_sym
      return self
    end
    
    def keys; @content.keys; end

    def key_converter=(sym)
      raise RuntimeError, "Either :to_s or :to_sym" unless [:to_s, :to_sym].include? sym
      @key_converter = sym
    end
  
    def [](key)
      return @content[key.send(@key_converter)]
    end
  
    def []=(key, value)
      @content[key.send(@key_converter)] = value
    end  
  
    def method_missing name, *args, &block
      unless name.to_s.match(@match)
        raise RuntimeError, "Undefined method #{name}"
      end
    
      case name.to_s
      when /=$/
        key = name.to_s.chop.to_sym
        @content[key.send(@key_converter)] = args.first
      else
        return @content[name.send(@key_converter)]
      end
    end
  
    def inspect
      return @content.inspect
    end

    alias :to_hash :content
  end
  
  class Mesh
    class ParsingError < Exception; end
    
    attr_reader :segments, :s0, :default_mesh, :wrong_indexes

    def initialize(s0=0, ms=0.01)
      @segments               = []
      @s0                     = s0
      @default_mesh_grid_size = ms
      @wrong_indexes          = []
    end
  
    def << arg
      s = {}
      case arg
      when Numeric
        s = { :length => arg.to_f, :n => len_to_n(arg)}
      when Hash
        # la lunghezza deve esserci sempre!
        if arg[:n] then
          s = {
            :length => arg[:length].to_f, 
            :n      => arg[:n].to_i
          }
        elsif arg[:grid_size] then
          s = {
            :length => arg[:length].to_f, 
            :n      => len_to_n(arg[:length], arg[:grid_size])
          }
        elsif arg[:length]
          s = {
            :length => arg[:length].to_f,
            :n      => len_to_n(arg[:length])
          }
        else
          raise ParsingError
        end
      else
        raise ParsingError
      end
      raise ParsingError unless s[:length].kind_of?(Float) && s[:n].kind_of?(Fixnum)
    rescue ParsingError
      s = arg
      @wrong_indexes << @segments.length
    ensure
      @segments << s
    end
  
    def consistent?
      return @wrong_indexes.empty?
    end
  
    def to_hash
      hash = {}
      self.instance_variables.map { |var| hash[var.to_s[1..-1].to_sym] = self.instance_variable_get(var) }
      return hash
    end

  private # da qui in poi i metodi sono privati, occhio!
    def len_to_n(arg, gs=@default_mesh_grid_size)
      ((arg + gs) / gs).to_i
    rescue
      nil
    end
  end

  class Road < Container
    def initialize()
      super("road")
      self.MeshGridSize = 1
      self.RoadWidth    = 6
      self.Segments     = []
      self.Theta0       = 0  # initial orientation in x-y plane\n"
      self.S0           = 0  # initial curvilinear abscissa\n"
      self.X0           = 0  # initial x position of road middle line\n"
      self.Y0           = 0  # initial y position of road middle line\n"
      self.Z0           = 0  # initial z position of road middle line\n"
      self.Banking0     = 0  # initial y position of road middle line\n"
      self.IsSAE        = false
    end

    def << hsh
      raise ArgumentError, "Need a kind of Hash" unless hsh.respond_to? :to_hash
      hsh = hsh.to_hash
      if hsh[:Radius] then
        curvature = 1.0/hsh[:radius]
        if hsh[:Angle] then
          length = (hsh[:Angle] / hsh[:Curvature] ).abs
        elsif hsh[:AngleDeg] then
          length = ((Math::PI*hsh[:AngleDeg]/180.0) / hsh[:Curvature] ).abs
        else
          raise RuntimeError, "Missing Angle field in Road segment #{self.Segments.length}"
        end
        initialCurvature = finalCurvature = curvature
      else
        initialCurvature = hsh[:InitialCurvature] || hsh[:Curvature] || 0 ;
        finalCurvature   = hsh[:FinalCurvature]   || hsh[:Curvature] || 0 ;
        raise RuntimeError, "Missing Length field in Road segment #{self.Segments.length}" unless hsh[:Length]
        length           = hsh[:Length]
      end
      self.Segments << {
        Length:            length,
        GridSize:          hsh[:GridSize]                || self.MeshGridSize,
        Width:             hsh[:Width]                   || self.RoadWidth,
        InitialCurvature:  initialCurvature,
        FinalCurvature:    finalCurvature,
        InitialLeftWidth:  hsh[:InitialLeftWidth]        || hsh[:LeftWidth]  || self.RoadWidth / 2.0,
        FinalLeftWidth:    hsh[:FinalLeftWidth]          || hsh[:LeftWidth]  || self.RoadWidth / 2.0,
        InitialRightWidth: hsh[:InitialRightWidth]       || hsh[:RightWidth] || self.RoadWidth / 2.0,
        FinalRightWidth:   hsh[:FinalRightWidth]         || hsh[:RightWidth] || self.RoadWidth / 2.0,
        SpeedLimit:        hsh[:SpeedLimit]              || 300,
        Adherence:         hsh[:FrictionReductionFactor] || hsh[:adherence]  || 1,
        FinalZ:            hsh[:End_z]                   || hsh[:z]          || 0,
        FinalBanking:      hsh[:End_banking]             || hsh[:banking]    || 0
      }
  end
  def consistent?
      return true
    end
  end
  
  class ToolPath2D
    attr_reader :toolPath2D

    def initialize(pars = {})
      self.init pars
    end

    def init(pars = {})
      @toolPath2D = {:segments => [] }
      @defaultFeedRate     = pars[:defaultFeedRate]     || 0.1
      @defaultSpindleRate  = pars[:defaultSpindleRate]  || 500
      @defaultCrossSection = pars[:defaultCrossSection] || 0.01
      @defaultN            = pars[:defaultN]            || 10
    end

    def <<(pars)
      x0           = pars[:x0] 
      y0           = pars[:y0] 
      x1           = pars[:x1] 
      y1           = pars[:y1] 
      n            = pars[:n]            || @defaultN 
      feedRate     = pars[:feedRate]     || @defaultFeedRate
      spindleRate  = pars[:spindleRate]  || @defaultSpindleRate 
      crossSection = pars[:crossSection] || @defaultCrossSection 

      tmp = { 
        :x0           => x0,
        :y0           => y0,
        :x1           => x1,
        :y1           => y1,
        :feedRate     => feedRate,
        :spindleRate  => spindleRate,
        :crossSection => crossSection, 
        :n            => n
      }

      tmp[:angle0] = pars[:angle0] if pars.key? :angle0 
      tmp[:angle1] = pars[:angle1] if pars.key? :angle1 
      @toolPath2D[:segments] << tmp
    end
    alias add <<

    def to_h; @toolPath2D; end
    alias :to_hash :to_h

  end #class ToolPath2D

end

def self.mechatronix(name=nil, &block)
  Mechatronix.content = Mechatronix::Container.new name
  yield Mechatronix.content
end

alias require_relative_old require_relative
def require_relative(f, base)
  require_relative_old(f)
end

# EOF: Bike1D_ffi.rb
