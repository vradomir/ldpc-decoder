#include <vector>

class LDPC_Decoder {
public:
	LDPC_Decoder(std::vector<std::vector<char>> checkMatrix);
	~LDPC_Decoder();
	void inputInit(std::vector<double> input);

private:
	void updateVariableNodes(void);
	void updateControlNodes(void);
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

	unsigned short numIters;
	unsigned numVarNodes;
	unsigned numCtrlNodes;
};
