#pragma once

#include <array>

namespace dorm_energy::detection
{

    struct OnnxModelConfig
    {
        float threshold{1.26432f};

        std::array<float, 5> mean{0.41319445f, 2.4755468f, 253.50294f, 0.0f, 0.0f};

        std::array<float, 5> scale{0.49240714f, 2.5410394f, 291.89594f, 0.70710677f, 0.70710677f};
    };

} // namespace dorm_energy::detection