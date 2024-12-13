#include <iostream>
#include "miniGit.hpp"
#include <filesystem>

using namespace std;

/*
 * Purpose; displays a menu with options
 */
void displayMenu()
{
    cout << "Select a numerical option:" << endl;
    cout << "+=====Main Menu=========+" << endl;
    cout << " 1. init " << endl;
    cout << " 2. add " << endl;
    cout << " 3. rm " << endl;
    cout << " 4. commit " << endl;
    cout << " 5. checkout" << endl;
    cout << " 6. push" << endl;
    cout << " 7. delete repository" << endl;
    cout << " 8. quit " << endl;
    cout << "+-----------------------+" << endl;
    cout << "#> ";
}
int main(int argc, char* argv[]) {

    int num = 0;
    string commitID = "";
    string filename = "";
    string message = "";
    string key = "";
    string directory = "";

    MiniGit *git;

    while (num != 8){
        displayMenu();
        try{
            cin >> num;
            if (cin.fail()){
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw invalid_argument("Please enter a number from 1 - 7\n#> ");
            }
        } catch (const invalid_argument& e){

        }

        switch (num){
            case 1:
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                while (directory.empty()){
                    cout << "Please enter a directory name for your project" << endl << "#> ";
                    getline(cin, directory);
                }
                git = new MiniGit(directory);
                break;
            case 2:
                cout << "Please enter a filename" << endl << "#> ";
                cin >> filename;
                git->add(filename);
                filename = "";
                break;
            case 3:
                cout << "Please enter a filename" << endl << "#> ";
                cin >> filename;
                git->rm(filename);
                filename = "";
                break;
            case 4:
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                while (message.empty()){
                    cout << "Please enter a commit message" << endl << "#> ";
                    getline(cin, message);
                }

                git->commit(message);
 
                message = "";
                break;
            case 5:
                cout << "Note: you will lose your most recent changes by checking out before commiting" << endl;
                
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                git->checkout(commitID);

                break;
            case 6:
                git->push();
                break;
            case 7:
                delete git;
                break;
            case 8:
                break;
            default:
                cout << "Please enter a number from 1 - 7" << endl << "#> ";
                
                break;
        }
    }
   
    return 0;
}