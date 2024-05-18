#include <iostream>
#include <vector>
#include <unistd.h>    // For fork(), getpid(), pipe(), close()
#include <sys/wait.h>  // For wait()
#include <sys/types.h> // For pid_t
#include <cstring>     // For strncpy
#include <sys/stat.h>  // For S_IFREG constant
#include <fstream>     // For file operations
#include <fcntl.h>
#include "exe6.cpp"    // Ensure this contains the definition of the Contact class

// Function to read contacts from a pipe and find the contact with a specific identifier
void findPhone(int pipefd[0], const std::string& searchName) {
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
        // Compare the read contact's name with the search name
        if (contact.name == searchName) {
            std::cout << "Phone number for " << searchName << ": " << contact.phoneNumber << std::endl;
            break;
        }
        pipefd=pipefd++;
    }

    // Check for read errors
    if (bytesRead == -1) {
        std::cerr << "Error: Failed to read from pipe" << std::endl;
        return;
    }

    // Close the read end of the pipe after reading
    close(pipefd[0]);
}
