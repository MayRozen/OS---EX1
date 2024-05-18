#include <iostream>
#include <vector>
#include <unistd.h>    // For fork(), getpid(), pipe(), close()
#include <sys/wait.h>  // For wait()
#include <sys/types.h> // For pid_t
#include <cstring>     // For strncpy
#include <sys/stat.h>  // For S_IFREG constant
#include <fstream>  // For file operations
#include <fcntl.h>
#include <exe6.cpp>

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