#! ruby -Ks

class ElfDebugInfo
    attr_accessor :file
    attr_accessor :lineno
    attr_accessor :addr

    def initialize(f, l, a)
    	self.file = f
    	self.lineno = l
    	self.addr = a
    end

end

class ElfDebugManager
    attr_accessor :debugs

    def initialize()
      self.debugs = Array.new()
    end

    def add(f, l, a)
      self.debugs[self.debugs.length] = ElfDebugInfo.new(f, l, a);
    end

    def search_line(addr)
    	self.debugs.each do |elm|
    	#p elm
    	    if addr == elm.addr
    		return elm.lineno
    	    end
    	end
    	return -1
    end

end

class SourceInfo
    @mgr
    def self.init(path)
    	@mgr = ElfDebugManager.new()
    	f = open(path, "r")
    	f.each { | line|
    		elm = line.split(":")
    		@mgr.add(nil, elm[0].strip.to_i, elm[1].strip)
    		#p line
    	}
    	f.close
    end
    def self.get()
    	return @mgr
    end
end
