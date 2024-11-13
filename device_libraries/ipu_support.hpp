#pragma once

#include <boost/program_options.hpp>

#include <vector>
#include <algorithm>

#include <poplar/DeviceManager.hpp>
#include <poplar/Engine.hpp>
#include <poplar/Graph.hpp>
#include <poplar/IPUModel.hpp>
#include <poplar/Tensor.hpp>
#include <poplar/ReplicatedStreamMode.hpp>

#include <poputil/TileMapping.hpp>

enum HARDWARE {IPU, MODEL, CPU};
enum MAPPING {LINEAR, SET};
enum LAYERS {INPUT, CONSUMPTION, OUTPUT};
enum IO {IN, OUT};
enum STAGE {IN, COMP, OUT, STAGES};
enum COMPATSHAPE {TRIANGLEUP, TRIANGLEQR, TRIANGLEDOWN, SQUARE, LINE, CUSTOM};
enum TEMPLATE{V}
enum CODELETS {POPLIN, POPOPS, POPRAND};

struct layer {
    std::vector<poplar::Tensor> tensors;
};

struct model {
    std::vector<layer> layers;
};

class graphConfig {
    public:
        poplar::Graph graph;
        std::vector<poplar::Graph> IN_vgraphs(consumption_cores);
        std::vector<poplar::Graph> WORK_vgraphs(consumption_cores);
        std::vector<poplar::Graph> OUT_vgraphs(consumption_cores);
        poplar::Device device;
        model originalModel;
        std::vector<model> myModels;

        long unsigned int dim = 0;
        long unsigned int streams = 0;
        long unsigned int compat = 0;
        long unsigned int hw = 0;
        long unsigned int map = 0;
        long unsigned int buf_depth = 0;
        long unsigned int offset = 0;
        long unsigned int num_layers = 0;
        
        std::vector< long unsigned int > layer_lens;
        std::vector< poplar::program::Program > progs;

        std::vector< std::vector<poplar::ComputeSet> > cps;
        std::vector< std::vector< std::vector<poplar::VertexRef>  > > vtx;
        std::vector< std::vector< std::vector<poplar::DataStream> > > strm;

    public:

        graphConfig(boost::program_options::variables_map& vm);
        void getDevice();
        void buildLayer(int layer_id, int num_tensors);
        void addComputeSet(int io);
        void addStream(int num_port, int io);
        void addVertex();
        void connectVertex(poplar::Graph& graph, std::vector<poplar::VertexRef>& vtx, std::vector<model>& myModels, int num_streams, int top_layer, int bottom_layer, int top_tensor, int bottom_tensor, std::string in, std::string out);
        void connectEngineStream(poplar::Graph& graph, graphConfig& myGraphConfig, int num_port, int IO);
        void buildTensorTemplate(poplar::Graph& graph, graphConfig& myGraphConfig, int mode, int hw);
        void buildIOTemplate(poplar::Graph& graph, std::vector<model>& myModels, comPattern& comPat, std::pair<int,int> params, int num_streams, int mode);

}

//void buildLayer(poplar::Graph& graph, model& myModel, std::pair<int,int> params, int layer_id, int map, int num_tensors);
//void buildLayer(graphConfig& myGraphConfig, int layer_id, int num_tensors);
//void appendLayer(graphConfig& myGraphConfig, int layer_id, int num_tensors);

//void addComputeSet(poplar::Graph& graph, std::vector<poplar::ComputeSet>& cps, int num_streams, int IO);
//void addComputeSet(graphConfig& myGraphConfig, int io);

//void addStream(poplar::Graph& graph, std::vector<poplar::DataStream>& strm, std::pair<int,int> params, int buf_depth, int num_port, int num_streams, int io);
//void addStream(graphConfig& myGraphConfig, int num_port, int io);

//void addVertex(poplar::Graph& graph, std::vector<poplar::ComputeSet>& cps, std::vector<poplar::VertexRef>& vtx, int num_streams, int offset, int hw);
//void addVertex(graphConfig& myGraphConfig);

//void connectVertex(poplar::Graph& graph, std::vector<poplar::VertexRef>& vtx, std::vector<model>& myModels, int num_streams, int top_layer, int bottom_layer, int top_tensor, int bottom_tensor, std::string in, std::string out);
//void connectVertex(poplar::Graph& graph, std::vector<poplar::VertexRef>& vtx, std::vector<model>& myModels, int num_streams, int top_layer, int bottom_layer, int top_tensor, int bottom_tensor, std::string in, std::string out);

//void connectEngineStream(poplar::Graph& graph, std::vector<float>& cpu, int num_streams, int num_port, int IO);
//void connectEngineStream(poplar::Graph& graph, graphConfig& myGraphConfig, int num_port, int IO);

//void buildTensorTemplate(poplar::Graph& graph, std::vector<model>& myModels, std::pair<int,int> params, int num_streams, int mode, int hw);
//void buildTensorTemplate(poplar::Graph& graph, graphConfig& myGraphConfig, int mode, int hw);

//void buildIOTemplate(poplar::Graph& graph, std::vector<model>& myModels, comPattern& comPat, std::pair<int,int> params, int num_streams, int mode);
//void buildIOTemplate(poplar::Graph& graph, std::vector<model>& myModels, comPattern& comPat, std::pair<int,int> params, int num_streams, int mode);
