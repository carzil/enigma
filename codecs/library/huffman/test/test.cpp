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
    std::ifstream ifs("data/" + fname);
    // std::string content;
    clock_t begin, end;
    double elapsed_secs;
    begin = clock();
    char* buf = new char[4096];
    while (!ifs.eof()) {
        ifs.read(buf, 4096);
        size_t readed = -1;
        if (ifs) {
            readed = 4096;
        } else {
            readed = ifs.gcount();
        }
        std::experimental::string_view view(buf, readed);
        codec.learn(view);
    }
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "reading done in " << elapsed_secs << " secs" << std::endl;
    std::ofstream ofs("data/" + fname + ".cpd");
    Codecs::obitstream bs(ofs);
    begin = clock();
    size_t file_size = 0, compressed_size = 0;
    ifs.clear();
    ifs.seekg(0, std::ios::beg);
    while (!ifs.eof()) {
        ifs.read(buf, 4096);
        size_t readed = -1;
        if (ifs) {
            readed = 4096;
        } else {
            readed = ifs.gcount();
        }
        // std::cout << "readed = " << readed << std::endl;
        file_size += readed;
        std::experimental::string_view view(buf, readed);
        codec.encodeChunk(bs, view);
    }
    codec.stopEncoding(bs);
    end = clock();
    bs.flush();
    bs.dump();
    ofs.close();

    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "compression done in " << elapsed_secs << " secs" << std::endl;
    std::cout << "uncompressed memory: " << int(1.0 * file_size / 1024 / 1024) << " MBytes" << std::endl;
    std::cout << "compressed memory: " << int(1.0 * compressed_size / 1024 / 1024) << " MBytes" << std::endl;
    std::cout << "saved memory: " << int(1.0 * (file_size - compressed_size) / 1024 / 1024) << " MBytes" << std::endl;

    std::ifstream w("data/" + fname + ".cpd");
    std::ofstream fout("data/" + fname + "_unpacked");
    // Codecs::obitstream streamu(fout);
    Codecs::ibitstream stream(w);
    // stream.dump();
    std::string decoded;
    // for (int i = 0; i < 2000; i++) {
    //     std::cout << stream.getBit();
    // }
    // std::cout << std::endl;
    begin = clock();
    codec.decode(decoded, stream);
    // streamu.flush();
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "decompression done in " << elapsed_secs << " secs" << std::endl;
    fout << decoded;
    return 0;
}
