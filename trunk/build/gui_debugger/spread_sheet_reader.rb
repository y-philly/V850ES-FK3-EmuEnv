#! ruby -Ks

class SpreadSeqReader
  attr_accessor :reader
  attr_accessor :keyColumn
  attr_accessor :startRow
  attr_accessor :valueMeta
  
  def initialize(keyColumn, startRow, reader)
    self.keyColumn = keyColumn
    self.startRow = startRow
    self.reader = reader
  end
  
  def begin()
    @row = self.startRow
  end
  
  def next_entry()
    @row += 1
    reader.ref(@row, keyColumn)
  end
  
  def exist?()
    value = self.key()
    if value == nil
	return false
    end
    if value == "Å°"
	return false
    end
    if value == "Å~"
	return false
    end
    if value == ""
	return false
    end

    return true
  end

  def hasEntry?()
    value = self.key()
    if value == nil
	return false
    end
    if value == "Å°"
	return false
    end
    if value == ""
	return false
    end

    return true
  end

  
  def key()
    reader.ref(@row, keyColumn)
  end
  
  def value(column)
    reader.ref(@row, column)
  end


end