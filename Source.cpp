//Plagiarism Detector
//Final Project
//Abdullah Amjad & Danish Ahmed

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>

using namespace std;

class plagiarism {

private:

	string line;
	string linetomatch;
	int** totallines;
	vector <int> totallines1;
	int** plagiarisedlines;
	vector <int> plagiarisedlines1;
	float** plagiarismresult;
	vector <float> plagiarismresult1;
	string directory;
	string filename;
	int numoffiles;
	ifstream* file;
	ifstream file1;
	ifstream filetocheck;
	vector <string> filenames;

public:

	plagiarism();
	void welcomescreen ();
	void getdirectory ();	
	void getfilename ();
	void getfilesindirectory (string str);
	void createtables ();
	void openfilesandmatchlines ();
	void openfilesandmatchlines1 ();
	void calculateplagiarism ();
	void printplagiarimslinebyline ();
};

plagiarism::plagiarism() {
	welcomescreen();
}

void plagiarism::welcomescreen () {	
	int opt;
	cout << "Welcome to PlagFind." << endl;
	cout << "Please select one from the following:" << endl;
	cout << "Select 1 to find plagiarism between each file in a given directory." << endl;
	cout << "Select 2 to find plagiarism between a file and other files in a given directory.";
	cout << "Select 0 to exit the program." << endl;
	cin >> opt;
	switch (opt) {
		case 1:
			getdirectory();
			getfilesindirectory(directory);
			createtables();
			openfilesandmatchlines();
			calculateplagiarism();
			printplagiarimslinebyline();
			break;
		case 2:
			getdirectory();
			getfilesindirectory(directory);
			getfilename();
			openfilesandmatchlines1();
			break;
		case 0:
			plagiarism::~plagiarism();
			break;
	}
}

void plagiarism::getdirectory() {
	string path;
	cout << "Enter the complete path of the directory where files to be checked are located: ";
	cin >> path;
	directory = path;
}

void plagiarism::getfilename() {
	string name;
	cout << "Enter the name of the file you want to check plagiarism of: ";
	cin >> name;
	filename = name;
}


void plagiarism::getfilesindirectory (string directory) {

	DIR *dpdf;
	class dirent *epdf;
	dpdf = opendir(directory.c_str());
	if (dpdf != NULL) {
		while (epdf = readdir(dpdf)) {
			if ( !strcmp( epdf->d_name, "."   )) continue;
			if ( !strcmp( epdf->d_name, ".."  )) continue;
			filenames.push_back(epdf->d_name);
		}
	}
	else {
		perror("");
		plagiarism::~plagiarism();
	}
}

void plagiarism::createtables() {
	
	numoffiles = filenames.size();
	totallines = new int* [numoffiles];
	plagiarisedlines = new int* [numoffiles];
	plagiarismresult= new float* [numoffiles];
	file = new ifstream[numoffiles];
	
	for (int i = 0; i < numoffiles; i++) {
		totallines[i] = new int[numoffiles];
		plagiarisedlines[i] = new int[numoffiles];
		plagiarismresult[i] = new float[numoffiles];
	}
	
	
	for(int i = 0; i < numoffiles; i++) {	
		for(int j = 0; j < numoffiles; j++) {
			totallines[i][j] = 0;
			plagiarisedlines[i][j] = 0;			
			plagiarismresult[i][j] = 0;
		}
	}
}

void plagiarism::openfilesandmatchlines() {
	
	for(int i = 0; i < numoffiles; i++) {
		for(int j = 0; j < numoffiles; j++) {
			
			if(i == j) {
				continue;
			}
			
			if(file[i].is_open()) {
				file[i].close();
			}
			
			if(file[j].is_open()) {
				file[j].close();
			}

			file[i].open((directory + filenames[i]));			
			file[j].open((directory + filenames[j]));
					
			
			while(!file[i].eof()) {
				getline(file[i], line);
				totallines[i][j]++;
				while(!file[j].eof()) {
					getline(file[j], linetomatch);					
					if(line == linetomatch) {
						plagiarisedlines[i][j]++;
					}
				}
				line.clear();
				linetomatch.clear();
			}
		}
	}
}

void plagiarism::openfilesandmatchlines1() {
		
	numoffiles=filenames.size();
	for (int i = 0; i < numoffiles; i++) {
		totallines1.push_back(0);
		plagiarisedlines1.push_back(0);
		plagiarismresult1.push_back(0);
	}
		
	for (int i = 0; i < numoffiles; i++) {
		
		if (filename == filenames[i]) continue;
		
		if(filetocheck.is_open()) {
			filetocheck.close();
		}
		
		if(file1.is_open()) {
			file1.close();
		}

		file1.open(directory + filename);
		filetocheck.open(directory + filenames[i]);
				
		while(!file1.eof()) {
			getline(file1, line);
			totallines1[i]++;
			while(!filetocheck.eof()) {
				getline(filetocheck, linetomatch);					
				if(line == linetomatch) {
					plagiarisedlines1[i]++;
				}
			}
			line.clear();
			linetomatch.clear();
		}		
		plagiarismresult1[i] = (float)plagiarisedlines1[i]/(float)totallines1[i];
		plagiarismresult1[i] = plagiarismresult1[i]*100;
		cout << "The file " << filename<<  " is " << plagiarismresult1[i] << "% plagiarised from " << filenames[i] <<endl;
	}
}

void plagiarism::calculateplagiarism() {
	for(int i = 0; i < numoffiles; i++) {
		for(int j = 0; j < numoffiles; j++) {
			
			if(i == j){
				continue;
			}

			plagiarismresult[i][j] = (float)plagiarisedlines[i][j]/(float)totallines[i][j];
			plagiarismresult[i][j] = plagiarismresult[i][j]*100;
		}
	}		
}

void plagiarism::printplagiarimslinebyline() {
	
	for(int i = 0; i < numoffiles; i++) {
		for(int j = 0; j < numoffiles; j++) {
			if(i == j) {
					continue;
			}
			cout << "The file " << filenames[i] <<  " is " << plagiarismresult[i][j] << "% plagiarised from " << filenames[j] <<endl;
		}
	}
}


int main () {

	plagiarism pobject;
	system("pause");
}
