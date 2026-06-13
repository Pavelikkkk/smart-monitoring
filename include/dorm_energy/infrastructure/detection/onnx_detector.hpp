#pragma once

#include "dorm_energy/application/config/onnx_model_config.hpp"
#include "dorm_energy/domain/detection/istate_detector.hpp"

#include <memory>
#include <string>

#include <onnxruntime_cxx_api.h>

namespace dorm_energy::detection
{

    class OnnxDetector final : public IStateDetector
    {
    public:
        explicit OnnxDetector(const std::string &modelPath);

        bool isAnomaly(const DetectionContext &context) const override;

        AnomalyInfo detect(const DetectionContext &context) const override;

    private:
        float calculateError(const DetectionContext &context) const;

    private:
        OnnxModelConfig config_;

        Ort::Env env_;

        Ort::SessionOptions options_;

        std::unique_ptr<Ort::Session> session_;
    };

} // namespace dorm_energy::detection