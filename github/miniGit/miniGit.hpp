#pragma once

#include<iostream>
#include<vector>

using namespace std;

enum GITOP {init, add, rm, commit, checkout, status};

struct FileNode {
    string name;
    int version;
    FileNode* next;
};

struct BranchNode {
    int commitID;
    string commitMessage;
    BranchNode* next;
    BranchNode* previous;
    FileNode* fileHead;   
}; 

//*** You can add helper functions if you desire ***//
class MiniGit {
private:    
    BranchNode* commitHead;
    BranchNode* currentBranch;
    int commits;
    int currentCommit;
    string directory_name;
    string remote_name;

    string readFile(string filename);
   

public:
    MiniGit(string directoryName);
    ~MiniGit();

    void add(string fileName);
    void rm(string fileName);
    void commit(string msg);
    void push();
    void checkout(string commitID);
    
    
};