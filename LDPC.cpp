#include <vector>

#include "LDPC.hpp"

LDPC_Decoder::LDPC_Decoder(std::vector<std::vector<char>> checkMatrix) {
	numVarNodes = checkMatrix[0].size();
	numCtrlNodes = checkMatrix.size();

	unsigned numConnections, maxConnections=0;

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
		if(maxConnections < numConnections)
			maxConnections = numConnections;

	}

	// allocate subarray
	controlNodesNeighbors[0] = new unsigned[numCtrlNodes*maxConnections];
	messageVariableToControl[0] = new double[numCtrlNodes*maxConnections];

	for(unsigned rowIdx=0; rowIdx < numCtrlNodes; rowIdx++) {
		controlNodesNeighbors[rowIdx] = controlNodesNeighbors[0] + rowIdx*maxConnections;
		messageVariableToControl[rowIdx] = messageVariableToControl[0] + rowIdx*maxConnections;

		// write connections to control nodes
		for(unsigned colIdx=0, connIdx=0; colIdx < numVarNodes; colIdx++)
			if(checkMatrix[rowIdx][colIdx])
				controlNodesNeighbors[rowIdx][connIdx++] = colIdx;
	}

	variableNodesNeighbors = new unsigned*[numVarNodes];
	messageControlToVariable = new double*[numVarNodes];
	variableNodeIndex = new unsigned[numVarNodes];
	numberConnsVariable = new unsigned[numVarNodes];

	maxConnections = 0;
	for(unsigned colIdx=0; colIdx < numVarNodes; colIdx++) {
		numConnections = 0;

		// count connections to variable nodes
		for(unsigned rowIdx=0; rowIdx < numCtrlNodes; rowIdx++)
			numConnections += checkMatrix[rowIdx][colIdx];

		numberConnsVariable[colIdx] = numConnections;
		if(maxConnections < numConnections)
			maxConnections = numConnections;
	}

	// allocate subarray
	variableNodesNeighbors[0] = new unsigned[numVarNodes*maxConnections];
	messageControlToVariable[0] = new double[numVarNodes*maxConnections];

	for(unsigned colIdx=0; colIdx < numVarNodes; colIdx++) {
		variableNodesNeighbors[colIdx] = variableNodesNeighbors[0] + colIdx*maxConnections;
		messageControlToVariable[colIdx] = messageControlToVariable[0] + colIdx*maxConnections;

		// write connections to variable nodes
		for(unsigned rowIdx=0, connIdx=0; rowIdx < numCtrlNodes; rowIdx++)
			if(checkMatrix[rowIdx][colIdx])
				variableNodesNeighbors[colIdx][connIdx++] = rowIdx;
	}
};

void LDPC_Decoder::reinitializeIndices(void) {
	for(unsigned rowIdx=0; rowIdx < numCtrlNodes; rowIdx++)
		controlNodeIndex[rowIdx] = 0;
	for(unsigned colIdx=0; colIdx < numVarNodes; colIdx++)
		variableNodeIndex[colIdx] = 0;
};

void LDPC_Decoder::inputInit(std::vector<double> input) {
	reinitializeIndices();

	for(unsigned varNodeIdx=0; varNodeIdx < numVarNodes; varNodeIdx++) {
		for(unsigned connIdx=0; connIdx < numberConnsVariable[varNodeIdx]; connIdx++) {
			unsigned ctrlNodeIdx = variableNodesNeighbors[varNodeIdx][connIdx];
			messageVariableToControl[ctrlNodeIdx][controlNodeIndex[ctrlNodeIdx]++] = input[varNodeIdx];
		}
	}
};

LDPC_Decoder::~LDPC_Decoder() {
	delete controlNodesNeighbors[0];
	delete messageVariableToControl[0];
	delete controlNodesNeighbors;
	delete messageVariableToControl;
	delete numberConnsControl;

	delete variableNodesNeighbors[0];
	delete messageControlToVariable[0];
	delete variableNodesNeighbors;
	delete messageControlToVariable;
	delete numberConnsVariable;
};
