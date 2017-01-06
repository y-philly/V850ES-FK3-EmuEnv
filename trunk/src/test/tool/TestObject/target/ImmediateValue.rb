require 'TestTargetFuncElement.rb'

class ImmediateValue < TestTargetFuncElement
  attr_accessor :bitsize
  
  def initialize(name, bitsize)
    super(name)
    self.bitsize = bitsize
  end
end
