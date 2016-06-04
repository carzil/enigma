#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <enigma/codecs/enigma.h>
#include <enigma/common/dictionary.h>
#include <enigma/colors.h>
#include <enigma/utils.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>

using std::experimental::string_view;
using std::vector;
using std::string;

class RUsage {
    private:
        rusage* usage;

    public:
        RUsage() {
            usage = (rusage*)malloc(sizeof(rusage));
            getrusage(RUSAGE_SELF, usage);
        }

        ~RUsage() {
            free(usage);
        }

        rusage* operator->() {
            return usage;
        }
};

class Stopwatch {
    public:
        Stopwatch() : begin(0), elapsed_secs(0) {}

        void Start() {
            begin = clock();
        }

        void Stop() {
            elapsed_secs += double(clock() - begin) / CLOCKS_PER_SEC;
        }

        void Reset() {
            elapsed_secs = 0;
        }

        double GetElapsedSeconds() {
            return elapsed_secs;
        }

    private:
        clock_t begin;
        double elapsed_secs;
};

const char* unit_names[] = {"Bytes", "KBytes", "MBytes", "GBytes", "TBytes", "PBytes", "EBytes", "ZBytes"};

std::string format_size(size_t size) {
    size_t i = 0;
    size_t sz = size;
    while (sz > 1024) {
      sz /= 1024;
      i++;
    }
    std::ostringstream s;
    s << sz << " " << unit_names[i];
    return s.str();
}

void print_usage() {
    RUsage usage;
    std::cout << "memory: " << format_size(usage->ru_maxrss * 1024);
    double user_time = usage->ru_utime.tv_sec + usage->ru_utime.tv_usec / 1000000.0;
    double system_time = usage->ru_stime.tv_sec + usage->ru_stime.tv_usec / 1000000.0;
    std::cout << std::fixed << std::setprecision(2) << ", user time: " << user_time << "s , system time: " << system_time << "s" << std::endl;
}

void test_from_file(std::vector<std::string>& records) {
    Enigma::EnigmaCodec* pre_codec = new Enigma::EnigmaCodec();
    Enigma::EnigmaCodec codec;
    Stopwatch sw;

    sw.Start();
    pre_codec->Learn(records);
    sw.Stop();
    Enigma::DataOutput* out = new Enigma::DataOutput();
    pre_codec->save(*out);
    Enigma::DataInput* in = new Enigma::DataInput(out->GetStr());
    codec.load(*in);
    delete pre_codec;
    delete in;
    delete out;

    // codec.PrintCodes();

    std::cout << "learning done in " << GREENB(sw.GetElapsedSeconds() << " secs") << std::endl;

    size_t file_size = 0, compressed_size = 0;

    Stopwatch sw_compression, sw_decompression;
    for (size_t i = 0; i < records.size(); i++) {
        std::string enc;
        enc.reserve(records[i].size() * 3);
        file_size += records[i].size();

        sw_compression.Start();
        size_t sz = codec.encode(records[i], &enc[0]);
        sw_compression.Stop();

        compressed_size += sz;

        std::string dec;
        dec.resize(records[i].size() * 2);
        sw_decompression.Start();
        size_t decsz = codec.decode(string_view(&enc[0], sz), &dec[0]);
        sw_decompression.Stop();

        dec.resize(decsz);
        if (dec != records[i]) {
            std::cout << "FUCK" << std::endl;
            std::cout << dec.size() << " " << dec << std::endl;
        }
    }

    std::cout << "compression done in " << GREENB(sw_compression.GetElapsedSeconds() << " secs") << std::endl;
    std::cout << "decompression done in " << YELLOWB(sw_decompression.GetElapsedSeconds() << " secs") << std::endl;
    std::cout << "uncompressed memory: " << GREENB(format_size(file_size)) << std::endl;
    std::cout << "compressed memory: " << GREENB(format_size(compressed_size)) << std::endl;
    std::cout << "saved memory: " << GREENB(format_size(file_size - compressed_size)) << std::endl;
    std::cout << "compression ratio ~ " << std::fixed << std::setprecision(3) << YELLOWB(100 - 100.0 * compressed_size / file_size << "%") << std::endl;
}

int read_file(std::ifstream& stream, std::vector<std::string>& v) {
    std::string line;

    while (getline(stream, line)) {
        v.push_back(line);
    }

    return 0;
}

int read_binary_file(std::ifstream& stream, std::vector<std::string>& v) {
    char buf[4];
    while (stream) {
        stream.read(buf, 4);
        if (stream) {
            size_t size = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
            char* content = new char[size];
            stream.read(content, size);
            if (stream) {
                v.push_back(std::string(content, size));
            } else {
                std::cout << "error: bad format of input file, got " << stream.gcount() << " bytes, instead of " << size << std::endl;
                return 1;
            }
            delete[] content;
        } else if (!stream.eof()) {
            std::cout << "error: bad format of input file, got " << stream.gcount() << " bytes, instead of 4" << std::endl;
            return 1;
        }
    }

    return 0;
}

int test_codec(char **argv, bool is_binary) {
    std::string fname = argv[1];
    std::ifstream ifs(fname);
    if (ifs.fail()) {
        std::cout << "error: " << strerror(errno) << std::endl;
        return 1;
    }
    std::vector<std::string> v;
    int result = 0;
    if (!is_binary) {
        result = read_file(ifs, v);
    } else {
        result = read_binary_file(ifs, v);
    }
    if (result != 0) {
        return 2;
    }
    test_from_file(v);
    return 0;
}

void usage() {
    std::cout << "usage: runner [--binary] [--help] FILE" << std::endl;
};

void help() {
    usage();
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -b, --binary    interpret file as array of 'size of record in LE uint32_t + record' " << std::endl; 
}

int main(int argc, char **argv) {

    static struct option long_options[] = {
        {"binary", no_argument, 0, 'b'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0},
    };

    int option_index;
    int c;
    bool is_binary = false;
    while ((c = getopt_long(argc, argv, "bh:", long_options, &option_index)) != -1) {
        switch (c) {
            case 'b':
                is_binary = true;
                break;
            case 'h':
                help();
                return 0;
            default:
                usage();
                return 1;
        }
    }
    if (optind == argc - 1) {
        argv += optind - 1;
        return test_codec(argv, is_binary);
    } else {
        usage();
        return 1;
    }
}
