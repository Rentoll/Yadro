#include "CsvReader.h"

int main(int argc, char* argv[]) {
	scv::CsvReader spreadsheet;
	if (argc > 1) {
		spreadsheet.readSpreadsheetFromFile(argv[1]);
	}
	return 0;
}
