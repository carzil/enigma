#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <enigma/codecs/enigma.h>
#include <enigma/huffman/huffman.h>
#include <enigma/common/dictionary.h>
#include <enigma/colors.h>

using std::experimental::string_view;
using std::vector;
using std::string;

int main(int argc, char **argv) {
    Codecs::EnigmaCodec codec;
    std::string fname = argv[1];
    std::vector<std::string> v;
    std::vector<std::string> encoded;
    clock_t begin, end;
    double elapsed_secs, elapsed_secs_d = 0;

    std::ifstream ifs(fname);
    // std::string content;

    std::string line;

    while (getline(ifs, line)) {
        v.push_back(line);
    }

    begin = clock();

    codec.Learn(v);
    codec.EndLearning();

    end = clock();

    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "learning done in " << GREENB(elapsed_secs << " secs") << std::endl;

    size_t file_size = 0, compressed_size = 0;

    encoded.resize(v.size());
    for (int i = encoded.size() - 1; i >= 0; i--) {
        std::string enc;
        enc.reserve(v[i].size() * 3);
        file_size += v[i].size();
        begin = clock();
        size_t sz = codec.encode(v[i], &enc[0]);
        end = clock();
        elapsed_secs += double(end - begin) / CLOCKS_PER_SEC;
        std::string dec;
        dec.resize(v[i].size() * 2);

        // begin = clock();
        // size_t decsz = codec.decode(string_view(&enc[0], sz), &dec[0]);
        // end = clock();
        // elapsed_secs_d += double(end - begin) / CLOCKS_PER_SEC;

        // dec.resize(decsz);
        // if (dec != v[i]) {
        //     std::cout << "FUCK" << std::endl;
        //     std::cout << dec.size() << " " << dec << std::endl;
        //     return 1;
        // }
        v.resize(v.size() - 1);
        compressed_size += sz;
    }

    std::cout << "compression done in " << GREENB(elapsed_secs << " secs") << std::endl;
    std::cout << "decompression done in " << YELLOWB(elapsed_secs_d << " secs") << std::endl;
    std::cout << "uncompressed memory: " << GREENB(file_size << " Bytes (" << file_size / 1024 / 1024 <<  " MBytes)") << std::endl;
    std::cout << "compressed memory: " << GREENB(compressed_size << " Bytes (" << compressed_size / 1024 / 1024 << " MBytes)") << std::endl;
    std::cout << "saved memory: " << GREENB((file_size - compressed_size) << " Bytes (" << (file_size - compressed_size) / 1024 / 1024 << " MBytes)") << std::endl;
    std::cout << "compression ratio ~ " << std::fixed << std::setprecision(3) << YELLOWB(100 - 100.0 * compressed_size / file_size << "%") << std::endl;

    // std::ofstream fout(fname + "_unpacked");
    // begin = clock();
    // std::string decoded;
    // for (std::string& str : encoded) {
    //     decoded.clear();
    //     codec.decode(str, decoded);
    //     fout << decoded << std::endl;
    // }
    // end = clock();

    // elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    // std::cout << "decompression done in " << elapsed_secs << " secs" << std::endl;
    return 0;
}
