#include <iostream>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <string>
#include <limits>
#include <ctime>

using namespace std;

void printFileMode(mode_t mode){
  // r - read;
  // w - write;
  // e - executable;
  // - - no permission 

  // File type
  if(S_ISREG(mode)) cout << "-";        // звичайний файл
  else if(S_ISDIR(mode)) cout << "d";   // директорія
  else if(S_ISLNK(mode)) cout << "l";   // символьне посилання
  else if(S_ISCHR(mode)) cout << "c";   // символьний прилад
  else if(S_ISBLK(mode)) cout << "b";   // блочний прилад
  else if(S_ISFIFO(mode)) cout << "p";  // pipe
  else if(S_ISSOCK(mode)) cout << "s";  // socket
  else cout << "?";
// Owner 
  cout << ((mode & S_IRUSR) ? "r" : "-");
  cout << ((mode & S_IWUSR) ? "w" : "-");
  cout << ((mode & S_IXUSR) ? "x" : "-");
// Group
  cout << ((mode & S_IRGRP) ? "r" : "-");
  cout << ((mode & S_IWGRP) ? "w" : "-");
  cout << ((mode & S_IXGRP) ? "x" : "-");
// Other 
  cout << ((mode & S_IROTH) ? "r" : "-");
  cout << ((mode & S_IWOTH) ? "w" : "-");
  cout << ((mode & S_IXOTH) ? "x" : "-");

}

int main(){

  string pathToFile;
  cout << "Input path to file(e.g. fileName.txt): ";
  while(!(cin >> pathToFile)){
    cout << "Invalid input, try again :(" << endl;
    cin.clear();
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
  }

  struct stat fileMetaData;
  if(stat(pathToFile.c_str(), &fileMetaData) == -1 ){
    cout << "stat access error." << endl;
    return 1;
  };
  
  uid_t uid = getuid();
  gid_t gid = getgid();
  struct passwd *pw = getpwuid(uid);
  struct group *gr = getgrgid(gid);


  int choice = -1;
  while(true){
    
    cout << "1. Get metadata about specific file. " << endl;
    cout << "2. Get user and group name. " << endl;
    cout << "3. Get information about file and your permission through access()" << endl;
    cout << "4. Get information about about permissions in symbol format. " << endl;

    cout << "Your choice: ";
    while(!(cin >> choice)){
      cout << "Invalid input :(, try again" << endl;
      cin.clear();
      cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    }

    switch(choice){
      case 1: {
        cout << "Inode id: " <<  fileMetaData.st_ino << endl;
        cout << "Hard Links: " << fileMetaData.st_nlink << endl;
        cout << "UID: " << fileMetaData.st_uid << endl;
        cout << "GID: " << fileMetaData.st_gid << endl;
        cout << "Size: " << fileMetaData.st_size << " Bytes" << endl;
        cout << "File type: " << fileMetaData.st_mode << endl;
        cout << "Last access time: " << ctime(&fileMetaData.st_atime) << endl;
        cout << "Last mutation time: " << ctime(&fileMetaData.st_mtime) << endl;

        break;
      }
      case 2: {
        
        if(pw != nullptr){
          cout << "Username: " << pw->pw_name << endl;
        } else {
          cout << "Username doesn't found" << endl;
        }
        
        if(gr != nullptr){
          cout << "Groupname: " << gr->gr_name << endl; 
        } else {
          cout << "Groupname doesn't found" << endl;
        }

        break;
      }
      case 3: {
        if(access(pathToFile.c_str(), F_OK) == 0){
          cout << "File does exist!" << endl;
        } else {
          cout << "Unlucky for you, file doesn't exist =(" << endl;
        }

        if(access(pathToFile.c_str(), W_OK) == 0){
          cout << "You have permission to write into: " << pathToFile.c_str() << endl; 
        } else {
          cout << "You don't have permission to write into: " << pathToFile.c_str() << endl;
        }

        if(access(pathToFile.c_str(), R_OK) == 0){
          cout << "You have permission to read: " << pathToFile.c_str() << endl; 
        } else {
          cout << "You don't have permission to read: " << pathToFile.c_str() << endl;
        }

        break;
      }
      case 4: {
        printFileMode(fileMetaData.st_mode);
        cout << endl;
        break;
      }
      default: {

        return 0;
        break;
      }
    }

  }


  return 0;
}
