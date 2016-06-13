#! ruby -Ks
require 'win32ole'
require '../gui_debugger/excel_reader'
require '../gui_debugger/spread_sheet_reader'
require '../gui_debugger/excel_writer'


class PcInfo
	@@table
	def self.init()
		@@table = Array.new()
		f = File.open("../cov.csv", "r")
		f.each { |line|
			str = line.split(",")[0]
			@@table[@@table.length] = str
		}
		f.close()
	end
	def self.ref()
		return @@table
	end
end

class AsmInfo
	@@table
	def self.init()
		File.open("../gui_debugger/table.dump", "r") do |file|
			@@table = Marshal.restore(file)
		end
	end

	def self.lineno(pc)
		l = 0
		@@table.each { | entry | 
			if entry != "NONE,NONE"
				n = entry.split(",")[0]
				if pc == n
					return l
				end
			end
			l = l + 1
		}
		return -1
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
		@@writer.active_set(lineno, 1, pc)
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



AsmInfo.init()
PcInfo.init()
CovMaster.init("./asm.xlsx")

	t = PcInfo.ref()
	t.each { |pc|
		lineno = AsmInfo.lineno(pc)
		CovMaster.activate(lineno - 2, pc)
	}

CovMaster.fin()

