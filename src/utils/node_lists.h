#ifndef __LISTS_H_
#define __LISTS_H_

#include <vector>

class CStatement_Node;
class CDeclaration_Node;
class CExpression_Node;

typedef std::vector<CStatement_Node*> statement_list_t;
typedef std::vector<CDeclaration_Node*> declaration_list_t;
typedef std::vector<CExpression_Node*> expression_list_t;



#endif // __LISTS_H_