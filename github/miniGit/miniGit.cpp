#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <string>
using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include "miniGit.hpp"
#include <vector>

MiniGit::MiniGit(string directoryName) {
    directory_name = "../" + directoryName + "/";
    remote_name = "../." + directoryName + "_remote/";
    fs::remove_all(remote_name);
    fs::create_directory(remote_name);

    fs::remove_all(directory_name);
    fs::create_directory(directory_name);
    commits = 0;
    currentCommit = 0;

    commitHead = new BranchNode{0, "", nullptr, nullptr, nullptr};
    currentBranch = commitHead;
}

MiniGit::~MiniGit() { 
    fs::remove_all(remote_name);
    fs::remove_all(directory_name);

    BranchNode *currBranch = commitHead;
    BranchNode *tempBranch;

    while (currBranch != nullptr){
        tempBranch = currBranch;
        // delete the SLL files
        FileNode *currFile = currBranch->fileHead;
        FileNode *tempFile;

        while (currFile != nullptr){
            tempFile = currFile;


            currFile = tempFile->next;
            delete tempFile;
        }

        currBranch = currBranch->next;
        delete tempBranch;
    }
}

void MiniGit::add(string fileName){

    if (currentCommit != currentBranch->commitID){
        cout << "Must be on the current branch to make changes" << endl;
        return;
    }

    
    //1. check if file exists
    
    while (!fs::exists(directory_name + fileName)){
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        cout << "File does not exist. Please enter a valid file name" << endl << "#> ";
        cin >> fileName;
    }
    

    //2. check if already added to commit
    FileNode *currFile = currentBranch->fileHead;
    FileNode *prev = nullptr;

    while (currFile != nullptr){
        if (currFile->name == fileName){
            cout << "File has already been added to commit" << endl;
            return;
        }
        prev = currFile;
        currFile = currFile->next;
    }

    //3. create a new filenode and add to commit
    FileNode *newNode = new FileNode{fileName, 0, nullptr};

    if (prev == nullptr){
        currentBranch->fileHead = newNode;
    } else {
        prev->next = newNode;
    }

}

void MiniGit::rm(string fileName) {

    if (currentCommit != currentBranch->commitID){
        cout << "Must be on the current branch to make changes" << endl;
        return;
    }

    FileNode *currFile = currentBranch->fileHead;
    FileNode *prev = nullptr;

    // 1. Check if you can remove the fileName
    while (currFile != nullptr){
        // delete file
        if (currFile->name == fileName){
            if (prev == nullptr){
                currentBranch->fileHead = currFile->next;
            } else {
                prev->next = currFile->next;
            }
            delete currFile;
            return;
        }

        prev = currFile;
        currFile = currFile->next;
    }

    cout << "Cannot remove file because it was not added to the commit" << endl;
}



void MiniGit::commit(string msg) {

    if (currentCommit != currentBranch->commitID){
        cout << "Must be on the current branch to make changes" << endl;
        return;
    }

    FileNode *currFile = currentBranch->fileHead;
    currentBranch->commitMessage = msg;

    BranchNode *newBranch = new BranchNode{currentBranch->commitID + 1, "", nullptr, currentBranch, nullptr};
    FileNode *prev = nullptr;
    currentBranch->next = newBranch;


    while (currFile != nullptr){
        string currNotes = readFile(directory_name + currFile->name);
        if (!fs::exists(remote_name + currFile->name + "_" + to_string(currFile->version))){
            ofstream file;
            file.open(remote_name + currFile->name + "_" + to_string(currFile->version));
            file << currNotes;
            file.close();
        } else {
            string oldNotes = readFile(remote_name + currFile->name + "_" + to_string(currFile->version));

            if (oldNotes != currNotes){
                currFile->version += 1;
                ofstream file;
                file.open(remote_name + currFile->name + "_" + to_string(currFile->version));
                file << currNotes;
                file.close();
            }
        }


        FileNode *newFile = new FileNode{currFile->name, currFile->version, nullptr};

        if (prev == nullptr){
            newBranch->fileHead = newFile;
        } else {
            prev->next = newFile;
        }
        prev = newFile;

        currFile = currFile->next;
    }


    

    commits += 1;
    currentCommit += 1;
    currentBranch = currentBranch->next;
}

void MiniGit::checkout(string commitID) {

    while (true){
        if (commitID.empty()){
            cout << "Please enter a commit ID" << endl << "#> ";
            getline(cin, commitID);
            continue;
        }

        try{
            int commitNum = stoi(commitID);

            if (commitNum < 0 || commitNum > commits){
                throw (commitNum);
            }
            currentCommit = commitNum;
            break;
        } catch (...){
            cout << "Please enter a number from 0 - " << commits << endl << "#> ";
            getline(cin, commitID);
        }
    }


    BranchNode *oldBranch = commitHead;

    for (int i = 0; i < stoi(commitID); i++){
        oldBranch = oldBranch->next;
    }

    FileNode *oldFile = oldBranch->fileHead;

    while (oldFile != nullptr){
        ofstream file;
        string oldNotes = readFile(remote_name + oldFile->name + "_" + to_string(oldFile->version));
        string filepath = directory_name + oldFile->name;
        file.open(filepath);
        file << oldNotes;
        file.close();

        oldFile = oldFile->next;
    }

    currentCommit = stoi(commitID);

}

void MiniGit::push(){
    
}

string MiniGit::readFile(string filename){
    ifstream myFile(filename);
    string line = "";
    string str = "";
    

    while (getline(myFile, line)){
        str += line + "\n";
    }

    str = str.substr(0, str.size() - 1);


    myFile.close();

    return str;
}