#!/usr/bin/env python

import numpy as np
import cppyy
from cppyy.gbl.std import vector

cppyy.include("LDPC.hpp")
cppyy.load_library("ldpc")
from cppyy.gbl import LDPC_Decoder


H = vector[vector[int]]([
    [0, 1, 0, 1, 1, 0, 0, 1],
    [1, 1, 1, 0, 0, 1, 0, 0],
    [0, 0, 1, 0, 0, 1, 1, 1],
    [1, 0, 0, 1, 1, 0, 1, 0]
])
codewordLen = len(H[0])
numInfoBits = codewordLen - len(H)

decoder = LDPC_Decoder(H)

numIters = 1000
codeword = np.ones(codewordLen)

noiseDeviations = np.logspace(-2, 1, 20)
snr = 10*np.log10(1/noiseDeviations)

ber = np.zeros_like(noiseDeviations)

for i, noiseDeviation in enumerate(noiseDeviations):
    sum = 0
    for _ in range(numIters):
        inputSamples = codeword + np.random.normal(scale=noiseDeviation, size=codewordLen)

        outputLLRs = np.array(decoder.decode(inputSamples, noiseDeviation**2))
        outputHard = outputLLRs[:numInfoBits] < 0
        sum += np.count_nonzero(outputHard)

    ber[i] = sum / (numInfoBits*numIters)

import matplotlib.pyplot as plt

plt.semilogy(snr, ber)
plt.xlabel("SNR [dB]")
plt.ylabel("BER")
plt.grid()
plt.show()
