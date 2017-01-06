require 'GeneralRegister.rb'
require 'SystemRegister.rb'
require 'ImmediateValue.rb'
require 'TestTargetFunc.rb'
require 'TestTargetFuncInput.rb'
require 'TestTargetFuncOutput.rb'
require 'TestSpec.rb'

target = TestTargetFunc.new("add")

cls1 = eval "#{GeneralRegister}"
cls2 = eval "#{SystemRegister}"



target.addInput(TestTargetFuncInput.new(cls1.new("r20")))
target.addInput(TestTargetFuncInput.new(cls1.new("r21")))

target.addOutput(TestTargetFuncOutput.new(cls1.new("r21"), cls1.new("r21"), cls1.new("r22")))
target.addOutput(TestTargetFuncOutput.new(cls2.new("psw"), cls1.new("r23"), cls1.new("r24")))
 
spec = TestSpec.new("ADD1")
spec.setTarget(target)

p spec.ins()

