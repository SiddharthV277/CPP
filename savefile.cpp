#include <iostream>
#include <string>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <vector>
#include <cstring>

#ifdef _WIN32
#include <direct.h> // For _mkdir on Windows
#else
#include <sys/stat.h> // For mkdir on Unix-like systems
#endif


using namespace std;

bool isRegularFile(const string& path) {
    struct stat fileInfo;
    return stat(path.c_str(), &fileInfo) == 0 && S_ISREG(fileInfo.st_mode);
}

bool isDirectory(const string& path) {
    struct stat fileInfo;
    return stat(path.c_str(), &fileInfo) == 0 && S_ISDIR(fileInfo.st_mode);
}

void copyFile(const string& source, const string& destination) {
    ifstream sourceStream(source, ios::binary);
    if (!sourceStream) {
        cerr << "Error opening source file: " << source << endl;
        return;
    }

    ofstream destStream(destination, ios::binary);
    if (!destStream) {
        cerr << "Error opening destination file: " << destination << endl;
        return;
    }

    destStream << sourceStream.rdbuf();
}

bool containsImportantKeyword(const string& name) {
    string lowerName = name;
    transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    return lowerName.find("imp") != string::npos || lowerName.find("important") != string::npos;
}

void copyFiles(const string& sourceFolder, const string& destinationFolder, vector<string> parentFolders = {}, bool shouldCopy = false) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(sourceFolder.c_str())) == NULL) {
        cerr << "Error opening source folder: " << sourceFolder << endl;
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        string filename = entry->d_name;
        if (filename == "." || filename == "..") continue;

        string sourcePath = sourceFolder + "/" + filename;
        string destinationPath = destinationFolder + "/" + filename;

        if (isRegularFile(sourcePath)) {
            if (shouldCopy || containsImportantKeyword(filename)) {
                cout << "Copying: ";
                for (const auto& folder : parentFolders) cout << folder << "/";
                cout << filename << endl;
                copyFile(sourcePath, destinationPath);
            }
        } else if (isDirectory(sourcePath)) {
            bool isImportant = containsImportantKeyword(filename);
            vector<string> newParentFolders = parentFolders;
            newParentFolders.push_back(filename);

            if (shouldCopy || isImportant) {
                cout << "Copying folder: ";
                for (const auto& folder : newParentFolders) cout << folder << "/";
                cout << endl;

                #ifdef _WIN32
                if (_mkdir(destinationPath.c_str()) != 0 && errno != EEXIST) {
                #else
                if (mkdir(destinationPath.c_str(), 0755) != 0 && errno != EEXIST) {
                #endif
                    cerr << "Failed to create directory: " << destinationPath << ". Error: " << strerror(errno) << endl;
                    continue;
                }
                copyFiles(sourcePath, destinationPath, newParentFolders, true);
            } else {
                copyFiles(sourcePath, destinationPath, parentFolders, false);
            }
        }
    }
    closedir(dir);
}

int main() {
    string sourceFolder; 
    string destinationFolder; 

    // replace with device's destination path and source path
    sourceFolder = "C:/IMPORTANT FILES"; //source folder"
    destinationFolder = "D:/imporntant 2"; //destination folder"

    if (!isDirectory(sourceFolder)) {
        cerr << "Error: Source folder does not exist or is inaccessible." << endl;
        return 1;
    }

    if (!isDirectory(destinationFolder)) {
        cerr << "Error: Destination folder does not exist or is inaccessible." << endl;
        return 1;
    }

    cout << "Copying files recursively..." << endl;
    copyFiles(sourceFolder, destinationFolder);
    cout << "File copy completed." << endl;

    return 0;
}
