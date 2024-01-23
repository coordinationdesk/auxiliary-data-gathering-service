class LibReadDBConf
  attr_accessor :fileconf,:hostname,:user,:password,:port,:database
  def initialize(fileconf)
    @fileconf = fileconf
    @hostname=""
    @database=""
    @user=""
    @password=""
    @port=""
  end

  def redconf()
    File.open(@fileconf, "r") do |infile|
      while (line = infile.gets)
        if line =~ /dbConnectionArbiterCParam.server/
          @hostname=line.match(/dbConnectionArbiterCParam.server.(.*)/)[1].strip
        elsif line =~ /dbConnectionArbiterCParam.user/
          @user=line.match(/dbConnectionArbiterCParam.user.(.*)/)[1].strip
        elsif line =~ /dbConnectionArbiterCParam.db/
          @database=line.match(/dbConnectionArbiterCParam.db.(.*)/)[1].strip
        elsif line =~ /dbConnectionArbiterCParam.password:/
          @password=line.match(/dbConnectionArbiterCParam.password.(.*)/)[1].strip
        elsif line =~ /dbConnectionArbiterCParam.port/
          @port=line.match(/dbConnectionArbiterCParam.port.(.*)/)[1].strip
        end
      end
    end
  end
end
