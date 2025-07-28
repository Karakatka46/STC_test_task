#include "algKarplus.hpp"

int main(){
    std::string note, ans, filename;
    std::vector<int16_t> sound;
    std::vector<int16_t> combined;

    std::map<std::string, double> note_frequencies = {
        {"a4", 440.00},   
        {"e4", 329.63},  
        {"b4", 493.88},
        {"f#", 369.99},
        {"f#4", 369.99},
        {"c#", 277.18},
        {"c#4", 277.18}
    };

    Karplus_alg synth(44100.0);

    std::cout << "Enter a note (A4, E4, B4): \n";
    std::getline(std::cin, note);
    note = to_lower(note);

    // Проверяем, есть ли нота в нашем словаре
    if (note_frequencies.find(note) != note_frequencies.end()) {
        double frequency = note_frequencies[note];
        sound = synth.gen_note(frequency, 2.0);

        filename = "note_" + note + ".wav";

        combined.insert(combined.end(), sound.begin(), sound.end());
        synth.save_to_listen(combined, filename);

        std::cout << "Generating " << note << " (" << frequency << " Hz)\n";
    } 
    else {
        std::cerr << "Invalid note!\n";
    }

    std::cout << "Do you want to listen an accord? (Yes or No)\n";
    std::getline(std::cin, ans);

    if (ans == "Yes") {
        
        auto a4 = synth.gen_note(440.0, 2.0);
        auto e4 = synth.gen_note(329.63, 2.0);
        auto b4 = synth.gen_note(493.88, 2.0);

        size_t max_size = std::max({a4.size(), e4.size(), b4.size()});
        combined.resize(max_size, 0);

        for (size_t i = 0; i < max_size; ++i) {
            int32_t mixed = 0;
            if (i < a4.size()) mixed += a4[i] / 3;
            if (i < e4.size()) mixed += e4[i] / 3;
            if (i < b4.size()) mixed += b4[i] / 3;
            
            mixed = std::max(-32768, std::min(32767, mixed));
            combined[i] = static_cast<int16_t>(mixed);
        }
        filename = "chord_a4_e4_b4.wav";
        
        std::cout << "Generating A4 + E4 + B4 chord\n";
        
        synth.save_to_listen(combined, filename);        
    }
    else return 0;

    return 0;
}