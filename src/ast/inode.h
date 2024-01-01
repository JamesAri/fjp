#ifndef __INODE_H_
#define __INODE_H_

class INode {
	public:
		virtual ~INode() = default;
		virtual void Compile() = 0;
};

#endif // __INODE_H_