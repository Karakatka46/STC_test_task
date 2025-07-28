#include "algKarplus.hpp"

std::vector<int16_t> Karplus_alg::gen_note(double freq, double duration){
    const size_t delay = static_cast<size_t>(sampling_rate_ / freq);
    std::vector<double> buff(delay, 0.0);

    std::uniform_real_distribution<double> dist (-1.0, 1.0);

    std::generate(buff.begin(), buff.end(), [&]() {
        return dist(engine_);
    });

    const size_t total_samples = static_cast<size_t>(sampling_rate_ * duration);
    std::vector<double> samples(total_samples);

    // Algorithm Karplus-Strong
    for (size_t i = 0; i < total_samples; ++i) {
        const size_t pos = i % delay;
        const double value = buff[pos];

        // LP filter
        const double next_val = (value + buff[(pos + 1) % delay]) * 0.5 * 0.99;

        buff[pos] = next_val;
        samples[i] = value;
    }

    // Normalization and converting to size16_t
    const double max_val = *std::max_element(samples.begin(), samples.end(), [](double a, double b) {
        return std::abs(a) < std::abs(b); 
    });
    
    const double scale = (std::numeric_limits<int16_t>::max() - 1) / std::abs(max_val);

    std::vector<int16_t> result(samples.size());
    std::transform(samples.begin(), samples.end(), result.begin(), [scale](double x) {
        return static_cast<int16_t>(x*scale);
    });

    return result;
}

void Karplus_alg::save_to_listen(std::span<const int16_t> samples, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
        
    if (!file) {
        throw std::runtime_error("Could not open file for writing");
    }

    // WAV header structure
    struct WavHeader {
        // RIFF chunk
        char riff[4] = {'R', 'I', 'F', 'F'};
        uint32_t overall_size;
        char wave[4] = {'W', 'A', 'V', 'E'};
        
        // fmt subchunk
        char fmt[4] = {'f', 'm', 't', ' '};
        uint32_t fmt_size = 16;
        uint16_t audio_format = 1;  // PCM
        uint16_t num_channels = 1;
        uint32_t sample_rate;
        uint32_t byte_rate;
        uint16_t block_align;
        uint16_t bits_per_sample = 16;
        
        // data subchunk
        char data[4] = {'d', 'a', 't', 'a'};
        uint32_t data_size;
    };

    WavHeader header;
    header.overall_size = 36 + samples.size() * sizeof(int16_t);
    header.sample_rate = sampling_rate_;
    header.byte_rate = sampling_rate_ * header.num_channels * sizeof(int16_t);
    header.block_align = header.num_channels * sizeof(int16_t);
    header.data_size = samples.size() * sizeof(int16_t);

    // Write header
    file.write(reinterpret_cast<const char*>(&header), sizeof(WavHeader));
    
    // Write audio data
    file.write(reinterpret_cast<const char*>(samples.data()), samples.size() * sizeof(int16_t));

    std::cout << "Sound saved as " << filename << std::endl;
}

std::string to_lower(const std::string& str) {
    std::string lower;
    for (char c : str) {
        lower += tolower(c);
    }
    return lower;
}