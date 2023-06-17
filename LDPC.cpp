#include <vector>

class LDPC_Decoder {
public:
	LDPC_Decoder(std::vector<std::vector<char>> checkMatrix) {
		numVarNodes = checkMatrix[0].size();
		numCtrlNodes = checkMatrix.size();

		unsigned numConnections;

		controlNodesNeighbors = new unsigned*[numCtrlNodes];
		messageVariableToControl = new double*[numCtrlNodes];
		controlNodeIndex = new unsigned[numCtrlNodes];
		numberConnsControl = new unsigned[numCtrlNodes];

		for(unsigned rowIdx=0; rowIdx < numCtrlNodes; rowIdx++) {
			numConnections = 0;

			// count connections to control nodes
			for(unsigned colIdx=0; colIdx < numVarNodes; colIdx++)
				numConnections += checkMatrix[rowIdx][colIdx];

			numberConnsControl[rowIdx] = numConnections;
			// allocate subarray
			controlNodesNeighbors[rowIdx] = new unsigned[numConnections];
			messageVariableToControl[rowIdx] = new double[numConnections];

			// write connections to control nodes
			for(unsigned colIdx=0, connIdx=0; colIdx < numVarNodes; colIdx++)
				if(checkMatrix[rowIdx][colIdx])
					controlNodesNeighbors[rowIdx][connIdx++] = colIdx;
		}

		variableNodesNeighbors = new unsigned*[numVarNodes];
		messageControlToVariable = new double*[numVarNodes];
		variableNodeIndex = new unsigned[numVarNodes];
		numberConnsVariable = new unsigned[numVarNodes];

		for(unsigned colIdx=0; colIdx < numVarNodes; colIdx++) {
			numConnections = 0;

			// count connections to variable nodes
			for(unsigned rowIdx=0; rowIdx < numCtrlNodes; rowIdx++)
				numConnections += checkMatrix[rowIdx][colIdx];

			numberConnsVariable[colIdx] = numConnections;
			// allocate subarray
			variableNodesNeighbors[colIdx] = new unsigned[numConnections];
			messageControlToVariable[colIdx] = new double[numConnections];

			// write connections to variable nodes
			for(unsigned rowIdx=0, connIdx=0; rowIdx < numCtrlNodes; rowIdx++)
				if(checkMatrix[rowIdx][colIdx])
					variableNodesNeighbors[colIdx][connIdx++] = rowIdx;
		}
	};

	void reinitializeIndices(void) {
		for(unsigned rowIdx=0; rowIdx < numCtrlNodes; rowIdx++)
			controlNodeIndex[rowIdx] = 0;
		for(unsigned colIdx=0; colIdx < numVarNodes; colIdx++)
			variableNodeIndex[colIdx] = 0;
	};

	~LDPC_Decoder() {
		for(unsigned rowIdx=0; rowIdx < numCtrlNodes; rowIdx++) {
			if(controlNodesNeighbors[rowIdx] != nullptr)
				delete controlNodesNeighbors[rowIdx];
				delete messageVariableToControl[rowIdx];
		}
		delete controlNodesNeighbors;
		delete messageVariableToControl;
		delete numberConnsControl;

		for(unsigned colIdx=0; colIdx < numVarNodes; colIdx++) {
			if(variableNodesNeighbors[colIdx] != nullptr)
				delete variableNodesNeighbors[colIdx];
				delete messageControlToVariable[colIdx];
		}
		delete variableNodesNeighbors;
		delete messageControlToVariable;
		delete numberConnsVariable;
	};

private:
	void updateVariableNodes(void);
	void updateControlNodes(void);
	double** messageVariableToControl;
	double** messageControlToVariable;
	unsigned **variableNodesNeighbors;
	unsigned **controlNodesNeighbors;
	unsigned *variableNodeIndex;
	unsigned *controlNodeIndex;
	unsigned *numberConnsControl;
	unsigned *numberConnsVariable;

	unsigned short numIters;
	unsigned numVarNodes;
	unsigned numCtrlNodes;
};
