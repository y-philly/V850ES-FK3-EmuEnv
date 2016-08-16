class SourceNaviEntry
  attr_accessor :file_name
  attr_accessor :file_line
  attr_accessor :lineno
  
  def initialize(f, f_l, l)
    self.file_name = f
    self.file_line = f_l
    self.lineno = l
  end
  
end

class SourceNavi
  attr_accessor :array

  def initialize()
    self.array = Array.new()
  end
  def add(f, f_l, l)
    self.array[self.array.length] = SourceNaviEntry.new(f, f_l, l);
  end
  
  def search(lineno)
    self.array.each do |elm|
        if lineno == elm.lineno
          return elm
        end
    end
    return nil
  end
  
end