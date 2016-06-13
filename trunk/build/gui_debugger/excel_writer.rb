class ExcelWriter
    	@@active_row = -1
	attr_accessor :workbook
	attr_accessor :sheetName

	def initialize(workbook, sheetName)
		self.workbook = workbook
		self.sheetName = sheetName
	end

	def exist?()
		ret = false
		self.workbook.WorkSheets.each do |elm|
			if elm.name == self.sheetName
				ret = true
			end
		end
		return ret
	end
  
	
	def set(row, column, value)
		self.workbook.WorkSheets(self.sheetName).rows[row].columns[column].Value = value;
	end
	def clear_set(row, column, value)
		sheet = self.workbook.WorkSheets(self.sheetName)
		range = sheet.range("A"+row.to_s+":"+"E"+row.to_s)
		range.interior.colorindex = 2 #white
		sheet.rows[row].columns[column].Value = value;
	end
	def merge_row(row, column1, column2)
		sheet = self.workbook.WorkSheets(self.sheetName)
		range = sheet.range(column1+row.to_s+":"+column2+row.to_s)
		range.merge()
	end
	def active_set(row, column, value)
		sheet = self.workbook.WorkSheets(self.sheetName)
		range = sheet.range("A"+row.to_s+":"+"E"+row.to_s)
	    	if row != @@active_row
		    	range.activate
			sheet.range("A"+row.to_s).activate
			@@active_row = row
		end
		range.interior.colorindex = 4 #white
		sheet.rows[row].columns[column].Value = value;
	end
	def range_clr_focus(row, column1, column2)
		sheet = self.workbook.WorkSheets(self.sheetName)
		range = sheet.range(column1+row.to_s+":"+column2+row.to_s)
		range.interior.colorindex = 2 #white
	end
	def range_set_focus(row, column1, column2)
		sheet = self.workbook.WorkSheets(self.sheetName)
		range = sheet.range(column1+row.to_s+":"+column2+row.to_s)
		range.interior.colorindex = 4 #white
	end
end
