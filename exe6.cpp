#include <iostream>
#include <vector>
#include <unistd.h>    // For fork(), getpid(), pipe(), close()
#include <sys/wait.h>  // For wait()
#include <sys/types.h> // For pid_t
#include <cstring>     // For strncpy

// Define a structure to represent a contact
struct Contact {
    char name[100];
    char phoneNumber[100];
};

// Function to add a contact to a pipe
void add2PB(int pipefd[2], const Contact& contact) {
    // Close the read end of the pipe since we are writing
    close(pipefd[0]);

    // Write the contact data to the pipe
    write(pipefd[1], &contact, sizeof(contact));

    // Close the write end of the pipe after writing
    close(pipefd[1]);
}

// Function to read contacts from a pipe
void findPhone(int pipefd[2], std::vector<Contact>& telephoneBook) {
    // Close the write end of the pipe since we are reading
    close(pipefd[1]);

    // Read contacts from the pipe
    Contact contact;
    while (read(pipefd[0], &contact, sizeof(contact)) > 0) {
        telephoneBook.push_back(contact);
    }

    // Close the read end of the pipe after reading
    close(pipefd[0]);
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <full_name> <phone_number>" << std::endl;
        return 1;
    }

    // Extracting the full name and phone number from command-line arguments
    char* full_name = argv[1];
    char* phone_number = argv[2];

    // Find the position of the last comma in the full name
    char* last_comma_position = strrchr(full_name, ',');
    if (last_comma_position == nullptr) {
        std::cerr << "Error: No comma found in the full name." << std::endl;
        return 1;
    }

    // Splitting the full name into first name and last name
    *last_comma_position = '\0'; // Replace the last comma with null terminator
    char* first_name = full_name;
    char* last_name = last_comma_position + 1;

    // Create a pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid_grep, pid_cut, pid_sed;

    // First child process for the grep command
    if ((pid_grep = fork()) == 0) {
        std::cout << "Child1 process (PID: " << getpid() << ")" << std::endl;
        dup2(pipefd[1], STDOUT_FILENO); // Redirect the output of the process to the write end of the pipe
        close(pipefd[0]); // Close the read end of the pipe
        close(pipefd[1]); // Close the write end of the pipe
        execlp("grep", "grep", last_name, "phonebook.txt", NULL); // Execute the grep command
        perror("execlp");
        return 1;
    } else if (pid_grep == -1) {
        perror("fork");
        return 1;
    }

    // Second child process for the cut command
    if ((pid_cut = fork()) == 0) {
        std::cout << "Child2 process (PID: " << getpid() << ")" << std::endl;
        dup2(pipefd[0], STDIN_FILENO); // Redirect the input of the process to the read end of the pipe
        close(pipefd[0]); // Close the read end of the pipe
        close(pipefd[1]); // Close the write end of the pipe
        execlp("cut", "cut", "-d,", "-f2", NULL); // Execute the cut command
        perror("execlp");
        return 1;
    } else if (pid_cut == -1) {
        perror("fork");
        return 1;
    }

    // Third child process for the sed command
    if ((pid_sed = fork()) == 0) {
        std::cout << "Child3 process (PID: " << getpid() << ")" << std::endl;
        close(pipefd[0]); // Close the read end of the pipe
        close(pipefd[1]); // Close the write end of the pipe
        execlp("sed", "sed", "s/ //g", NULL); // Execute the sed command
        perror("execlp");
        return 1;
    } else if (pid_sed == -1) {
        perror("fork");
        return 1;
    }

    std::cout << "Parent process (PID: " << getpid() << ")" << std::endl;
    // Close all pipe ends in the parent process
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for all child processes to finish
    waitpid(pid_grep, NULL, 0);
    waitpid(pid_cut, NULL, 0);
    waitpid(pid_sed, NULL, 0);

    return 0;
}
