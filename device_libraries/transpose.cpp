#include <poplar/Vertex.hpp>

using namespace poplar;

class transposeVertex : public Vertex {
public:

Vector<Input<Vector<float>>> strm_in;
Vector<Output<Vector<float>>> strm_out;

// Compute function
    bool compute() {
        for (unsigned i = 0; i < strm_out.size(); i++) {
            for (unsigned j = 0; j < strm_out.size(); j++) {
                strm_out[j][i] = strm_in[i][j];
            }
        }
        return true;
    }
};