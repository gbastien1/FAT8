#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <time.h>
#include <math.h>

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
};

class OS {

	HardDrive *hd;
	int FAT [BLOCK_COUNT];
	map<string, int> files; //corresponding filename and fileID
	static int fileID;
	
public:
	OS() { hd = new HardDrive(); }
	~OS() { delete hd; }

	void addFile(string filename) {
		files.insert(std::pair<string, int>(filename, fileID++));
	}

	void read(string nomFichier, int position, int nombreCaracteres, string &tampLecture) {
		int index = files[nomFichier];
		string output = "";
		while (index != 0)
		{
			hd->ReadBlock(position, tampLecture);
			output += tampLecture;
			index = FAT[index];
		}
		tampLecture = output;
	}

	void write(string nomFichier, int position, int nombreCaracteres, string tampLecture) {
		int index = files[nomFichier];
		if (index == 0) {
			files[nomFichier] = fileID;
			fileID++;
		}
		int numBlocks = floor(tampLecture.length() / BLOCK_SIZE);
		for (int i = 0; i < numBlocks; i++) {
			string temp = tampLecture.substr(i, BLOCK_SIZE);
			hd->WriteBlock(index, temp);
			index = FAT[index];
		}
		//trouver position a ecrire dans hard disk (si -1, message d'erreur)
		//écrire (writeBlock) à la position retournée par la fonction ^
		//Ajouter no bloc à FAT à bonne position***
			//à indice du fileID, ajouter no du prochain bloc, puis
			//à no du prochain bloc dans FAT, ajouter no du prochain prochain bloc...
	}

	void deleteEOF(string nomFichier, int position) {
	
	}

	void printHD() {

	}
};

int main(void) {

	
	OS PatentedAwesomeTerminalOS;
	int RFile, ROperation;
	srand(time(NULL));
	string theFile, tampon = "";

	for (int i = 0; i< 100; i++)
	{
		RFile = rand() % 3 + 97;
		ROperation = rand() %3 + 1;

		theFile = RFile + ".txt";
		int nbrChar = rand() % 256 + 1;
		
		switch (ROperation)
		{
			case 1: //READ
				PatentedAwesomeTerminalOS.read(theFile, rand() % 256, nbrChar, tampon);
				break;
			case 2: //WRITE
				for (int j=0;j<nbrChar;j++) tampon.push_back((char)RFile);
				PatentedAwesomeTerminalOS.write(theFile, rand() % 256, nbrChar, tampon);
				break;
			case 3: //DELETEEOF
				PatentedAwesomeTerminalOS.deleteEOF(theFile, rand() % 256);
				break;
		}
	

	}

	return 0;
}