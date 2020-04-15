#pragma once
#include <vector>

namespace CGALMethods{
namespace utils{

    namespace {
        using namespace std;
    }

    template <class dtype>
    void extend_vector(vector<dtype> & main_vector, vector<dtype> & vector_to_insert){
        for (dtype item: vector_to_insert){
            main_vector.push_back(item);
        }
    }

}
}