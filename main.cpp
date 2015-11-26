#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

class HardDrive {

};

class OS {

	HardDrive *hd;
	int FAT [256];
	vector< vector< int > > fileBlock;

public:
	OS() { hd = new HardDrive(); }
	~OS() { delete hd; }

	void read(string nomFichier, int position, int nombreCaracteres, string &tampLecture) {
		
	}

	void write(String nomFichier, int position, int nombreCaracteres, String tampLecture) {
		ofstream file(nomFichier);

	}

	void deleteEOF(String nomFichier, int position) {

	}

	void printHD() {

	}
};

int main(void) {
	
	return 0;
}