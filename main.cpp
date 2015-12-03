#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

#define BLOCK_SIZE 64
#define BLOCK_COUNT 256
#define DISK_FILENAME "HD.DH"

class HardDrive {
private:
	string FillWithWhiteSpace(int number){
		string output = "";
		for (int i=0; i<number; i++) {
			output += " ";
		}
		return output;
	}

public:

	void ReadBlock(int numeroBlock, string &tampLecture) {
		ifstream file(DISK_FILENAME);
		string memory = "";
		tampLecture = "";

		while (!file.eof()) {
			string temp;
			file >> temp;
			memory += temp;
		}
		memory += FillWithWhiteSpace(BLOCK_SIZE - memory.length);

		for (int i=0; i < memory.length(); i++) {
			if (i > BLOCK_SIZE * numeroBlock) {
				for (int j=i; j < i + BLOCK_SIZE && j+i < memory.length(); j++) {
					tampLecture += memory[j];
				}
				break;
			}
		}

		file.close();
	}

	void WriteBlock(int numeroBlock, string tampEcriture) {
		ofstream file(DISK_FILENAME);
		ifstream getFile(DISK_FILENAME);
		string disk[BLOCK_COUNT];

		for (int i=0; i < BLOCK_COUNT; i++) {
			if (numeroBlock != i) {
				if (!getFile.eof()) {
					getFile >> disk[i];
				}
				else {
					disk[i] = " ";
				}
			}
			else {
				disk[i] = tampEcriture;
			}
		}

		for (int i=0; i < BLOCK_COUNT; i++) {
			file << disk[i];
		}

		file.close();
		getFile.close();
	}
};

class OS {

	HardDrive *hd;
	int FAT [256];
	vector< vector< int > > fileBlock;
	map<string, int> files; //corresponding filename and fileID
	
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