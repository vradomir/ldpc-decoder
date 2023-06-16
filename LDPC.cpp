class LDPC_Decoder {
public:

private:
	void updateVariableNodes(void);
	void updateControlNodes(void);
	double** messageVariableToControl;
	double** messageControlToVariable;
	unsigned** variableNodeConns;
	unsigned** controlNodeConns;

	unsigned short numIters;
};
