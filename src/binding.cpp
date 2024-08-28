#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <emscripten.h>

#include <Eigen/Eigen>
#include <smplx/smplx.hpp>
#include <string>

#include "smplx/defs.hpp"

using namespace emscripten;
using namespace smplx;
using namespace std;

EMSCRIPTEN_DECLARE_VAL_TYPE(Float32Memory)
EMSCRIPTEN_DECLARE_VAL_TYPE(Int32Memory)

template <typename ModelConfig>
void bindModel(const char *modelName, const char *bodyName)
{
    using M = Model<ModelConfig>;
    using B = Body<ModelConfig>;
    // clang-format off
    class_<M>(modelName)
        .constructor<std::string, std::string, Gender>()
        .function("n_uv_verts", &M::n_uv_verts)
        .template property<Int32Memory>("faces",
            [](const M &self) {
                return val(typed_memory_view<unsigned int>(self.faces.size(), self.faces.data()));
            })
        .template property<Float32Memory>("uv",
            [](const M &self) {
                return val(typed_memory_view<float>(self.uv.size(), self.uv.data()));
            })
    ;

    class_<B>(bodyName)
        .constructor<const M &, bool>()
        .function("saveObj", &B::save_obj)
        .function("setRandom", &B::set_random)
        .template property<Float32Memory>("pose",
            [](const B &self) {
                return val(typed_memory_view<float>(self.pose().size(), self.pose().data()));
            })
        .template property<Float32Memory>("shape",
            [](const B &self) {
                return val(typed_memory_view<float>(self.shape().size(), self.shape().data()));
            })
        .template property<Float32Memory>("joints",
            [](const B &self) {
                return val(typed_memory_view<float>(self.joints().size(), self.joints().data()));
            })
        .template property<Float32Memory>("verts",
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
    emscripten::register_type<Float32Memory>("Float32Array");
    emscripten::register_type<Int32Memory>("Int32Array");

    enum_<Gender>("Gender")
            .value("unknown", Gender::unknown)
            .value("neutral", Gender::neutral)
            .value("male", Gender::male)
            .value("female", Gender::female);
    bindModel<model_config::SMPL_v1>("Model", "Body");
    bindModel<model_config::SMPL>("Model_shape300", "Body_shape300");
    bindModel<model_config::SMPLH>("ModelH", "BodyH");
    bindModel<model_config::SMPLX>("ModelX", "BodyX");
    bindModel<model_config::SMPLXpca>("ModelXpca", "BodyXpca");
}
