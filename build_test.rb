#!/usr/bin/env ruby
%w{pry colorize yaml gnuplot artii csv}.each do |gem|
  raise LoadError, "You must install gem: #{gem}\n\t $ sudo gem instal #{gem}" if not require gem
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
      load_csv("#{@conf[:path]}/data/#{@conf[:test]}_out.txt")
      binding.pry
    end
    
    def exec
      puts Dir.pwd
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
      # system "cd #{@conf[:path]} && rake clobber && rake maple && rake clean && rake main && bin/main && cd .."
    end
    
    def load_csv(file)
      @data = {}
      ary = CSV::read(file, col_sep: "\t", converters: :numeric)
      ary[0].each_with_index { |k, i|
        @data[k] = []
        @data[i] = @data[k]
      }
      for i in 1...ary.size
        for j in 0...@data.keys.size
          @data[j] << ary[j][i]
        end
      end
    end
    
    def inspect
      puts
      puts (Artii::Base.new({}).asciify("TEST - " + @conf[:test])).green
      puts
      puts "  Configuration:".green
      puts "   - Backup old data = #{@conf[:data_backup]}".green
      puts "   - Variables       = #{@conf[:vars].size}".green
      puts "   - Graphs          = #{@conf[:graph].size}".green
      puts "   - Path            = #{@conf[:path]}".green
      puts
    end
  end
  
end

if $0 == __FILE__ then
  include OCPTest
  CreateTest.new(".")
end
