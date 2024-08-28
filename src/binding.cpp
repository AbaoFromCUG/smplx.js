#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <emscripten.h>

#include <Eigen/Eigen>
#include <smplx/smplx.hpp>
#include <string>

#include "DenseMatrix.h"
#include "smplx/defs.hpp"

using namespace emscripten;
using namespace smplx;
using namespace std;

using DDM = DenseMatrix<float>;

template <typename ModelConfig>
void bindModel(const char *modelName, const char *bodyName)
{
    using M = Model<ModelConfig>;
    using B = Body<ModelConfig>;
    // clang-format off
    class_<M>(modelName)
        .constructor<std::string, std::string, Gender>()
        .function("n_uv_verts", &M::n_uv_verts)
        .template property<val>("faces",
            [](const M &self) {
                return val(typed_memory_view<unsigned int>(self.faces.size(), self.faces.data()));
            })
        .template property<val>("uv",
            [](const M &self) {
                return val(typed_memory_view<float>(self.uv.size(), self.uv.data()));
            })
    ;

    class_<B>(bodyName)
        .constructor<const M &, bool>()
        .function("saveObj", &B::save_obj)
        .function("setRandom", &B::set_random)
        .template property<DDM>("pose",
            [](const B &self) { return DDM(self.pose()); },
            [](B &self, const DDM &matrix) { self.pose() = matrix.data; })
        .template property<DDM>("shape",
            [](const B &self) { return DDM(self.shape()); },
            [](B &self, const DDM &matrix) { self.shape() = matrix.data; })
        .template property<val>("verts",
            [](const B &self) {
                return val(typed_memory_view<float>(self.verts().size(), self.verts().data()));
            })
        .function("update", &B::update)
    ;
    // clang-format on
}

EMSCRIPTEN_BINDINGS(smplx_wasm)
{

    emscripten::register_vector<float>("Vector");
    emscripten::register_vector<std::vector<float>>("Vector2d");
    class_<DDM>("Matrix")
            .constructor<int, int>()
            .constructor<const DDM &>()
            .class_function("identity", &DDM::identity)
            .class_function("ones", &DDM::ones)
            .class_function("constant", &DDM::constant)
            .class_function("random", &DDM::random)
            .class_function("diagonal", &DDM::diagonal)
            .class_function("fromVector", &DDM::fromVector)
            .function("transpose", &DDM::transpose)
            .function("transposeSelf", &DDM::transposeSelf)
            .function("inverse", &DDM::inverse)
            .function("rows", &DDM::rows)
            .function("cols", &DDM::cols)
            .function("norm", &DDM::norm)
            .function("normSqr", &DDM::normSqr)
            .function("l1Norm", &DDM::l1Norm)
            .function("lInfNorm", &DDM::lInfNorm)
            .function("rank", &DDM::rank)
            .function("det", &DDM::det)
            .function("sum", &DDM::sum)
            .function("block", &DDM::block)
            .function("setBlock", &DDM::setBlock)
            .function("mul", &DDM::mul)
            .function("mulSelf", &DDM::mulSelf)
            .function("div", &DDM::div)
            .function("divSelf", &DDM::divSelf)
            .function("matAdd", &DDM::matAdd, allow_raw_pointers())
            .function("matAddSelf", &DDM::matAddSelf, allow_raw_pointers())
            .function("matSub", &DDM::matSub, allow_raw_pointers())
            .function("matSubSelf", &DDM::matSubSelf, allow_raw_pointers())
            .function("matMul", &DDM::matMul, allow_raw_pointers())
            .function("matMulSelf", &DDM::matMulSelf, allow_raw_pointers())
            .function("get", &DDM::get)
            .function("set", &DDM::set)
            .function("hcat", &DDM::hcat, allow_raw_pointers())
            .function("vcat", &DDM::vcat, allow_raw_pointers())
            .function("print", &DDM::print)
            .function("clamp", &DDM::clamp)
            .function("clampSelf", &DDM::clampSelf)
            // Vector ops
            .function("length", &DDM::length)
            .function("vGet", &DDM::vGet)
            .function("vSet", &DDM::vSet)
            .function("dot", &DDM::dot);

    enum_<Gender>("Gender")
            .value("unknown", Gender::unknown)
            .value("neutral", Gender::neutral)
            .value("male", Gender::male)
            .value("female", Gender::female);
    bindModel<model_config::SMPL_v1>("Model", "Body");
    bindModel<model_config::SMPLH>("ModelH", "BodyH");
    bindModel<model_config::SMPLX>("ModelX", "BodyX");
    bindModel<model_config::SMPLXpca>("ModelXpca", "BodyXpca");
}
