
class TestSpec
  attr_accessor :name
  attr_accessor :target
  attr_accessor :items

  def initialize(name)
    self.name = name
    self.items = Array.new()
  end
  
  def setTarget(target)
    self.target = target
  end
  
  def addItem(item)
    self.items[self.items.length] = item
  end
  
  def prepare_ins(item)
    prepare_instructions = Array.new()
    prepare_instructions[prepare_instructions.length] = "/* set input */\n"
    item.inp.each { |inp|
      prepare_instructions[prepare_instructions.length] = "mov 0x" + inp.value + ", " + target.inp[item.inp.index(inp)].elm.name + "\n"
    }
    
    prepare_instructions[prepare_instructions.length] = "/* set expect */\n"
    item.out.each { |out|
      prepare_instructions[prepare_instructions.length] = "mov 0x" + out.value + ", " + target.out[item.out.index(out)].reg_expect.name + "\n"
    }
    
    return prepare_instructions
  end
  
  def test_ins()
    inputs = ""
    target.inp.each { |e|
      if inputs.length == 0
        inputs = inputs + " " + e.elm.name
      else
        inputs = inputs + ", " + e.elm.name
      end
    }
    return target.name + inputs
  end

  def check_ins(item)
    check_instructions = Array.new()
    target.out.each { |elm|
      check_instructions[check_instructions.length] = "/* get test results*/\n"
      if elm.test_output.instance_of?(SystemRegister)
        check_instructions[check_instructions.length] =  "ldsr " + elm.reg_output.name + ", " + elm.test_output.name + "\n"
      end
      check_instructions[check_instructions.length] =  "cmp " + elm.reg_output.name + ", " + elm.reg_expect.name + "\n"
      check_instructions[check_instructions.length] =  "bne "+ item.name + "_test_fail\n"
    }
    return check_instructions
  end
  
end