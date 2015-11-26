#include <iostream>
using namespace std;

class HardDrive {

};

class OS {

	HardDrive *hd;

public:
	OS() { hd = new HardDrive(); }
	~OS() { delete hd; }

	void read();
	void write();
	void deleteEOF();
	void printHD();
};

int main(void) {
	
	return 0;
}