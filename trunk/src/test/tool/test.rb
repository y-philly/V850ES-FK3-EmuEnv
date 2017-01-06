require 'GeneralRegister.rb'
require 'SystemRegister.rb'
require 'ImmediateValue.rb'
require 'TestTargetFunc.rb'
require 'TestTargetFuncInput.rb'
require 'TestTargetFuncOutput.rb'

ins_add = TestTargetFunc.new("add")

cls1 = eval "#{GeneralRegister}"
cls2 = eval "#{SystemRegister}"



ins_add.addInput(TestTargetFuncInput.new(cls1.new("r20")))
ins_add.addInput(TestTargetFuncInput.new(cls1.new("r21")))

ins_add.addOutput(TestTargetFuncOutput.new(cls1.new("r21"), cls1.new("r21"), cls1.new("r22")))
ins_add.addOutput(TestTargetFuncOutput.new(cls2.new("psw"), cls1.new("r23"), cls1.new("r24")))
 
p ins_add.name + " " + ins_add.inp[0].elm.name + ", " + ins_add.inp[1].elm.name

