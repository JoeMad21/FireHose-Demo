#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <fstream>

#define PATH_MAX 2048

using namespace std;

void configure_benchmark(int fe_device, int be_device, int consumption_task, int source) {

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return;
    }
    
    string cm_cwd = cwd;

    string cm_compiler = "--std=c++11";
    string cm_firehose = "firehose_main.cpp";
    string cm_library_loc = "-L./device_libraries";
    string cm_library = "";
    switch(be_device) {
    case 1: 
        cm_library = "-lipu";
        break;

    //Add new devices here as support grows
    default:
        cout << "error: no be_device argument passed..." << endl;
        return;
    }
    string cm_output = "-o firehose";

    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "Fork failed" << std::endl;
        return;
    }
    else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0);  // Wait for the child process to finish
        if (WIFEXITED(status)) {
            std::cout << "Compilation completed with status " << WEXITSTATUS(status) << std::endl;
        }
    }
    else {
        // Child process
        execlp("g++", "g++", cm_compiler.c_str(), cm_firehose.c_str(), cm_library_loc.c_str(), cm_library.c_str(), cm_output.c_str(), NULL);
        // If execvp returns, an error occurred
        std::cerr << "Error executing compiler" << std::endl;
        _exit(1);  // Use _exit in child to avoid flushing buffers
    }

}


int main() {

int fe_device = 0;
cout << "Welcome to the FireHose Interface. What device will be the front-end?" << endl;
cout << "1. CPU" << endl;
cout << "2. Graphcore IPU" << endl;
cout << "Selected Device: ";
cin >> fe_device;

switch(fe_device) {
    case 1: 
        cout << "CPU Selected" << endl;
        break;

    //Add new devices here as support grows

    default:
        cout << "No device selected, ending program..." << endl;
        return 0;
}
cout << endl;

int be_device = 0;
cout << "What device will be the back-end?" << endl;
cout << "1. Graphcore IPU" << endl;
cout << "2. UPMEM DPU" << endl;
cout << "Selected Device: ";
cin >> be_device;

switch(be_device) {
    case 1: 
        cout << "IPU Selected" << endl;
        break;

    //Add new devices here as support grows

    default:
        cout << "No device selected, ending program..." << endl;
        return 0;
}
cout << endl;

int consumption_task = 0;
bool source_control = false;
cout << "What consumption task would you like to do on the back-end?" << endl;
cout << "1. Matrix multiplication" << endl;
cout << "2. Tensor Decomposition" << endl;
cout << "3. Hashing" << endl;
cout << "Consumption Task: ";
cin >> consumption_task;

switch(consumption_task) {
    case 1: 
        cout << "Matrix Multiplication Selected" << endl;
        source_control = true;
        break;

    case 2: 
        cout << "Tensor Decomposition Selected" << endl;
        source_control = true;
        break;

    //Add new consumption tasks here

    default:
        cout << "No consumption task selected, ending program..." << endl;
        return 0;
        break;
}
cout << endl;

int source = 0;
if(source_control) {

    int source = 0;
    cout << "Where should we source the data?" << endl;
    cout << "1. Random Generation" << endl;
    cout << "2. From file" << endl;
    cout << "Choice of Source: ";
    cin >> source;

    switch(source) {
      case 1:
        cout << "Random Generation Selected" << endl;
        break;

        //Add new generation methods here

      default:
        cout << "No source selected, ending program..." << endl;
        return 0;
        break;
    }
}
cout << endl;

cout << "Compiling code..." << endl;

configure_benchmark(fe_device, be_device, consumption_task, source);

}