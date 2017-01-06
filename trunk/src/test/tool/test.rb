require 'GeneralRegister.rb'
require 'SystemRegister.rb'
require 'ImmediateValue.rb'
require 'TestTargetFunc.rb'
require 'TestTargetFuncInput.rb'
require 'TestTargetFuncOutput.rb'
require 'TestSpec.rb'
require 'TestItem.rb'
require 'TestTargetFuncInputValue.rb'
require 'TestTargetFuncExpectedValue.rb'

target = TestTargetFunc.new("add")

cls1 = eval "#{GeneralRegister}"
cls2 = eval "#{SystemRegister}"



target.addInput(TestTargetFuncInput.new(cls1.new("r20")))
target.addInput(TestTargetFuncInput.new(cls1.new("r21")))

target.addOutput(TestTargetFuncOutput.new(cls1.new("r21"), cls1.new("r21"), cls1.new("r22")))
target.addOutput(TestTargetFuncOutput.new(cls2.new("psw"), cls1.new("r23"), cls1.new("r24")))
 
spec = TestSpec.new("ADD1")
spec.setTarget(target)

item = TestItem.new(spec.name + "_1")
item.addInput(TestTargetFuncInputValue.new("0x2"))
item.addInput(TestTargetFuncInputValue.new("0x7FFFFFFE"))

item.addOutput(TestTargetFuncExpectedValue.new("0x80000000"))
item.addOutput(TestTargetFuncExpectedValue.new("0x6"))

spec.addItem(item)



print "/* set input */\n"
print "mov " + item.inp[0].value + ", " + target.inp[0].elm.name + "\n"
print "mov " + item.inp[1].value + ", " + target.inp[1].elm.name + "\n"

print "/* set expect */\n"
print "mov " + item.out[0].value + ", " + target.out[0].reg_expect.name + "\n"
print "mov " + item.out[1].value + ", " + target.out[1].reg_expect.name + "\n"

print "/* do test */ \n"
print spec.ins() + "\n"

print "/* get test results*/\n"
print "cmp " + target.out[0].reg_output.name + ", " + target.out[0].reg_expect.name + "\n"
print "bne test_fail\n"

print "cmp " + target.out[1].reg_output.name + ", " + target.out[1].reg_expect.name + "\n"
print "bne test_fail\n"



print "/* check results*/\n"
