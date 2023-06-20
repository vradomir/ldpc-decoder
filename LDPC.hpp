#ifndef __LDPC_HPP__
#define __LDPC_HPP__

#include <vector>

class LDPC_Decoder {
public:
	LDPC_Decoder(std::vector<std::vector<int>> checkMatrix);
	~LDPC_Decoder();
	std::vector<double> decode(std::vector<double> input, double noiseVariance, unsigned numIters=10);

private:
	std::vector<double> lambda;
	double *LLRs;
	double **messageVariableToControl;
	double **messageControlToVariable;
	unsigned **variableNodesNeighbors;
	unsigned **controlNodesNeighbors;
	unsigned *variableNodeIndex;
	unsigned *controlNodeIndex;
	unsigned *numberConnsControl;
	unsigned *numberConnsVariable;

	void reinitializeIndices(void);
	void inputInit(std::vector<double> input, double noiseVariance);
	void updateVariableNodes(void);
	void updateControlNodes(void);

	unsigned short numIters;
	unsigned numVarNodes;
	unsigned numCtrlNodes;
};

#endif
