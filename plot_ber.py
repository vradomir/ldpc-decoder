#!/usr/bin/env python

import numpy as np
import cppyy
from cppyy.gbl.std import vector

cppyy.include("LDPC.hpp")
cppyy.load_library("ldpc")
from cppyy.gbl import LDPC_Decoder


def get_5g_code(Zc):
	import ldpc_code_defs

	def circulant(shift):
	    # eye roll
	    return np.roll(np.eye(Zc, dtype=int), shift, axis=1)

	set_index = [Zc in set for set in ldpc_code_defs.LiftSizes].index(True)

	circulantShifts = np.array(ldpc_code_defs.BaseGraph1)[:, [0, 1, 2+set_index]]

	H = np.zeros((46*Zc, 68*Zc), dtype=int)

	for i, j, shift in circulantShifts:
	    H[i*Zc:(i+1)*Zc, j*Zc:(j+1)*Zc] = circulant(shift)

	return H

def convert_to_vector(nparray):
	import array

	array = [array.array('i', row) for row in nparray]
	return vector[vector[int]](array)

H = convert_to_vector(get_5g_code(2))
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
