#include <iostream>
#include <vector>
#include <stdexcept>
#include <numeric>
#include <algorithm>

// Function to calculate modular inverse using brute-force search
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    throw std::runtime_error("Inverse element does not exist");
}

// Function to predict next LCG number given a sequence
int predictNextLCG(const std::vector<int>& sequence, int m = 32768) {
    if (sequence.size() != 4) {
        throw std::invalid_argument("Exactly 4 previous numbers are required");
    }

    int X0 = sequence[0];
    int Xn_1 = sequence[1];
    int Xn = sequence[2];
    int Xn_next = sequence[3];

    // Calculate differences between consecutive elements
    int delta1 = (Xn_1 - X0 + m) % m;
    int delta2 = (Xn - Xn_1 + m) % m;
    int delta3 = (Xn_next - Xn + m) % m;

    // Check if parameters can be calculated
    if (delta1 == 0 || delta2 == 0) {
        throw std::runtime_error("Cannot determine parameters (zero differences)");
    }

    // Calculate parameter a
    int a;
    try {
        int inv_delta1 = modInverse(delta1, m);
        a = (delta2 * inv_delta1) % m;
    } catch (const std::runtime_error& e) {
        throw std::runtime_error("Cannot calculate parameter a");
    }

    // Calculate parameter c
    int c = (Xn - a * Xn_1 + m * m) % m;

    // Verify parameter correctness
    if ((a * Xn + c) % m != Xn_next) {
        throw std::runtime_error("Calculated parameters don't match the sequence");
    }

    if ((a * Xn_1 +c) % m == Xn &&
        (a * Xn + c) % m == Xn_next)
        return (a * Xn_next + c) % m;

    else return 0;
}

int main() {
    try {
        std::vector<int> sequence(4);
        std::cout << "Enter 4 numbers of the sequence: ";
        
        // Input handling
        for (auto& num : sequence) {
            if (!(std::cin >> num)) throw std::runtime_error("Invalid input");
            if (num < 0) throw std::runtime_error("Numbers must be positive");
        }

        // Search for suitable modulus
        bool solution_found = false;
        for (int m = *std::max_element(sequence.begin(), sequence.end()) + 1; m <= 65535; ++m) {
            try {
                int next_num = predictNextLCG(sequence, m);
                if (next_num == 16605) {
                    std::cout << "Solution found: m = " << m 
                              << ", next number = " << next_num << std::endl;
                    solution_found = true;
                    break;
                }
            } catch (...) {
                continue;
            }
        }

        if (!solution_found) {
            std::cout << "No solution found for m ≤ 65535" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}