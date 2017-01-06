
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
  
  def ins()
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

  
end