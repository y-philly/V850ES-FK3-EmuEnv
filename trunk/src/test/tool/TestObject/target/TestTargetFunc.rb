require 'GeneralRegister.rb'
require 'SystemRegister.rb'
require 'ImmediateValue.rb'

class TestTargetFunc
  attr_accessor :name
  attr_accessor :inp
  attr_accessor :out
  
  def initialize(name)
    self.name = name
    self.inp = Array.new()
    self.out = Array.new()
  end
  
  def addInput(input)
    self.inp[self.inp.length] = input
  end
  
  def addOutput(output)
    self.out[self.out.length] = output
  end
  
end