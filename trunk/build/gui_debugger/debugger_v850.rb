#! ruby -Ks
require 'win32ole'
require './excel_reader'
require './spread_sheet_reader'
require './excel_writer'
require 'timeout'

class CpuInfo
    @@r = Array.new(32)
    @@pc = "0x0"
    @@eipc = "0x0" 
    @@eipsw = "0x0" 
    @@ecr = "0x0"
    @@psw = "0x0"
    @@fepc = "0x0"
    @@fepsw = "0x0"
    @@ctbp = "0x0"
    def self.get_sp()
	return @@r[3]
    end
    def self.load(path, workbook)
	#p "cpu"
    	writer = ExcelWriter.new(workbook, "CPU")
    	array = Array.new()
    	f = open(path, "r")
    	f.each { | line|
    	    array[array.length] = line.split()[1]
    	}
    	f.close
	#p "cpuX"
    	for i in 1..32
    	  if @@r[i-1] != array[i]
    	    writer.set(2 + i, 3, array[i])
    	  end
    	  @@r[i-1] = array[i]
    	end
	#p "cpu2"
    	if array[0] != @@pc
    	  writer.set(3, 5, array[0]) #PC
    	end
    	@@pc = array[0]
    	
	#p "cpu3"
    	if array[33] != @@eipc
    	  writer.set(4, 5, array[33]) #EIPC
    	end
    	@@eipc = array[33]
    	
	#p "cpu4"
      if array[34] != @@eipsw
        writer.set(5, 5, array[34]) #EIPSW
      end
      @@eipsw = array[34]
      
      if array[35] != @@ecr
        writer.set(6, 5, array[35]) #ECR
      end
      @@ecr = array[35]
      
	#p "cpu5"
      if array[36] != @@psw
        writer.set(7, 5, array[36]) #PSW
      end
      @@psw = array[36]
      
	#p "cpu6"
      if array[37] != @@fepc
        writer.set(8, 5, array[37]) #FEPC
      end
      @@fepc = array[37]
      
	#p "cpu7"
      if array[38] != @@fepsw
        writer.set(9, 5, array[38]) #FEPSW
      end
      @@fepsw = array[38]
      
	#p "cpu8"
      if array[39] != @@ctbp
        writer.set(10, 5, array[39]) #CTBP
      end
      @@ctbp = array[39]
    	
    end
end

class StackInfo
    @@sp_focus = -1
    @@sp = Array.new(1024)
    def self.load(path, workbook)
	#p "stack_start"
      	writer = ExcelWriter.new(workbook, "STACK")
      	f = open(path, "r")
      	i = 3
    	f.each { |line|
	    elm = line.split[1].strip
	    if @@sp[i] != elm
    		writer.set(i, 4, elm)
	    end
	    @@sp[i] = elm
	    i = i + 1
    	}
    	f.close
	#p "stack_x1"
	sp = CpuInfo.get_sp().hex
	#p "stack_x1.1"
	base = "0x3fffb3a8".hex
	#p "stack_x1.2:" + sp.to_s + ":" + ":" + base.to_s + ":" + (base - 512).to_s

	if ((sp <= base) && (sp >= (base - 512)))
	    focus = (base - sp) / 4

	    p focus.to_s
	    if focus != @@sp_focus
		if @@sp_focus != -1
			writer.range_clr_focus(@@sp_focus +3, "B", "E")
		end
		writer.range_set_focus(focus +3, "B", "E")
	    end
	    @@sp_focus = focus
	end

	@@sp_focus
    end
end

class SymbolInfo
    attr_accessor :type
    attr_accessor :name
    attr_accessor :addr
    attr_accessor :size

    @@list_func = Array.new()
    @@list_gl = Array.new()

    def initialize(type, name, addr, size)
	self.type = type
	self.name = name
	self.addr = addr
	self.size = size
    end


    def self.load(path, workbook)
    	func_writer = ExcelWriter.new(workbook, "FUNC")
    	gl_writer = ExcelWriter.new(workbook, "GLOBAL")
    	f = open(path, "r")
    	func_l = 3
    	gl_l = 3
    	f.each { | line|
    	    array = line.split()
    	    addr = array[1].strip()
    	    size = array[2].strip()
    	    type = array[3].strip()
    	    name = array[7].strip()
    	    if type == "FUNC"
        		func_writer.set(func_l, 2, name)
        		func_writer.set(func_l, 3, "0x" + addr)
        		func_writer.set(func_l, 4, size)
			@@list_func[@@list_func.length] = SymbolInfo.new(type, name, addr, size)
        		func_l = func_l + 1
    	    else
        		gl_writer.set(gl_l, 2, name)
        		gl_writer.set(gl_l, 3, "0x" + addr)
        		gl_writer.set(gl_l, 4, size)
			@@list_gl[@@list_gl.length] = SymbolInfo.new(type, name, addr, size)
        		gl_l = gl_l + 1
    	    end
      	}
      	f.close

	f = open("symbol.c", "w")
	f.print("#include \"dbg.h\"\n")
	f.print("#define SYMBOL_FUNC_SIZE	" + @@list_func.length.to_s + "\n")
	f.print("const uint32 symbol_func_size = SYMBOL_FUNC_SIZE;\n")
	f.print("DbgSymbolType symbol_func[SYMBOL_FUNC_SIZE] = { \n")
	@@list_func.each { |elm|
		f.print("{ \""+  elm.name + "\", 0x" + elm.addr  + ", " + elm.size + " }, \n")
	}
	f.print("};\n")
	f.print("#define SYMBOL_GLOBAL_SIZE	" + @@list_gl.length.to_s + "\n")
	f.print("const uint32 symbol_gl_size = SYMBOL_GLOBAL_SIZE; \n")
	f.print("DbgSymbolType symbol_gl[SYMBOL_GLOBAL_SIZE] = { \n")
	@@list_gl.each { |elm|
		f.print("{ \""+  elm.name + "\", 0x" + elm.addr  + ", " + elm.size + " }, \n")
	}
	f.print("};\n")
	f.close
    	end
end

class ElfDebugInfo
    attr_accessor :file
    attr_accessor :lineno
    attr_accessor :addr

    def initialize(f, l, a)
    	self.file = f
    	self.lineno = l
    	self.addr = a.hex
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

    def search_source(addr)
    	min=99999
    	ret = nil
    	self.debugs.each do |elm|
    	    if elm.addr > addr
    		tmp = elm.addr - addr
    	    else
    		tmp = addr - elm.addr
    	    end
    
    	    if tmp < min
    		min = tmp
    		ret = elm
    	    end
    	end
    	return ret
    end

    def search_addr(f, l)
    	self.debugs.each do |elm|
    	    if f == elm.file && l == elm.lineno
    		return elm
    	    end
    	end
    	return nil
    end

end

class SourceInfo
    @mgr
    def self.init(path)
    	@mgr = ElfDebugManager.new()
    	f = open(path, "r")
    	f.each { | line|
    		elm = line.split()
    		if elm.length == 3
    			@mgr.add(elm[0], elm[1], elm[2])
    		end
    		#p line
    	}
    	f.close
    end
    def self.get()
    	return @mgr
    end
end


class AsmInfo
    @lastno
    @table
    def self.init()
	@lastno = -1
	@table = Array.new()
	p "AsmInfo.init:start"
	SourceInfo.init("../line.txt")
	p "AsmInfo.init:end"
    end
    def self.load_table()
	File.open("./table.dump", "r") do |file|
	    @table = Marshal.restore(file)
	end
    end
    def self.load(path, workbook)
	p "AsmInfo.load:start"
	#writer = ExcelWriter.new(workbook, "ASM")
	f_asm = open("asm.csv", "w")
	array = Array.new()

	mgr = SourceInfo.get()
	f = open(path, "r")
	l = 3
	@table[@table.length] = "NONE" + "," + "NONE"
	@table[@table.length] = "NONE" + "," + "NONE"
	@table[@table.length] = "NONE" + "," + "NONE"
	f.each { | line|
	    array = line.split(":")
	    if array.length >= 2
		if array[0].split().length == 1
		        pcaddr =  "0x" + array[0].strip
			elm = mgr.search_source(pcaddr.hex)
			#writer.set(l, 2, pcaddr)
			#writer.set(l, 3, elm.file)
			#writer.set(l, 4, elm.lineno)
			#writer.set(l, 5, array[1].strip)
			#p pcaddr
			#p elm
			f_asm.print(pcaddr + ",")
			f_asm.print(elm.file + ",")
			f_asm.print(elm.lineno + ",")
			f_asm.print("\"" + array[1].strip + "\", \n")
			@table[@table.length] = pcaddr + "," + l.to_s
		else
			#writer.set(l, 2, array[0].strip)
			#writer.merge_row(l, "B", "D")
			f_asm.print(array[0].split()[1].strip + ", \n")
			@table[@table.length] = "NONE" + "," + "NONE"
		end
		l = l + 1
	    end
	    #p array
	}
	f_asm.close
	f.close
	File.open("./table.dump", "wb") do |file|
	    Marshal.dump(@table, file)
	end
	p "AsmInfo.load:End"
    end

    def self.update(path, workbook)
	#p "update:" + path
	writer = ExcelWriter.new(workbook, "ASM")
	f = open(path, "r")
	last_line = nil
	f.each { | line|
	    last_line = line
	}
	f.close
	caddr="0x" + last_line.strip

	#p last_line
	#p @table.length.to_s
	l = -1
	@table.each { |elm|
	    pcaddr = elm.split(",")[0]
	    #p pcaddr
	    if pcaddr == caddr
	    	l = elm.split(",")[1].to_i
		#p pcaddr + ":" + l.to_s
		break
	    end
	}

	if l != $lastno
		if @lastno >= 0
			writer.clear_set(@lastno, 1, "")
		end
		if l > 0
			writer.active_set(l, 1, "??")
			@lastno = l
		end
	end
    end
end


class DebuggerV850
    	@@work = nil
    	@@ex = nil
	def self.init(path)
		excel = nil
		workbook = nil
		begin
			fso = WIN32OLE.new('Scripting.FileSystemObject')
			absPath = fso.GetAbsolutePathName(path)

			excel = WIN32OLE.new('Excel.Application')
			excel.visible = true
			@@ex = excel

			workbook = excel.Workbooks.Open absPath

			SymbolInfo.load("../symbol.txt", workbook)
			AsmInfo.init()
			AsmInfo.load("../asm.dump", workbook)
			excel.quit
			excel = nil
			@@work = workbook
		rescue => e
			if workbook != nil
				workbook.close(:SaveChanges => false)
			end
			if excel != nil
				excel.quit
				excel = nil
			end
			raise e
		end
	end
	def self.fin()
	    	p "FIN!"
		if @@work != nil
			@@work.close(:SaveChanges => false)
		end
		if @@ex != nil
			@@ex.quit
			@@ex = nil
		end
	end
	def self.start(path)
		excel = nil
		workbook = nil
		begin
		    	is_fin = false
			fso = WIN32OLE.new('Scripting.FileSystemObject')
			absPath = fso.GetAbsolutePathName(path)

			excel = WIN32OLE.new('Excel.Application')
			excel.visible = true
			@@ex = excel

			workbook = excel.Workbooks.Open absPath
			@@work = workbook

			AsmInfo.init()
			AsmInfo.load_table()
			while is_fin == false do
				begin
					CpuInfo.load("../cpuinfo.txt", workbook)
		 			StackInfo.load("../stack.txt", workbook)
					AsmInfo.update("../stderr.txt", workbook)
				  	sleep(1)
					r = File.exist?("state.txt")
					if r == false
					    is_fin = true
					end
				rescue =>e
				    p "error skip"
				    sleep(1)
				end
			end 
			workbook.close(:SaveChanges => false)
			excel.quit
			excel = nil
		rescue => e
			if workbook != nil
				workbook.close(:SaveChanges => false)
			end
			if excel != nil
				excel.quit
				excel = nil
			end
			raise e
		end
	end

end

if ARGV[0] == "init"
	DebuggerV850.init("Debugger.xlsx");
else
	File.open("state.txt", "wb") do |file|
	    file.puts("start")
	end
	DebuggerV850.start("Debugger.xlsx");
end

