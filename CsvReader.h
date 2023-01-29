#ifndef CSV_READER_H
#define CSV_READER_H
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <set>
#include <algorithm>

namespace scv {
	struct Spreadsheet {
		std::vector<std::string> columnNames;
		std::vector<std::string> rowNames;
		std::unordered_map<std::string, std::string> sheet;
	};

	class CsvReader {
	private:
		Spreadsheet spreadsheet;
		std::vector<std::string> splitLineInVector(std::string &firstSpreadsheetLine);
		void addRowInSpreadsheet(std::vector<std::string> line);
		void calculateCellFormula(std::string cell);
		void calculateSpreadsheet();
		void checkIfCellDigit(std::vector<std::string> lineWithCell, int cellNumber);
		void checkIfCellDigit(std::string &cellName);
		void checkCellFormula(std::string &cell);
	public:
		void readSpreadsheetFromFile(const char* fileName);
		void printSpreadsheet();
		void printNiceSpreadsheet();
	};
}

#endif