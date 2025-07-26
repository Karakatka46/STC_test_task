#ifndef ALG_KARPLUS_HPP
#define ALG_KARPLUS_HPP

#include <iostream>
#include <map>
#include <cmath>
#include <random>
#include <algorithm>
#include <numbers>
#include <fstream>
#include <numeric>
#include <span>

class Karplus_alg{
    private:
        double sampling_rate_;
        std::default_random_engine engine_;

    public:
        Karplus_alg(double sampling_rate) : sampling_rate_(sampling_rate), engine_(std::random_device{}()){}

        // generation note with frequensy and duration by user definition
        std::vector<int16_t> gen_note(double freq, double duration);

        // save to file
        void save_to_listen(std::span<const int16_t> samples, const std::string& filename);
};

std::string to_lower(const std::string& str);

#endif