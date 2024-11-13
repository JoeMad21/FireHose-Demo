#include "firehose_ipu.hpp"

//#define NUM_PROGRAMS 3
#define PRODUCER 0
#define CONSUMER 1

void matMul(boost::program_options::variables_map& vm) {

    long unsigned int c = 0;
    long unsigned int tmp = TEMPLATE::V;
    long unsigned int size = 16;

    graphConfig myGraphConfig(vm);

    std::cout << "Adding Codelets..." << std::endl;

    myGraphConfig.addCodelet(CODELETS::POPLIN);
    myGraphConfig.addCodelet(CODELETS::POPOPS);

    // Add custom codelets
    myGraphConfig.addCustomCodelet("io_codelet.gp");

    std::cout << "Added Codelets!" << std::endl;

    // Build Model
    std::cout << "Building Model..." << std::endl;

    myGraphConfig.buildInternalTemplate(c, tmp, size);

    std::cout << "Built Model!" << std::endl;

    // Adding Programs

    std::cout << "Adding Programs..." << std::endl;

    myGraphConfig.buildExternalTemplate(tmp);

    std::cout << "Added Programs!" << std::endl;

}
