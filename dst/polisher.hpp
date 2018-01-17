/*!
 * @file polisher.hpp
 *
 * @brief Polisher class header file
 */

#pragma once

#include <stdlib.h>
#include <vector>
#include <memory>

namespace bioparser {
    template<class T>
    class Parser;
}

namespace thread_pool {
    class ThreadPool;
}

namespace spoa {
    class AlignmentEngine;
}

namespace racon {

class Sequence;
class Overlap;
class Window;

enum class PolisherType {
    kC, // Contig polishing
    kF // Fragment error correction
};

class Polisher;
std::unique_ptr<Polisher> createPolisher(const std::string& sequences_path,
    const std::string& overlaps_path, const std::string& target_path,
    PolisherType type, uint32_t window_length, double quality_threshold,
    double error_threshold, int8_t match, int8_t mismatch, int8_t gap,
    uint32_t num_threads);

class Polisher {
public:
    ~Polisher();

    void initialize();

    void polish();

    friend std::unique_ptr<Polisher> createPolisher(const std::string& sequences_path,
        const std::string& overlaps_path, const std::string& target_path,
        PolisherType type, uint32_t window_length, double quality_threshold,
        double error_threshold, int8_t match, int8_t mismatch, int8_t gap,
        uint32_t num_threads);
private:
    Polisher(std::unique_ptr<bioparser::Parser<Sequence>> sparser,
        std::unique_ptr<bioparser::Parser<Overlap>> oparser,
        std::unique_ptr<bioparser::Parser<Sequence>> tparser,
        PolisherType type, uint32_t window_length, double quality_threshold,
        double error_threshold, int8_t match, int8_t mismatch, int8_t gap,
        uint32_t num_threads);
    Polisher(const Polisher&) = delete;
    const Polisher& operator=(const Polisher&) = delete;

    std::unique_ptr<bioparser::Parser<Sequence>> sparser_;
    std::unique_ptr<bioparser::Parser<Overlap>> oparser_;
    std::unique_ptr<bioparser::Parser<Sequence>> tparser_;

    PolisherType type_;
    double quality_threshold_;
    double error_threshold_;
    std::vector<std::shared_ptr<spoa::AlignmentEngine>> alignment_engines_;

    uint32_t window_length_;
    std::vector<std::unique_ptr<Window>> windows_;

    std::unique_ptr<thread_pool::ThreadPool> thread_pool;
};

}