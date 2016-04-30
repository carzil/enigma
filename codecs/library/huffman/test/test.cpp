#include <iostream>
#include <fstream>
#include <vector>
#include <library/huffman/huffman.h>
#include <library/tests_common/tests_common.h>
#include <library/common/bit/bitstring.h>
#include <library/common/bit/bitstream.h>

TEST(TrivialCodecTest, Works) {
    // printf("\n");
    std::ifstream fin("hello.txt");
    char* buffer = new char[4096];
    while (!fin.eof()) {
        fin.read(buffer, 4096);
    }
    // Codecs::TestSimple(codec);
};

int main(int argc, char **argv) {
    // ::testing::InitGoogleTest(&argc, argv);
    // return RUN_ALL_TESTS();
    // srand(time(NULL));
    // // int n = 1;
    // // // little endian if true
    // // if(*(char *)&n == 1) {
    // //     std::cout << "LE" << std::endl;
    // // } else {
    // //     std::cout << "BE" << std::endl;
    // // }
    // int n = 100;
    // std::vector<int> bits;
    // bits.resize(n);
    // for (int i = 0; i < n; i++) {
    //     bits[i] = rand() % 2;
    // }
    // std::ofstream ofs("bitfile.txt", std::ios::out | std::ios::binary);
    // Codecs::obitstream bs(ofs);
    // for (int i = 0; i < n; i += 4) {
    //     char t = 0;
    //     t |= bits[i];
    //     t |= bits[i + 1] << 1;
    //     t |= bits[i + 2] << 2;
    //     t |= bits[i + 3] << 3;
    //     bs.writeBits(t, 4);
    // }
    // bs.dump();
    // bs.flush();
    // ofs.close();
    // std::ifstream ifs("bitfile.txt", std::ios::in | std::ios::binary);
    // Codecs::ibitstream ibs(ifs);
    // ibs.dump();
    // for (int i = 0; i < n; i++) {
    //     int bit = ibs.getBit();
    //     // std::cout << bit << " | " << bits[i] << std::endl;
    //     if (bit != bits[i]) {
    //         std::cout << "differ at pos = " << i << std::endl;
    //         break;
    //     }
    // }
    Codecs::HuffmanCodec codec;
    std::string fname = argv[1];
    std::vector<std::string> v;
    std::ifstream ifs(fname);
    // std::string content;
    clock_t begin, end;
    double elapsed_secs;
    begin = clock();
    while (!ifs.eof()) {
        std::string line;
        getline(ifs, line);
        v.push_back(line);
    }
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "reading done in " << elapsed_secs << " secs" << std::endl;
    std::vector<std::string> encoded;
    begin = clock();
    size_t file_size = 0, compressed_size = 0;
    for (std::string str : v) {
        codec.learn(std::experimental::string_view(str));
    }

    for (std::string str : v) {
        std::string encoded_line;
        codec.encode(encoded_line, str);
        encoded.push_back(encoded_line);
    }
    end = clock();

    std::cout << encoded.size() << " " << v.size() << std::endl;

    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "compression done in " << elapsed_secs << " secs" << std::endl;
    std::cout << "uncompressed memory: " << int(1.0 * file_size / 1024 / 1024) << " MBytes" << std::endl;
    std::cout << "compressed memory: " << int(1.0 * compressed_size / 1024 / 1024) << " MBytes" << std::endl;
    std::cout << "saved memory: " << int(1.0 * (file_size - compressed_size) / 1024 / 1024) << " MBytes" << std::endl;

    std::ofstream fout(fname + "_unpacked");
    begin = clock();
    for (std::string str : encoded) {
        std::string decoded_line;
        codec.decode(str, decoded_line);
        fout << decoded_line << std::endl;
    }
    end = clock();

    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "decompression done in " << elapsed_secs << " secs" << std::endl;
    return 0;
}
