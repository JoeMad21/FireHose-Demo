#include "ipu_support.hpp"

/**
 * @brief Quick use constructor to try out a design
 * 
 * Way of constructing a graphConfig object if you want to throw a design together quickly.
 * Not recommended for general design.
 * 
 * @param  map:  Standard map of values used to create poplar graph, indexed using strings
 * 
 * @return Creates a graphConfig object
 */

graphConfig::graphConfig() {
    // Read JSON file
    std::ifstream jsonFile("graphconfig.json");
    if (!jsonFile.is_open()) {
        throw std::runtime_error("Could not open JSON file");
    }

    // Parse JSON data
    nlohmann::json jsonData;
    jsonFile >> jsonData;

    // Directly assign JSON values to member variables
    this->op = jsonData["op"];
    this->num_inputs = jsonData["num_inputs"];
    this->num_outputs = jsonData["num_outputs"];
    this->dtype = jsonData["dtype"];
    this->hardware = jsonData["hardware"];
    this->num_devices = jsonData["num_devices"];
    this->num_cores = jsonData["num_cores"];
    this->map_tensor = jsonData["map_tensor"];
    this->map_vertex = jsonData["map_vertex"];
    this->comPat_Internal = jsonData["comPat_Internal"];
    this->comPat_External = jsonData["comPat_External"];
    this->core_tiles = jsonData["start_idx"];
    this->num_tiles = jsonData["num_tiles"];
    this->max_layers_per_core = jsonData["max_layers_per_core"];
    this->max_tensors_per_layer = jsonData["max_tensors_per_layer"];
    this->max_progs_per_core = jsonData["max_progs_per_core"];
    this->buf_depth = jsonData["buf_depth"];

    // Get Device
    poplar::Device device = getDevice(this->hardware, this->num_devices);
    this->device = device;

    // Graph Setup
    std::cout << "Creating Graph..." << std::endl;
    poplar::Graph graph(this->device.getTarget());
    this->graph = std::move(graph);
    this->allocateVirtualGraphs(); // this->vgraph member
    this->buildCores();
    std::cout << "Created Graph!" << std::endl;

    // Allocate DataStreams (Connections from CPU to IPU)
    this->allocateStreams(); //this->strms member

    // Allocate Program Memory
    this->allocateComputeSets(); // this->cps member
    this->allocateVertices(); // this->vtx member
    this->allocatePrograms(); // this->prgs member
}

//void graphConfig::graphConfig(boost::program_options::variables_map& vm) {

    // Application Settings
    //this->op = graphDesc["op"].as<long unsigned int>();
    //this->num_inputs = graphDesc["num_inputs"].as<long unsigned int>();
    //this->num_outputs = graphDesc["num_outputs"].as<long unsigned int>();
    //this->dtype = graphDesc["dtype"].as<long unsigned int>();

    // Device Settings
    //this->hardware = graphDesc["hardware"].as<long unsigned int>();
    //this->num_devices = graphDesc["num_devices"].as<long unsigned int>();

    // Design Settings
    //this->num_cores = graphDesc["num_cores"].as<long unsigned int>();
    //this->map_tensor = graphDesc["map_tensor"].as<long unsigned int>();
    //this->map_vertex = graphDesc["map_vertex"].as<long unsigned int>();
    //this->comPat_Internal = graphDesc["comPat_Internal"].as<long unsigned int>();
    //this->comPat_External = graphDesc["comPat_External"].as<long unsigned int>();
    //this->core_tiles = graphDesc["start_idx"].as<long unsigned int>();
    //this->num_tiles = graphDesc["num_tiles"].as<long unsigned int>();
    //this->max_layers_per_core = graphDesc["max_layers_per_core"].as<long unsigned int>();
    //this->max_tensors_per_layer = graphDesc["max_tensors_per_layer"].as<long unsigned int>();
    //this->max_progs_per_core = graphDesc["max_progs_per_core"].as<long unsigned int>();

    // Optimization Settings
    //this->buf_depth = graphDesc["buf_depth"].as<long unsigned int>();

    // Get Device
    //poplar::Device device = getDevice(graphDesc["hardware"].as<long unsigned int>(), graphDesc["num_devices"].as<long unsigned int>());
    //this->device = device;

    // Graph Setup
    //std::cout << "Creating Graph..." << std::endl;
    //poplar::Graph graph(this->device.getTarget());
    //this->graph = graph;
    //this->allocateVirtualGraphs(); // this->vgraph member
    //this->buildCores();
    //std::cout << "Created Graph!" << std::endl;

    // Allocate DataStreams (Connections from CPU to IPU)
    //this->allocateStreams(); //this->strms member

    //Allocate Program Memory
    //this->allocateComputeSets(); // this->cps member
    //this->allocateVertices(); // this->vtx member
    //this->allocatePrograms(); // this->prgs member

    //return;
//}


/**
 * @brief Quick use constructor to try out a design
 * 
 * Way of constructing a graphConfig object if you want to throw a design together quickly.
 * Not recommended for general design.
 * 
 * @param  map:  Standard map of values used to create poplar graph, indexed using strings
 * 
 * @return Creates a graphConfig object
 */

//void graphConfig::graphConfig(std::map<std::string, long unsigned int>& graphDesc) {

    // Application Settings
    //this->op = graphDesc["op"];
    //this->num_inputs = graphDesc["num_inputs"];
    //this->num_outputs = graphDesc["num_outputs"];
    //this->dtype = graphDesc["dtype"];

    // Device Settings
    //this->hardware = graphDesc["hardware"];
    //this->num_devices = graphDesc["num_devices"];

    // Design Settings
    //this->num_cores = graphDesc["num_cores"];
    //this->map_tensor = graphDesc["map_tensor"];
    //this->map_vertex = graphDesc["map_vertex"];
    //this->comPat_Internal = graphDesc["comPat_Internal"];
    //this->comPat_External = graphDesc["comPat_External"];
    //this->core_tiles = graphDesc["start_idx"];
    //this->num_tiles = graphDesc["num_tiles"];
    //this->max_layers_per_core = graphDesc["max_layers_per_core"];
    //this->max_tensors_per_layer = graphDesc["max_tensors_per_layer"];
    //this->max_progs_per_core = graphDesc["max_progs_per_core"];

    // Optimization Settings
    //this->buf_depth = graphDesc["buf_depth"];

    // Get Device
    //poplar::Device device = getDevice(graphDesc["hardware"], graphDesc["num_devices"]);
    //this->device = device;

    //const unsigned totalTiles = device.getTarget().getNumTiles();

    // Graph Setup
    //std::cout << "Creating Graph..." << std::endl;
    //poplar::Graph graph(this->device.getTarget());
    //this->graph = graph;
    //this->allocateVirtualGraphs(); // this->vgraph member
    //this->buildCores();
    //std::cout << "Created Graph!" << std::endl;

    // Allocate DataStreams (Connections from CPU to IPU)
    //this->allocateStreams(); //this->strms member

    //Allocate Program Memory
    //this->allocateComputeSets(); // this->cps member
    //this->allocateVertices(); // this->vtx member
    //this->allocatePrograms(); // this->prgs member

    //return;
//}


/**
 * @brief Creates a pointer to the IPU device
 * 
 * Standard way of creating a link to the IPU device. All poplar types are deployed on the device using this link.
 * 
 * @return void: Modifies the graphConfig's device member
 */
void graphConfig::getDevice() {
    std::cout << "Getting Device..." << std::endl;

    auto manager = poplar::DeviceManager::createDeviceManager();
    std::vector<poplar::Device> temp_devices;
    poplar::Device temp_device;

    switch(this->hardware) {
        case HARDWARE::IPU:
            temp_devices = manager.getDevices(poplar::TargetType::IPU, this->num_devices);
            break;

        case HARDWARE::MODEL:
            temp_devices = manager.getDevices(poplar::TargetType::IPU_MODEL, this->num_devices);
            break;

        case HARDWARE::CPU:
            temp_devices = manager.getDevices(poplar::TargetType::CPU, this->num_devices);
            break;
    }

    auto it = std::find_if(temp_devices.begin(), temp_devices.end(), [](poplar::Device &temp_device) { return device.attach(); });
    poplar::Device device;

    if (it != temp_devices.end()) {
        temp_device = std::move(*it);
    }

    std::cout << "Got Device!" << std::endl;

    this->device = std::move(temp_device);

    return;
}

/**
 * @brief Pre-allocates the memory in GraphConfig for the virtual graphs
 * 
 * We need to prepopulate the vgraphs member to modify it later.
 * This is a 1D vector. A list of all the cores we will be using.
 * 
 * @note We are NOT YET modifying memory on the device!
 * 
 * @return void: Modifies the graphConfig's vgraphs member
 */
void graphConfig::allocateVirtualGraphs() {

    long unsigned int dim1 = this->num_cores;
    
    std::vector<poplar::Graph> vgraphs(dim1);

    this->vgraphs = vgraphs;

    return;
}

/**
 * @brief Creates all of the virtual graphs that we will be using later.
 * 
 * We are accessing the graph member of GraphConfig to split it into asynchronous regions using createVirtualGraph().
 * These are our cores.
 * 
 * Each core starts at its core tile and ends at start index + number of tiles allocated - 1.
 * For example: If I want a core to start at tile 2 and end at tile 7
 * -> start_idx = 2
 * -> num_tiles = 6
 * 
 * @note We are NOW modifying memory on the device!
 * 
 * @return void: Modifies the graphConfig's vgraph's member
 */
void graphConfig::buildCores() {

    poplar::Graph base_graph = this->graph;
    long unsigned int start_idx;
    long unsigned int num_tiles;

    for(int i = 0; i < this->num_cores; i++) {
        start_idx = this->core_tiles[i];
        num_tiles = this->num_tiles[i];
        this->vgraphs[i] = base_graph.createVirtualGraph(start_idx, start_idx+num_tiles-1);
    }

    return;
}

/**
 * @brief Pre-allocates the memory in GraphConfig for the dataStreams.
 * 
 * We need to prepopulate the strms member to modify it later.
 * This is a 2D vector. The input access order is: I/O direction, stream ID.
 * 
 * For example, if I want to access the second input stream.
 * ->strms[IO::IN][1]
 * 
 * If I want to access the third output stream.
 * ->strms[IO::OUT][2]
 * 
 * @note We are NOT YET modifying memory on the device!
 * 
 * @return void: Modifies the graphConfig's strms member
 */
void graphConfig::allocateStreams() {
    long unsigned int dim1 = 2;
    long unsigned int dim2 = std::max(this->num_inputs, this->num_outputs); // We don't mind wasting a little space


    std::vector< std::vector<poplar::DataStream>> strms(dim1, sstd::vector<poplar::VertexRef>>(dim2));

    this->strms = strms;

    return;
}

/**
 * @brief Pre-allocates the memory in GraphConfig for the computeSets.
 * 
 * We need to prepopulate the cps member to modify it later.
 * This is a 2D vector. The input access order is: Core ID, ComputeSet ID.
 * 
 * For example, if I want to access the second computeSet on core 3.
 * ->cps[2][1]
 * 
 * @note We are NOT YET modifying memory on the device!
 * 
 * @return void: Modifies the graphConfig's cps member
 */
void graphConfig::allocateComputeSets() {
    // Assumption: Each core only needs one compute set

    long unsigned int dim1 = this->num_cores;
    long unsigned int dim2 = this->max_cps; // We don't mind wasting a little space

    std::vector<std::vector<poplar::ComputeSet>> cps(dim1, std::vector<poplar::ComputeSet>(dim2));

    this->cps = cps;

    return;
}

/**
 * @brief Pre-allocates the memory in GraphConfig for the vertices.
 * 
 * We need to prepopulate the vtx member to modify it later.
 * This is a 3D vector. The input access order is: Core ID, Vertex Layer ID, Vertex ID
 * 
 * This is where it gets a bit confusing. Below is a diagram of tensor layers and vertex layers.
 * 
 * / TSR Layer 0 /
 *         |
 *         V
 * / VTX Layer 0 /
 *         |
 *         V
 *  / TSR Layer 1 /
 *         |
 *         V
 *  / VTX Layer 1 /
 *         |
 *         V
 *  / TSR Layer 2 /
 * 
 * In this diagram, there is three tensor layers and two vertex layers. There will always be one less vertex layers than tensor layers.
 * The vertex layer ID will always be the same as the tensor layer above it.
 * The reason for this setup is to enforce that vertex layers can only connect tensors above and below themselves.
 * 
 * For example, if I want to access the 4th vertex in vertex layer 1 on core 3
 * ->vtx[2][1][3]
 * 
 * @note We are NOT YET modifying memory on the device!
 * 
 * @return void: Modifies the graphConfig's vtx member
 */
void graphConfig::allocateVertices() {
    // Assumption: Between each layer is a set of vertices, so one less than the max number of tensor layers

    long unsigned int dim1 = this->num_cores;
    long unsigned int dim2 = this->max_tensor_layers-1; // We don't mind wasting a little space
    long unsigned int dim3 = this->max_vtx; // We don't mind wasting a little space

    std::vector<std::vector<std::vector<poplar::VertexRef>>> vtx(dim1, std::vector<std::vector<poplar::VertexRef>>(dim2, std::vector<poplar::VertexRef>(dim3)));

    this->vtx = vtx;

    return;
}

/**
 * @brief Pre-allocates the memory in GraphConfig for the programs.
 * 
 * We need to prepopulate the progs member to modify it later.
 * This is a 2D vector. The input access order is: Core ID, Program ID
 * 
 * For example, if I want to access the 4th program on core 0.
 * ->progs[0][3]
 * 
 * @note In most cases, we are assuming only one program per core!
 * @note We are NOT YET modifying memory on the device!
 * 
 * @return void: Modifies the graphConfig's progs member
 */
void graphConfig::allocatePrograms() {
    // Assumption: Between each layer is a set of vertices, so one less than the max number of tensor layers

    long unsigned int dim1 = this->num_cores;
    long unsigned int dim2 = this->max_progs_per_core;

    std::vector<std::vector<poplar::program::Program>> progs(dim1, std::vector<poplar::program::Program>(dim2));

    this->progs = progs;

    return;
}

/**
 * @brief Pre-allocates the memory in GraphConfig for the tensors
 * 
 * We need to prepopulate the tensors member to modify it later.
 * This is a 3D vector. The input access order is: Core ID, Tensor Layer ID, Tensor ID
 * 
 * Layer structure is utilized to enforce waterfall design pattern.
 * 
 * For example, if I want to access the 6th tensor in layer 2 on core 0.
 * ->vtx[0][1][5]
 * 
 * @note We are NOT YET modifying memory on the device!
 * 
 * @return void: Modifies the graphConfig's tensors member
 */
void graphConfig::allocateTensors() {
    long unsigned int dim1 = this->num_cores;
    long unsigned int dim2 = this->num_layers;
    long unsigned int dim3 = this->max_layer_width;

    std::vector<std::vector<std::vector<poplar::Tensors>>> tensors(dim1, std::vector<std::vector<poplar::Tensors>>(dim2, std::vector<poplar::Tensors>(dim3)));
}

/**
 * @brief Adds codelets from the Poplar Library to our design.
 * 
 * You use this function when you want to add a pre-made poplar codelet to the design.
 * 
 * @return void: Modifies the graph member
 */
void graphConfig::addCodelet(long unsigned int cdlt) {

    switch(cdlt) {
        case CODELETS::POPLIN:
            poplin::addCodelets(this->graph);
            break;

        case CODELETS::POPOPS:
            popops::addCodelets(this->graph);
            break;

        case CODELETS::POPRAND:
            poprand::addCodelets(this->graph);
            break;

        default:
            break;
    }

    return;
}

/**
 * @brief Adds custom codelets to the graph design.
 * 
 * You use this function when you want to add a CUSTOM codelet to the design.
 * 
 * @note The codelets are assumed to be in the device_libraries folder!
 * 
 * @return void: Modifies the graph member
 */
void graphConfig::addCustomCodelet(std::string filename) {

    std::string fileAddress;
    fileAddress = "./device_libraries/" + filename;

    this->graph.addCodelets(fileAddress);

    return;
}

/**
 * @brief Adds tensors to a core (virtual graph)
 * 
 * You use this function add tensors to some layer in a core.
 * 
 * @param core: Core we are adding tensors to
 * @param layer_id: Layer in the core we are adding a tensor to
 * @param tensor_id: Starting index to begin adding tensors
 * @param num_tensors: Number of tensors to add to the layer
 * @param dim: Dimensions of the square tensors you are adding
 * @param mappings: Optional parameter for when you want to specify the tile mappings of the tensors
 * 
 * For example, if I wanted to add 3 16x16 tensors starting at index 5 in layer 7 of core 0.
 * ->buildLayer(0, 6, 4, 3, 16)
 * 
 * @return void: Modifies the graph and the tensors member
 */
void graphConfig::buildLayers(long unsigned int& core, long unsigned int& layer_id, long unsigned int& tensor_id, long unsigned int& num_tensors, long unsigned int& dim, std::vector<long unsigned int>& mappings = {}) {

    std::string db_name;
    long unsigned int start_idx = tensor_id;
    long unsigned int max_idx = this->max_tensors_per_layer-1;

    for(long unsigned int i = 0; i < num_tensors; i++) {
        if (i >= max_idx) {
            break;
        }
        db_name = "Tensor " + std::to_string(start_idx+i) " in Layer " + std::to_string(layer_id) + " of Core " + std::to_string(core); 
        this->tensors[core][layer_id][start_idx+i] = this->vgraphs[core].addVariable(poplar::FLOAT, {dim, dim}, db_name);

        switch(this->map_tensors) {
            case MAPPING::LINEAR:
                poputil::mapTensorLinearly(this->vgraphs[core], this->tensors[core][layer_id][start_idx+i]);
                break;
            case MAPPING::INJECTIVE:
                this->vgraphs[core].setTileMapping(this->tensors[core][layer_id][start_idx+i], start_idx+i);
                break;
            case MAPPING::SPECIFIED:
                this->vgraphs[core].setTileMapping(this->tensors[core][layer_id][start_idx+i], mappings[i]);
                break;
            case MAPPING::ZERO:
                this->vgraphs[core].setTileMapping(this->tensors[core][layer_id][start_idx+i], 0);
                break;
            case MAPPING::SIM:
                this->vgraphs[core].setTileMapping(this->tensors[core][layer_id][start_idx+i], 0);
                break;
            default:
                poputil::mapTensorLinearly(this->vgraphs[core], this->tensors[core][layer_id][start_idx+i]);
                std::cout << "WARNING: DEFAULTED IN appendLayer()" << std::endl;
                break;
        }
    }
}

/**
 * @brief Adds tensors to a core (virtual graph)
 * 
 * You use this function add tensors to some layer in a core.
 * Capable of building more than 2D square tensors
 * 
 * @param core: Core we are adding tensors to
 * @param layer_id: Layer in the core we are adding a tensor to
 * @param tensor_id: Starting index to begin adding tensors
 * @param num_tensors: Number of tensors to add to the layer
 * @param dims: Dimensions of the tensors you are adding
 * @param mappings: Optional parameter for when you want to specify the tile mappings of the tensors
 * 
 * @note The difference between this and the other function is that you can build tensors that are NOT square
 * 
 * @return void: Modifies the graph and the tensors member
 */
void graphConfig::buildLayers(long unsigned int& core, long unsigned int& layer_id, long unsigned int& tensor_id, long unsigned int& num_tensors, std::vector<long unsigned int>& dims, std::vector<long unsigned int>& mappings = {}) {

    std::string db_name;
    long unsigned int start_idx = tensor_id;
    long unsigned int max_idx = this->max_tensors_per_layer-1;
    long unsigned int dim_counter = 0;

    for(long unsigned int i = 0; i < num_tensors; i++) {
        if (i >= max_idx) {
            break;
        }

        db_name = "Tensor " + std::to_string(start_idx+i) + " of Layer " + std::to_string(layer_id) + " in Core " + std::to_string(core);
        switch(dims.size() / num_tensors) {
            case 2:
                this->tensors[core][layer_id][start_idx+i] = this->vgraphs[core].addVariable(poplar::FLOAT, {dims[dim_counter], dim[dim_counter+1]}, db_name);
                dim_counter += 2;
                break;
            case 3:
                this->tensors[core][layer_id][start_idx+i] = this->vgraphs[core].addVariable(poplar::FLOAT, {dims[dim_counter], dims[dim_counter+1], dims[dim_counter+2]}, db_name);
                dim_counter += 3;
                break;
        }

        switch(this->map_tensors) {
            case MAPPING::LINEAR:
                poputil::mapTensorLinearly(this->vgraphs[core], this->tensors[core][layer_id][start_idx+i]);
                break;
            case MAPPING::INJECTIVE:
                this->vgraphs[core].setTileMapping(this->tensors[core][layer_id][start_idx+i], start_idx+i);
                break;
            case MAPPING::SPECIFIED:
                this->vgraphs[core].setTileMapping(this->tensors[core][layer_id][start_idx+i], mappings[i]);
                break;
            case MAPPING::ZERO:
                this->vgraphs[core].setTileMapping(this->tensors[core][layer_id][start_idx+i], 0);
                break;
            case MAPPING::SIM:
                this->vgraphs[core].setTileMapping(this->tensors[core][layer_id][start_idx+i], 0);
                break;
            default:
                poputil::mapTensorLinearly(this->vgraphs[core], this->tensors[core][layer_id][start_idx+i]);
                std::cout << "WARNING: DEFAULTED IN appendLayer()" << std::endl;
                break;
        }
    }
}

/**
 * @brief Adds vertices to a graph
 * 
 * You use this function add connections between tensors (vertices) to a graph.
 * 
 * 
 * @param core: Core we are adding tensors to
 * @param layer_id: Layer in the core we are adding a vertex to
 * @param vertex_idx: Starting index to add vertex
 * @param cps_id: The computeset in the core we are adding to
 * @param num_input: The number of inputs in the vertex
 * @param num_output: The number of outputs in the vertex
 * @param tile_id: What tile to store the vertex on
 * @param vertexName: The name of the vertex, used for debugging
 * 
 * @note All vertices must now use the standardized port names "in0", "in1", "out0", "out1", etc.
 * 
 * @return void: Modifies the graph and the vtx member
 */
void graphConfig::plotVertex(long unsigned int& core, long unsigned int& layer_id, long unsigned int& vertex_idx, long unsigned int& cps_id, long unsigned int& input, long unsigned int& output, long unsigned int& tile_id, std::string& vertexName) {
    
    std::string portName;
    
    this->vtx[core][layer_id][vertex_idx] = this->graph.addVertex(this->cps[core][cps_id], vertexName);
    
    switch(this->hardware) {
            case HARDWARE::IPU:
                this->vgraphs[core].setTileMapping(this->vtx[core][layer_id][vertex_idx], tile_id);
                break;
            default:
                this->vgraphs[core].setTileMapping(this->vtx[core][layer_id][vertex_idx], 0);
    }

    this->vgraphs.setPerfEstimate(this->vtx[core][layer_id][vertex_idx], 40, 40);

    for(int i = 0; i < input; i++) {
        portName = "in" + std::to_string(i);
        this->vgraphs[core].connect(this->vtx[core][layer_id][vertex_idx][portName], tensors[core][layer_id][i]);
    }

    for(int i = 0; i < output; i++) {
        portName = "out" + std::to_string(i);
        this->vgraphs[core].connect(this->vtx[core][layer_id][vertex_idx][portName], tensors[core][layer_id+1][i]);
    }

    return;
}

/**
 * @brief Connect tiles to the CPU
 * 
 * This function connects cores to the CPU. It can handle input and output.
 * Takes program 0 in a core and adds the necessary stream copy to either the beginning or end.
 * 
 * @param io: The direction of the connection (input or output)
 * @param core: Core we are connecting to
 * @param cpu: The vector (buffer) in the CPU we are connecting to
 * @param layer_id: Layer in the core we are adding a vertex to
 * @param tensor_idx: Tensor we are connecting to CPU
 * @param strean_id: The ID of the stream we are connecting to
 * @param prog_idx: The ID of the program we are adding to (typically 0)
 * 
 * @return void: Modifies the graph, the progs member, and the strms member
 */
void graphConfig::connectCPU(long unsigned int& io, long unsigned int& core, std::vector<long unsigned int>& cpu, long unsigned int& layer_id, long unsigned int& tensor_idx, long unsigned int& stream_id, long unsigned int& prog_idx = 0) {
    
    poplar::program::Sequence combined;
    poplar::program::Sequence original = this->progs[core][prog_idx];

    poplar::OptionFlags streamOpts {
      {"bufferingDepth", std::to_string(this->buf_depth)},
    };

    std::string tnsr_name;

    switch(io) {
        case IO::IN:
            tnsr_name = "Tensor " + std::to_string(tensor_idx) + " of Layer " + std::to_string(layer_id) + " in Core " + std::to_string(core);
            this->strms[IO::IN][stream_id] = this->vgraphs[core].addHostToDeviceFIFO(tnsr, poplar::FLOAT, dim*dim, poplar::ReplicatedStreamMode::REPLICATE, streamOpts);
            combined.add(poplar::program::Copy(strms[IO::IN][stream_id], tensors[core][layer_id][tensor_idx]));
            combined.add(original);
            progs[core][prog_idx] = combined;
            break;

        case IO::OUT:
            tnsr_name = "Tensor " + std::to_string(tensor_idx) + " of Layer " + std::to_string(layer_id) + " in Core " + std::to_string(core);
            this->strms[IO::OUT][stream_id] = this->vgraphs[core].addDeviceToHostFIFO(tnsr_name, poplar::FLOAT, , streamOpts);
            combined.add(original);
            combined.add(poplar::program::Copy(tensors[core][layer_id][tensor_idx], strms[IO::OUT][stream_id]));
            progs[core][prog_idx] = combined;
            break;
    }

    return;
}

/**
 * @brief Connect different cores in the IPU
 * 
 * This function connects two different cores in the IPU.
 * 
 * 
 * @param srcCore: The core that data is being transferred from
 * @param srcLayer_id: The layer the source data is in
 * @param srcIdx: The ID of the tensor with the source data
 * @param dstCore: The core that data is being transferred to
 * @param dstLayer_id: The layer that the data will be sent to
 * @param dstIdx: The ID of the tensor that will receive the data
 * @param progIdx: The program ID in the destination core to add the copy command to
 * 
 * @return void: Modifies the graph and the vtx member
 */
void graphConfig::connectCores(long unsigned int& srcCore, long unsigned srcLayer_id, long unsigned srcIdx, long unsigned int& dstCore, long unsigned int& dstLayer_id, long unsigned dstIdx, long unsigned progIdx = 0) {
    
    poplar::program::Sequence combined;
    poplar::program::Sequence original;

    original = this->progs[dstCore][progIdx];

    poplar::Tensor srcTensor = this->tensors[srcCore][srcLayer_id][srcIdx];
    poplar::Tensor dstTensor = this->tensors[dstCore][dstLayer_id][dstIdx];

    combined.add(poplar::program::Copy(srcTensor, dstTensor));
    combined.add(original);

    this->progs[dstCore][progIdx];
    
    return;
}

/**
 * @brief Adds standard operations to the graph program
 * 
 * This function adds standard operations like matrix add and multiply.
 * It's a good shortcut for adding necessary operations.
 * 
 * 
 * @param core: Which core the operation is being added to
 * @param layer: Which layer has the top tensors of the operation
 * @param top: The ID of the first tensor in the top layer
 * @param bottom: The ID of the first tensor in the bottom layer
 * @param op: The operation we are adding to the graph
 * @param progIdx: The ID of the program we are adding to (typically 0)
 * 
 * @return void: Modifies the graph and the progs member
 */
void graphConfig::addOp(long unsigned int& core, long unsigned int& layer, long unsigned int& top, long unsigned int& bottom, long unsigned int& op, long unsigned int& progIdx) {

    switch(op) {
        case OP::MULT:

            poplar::Graph chosen_core = this->vgraphs[core];
            poplar::Tensor in0 = this->tensors[core][layer][top];
            poplar::Tensor in1 = this->tensors[core][layer][top+1];
            poplar::Tensor out0 = this->tensors[core][layer+1][bottom];
            poplar::program::Sequence seq;

            poplar::Tensor matmul_out = poplin::matMul(chosen_core, in0, in1, seq, "MatMul")
            seq.add(poplar::program::Copy(matmul_out, out0));

            this->progs[core][0] = seq;
            break;
    }
}

/**
 * @brief Template function to construct the internal memory and connections in a core
 *  
 * @param core: Which core we are buidling the internals for
 * @param template: What template we are using
 * @param dim: Dimensions of the tensors we are adding
 * 
 * @return void: Modifies several members in graphConfig
 */
void graphConfig::buildInternalTemplate(long unsigned int& core, long unsigned int& templte, long unsigned int& dim) {

    switch(templte) {
        case TEMPLATE::V:
            this->buildLayers(core, 0, 2, dim);
            this->buildLayers(core, 1, 1, dim);
            
            this->addOp(core, 0, 0, 0, this->op);
            break;
    }

    return;
}


/**
 * @brief Copies the tensor layout from one core to another
 *  
 * @param srcCore: Which core we are copying from
 * @param dstCore: Which core we are copying to
 * 
 * @return void: Modifies the tensors member
 */
void graphConfig::dupeCore(long unsigned int& srcCore, long unsigned int& dstCore) {
    this->tensors[dstCore] = this->tensors[srcCore];

    return;
}

/**
 * @brief Template function to construct several different cores and connect them
 *  
 * @param template: The template to implement in the graph.
 * 
 * @return void: Modifies several members in graphConfig
 */
void graphConfig::buildExternalTemplate(long unsigned int& templte) {

    switch(templte) {
        case TEMPLATE::V:
            this->dupeCore(0, 1);
            this->dupeCore(0, 2);

            this->addOp(1, 0, 0, 0, this->op);
            this->addOp(2, 0, 0, 0, this->op);

            this->connectCPU(IO::IN, 0, cpu, 0, 0, 0, 0);
            this->connectCPU(IO::IN, 0, cpu, 0, 1, 1, 0);

            this->connectCPU(IO::IN, 1, cpu, 0, 0, 2, 0);
            this->connectCPU(IO::IN, 1, cpu, 0, 1, 3, 0);

            this->connectCPU(IO::OUT, 2, cpu, this->max_layer[2], 0, 0, 0);

            this->connectCores(0, this->max_layer[0], 0, 2, 0, 0, 0);
            this->connectCores(1, this->max_layer[1], 0, 2, 0, 1, 0);

            for (int i = 0; i < num_cores; i++) {
                poplar::program::Program temp = this->progs[i][0];
                poplar::program::Program repeated = Repeat(this->num_transfers, temp);
                this->progs[i][0] = repeated;
            }

            break;
    }
}
