#! ruby -Ks
require 'win32ole'

class ExcelRangeCopy
    	@@work_src = nil
    	@@work_dst = nil
    	@@ex = nil
	def self.init(src_path, dst_path)
		excel = nil
		workbook = nil
		begin
		    	p "init:start"
			fso = WIN32OLE.new('Scripting.FileSystemObject')
			src_absPath = fso.GetAbsolutePathName(src_path)
			dst_absPath = fso.GetAbsolutePathName(dst_path)

			excel = WIN32OLE.new('Excel.Application')
			excel.visible = false
			@@ex = excel

			@@work_src = excel.Workbooks.Open src_absPath
			@@work_dst = excel.Workbooks.Open dst_absPath

			p "init:end"
		rescue => e
			if @@work_src != nil
				@@work_src.close(:SaveChanges => false)
				@@work_src = nil
			end
			if @@work_dst != nil
				@@work_dst.close(:SaveChanges => false)
				@@work_dst = nil
			end
			if excel != nil
				excel.quit
				excel = nil
			end
			raise e
		end
	end
	def self.copy(src_sheet_name, src_range, dst_sheet_name, dst_range)
	    p "Range Copy start"
	    @@work_src.Worksheets(src_sheet_name).Range(src_range).Copy(@@work_dst.Worksheets(dst_sheet_name).Range(dst_range))
	    p "Range Copy end"
	end
	def self.fin(flag)
	    	p "FIN!"
		if @@work_src != nil
			@@work_src.close(:SaveChanges => flag)
			@@work_src = nil
		end
		if @@work_dst != nil
			@@work_dst.close(:SaveChanges => flag)
			@@work_dst = nil
		end
		if @@ex != nil
			@@ex.quit
			@@ex = nil
		end
	end

end

if ARGV.length != 6
	p "Invalid Argument!"
	p "Usage:ruby excel_range_copy <src_file> <src_sheet> <src_range> <dst_file> <dst_sheet> <dst_range>"
	exit(1)
end

src_file  = ARGV[0]
src_sheet = ARGV[1]
src_range = ARGV[2]

dst_file  = ARGV[3]
dst_sheet = ARGV[4]
dst_range = ARGV[5]

begin
	ExcelRangeCopy.init(src_file, dst_file);
	ExcelRangeCopy.copy(src_sheet, src_range, dst_sheet, dst_range);
	ExcelRangeCopy.fin(true);
rescue => e
	ExcelRangeCopy.fin(false);
end

