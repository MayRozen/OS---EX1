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
void add2PB(int pipefd[1], const Contact& contact, std::vector<Contact>& telephoneBook) {
    // Close the read end of the pipe since we are writing
    // close(pipefd[0]);

    // Write the contact data to the pipe
    ssize_t bytesWritten = write(pipefd[1], &contact, sizeof(contact));
    if (bytesWritten != sizeof(Contact)) {
        std::cerr << "Error: Incomplete write to pipe" << std::endl;
        return;
    }

    // Add the contact to the telephoneBook vector
    telephoneBook.push_back(contact);

    // Write the contact to the file
    std::fstream phonebook("phonebook.txt", std::ios::app); // Open file in append mode
    if (!phonebook) { // Checking if the file open successfully
        std::cerr << "Error: Failed to open phonebook.txt for writing" << std::endl;
        return;
    }
    phonebook << contact.name << ", " << contact.phoneNumber << std::endl;

    // Close the file
    phonebook.close();

    // Close the write end of the pipe after writing
   // close(pipefd[1]);
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
