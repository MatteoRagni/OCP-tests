#!/usr/bin/env ruby
%w{pry colorize yaml gnuplot artii csv}.each do |gem|
  raise LoadError, "You must install gem: #{gem}\n\t $ sudo gem instal #{gem}" if not require gem
end

class CSV
  def CSV.read_pins(fn)
    data = {}
    
    ary = read(fn, col_sep: "\t", converters: :numeric)
    ary[0].each_with_index { |k, i|
      data[k] = Array.new
      data[i] = data[k]
    }
    for i in 1...ary.size
      for j in 0...ary[0].size
        data[j] << ary[i][j]
      end
    end
    return data
  end
end




module OCPTest
  class OCPTestError < RuntimeError; end
  
  class CreateTest
    def initialize(l)
      if Dir.exists?(l) then
        @path = l
      else
        raise OCPTestError, "Specify a directory. #{l} is not accepted.".red
      end
      
      @tests = []
      identify_test_dir

      @tests.each { |conf| Test.new(conf) }
    end
    
    def identify_test_dir
      dir_list = Dir.entries(@path).map { |e| e.scan(/test-.+/)}.flatten
      dir_list.each { |dir|
        yaml_data = {}
        if File.exists? (dir + "/config.yaml")
          yaml_data = YAML.load_file(dir + "/config.yaml")
          if yaml_data[:active] == true then
            yaml_data[:path] = dir
            @tests << yaml_data
            puts ">>> Project #{dir} added".green
          elsif yaml_data[:active] == false then
            warn ">>> Found disabled project in #{dir}".yellow
          else
            warn ">>> #{dir + '/config.yaml'} do not contains :active definition. Skipped.".red
          end
        else
          warn ">>> No configuration file found in #{dir}".red
        end
      }
    end
  end
  
  class Test
    def initialize(conf)
      @conf = conf
      inspect
      #exec
      @data = CSV.read_pins("#{@conf[:path]}/data/#{@conf[:test]}_out.txt")
      graph
    end
    
    def exec
      puts Dir.pwd
      # Save in memory old datas
      
      # Wait until rake as ended with thread
      %w[clobber maple clean main].each do |target|
        thread = Thread.new do
          puts ">>> Invoking target: ".yellow + "#{target}".green
          system "cd #{@conf[:path]} && rake #{target}"
        end
        thread.join
      end
      thread = Thread.new do
        system "cd #{@conf[:path]} && bin/main"
      end
      thread.join
      puts ">>> Test completed".yellow
      # The one below is a little better than this:
      # system "cd #{@conf[:path]} && rake clobber && rake maple && rake clean && rake main && bin/main && cd .."
    end
    
    def graph
      @conf[:graph].each do |g|
        raise SyntaxError, "#{g[:x_axis]} for x_axis do not exists in results data".red unless @data[g[:x_axis]]
        g[:y_axis].each { |y| raise SyntaxError, "#{y[:name]} for y_axis do not exists in results data".red unless @data[y[:name]] }
        
        Gnuplot.open do |gp|
          @conf[:graph].each do |graph|
            Gnuplot::Plot.new(gp) do |plot|
              plot.terminal "png"
              plot.output "./#{@conf[:path]}/#{graph[:name]}.png"
              plot.title graph[:name]
              plot.xlabel graph[:x_axis]
              
              g[:y_axis].each do |y|
                plot.data << Gnuplot::DataSet.new( [@data[g[:x_axis]], @data[y[:name]]] ) do |ds|
                  ds.with = "lines lt rgb \"#{y[:color]}\""
                  ds.title = y[:legend]
                end
              end
            end
          end
        end
      end if @conf[:graph]
    end
    
    def inspect
      puts
      puts (Artii::Base.new({}).asciify("TEST - " + @conf[:test])).green
      puts
      puts "  Configuration:".green
      puts "   - Backup old data = #{@conf[:data_backup]}".green
      puts "   - Variables       = #{@conf[:vars].size}".green if @conf[:vars]
      puts "   - Graphs          = #{@conf[:graph].size}".green if @conf[:graph]
      puts "   - Path            = #{@conf[:path]}".green
      puts
    end
  end
  
end

if $0 == __FILE__ then
  include OCPTest
  CreateTest.new(".")
end
