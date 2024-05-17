#include <iostream>
#include <vector>
#include <unistd.h>    // For fork(), getpid(), pipe(), close()
#include <sys/wait.h>  // For wait()
#include <sys/types.h> // For pid_t
#include <cstring>     // For strncpy
#include <sys/stat.h>  // For S_IFREG constant
#include <fstream>  // For file operations

// Define a structure to represent a contact
struct Contact {
    char name[100];
    char phoneNumber[100];
};

// Function to add a contact to a pipe
void add2PB(int argc, char* argv[]) {
            
    const char *filename = "phonebook.txt";
    std::ofstream file;
    file.open(filename, std::ios::app);
    if (!file) {
        std::perror("Failed to open file");
        return;
    }

    file << argv[1] << argv[2] << '\n';
    file.close();
}

// Function to read contacts from a pipe
void findPhone(int pipefd[2], std::vector<Contact>& telephoneBook) {
    // Close the write end of the pipe since we are reading
    close(pipefd[1]);

    // Read contacts from the pipe
    Contact contact;
    ssize_t bytesRead;
    while ((bytesRead = read(pipefd[0], &contact, sizeof(Contact))) > 0) {
        if (bytesRead != sizeof(Contact)) {
            std::cerr << "Error: Incomplete read from pipe" << std::endl;
            return;
        }
        telephoneBook.push_back(contact);
    }

    // Check for read errors
    if (bytesRead == -1) {
        std::cerr << "Error: Failed to read from pipe" << std::endl;
        return;
    }

    // Close the read end of the pipe after reading
    close(pipefd[0]);
}
// Function to create and populate the phonebook.txt file
int createPhoneBookFile() {
    std::ofstream phoneBookFile("phonebook.txt"); // Create the file
    if (!phoneBookFile) {
        std::cerr << "Error: Failed to create phonebook.txt" << std::endl;
        return -1;
    }

    // Write sample data to the file
    phoneBookFile << "John Doe, 1234567890" << std::endl;
    phoneBookFile << "Jane Smith, 9876543210" << std::endl;

    // Close the file
    phoneBookFile.close();
    return 1;
}

int main(int argc, char* argv[]) {

    if (argc <= 1){
        std::cerr<<"Error: not enough arguments"<<std::endl;
        return 1;
    }

    else if (argc > 3){
        std::cerr<<"Error: too many arguments"<<std::endl;
        return 1;
    }
    
    // Create the phonebook.txt file
    createPhoneBookFile();

    char *name_to_search = argv[1]; // The name which want to search

    // Create a pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::cerr<<"Error: pipe"<<std::endl;
        return 1;
    }
    if(argc == 3){
       add2PB(argc,argv); 
    }
    pid_t pid_grep, pid_cut, pid_sed;
    // Child process for the grep command
    // grep, egrep, fgrep, rgrep - print lines that match patterns
    if ((pid_grep = fork()) == 0)
    {
        // Child process
        std::cout<<"Child1 process"<<getpid()<<std::endl;
        dup2(pipefd[1], STDOUT_FILENO); // Redirect the output of the process to the write end of the pipe
        close(pipefd[0]); // Close the read end of the pipe
        close(pipefd[1]); // Close the write end of the pipe
        execlp("grep", "grep", argv[1], "phonebook.txt", NULL); // Execute the grep command
        std::cerr<<"Error: execlp"<<std::endl; // Usually after execlp it goes out form the fork
        return 1; // For the error
    } else if (pid_grep == -1){
        std::cerr<<"Error: fork"<<std::endl; 
        return 1;
    }

    // Second child process for cut command
    // cut - remove sections from each line of files
    if ((pid_cut = fork()) == 0){
        // Child process
        std::cout<<"Child2 process"<< getpid()<<std::endl; 
        dup2(pipefd[1], STDIN_FILENO); // Redirect the input of the process to the read end of the pipe
        close(pipefd[0]); // Close the read end of the pipe
        close(pipefd[1]); // Close the write end of the pipe
        execlp("cut", "cut", "-d,", "-f2", NULL); // Execute the sed command, -d, is the delimiter and -f2 is the field number
                                                  // -f 1 will return the name, -f 2 will return the phone number
        std::cerr<<"Error: execlp"<<std::endl; // Usually after execlp it goes out form the fork
        return 1; // For the error
    } else if (pid_cut == -1){
        std::cerr<<"Error: fork"<<std::endl;
        return 1;
    }

    // for changing the space to nothing by sed
    // sed - stream editor for filtering and transforming text
    if ((pid_sed = fork()) == 0){
        // Child process
        std::cout << "Child3 process" << getpid() << std::endl;
        dup2(pipefd[0], STDIN_FILENO); // Redirect the input of the process to the read end of the pipe
        close(pipefd[0]); // Close the read end of the pipe
        close(pipefd[1]); // Close the write end of the pipe
        execlp("sed", "sed", "s/ //g", NULL); // Execute the cut command
        std::cerr<<"Error: execlp"<<std::endl; // Usually after execlp it goes out form the fork
        return 1; // For the error
    } else if (pid_sed == -1){
        std::cerr<<"Error: fork"<<std::endl;
        return 1;
    }

    std::cout<<"Parent process"<<getpid()<<std::endl;
    // closing parent process
    close(pipefd[0]); // Close the read end of the pipe
    close(pipefd[1]); // Close the write end of the pipe

    // waiting for the child to finish. By waiting for all 3 of them it will be one after another
    // All of this replacing the grep "name" phonebook.txt | cut -d"," -f 2 | sed 's/ //g'
    waitpid(pid_grep, NULL, 0);
    waitpid(pid_cut, NULL, 0);
    waitpid(pid_sed, NULL, 0);

    return 0;
}