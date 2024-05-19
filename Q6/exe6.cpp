#include <iostream>
#include <vector>
#include <unistd.h> // For fork(), getpid(), pipe(), close()
#include <sys/wait.h> // For wait()
#include <sys/types.h> // For pid_t
#include <cstring> // For strncpy
#include <sys/stat.h> // For S_IFREG constant
#include <fstream> // For file operations
#include <fcntl.h>

// Define a structure to represent a contact
struct Contact {
    char name[100];
    char phoneNumber[100];
};

int add2PB(int argc,char* argv[]){
    if (argc < 2){
        std::cout << "Error: no arguments" << std::endl;
        return -1;
    }

    if (argc > 4){
        std::cout <<"Error: too many arguments" << std::endl;
        return -1;
    }

    // Going over the commands line arguments
    int fileDescriptor = open("phonebook.txt", O_RDWR | O_APPEND); // Open the file in read only and apped mode

    // if (fileDescriptor == NULL)
    if (fileDescriptor == -1){
        std::cerr <<"Error: open"<< std::endl;
        return -1;
    }

    // if we get the file that don't end with a new line
    // we are adding a new line before we will write the new contact.

    // Seek to the end of the file
    off_t endFile = lseek(fileDescriptor, 0, SEEK_END);
    if (endFile == -1){
        std::cerr <<"Error: lseek"<< std::endl;
        return -1;
    }

    // Move back one character
    off_t lastCharacter = lseek(fileDescriptor, -1, SEEK_CUR);
    if (lastCharacter == -1){
        std::cerr <<"Error: lseek"<< std::endl;
        return -1;
    }

    // Read the last character
    char lastChar;
    ssize_t readChar = read(fileDescriptor, &lastChar, 1);
    if (readChar == -1){
        std::cerr <<"Error: read"<< std::endl;
        return -1;
    }

    // If the last character is not a newline, append a newline
    if (lastChar != '\n'){
        write(fileDescriptor, "\n", 1);
    }

    for (int i = 1; i < argc; i++){
        // building the first child process
        int pid1;
        if ((pid1 = fork()) == 0){
            // writing the output too the phonebook file
            dup2(fileDescriptor, STDOUT_FILENO); // Redirect the output of the process to the write end of the pipe
            close(fileDescriptor);  // Close the file descriptor, beacuse we have redirected the output to the file

            // writing the arguments by echo commant,
            if (i == argc - 1){ // last argument, after it do echo to go down to the next line
                execlp("echo", "echo", argv[i], NULL); // Execute the echo command
            }
            else{ // If we have spaces between the arguments
                execlp("echo", "echo", "-n", argv[i], "", NULL); // Execute the echo command
            }
             // -n tells don't print the new line after each echo
            std::cerr <<"Error: execlp"<< std::endl; // Usually after execlp it goes out form the fork
            return -1;
        }

        else if (pid1 == -1){
            std::cerr <<"Error: fork"<< std::endl;
            return -1;
        }

        else{
            waitpid(pid1, NULL, 0);
        }
    }
    return 0;
}

int findPhone(int argc,char* argv[]){
    if (argc <= 1){
        std::cout << "Error: no arguments" << std::endl;
        return -1;
    }

    if (argc != 2){
        std::cout << "Error: too many arguments" << std::endl;
        return -1;
    }

    int pipefd[2]; // The pipes is for the connection between the to processes like the child and the parent

    int pid_grep, pid_cut, pid_sed; // The two processes one for grep and the other for cut

    if (pipe(pipefd) == -1){
        std::cerr <<"Error: pipe"<< std::endl;
        return 1;
    }

    // First child process for the grep command
    // grep, egrep, fgrep, rgrep - print lines that match patterns
    if ((pid_grep = fork()) == 0){
        // Child process
        dup2(pipefd[1], STDOUT_FILENO); // Redirect the output of the process to the write end of the pipe
        close(pipefd[0]); // Close the read end of the pipe
        close(pipefd[1]); // Close the write end of the pipe
        execlp("grep", "grep", argv[1], "phonebook.txt", NULL); // Execute the grep command

        std::cerr <<"Error: execlp"<< std::endl; // Usually after execlp it goes out form the fork
        return -1;
    }

    else if (pid_grep == -1){
        std::cerr <<"Error: fork"<< std::endl;
        return -1;
    }

    // Second child process for cut command
    // cut - remove sections from each line of files
    if ((pid_cut = fork()) == 0){
        // the child process
        dup2(pipefd[0], STDIN_FILENO); // Redirect the input of the process to the read end of the pipe
        close(pipefd[0]); // Close the read end of the pipe
        close(pipefd[1]); // Close the write end of the pipe
        execlp("cut", "cut", "-d,", "-f2", NULL); // Execute the sed command, -d, is the delimiter and -f2 is the field number
                                                  // -f 1 will return the name, -f 2 will return the phone number
        std::cerr <<"Error: execlp"<< std::endl; // Usually after execlp it goes out form the fork
        return -1;
    }

    else if (pid_cut == -1){
        std::cerr <<"Error: fork"<< std::endl;
        return -1;
    }

    // for changing the space to nothing by sed
    // sed - stream editor for filtering and transforming text
    if ((pid_sed = fork()) == 0){
        // The child process
        dup2(pipefd[0], STDIN_FILENO); // Redirect the input of the process to the read end of the pipe
        close(pipefd[0]); // Close the read end of the pipe
        close(pipefd[1]); // Close the write end of the pipe
        execlp("sed", "sed", "s/ //g", NULL); // Execute the cut command
        std::cerr <<"Error: execlp"<< std::endl; // Usually after execlp it goes out form the fork
        return -1;
    }

    else if (pid_sed == -1){
        std::cerr <<"Error: fork"<< std::endl;
        return -1;
    }

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


// Function to create and populate the phonebook.txt file
void createPhoneBookFile() {
    // Check if the file already exists
    std::ifstream existingFile("phonebook.txt");
    if (existingFile) {
        // File already exists
        return;
    }
    std::ofstream phonebook("phonebook.txt"); // Create the text file
    if (!phonebook) {
        std::cerr << "Error: Failed to create phonebook.txt" << std::endl;
        return;
    }

    // Write sample data to the file
    phonebook << "John Doe,1234567890" << std::endl;
    phonebook << "Jane Smith,9876543210" << std::endl;

    // Close the file
    phonebook.close();
}

int main(int argc, char* argv[]) {

    if (argc <= 1){
        std::cerr<<"Error: no enough arguments"<<std::endl;
        return 1;
    }

    if (argc > 3){
        std::cerr<<"Error: too many arguments"<<std::endl;
        return 1;
    }
    
    // Create the phonebook.txt file
    createPhoneBookFile();
    if(argc==2){
        findPhone(argc,argv);
    }
    else if(argc == 3 || argc == 4){
        add2PB(argc,argv);
    }

    // open the file in read only and apped mode
    int fileDescriptor = open("phonebook.txt", O_RDWR | O_APPEND);

    return 0;
}