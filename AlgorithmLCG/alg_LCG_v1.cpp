#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <stdexcept>

// Function to find modular inverse
int64_t modInverse(int64_t a, int64_t m) {
    a = a % m;
    for (int64_t x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    throw std::runtime_error("Inverse element does not exist");
}

// Function to solve for LCG parameters
void solveLCG(const std::vector<int64_t>& seq) {
    const int64_t max_m = 65535;  // Maximum allowed modulus
    int64_t max_val = *std::max_element(seq.begin(), seq.end());
    
    // Try all possible m values
    for (int64_t m = max_val + 1; m <= max_m; ++m) {
        try {
            // X1 = (a*X0 + c) mod m
            // X2 = (a*X1 + c) mod m
            
            int64_t X0 = seq[0];
            int64_t X1 = seq[1];
            int64_t X2 = seq[2]; 
            int64_t X3 = seq[3];
            
            // Calculate differences between consecutive elements
            int64_t dX0 = (X1 - X0 + m) % m;  
            int64_t dX1 = (X2 - X1 + m) % m;
            
            if (dX0 == 0) continue; 
            
            // Calculate parameter a:
            // a = (X2 - X1) * (X1 - X0)^(-1) mod m
            int64_t inv_dX0 = modInverse(dX0, m);
            int64_t a = (dX1 * inv_dX0) % m;
            
            // Calculate parameter c:
            // c = (X1 - a*X0) mod m
            int64_t c = (X1 - a * X0 % m + m) % m;
            
            // Verify parameters against full sequence
            if ((a * X1 + c) % m == X2 &&
                (a * X2 + c) % m == X3) {
                
                // Calculate next number in sequence
                int64_t next = (a * X3 + c) % m;
                std::cout << "Valid solution found:\n";
                std::cout << "m = " << m << "\n";
                std::cout << "a = " << a << "\n";
                std::cout << "c = " << c << "\n";
                std::cout << "Next number = " << next << "\n";
                return;
            }
        } catch (...) {
            continue;
        }
    }
    std::cout << "No valid solution found for m ≤ " << max_m << std::endl;
}

int main() {
    try {
        std::vector<int64_t> seq(4);
        std::cout << "Enter 4 numbers of the sequence: \n";
        
        // Read input sequence
        for (auto& num : seq) {
            if (!(std::cin >> num)) throw std::runtime_error("Invalid input");
            if (num < 0) throw std::runtime_error("Numbers must be positive");
        }
        
        solveLCG(seq);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}