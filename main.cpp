#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>
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

	/**
	 * Read all memory into string, then put 
	 * right block of memory into tampLecture
	 * @param numeroBlock position of the bloc to read
	 * @param tampLecture string to contain the block value
	 */
	void ReadBlock(int numeroBlock, string &tampLecture) {
		ifstream file(DISK_FILENAME);
		string memory = "";
		tampLecture = "";

		while (!file.eof()) {
			string temp;
			file >> temp;
			memory += temp;
		}
		memory += FillWithWhiteSpace(BLOCK_SIZE - memory.length());

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

	/**
	 * read all memory blocks until given block position, 
	 * then write to disk content from tampEcriture, then
	 * continue reading the rest of the memory. After that,
	 * write all memory to disk.
	 * @param numeroBlock  position of the block to write
	 * @param tampEcriture content to write in block
	 */
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

	/*  
	 * Get the first available empty memory space.
	 */
	int GetFirstAvailableMemorySpace() {
		ifstream file(DISK_FILENAME);

		for (int i=1; 1; i++) {
			if (i == BLOCK_COUNT - 1) return -1;
			if (file.eof()) return i;
			string out;
			file >> out;
			if (out[0] == ' ') return i;
		}

		file.close();
	}
};

class OS {

	HardDrive *hd;
	int FAT [256];
	map<string, int> files; //corresponding filename and fileID
	static int fileID;
	
public:
	OS() { hd = new HardDrive(); }
	~OS() { delete hd; }

	void addFile(string filename) {
		files.insert(std::pair<string, int>(filename, fileID++));
	}

	void read(string nomFichier, int position, int nombreCaracteres, string &tampLecture) {

	}

	void write(string nomFichier, int position, int nombreCaracteres, string tampLecture) {

	}

	void deleteEOF(string nomFichier, int position) {
		int startBlock = floor(position / BLOCK_SIZE); //The block to start with
		int index = files[nomFichier];

		for (int i=0; i < startBlock; i++) {
			index = FAT[index];
		}

		while (FAT[index] != 0) {

		}
	}

	void printHD() {

	}
};

int main(void) {
	
	return 0;
}