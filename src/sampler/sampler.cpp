#include <cassert>
#include <sampler.h>
#include <util.h>

namespace lumen {
static const nex::vector y_axis(0.0001f, 1.0f, 0.0001f);

nex::vector sample_hemisphere(const sample& sample, const nex::vector& normal)
{
        float cos_phi = std::cos(nex::TWO_PI * sample.x);
        float sin_phi = std::sin(nex::TWO_PI * sample.x);

        float cos_theta = std::sqrt(sample.y);
        float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);

        nex::vector x_axis(nex::cross(normal, y_axis));
        nex::vector z_axis(nex::cross(normal, x_axis));

        nex::normalize(x_axis);
        nex::normalize(z_axis);

        return (cos_phi * sin_theta) * x_axis +
                cos_theta * normal +
                (sin_phi * sin_theta) * z_axis;
}

nex::vector sample_phong_lobe(const sample& sample, const nex::vector& normal, float exponent)
{
        float cos_theta = std::pow(sample.x, 1.0f / (exponent + 1.0f));
        float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);

        float cos_phi = std::cos(nex::TWO_PI * sample.y);
        float sin_phi = std::sin(nex::TWO_PI * sample.y);

        nex::vector x_axis(nex::cross(normal, y_axis));
        nex::vector z_axis(nex::cross(normal, x_axis));

        nex::normalize(x_axis);
        nex::normalize(z_axis);

        return (cos_phi * sin_theta) * x_axis +
                cos_theta * normal +
                (sin_phi * sin_theta) * z_axis;
}

nex::point sample_disk(const sample& sample)
{
        float r = std::sqrt(sample.x);
        float theta = nex::TWO_PI * sample.y;

        return nex::point(r * std::cos(theta), r * std::sin(theta), 0.0f);
}

size_t sampler::request_samples(size_t n)
{
        sample_set set(n);
        samples.push_back(set);

        return samples.size() - 1;
}

const sample_set& sampler::get_samples(size_t set) const
{
        assert(set < samples.size());

        return samples[set];
}
}
