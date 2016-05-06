#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <enigma/huffman/huffman.h>

int main(int argc, char **argv) {
    Codecs::HuffmanCodec codec;
    std::string fname = argv[1];
    std::vector<std::string> v;
    std::vector<std::string> encoded;
    std::vector<std::experimental::string_view> vv;
    clock_t begin, end;
    double elapsed_secs;
    begin = clock();

    std::ifstream ifs(fname);
    // std::string content;

    std::string line;

    while (getline(ifs, line)) {
        v.push_back(line);
    }

    encoded.resize(v.size());

    for (size_t i = 0; i < v.size(); i++) {
        vv.push_back(std::experimental::string_view(v[i]));
    }

    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "reading done in " << elapsed_secs << " secs" << std::endl;
    int file_size = 0, compressed_size = 0;
    elapsed_secs = 0;

    begin = clock();

    codec.learn(v);

    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "learning done in " << elapsed_secs << " secs" << std::endl;

    begin = clock();
    for (size_t i = 0; i < encoded.size(); i++) {
        codec.encode(vv[i], encoded[i]);
        compressed_size += encoded[i].size();
        file_size += vv[i].size();
    }
    end = clock();

    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "compression done in " << elapsed_secs << " secs" << std::endl;
    std::cout << "uncompressed memory: " << file_size << " Bytes (" << file_size / 1024 / 1024 <<  " MBytes)" << std::endl;
    std::cout << "compressed memory: " << compressed_size << " Bytes (" << compressed_size / 1024 / 1024 << " MBytes)" << std::endl;
    std::cout << "saved memory: " << (file_size - compressed_size) << " Bytes (" << (file_size - compressed_size) / 1024 / 1024 << " MBytes)" << std::endl;
    std::cout << "compression ratio ~ " << std::fixed << std::setprecision(3) << 100 - 100.0 * compressed_size / file_size << "%" << std::endl;

    std::ofstream fout(fname + "_unpacked");
    begin = clock();
    std::string decoded;
    for (std::string& str : encoded) {
        decoded.clear();
        codec.decode(str, decoded);
        fout << decoded << std::endl;
    }
    end = clock();

    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "decompression done in " << elapsed_secs << " secs" << std::endl;
    return 0;
}
