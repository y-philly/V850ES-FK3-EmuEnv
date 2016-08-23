#! ruby -Ks
require 'win32ole'
require '../gui_debugger/excel_reader'
require '../gui_debugger/spread_sheet_reader'
require '../gui_debugger/excel_writer'
require '../gui_debugger/source_navi'
require '../gui_debugger/elf_debug_manager'


class PcInfo
	@@table
	def self.init()
		@@table = Array.new()
		f = File.open("../cov.csv", "r")
		f.each { |line|
			str = line.split(",")[0]
			@@table[@@table.length] = str.split("x")[1]
		}
		f.close()
	end
	def self.ref()
		return @@table
	end
end

class CovAsmInfo
	@@source
	def self.init()
		File.open("../gui_debugger/source.dump", "r") do |file|
			@@source = Marshal.restore(file)
		end
	end

	def self.lineno(pc)
		#p pc
		return @@source.search_line(pc)
	end
end

class CovMaster
	@@work = nil
	@@ex = nil
	@@writer = nil
	def self.init(path)
		excel = nil
		workbook = nil
		begin
		    	p "INIT!"
			fso = WIN32OLE.new('Scripting.FileSystemObject')
			absPath = fso.GetAbsolutePathName(path)

			excel = WIN32OLE.new('Excel.Application')
			excel.visible = false
			@@ex = excel
			workbook = excel.Workbooks.Open absPath
			@@work = workbook
			@@writer = ExcelWriter.new(workbook, "asm")
		rescue => e
		    	p "FIN!"
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
	def self.activate(lineno, pc)
		@@writer.active_set(lineno, 1, "PASSED(0x"+pc+")")
	end

	def self.fin()
	    	p "FIN!"
		if @@work != nil
			@@work.close(:SaveChanges => true)
		end
		if @@ex != nil
			@@ex.quit
			@@ex = nil
		end
	end

	
end



CovAsmInfo.init()
PcInfo.init()
CovMaster.init("./asm.xlsx")

	t = PcInfo.ref()
	t.each { |pc|
		#p pc
		lineno = CovAsmInfo.lineno(pc)
		CovMaster.activate(lineno, pc)
	}

CovMaster.fin()

