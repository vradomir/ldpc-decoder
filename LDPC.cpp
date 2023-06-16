#include <vector>

class LDPC_Decoder {
public:
	LDPC_Decoder(std::vector<std::vector<char>> checkMatrix) {
		unsigned numVarNodes = checkMatrix[0].size();
		unsigned numCtrlNodes = checkMatrix.size();

		variableNodeConns = new unsigned*[numVarNodes];
		controlNodeConns = new unsigned*[numCtrlNodes];

		unsigned numConnections;

		for(unsigned rowIdx=0; rowIdx < numCtrlNodes; rowIdx++) {
			numConnections = 0;

			// count connections to control nodes
			for(unsigned colIdx=0; colIdx < numVarNodes; colIdx++)
				numConnections += checkMatrix[rowIdx][colIdx];

			// allocate subarray
			controlNodeConns[rowIdx] = new unsigned[numConnections];

			// write connections to control nodes
			for(unsigned colIdx=0, connIdx=0; colIdx < numVarNodes; colIdx++)
				if(checkMatrix[rowIdx][colIdx])
					controlNodeConns[rowIdx][connIdx++] = colIdx;
		}

		for(unsigned colIdx=0; colIdx < numVarNodes; colIdx++) {
			numConnections = 0;

			// count connections to variable nodes
			for(unsigned rowIdx=0; rowIdx < numCtrlNodes; rowIdx++)
				numConnections += checkMatrix[rowIdx][colIdx];

			// allocate subarray
			variableNodeConns[colIdx] = new unsigned[numConnections];

			// write connections to variable nodes
			for(unsigned rowIdx=0, connIdx=0; rowIdx < numCtrlNodes; rowIdx++)
				if(checkMatrix[rowIdx][colIdx])
					variableNodeConns[colIdx][connIdx++] = rowIdx;
		}
	};

private:
	void updateVariableNodes(void);
	void updateControlNodes(void);
	double** messageVariableToControl;
	double** messageControlToVariable;
	unsigned** variableNodeConns;
	unsigned** controlNodeConns;

	unsigned short numIters;
};
