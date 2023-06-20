# LDPC Decoder

This project aims to create a reference for SPA LDPC decoding, and provides a python function to test the BER performance.  
5G LDPC codes are given as an example, although any parity matrix may be used.

To run the python test, C++ code has to be compiled into a dynamic library named `libldpc.so`. Following is a command that accomplishes that:
```
g++ -fpic -shared LDPC.cpp -o libldpc.so
```

The test uses `cppyy` to interface with the C++ code, and plots BER performance as a function of SNR.

Input to the decoder is a BPSK modulated codeword consisting of all zeroes with added AWGN.
Output of the decoder are LLRs, and are converted to hard output values based on the sign of the LLR.
