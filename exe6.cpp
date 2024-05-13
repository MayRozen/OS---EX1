// Need to be like this: Nezer Zaidenberg,054-5531415\n -> c language

#include <iostream>
#include <vector>
#include <unistd.h> // For fork(), getpid()
#include <sys/wait.h> // For wait()
#include <fstream> // For file I/O

// Define a structure to represent a contact
struct Contact {
    std::string name;
    std::string phoneNumber;
};

// Function to add a contact to a text file
void add2PB(const std::string& filename, const Contact& contact) {
    std::ofstream outFile(filename, std::ios_base::app); // Open file in append mode
    if (outFile.is_open()) {
        outFile << contact.name << "," << contact.phoneNumber << std::endl;
        std::cout << "Contact added to file: " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    outFile.close();
    return;
}


// This function is only for comparing with the runnings' results
std::string findPhone(const std::vector<Contact>& contacts, const std::string& name){
    for (const auto& contact : contacts) {
        if (contact.name == name) {
            return contact.name; // Name found
        }
    }
    std::cout<<"This name is not found."<<std::endl; // Name not found
    return;
}


int main(int argc, std::vector<Contact> telephoneBook) {
    // Fork a child process
    pid_t pid = fork();

    if (pid == 0) {
        // Child process: Data management
        std::cout << "Child process (PID: " << getpid() << ") for data management" << std::endl;

        std::string filename = "new_file.txt"; // Creating a new text file
        for(int i=0; i<argc; i++){
            add2PB(filename ,telephoneBook[i]);
        }

        // Terminate child process
        exit(0);
    } else if (pid > 0) {
        // Parent process: User interface
        std::cout << "Parent process (PID: " << getpid() << ") for user interface" << std::endl;

        // Wait for the child process to finish
        wait(nullptr);

        // Displaying contacts
        std::cout << "Contacts in telephone book:" << std::endl;
        for (const auto& contact : telephoneBook) {
            std::cout << "Name: " << contact.name << ", Phone: " << contact.phoneNumber << std::endl;
        }
    } else {
        // Fork failed
        std::cerr << "Fork failed" << std::endl;
        return 1;
    }

    return 0;
}