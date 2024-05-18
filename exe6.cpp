#include <iostream>
#include <vector>
#include <unistd.h>    // For fork(), getpid(), pipe(), close()
#include <sys/wait.h>  // For wait()
#include <sys/types.h> // For pid_t
#include <cstring>     // For strncpy
#include <sys/stat.h>  // For S_IFREG constant
#include <fstream>  // For file operations
#include <fcntl.h>

// Define a structure to represent a contact
struct Contact {
    char name[100];
    char phoneNumber[100];
};


// Function to create and populate the phonebook.txt file
void createPhoneBookFile() {
    std::ofstream phonebook("phonebook.txt"); // Create the file
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
    int fileDescriptor = open("phonebook.txt", O_RDWR | O_APPEND); // open the file in read only and apped mode
    
    if (fileDescriptor == -1){
        std::cerr << "Error: open" << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; i++){
        // creating the first child process
        int pid1;
        if ((pid1 = fork()) == 0)
        {
            // This is like, FILE *fileDescriptor = fopen("phonebook.txt", "w+");

            // instead of output to the screen we want it to the file
            dup2(fileDescriptor, STDOUT_FILENO); // Redirect the output of the process to the write end of the pipe
            close(fileDescriptor);               // Close the file descriptor, beacuse we;ve redirected the output to the file

            if (i == argc - 1) // last argument, after it do echo to go down to the next line
            {
                execlp("echo", "echo", argv[i], NULL); // Execute the echo command
            }
            else
            {                                                    // If we have spaces between the arguments
                execlp("echo", "echo", "-n", argv[i], "", NULL); // Execute the echo command
            } // -n tells don't print the new line after each echo
            perror("execlp"); // Usually after execlp it goes out form the fork
            return 1;         // For the error
        }
    }
    char *name_to_search = argv[1]; // The name to search

    // Create pipes for communication between processes
    int pipefd_grep_cut[2];
    int pipefd_cut_sed[2];

    if (pipe(pipefd_grep_cut) == -1) {
        std::cerr << "Error: pipe" << std::endl;
        return 1;
    }

    if (pipe(pipefd_cut_sed) == -1) {
        std::cerr << "Error: pipe" << std::endl;
        return 1;
    }

    pid_t pid_grep, pid_cut, pid_sed;

    // Child process for the grep command
    if ((pid_grep = fork()) == 0) {
        std::cout << "Child1 process " << getpid() << std::endl;
        dup2(pipefd_grep_cut[1], STDOUT_FILENO); // Redirect the output to the write end of the first pipe
        close(pipefd_grep_cut[0]); // Close the read end of the first pipe
        close(pipefd_grep_cut[1]); // Close the write end of the first pipe
        close(pipefd_cut_sed[0]);  // Close the read end of the second pipe
        close(pipefd_cut_sed[1]);  // Close the write end of the second pipe
        execlp("grep", "grep", argv[1], "phonebook.txt", NULL); // Execute the grep command
        std::cerr << "Error: execlp" << std::endl;
        return 1;
    } else if (pid_grep == -1) {
        std::cerr << "Error: fork" << std::endl;
        return 1;
    }

    // Second child process for the cut command
    if ((pid_cut = fork()) == 0) {
        std::cout << "Child2 process " << getpid() << std::endl;
        dup2(pipefd_grep_cut[0], STDIN_FILENO);  // Redirect the input to the read end of the first pipe
        dup2(pipefd_cut_sed[1], STDOUT_FILENO); // Redirect the output to the write end of the second pipe
        close(pipefd_grep_cut[0]); // Close the read end of the first pipe
        close(pipefd_grep_cut[1]); // Close the write end of the first pipe
        close(pipefd_cut_sed[0]);  // Close the read end of the second pipe
        close(pipefd_cut_sed[1]);  // Close the write end of the second pipe
        execlp("cut", "cut", "-d,", "-f2", NULL); // Execute the cut command
        std::cerr << "Error: execlp" << std::endl;
        return 1;
    } else if (pid_cut == -1) {
        std::cerr << "Error: fork" << std::endl;
        return 1;
    }

    // Third child process for the sed command
    if ((pid_sed = fork()) == 0) {
        std::cout << "Child3 process " << getpid() << std::endl;
        dup2(pipefd_cut_sed[0], STDIN_FILENO); // Redirect the input to the read end of the second pipe
        close(pipefd_grep_cut[0]); // Close the read end of the first pipe
        close(pipefd_grep_cut[1]); // Close the write end of the first pipe
        close(pipefd_cut_sed[0]);  // Close the read end of the second pipe
        close(pipefd_cut_sed[1]);  // Close the write end of the second pipe
        execlp("sed", "sed", "s/ //g", NULL); // Execute the sed command
        std::cerr << "Error: execlp" << std::endl;
        return 1;
    } else if (pid_sed == -1) {
        std::cerr << "Error: fork" << std::endl;
        return 1;
    }

    std::cout << "Parent process " << getpid() << std::endl;
    // Close all pipe ends in the parent process
    close(pipefd_grep_cut[0]); // Close the read end of the first pipe
    close(pipefd_grep_cut[1]); // Close the write end of the first pipe
    close(pipefd_cut_sed[0]);  // Close the read end of the second pipe
    close(pipefd_cut_sed[1]);  // Close the write end of the second pipe

    // Wait for all child processes to finish
    waitpid(pid_grep, NULL, 0);
    waitpid(pid_cut, NULL, 0);
    waitpid(pid_sed, NULL, 0);

    return 0;
}
