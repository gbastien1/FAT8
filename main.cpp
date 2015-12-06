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

	/**
	* Returns the space left in block in characters count
	* @param  block the block to get available space from
	* @return       (int)space available if block found, -1 otherwise
	*/
	int spaceLeftInBlock(int block) {
		ifstream file(DISK_FILENAME);
		string line = "";
		for (int i = 0; i <= block; i++) {
			file >> line;
			if (i == block) {
				return BLOCK_SIZE - line.length();
			}
		}
		return -1;
	}
};

class OS {

	HardDrive *hd;
	int FAT [BLOCK_COUNT];
	map<string, int> files; //corresponding filename and starting block in FAT
	
public:
	OS() { hd = new HardDrive(); }
	~OS() { delete hd; }

	/**
	* adds a new file manually to the files dictionnary
	* @param filename the filename of the added file, to which is mapped
	* its starting block in FAT
	*/
	void addFile(string filename) {
		files.insert(std::pair<string, int>(filename, hd->GetFirstAvailableMemorySpace()));
	}

	/**
	* Read from nomFichier nombreCaracteres characters from position,
	* and writes it to tampLecture that's passed as reference
	* @param nomFichier       The filename to read from
	* @param position         starting position of reading
	* @param nombreCaracteres number of chars to read
	* @param tampLecture      the buffer where to write the read chars
	*/
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


	/**
	* Writes the content of tampLecture to a given
	* file from position for nombreCaracteres
	* @param nomFichier       The filename where to append the content
	* @param position         The character position where to start writing
	* @param nombreCaracteres The number of characters to write (aka tampLecture.length())
	* @param tampLecture      The content to write to file
	*/
	void write(string nomFichier, int position, int nombreCaracteres, string tampLecture) {
		int block;
		string overwritten = "";
		//get index of file, if file does not exist, 
		//index = 0, so give it a correct index
		int index = files[nomFichier];

		//if file does not exist in dictionnary
		if (index == 0) { 
			block = hd->GetFirstAvailableMemorySpace(); //returns a block between 0 and 255 empty in FAT
			if (block == -1) {
				cout << "No memory available in hard drive!";
				return;
			}
			files[nomFichier] = block;
		}
		//if file exists in dictionnary
		else { 
			//get starting block from where to write
			int blockNumber = floor(position / BLOCK_SIZE);
			//then get to this block by going through FAT
			for (int i = 0; i < blockNumber; i++) {
				blockNumber = FAT[blockNumber];
			}
			block = blockNumber;
		}

		//calculates the char where to start writing into block (from 0 to 63)
		int offset = position < BLOCK_SIZE ? position % BLOCK_SIZE : position;

		//calculates the number of characters that have to be written in first block ( < 64 )
		int nbCharsInFirstBlock = BLOCK_SIZE - offset;

		//calculates the number of blocks to write (number of iterations in FAT)
		int blocksToWrite = 1 + ceil((nombreCaracteres - nbCharsInFirstBlock) / BLOCK_SIZE);

		//gets the first char block to write to incomplete first block
		string temp = tampLecture.substr(position, nbCharsInFirstBlock);
		//then change the position to which to write into file
		int newpos = position + nbCharsInFirstBlock;

		//for each block in FAT that have to be written
		for (int i = 1; i < blocksToWrite; i++) {
			string buffer = "";
			//read the block to keep the content that is being overwritten
			hd->ReadBlock(block, buffer);
			overwritten += buffer;

			//call hard drive's writeBlock method
			hd->WriteBlock(block, temp);
			

			//get the next block to which to write
			int prevBlock = block;
			block = FAT[block];
			//if it is the end of file, get a new available block and give it to FAT
			if (block == 0) {
				int b = hd->GetFirstAvailableMemorySpace();
				FAT[block] = b;
				FAT[b] = 0;
				block = b;
			}
			//get next 64 chars to write to file from tampLecture
			temp = tampLecture.substr(newpos, BLOCK_SIZE);
			//update the position from where to write
			newpos += BLOCK_SIZE;
		}

		//Append to file the content that was overwritten by write
		block = files[nomFichier];
		while (block != 0) {
			block = FAT[block]; //get ending block
		}

		//get the number of characters that can be written in last block
		int nbCharsToWrite = hd->spaceLeftInBlock(block); //create function (TODO ANTOINE)
		if (nbCharsToWrite == -1)  {
			cout << "error in write(), appending not working!"; return;
		}
		int pos = 0;
		//until everything was not appended, writeBlock
		while (pos < overwritten.length()) {
			//get chars to write to block corresponding to length that can ben written
			string buffer = overwritten.substr(pos, nbCharsToWrite);
			hd->WriteBlock(block, buffer);

			//get the number of characters that can be written to next empty block
			nbCharsToWrite = pos + BLOCK_SIZE < overwritten.length() ? BLOCK_SIZE : overwritten.length() - pos;
			//then update position. The while condition will check if the word is finished being written
			pos += nbCharsToWrite;
		}
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