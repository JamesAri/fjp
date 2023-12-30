class INode {
public:
	virtual ~INode() = default;
	virtual void Compile() = 0;
};