#include <iostream>
#include <vector>
#include <unistd.h> // For fork(), getpid()
#include <sys/wait.h> // For wait()

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
void readFromPipe(int pipefd[2], std::vector<Contact>& telephoneBook) {
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
    // Create a pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Fork a child process
    pid_t pid = fork();

    if (pid == 0) {
        // Child process: Data management
        std::cout << "Child process (PID: " << getpid() << ") for data management" << std::endl;

        // Close the unused end of the pipe
        close(pipefd[1]);

        // Read contacts from the pipe and add to telephone book
        std::vector<Contact> telephoneBook;
        readFromPipe(pipefd, telephoneBook);

        // Displaying contacts (for verification)
        std::cout << "Contacts in telephone book:" << std::endl;
        for (const auto& contact : telephoneBook) {
            std::cout << "Name: " << contact.name << ", Phone: " << contact.phoneNumber << std::endl;
        }

        // Terminate child process
        exit(0);
    } else if (pid > 0) {
        // Parent process: User interface
        std::cout << "Parent process (PID: " << getpid() << ") for user interface" << std::endl;

        // Close the unused end of the pipe
        close(pipefd[0]);

        // Prepare contacts
        std::vector<Contact> telephoneBook;
        for (int i = 1; i < argc; i += 2) {
            Contact contact;
            snprintf(contact.name, sizeof(contact.name), "%s", argv[i]);
            snprintf(contact.phoneNumber, sizeof(contact.phoneNumber), "%s", argv[i + 1]);
            add2PB(pipefd, contact);
        }

        // Wait for the child process to finish
        wait(nullptr);
    } else {
        // Fork failed
        std::cerr << "Fork failed" << std::endl;
        return 1;
    }

    return 0;
}
