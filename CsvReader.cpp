#include "CsvReader.h"

#include <iostream>

namespace scv {

	std::vector<std::string> CsvReader::splitLineInVector(std::string &lineToSplit) {
		std::vector<std::string> splittedLine;
        int start, end;
        start = end = 0;
        char dl = ',';

        while ((start = lineToSplit.find_first_not_of(dl, end)) != std::string::npos) {
            end = lineToSplit.find(dl, start);

			splittedLine.push_back(lineToSplit.substr(start, end - start));
        }

		if (!spreadsheet.columnNames.empty()) {
			try {
				if (splittedLine.size() - 1 != spreadsheet.columnNames.size()) {
					throw splittedLine[0];
				}
			}
			catch (std::string &exp) {
				std::cout << "Wrong number of cells. Error at row " << exp << std::endl << "Exiting...";
				std::exit(EXIT_FAILURE);
			}
		}
		return splittedLine;
	}
	void CsvReader::addRowInSpreadsheet(std::vector<std::string> line) {

		checkIfCellDigit(line, 0);

		spreadsheet.rowNames.push_back(line[0]);
		for (int i = 1; i < line.size(); i++) {
			if (line[i].at(0) != '=') {
				checkIfCellDigit(line, i);
			}
			spreadsheet.sheet.insert({ spreadsheet.columnNames[i - 1] + line[0], line[i] });
		}
	}

	void CsvReader::calculateCellFormula(std::string cell) {
		std::size_t arithmeticalOperation = spreadsheet.sheet[cell].find_first_of("+-*/");
		std::string firstCell, secondCell;
		if (arithmeticalOperation != std::string::npos) {
			firstCell = spreadsheet.sheet[cell].substr(1, arithmeticalOperation-1);
			secondCell = spreadsheet.sheet[cell].substr(arithmeticalOperation + 1);
			checkCellFormula(firstCell);
			checkCellFormula(secondCell);
			switch (spreadsheet.sheet[cell].at(arithmeticalOperation)) {
			case '+':
				spreadsheet.sheet[cell] = std::to_string(std::stoi(spreadsheet.sheet[firstCell]) + std::stoi(spreadsheet.sheet[secondCell]));
				break;
			case '-':
				spreadsheet.sheet[cell] = std::to_string(std::stoi(spreadsheet.sheet[firstCell]) - std::stoi(spreadsheet.sheet[secondCell]));
				break;
			case '*':
				spreadsheet.sheet[cell] = std::to_string(std::stoi(spreadsheet.sheet[firstCell]) * std::stoi(spreadsheet.sheet[secondCell]));
				break;
			case '/':
				try {
					if (std::stoi(spreadsheet.sheet[secondCell]) == 0) {
						throw std::string("Cannot divide by zero.");
					}
				}
				catch (std::string &exp) {
					std::cout << exp << std::endl << "Wrong formula at " << cell << std::endl << "Exiting...";
					std::exit(EXIT_FAILURE);
				}
				spreadsheet.sheet[cell] = std::to_string(std::stoi(spreadsheet.sheet[firstCell]) / std::stoi(spreadsheet.sheet[secondCell]));
				break;
			default:
				break;
			}
			
		}
	}

	void CsvReader::checkIfCellDigit(std::vector<std::string> lineWithCell, int cellNumber) {
		try {
			if (!std::all_of(lineWithCell[cellNumber].begin(), lineWithCell[cellNumber].end(), ::isdigit)) {
				throw std::string("Wrong cell content.");
			}
		}
		catch (std::string& exp) {
			if (cellNumber == 0) {
				std::cout << exp << std::endl << "Error in row name " << lineWithCell[0] << std::endl << "Exiting...";
			}
			else {
				std::cout << exp << std::endl << "Error at " << spreadsheet.columnNames[cellNumber - 1] << lineWithCell[0] << std::endl << "Exiting...";
			}
			std::exit(EXIT_FAILURE);
		}
	}
	
	void CsvReader::checkIfCellDigit(std::string& cellName) {
		try {
			if (!std::all_of(spreadsheet.sheet[cellName].begin(), spreadsheet.sheet[cellName].end(), ::isdigit)) {
				throw std::string("Wrong cell content.");
			}
		}
		catch (std::string& exp) {
			std::cout << exp << std::endl << "Error at " << cellName << std::endl << "Exiting...";
			std::exit(EXIT_FAILURE);
		}
	}

	void CsvReader::checkCellFormula(std::string& cell) {
		try {
			if (spreadsheet.sheet.find(cell) == spreadsheet.sheet.end()) {
				throw std::string("Wrong Cell Formula ");
			}
		}
		catch (std::string &exp) {
			std::cout << exp << std::endl  << "There is no cell " << cell << std::endl << "Exiting...";
			std::exit(EXIT_FAILURE);
		}
		checkIfCellDigit(cell);
	}

	void CsvReader::calculateSpreadsheet() {
		for (const auto& n : spreadsheet.sheet) {
			calculateCellFormula(n.first);
		}
	}

	void CsvReader::readSpreadsheetFromFile(const char* fileName) {
		std::ifstream csvFile;
		csvFile.open(fileName);
		
		std::string line; 
		std::getline(csvFile, line);

		spreadsheet.columnNames = splitLineInVector(line);
		
		try {
			std::set<std::string> setOfColumnNames(spreadsheet.columnNames.begin(), spreadsheet.columnNames.end());
			if (spreadsheet.columnNames.size() != setOfColumnNames.size()) {
				throw std::string("In this table some column names are the same");
			}
		}
		catch (std::string& exp) {
			std::cout << exp << std::endl << "Exiting...";
			std::exit(EXIT_FAILURE);
		}
		
		while (std::getline(csvFile, line)) {
			addRowInSpreadsheet(splitLineInVector(line));
		}

		try {
			std::set<std::string> setOfRowNames(spreadsheet.rowNames.begin(), spreadsheet.rowNames.end());
			if (spreadsheet.rowNames.size() != setOfRowNames.size()) {
				throw std::string("In this table some row names are the same");
			}
		}
		catch (std::string& exp) {
			std::cout << exp << std::endl << "Exiting...";
			std::exit(EXIT_FAILURE);
		}

		calculateSpreadsheet();

		printSpreadsheet();
	}

	void CsvReader::printNiceSpreadsheet() {
		std::cout << "\t";
		for (auto &i : spreadsheet.columnNames) {
			std::cout << i << "\t";
		}
		std::cout << std::endl;
		for (int i = 0; i < spreadsheet.rowNames.size(); i++) {
			std::cout << spreadsheet.rowNames[i] << "\t";
			for (int j = 0; j < spreadsheet.columnNames.size(); j++) {
				std::cout << spreadsheet.sheet[spreadsheet.columnNames[j] + spreadsheet.rowNames[i]] << "\t";
			}
			std::cout << std::endl;
		}
	}
	
	void CsvReader::printSpreadsheet() {
		for (auto &i : spreadsheet.columnNames) {
			std::cout << i << ",";
		}
		std::cout << std::endl;
		for (int i = 0; i < spreadsheet.rowNames.size(); i++) {
			std::cout << spreadsheet.rowNames[i] << ",";
			for (int j = 0; j < spreadsheet.columnNames.size(); j++) {
				std::cout << spreadsheet.sheet[spreadsheet.columnNames[j] + spreadsheet.rowNames[i]] << ",";
			}
			std::cout << std::endl;
		}
	}

	
}
