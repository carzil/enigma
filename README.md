Enigma is a library for compressing a large amount of small strings. It's main force is EnigmaCodec.
It uses dictionary based encoder greedily consuming substrings from given input records and then passes substring into Huffman coder.

# Building
You can build and install Enigma with:

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
make install
```

# Testing
Unit-tests can be built with ENIGMA_BUILD_TESTS flag turned to ON (you want, probably, Debug-build)

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENIGMA_BUILD_TESTS=ON
make
```

Then run:

```bash
tools/runner <file you want to compress>
```

This will run Enigma and tests it for compressing/decompressing and save/load.
