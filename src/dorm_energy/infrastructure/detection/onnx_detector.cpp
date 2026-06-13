#include "dorm_energy/infrastructure/detection/onnx_detector.hpp"

#include <cmath>
#include <array>
#include <chrono>
#include <iostream>

namespace dorm_energy::detection
{

    namespace
    {
        constexpr std::array<float, 5> MEAN = {
            0.41319444f,
            2.4755468f,
            253.50294f,
            0.0f,
            0.0f};

        constexpr std::array<float, 5> SCALE = {
            0.49240714f,
            2.5410394f,
            291.89593f,
            0.70710678f,
            0.70710678f};

        constexpr float THRESHOLD = 0.15f; // потом считать из threshold.txt
    }

    OnnxDetector::OnnxDetector(
        const std::string &modelPath) : env_(ORT_LOGGING_LEVEL_WARNING, "onnx")
    {
        options_.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

#ifdef _WIN32

        std::wstring widePath(modelPath.begin(), modelPath.end());

        session_ = std::make_unique<Ort::Session>(env_, widePath.c_str(), options_);

#else

        session_ = std::make_unique<Ort::Session>(env_, modelPath.c_str(), options_);

#endif
    }

    bool OnnxDetector::isAnomaly(
        const DetectionContext &context) const
    {
        return detect(context).isAnomaly;
    }

    AnomalyInfo OnnxDetector::detect(
        const DetectionContext &context) const
    {
        AnomalyInfo info;

        float error = calculateError(context);

        info.score = error;

        if (error > THRESHOLD)
        {
            info.isAnomaly = true;

            info.anomalyType = "ml_anomaly";

            info.description = "Autoencoder reconstruction error: " + std::to_string(error);

            info.severity = core::AlertSeverity::Warning;
        }

        return info;
    }

    float OnnxDetector::calculateError(
        const DetectionContext &context) const
    {
        const auto &state = context.current;

        auto time = std::chrono::system_clock::to_time_t(state.timestamp);

        std::tm localTm{};

#ifdef _WIN32
        localtime_s(&localTm, &time);
#else
        localtime_r(&time, &localTm);
#endif

        float hour = static_cast<float>(localTm.tm_hour);

        float hourSin = std::sin(2.0f * 3.14159265f * hour / 24.0f);

        float hourCos = std::cos(2.0f * 3.14159265f * hour / 24.0f);

        std::array<float, 5> inputValues = {
            state.motion ? 1.0f : 0.0f,
            static_cast<float>(state.power),
            static_cast<float>(state.light),
            hourSin,
            hourCos};

        for (std::size_t i = 0; i < inputValues.size(); ++i)
        {
            inputValues[i] = (inputValues[i] - MEAN[i]) / SCALE[i];
        }

        std::array<int64_t, 2> inputShape{1, 5};

        Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

        Ort::Value inputTensor = Ort::Value::CreateTensor<float>(memoryInfo,
                                                                 inputValues.data(),
                                                                 inputValues.size(),
                                                                 inputShape.data(),
                                                                 inputShape.size());

        const char *inputNames[] = {"input"};

        const char *outputNames[] = {"output"};

        auto outputs = session_->Run(Ort::RunOptions{nullptr},
                                     inputNames,
                                     &inputTensor,
                                     1,
                                     outputNames,
                                     1);

        float *output = outputs[0].GetTensorMutableData<float>();

        float mse = 0.0f;

        for (std::size_t i = 0; i < inputValues.size(); ++i)
        {
            float diff = inputValues[i] - output[i];
            mse += diff * diff;
        }

        mse /= static_cast<float>(inputValues.size());

        return mse;
    }

} // namespace dorm_energy::detection