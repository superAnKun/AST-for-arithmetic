#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
using namespace std;
class Node;
class NumberNode;
class OperatorNode;

/*
class Visitor {
public:
	virtual void visit(NumberNode *node) = 0;
	virtual void visit(OperatorNode *node) = 0;
};
*/
class Visitor;
class Node {
public:
	Node() {
	}
	virtual void Accept(Visitor *v) = 0;
	virtual ~Node() {

	}
	Node *lchild, *rchild;
};

class Visitor {
public:
	virtual void visit(NumberNode *node) = 0;
	virtual void visit(OperatorNode *node) = 0;
};
class NumberNode :public Node {
public:
	NumberNode(char *num) : num(0){
		for (int i = 0; num[i]; i++) {
			if (num[i] < '0' || num[i] > '9') continue;
			this->num = this->num * 10 + num[i] - '0';
		}
		this->lchild = this->rchild = NULL;
	}
	int getNum() {
		return num;
	}
	void Accept(Visitor *v) override {
		v->visit(this);
	}
private:
    int num;
};

class OperatorNode : public Node {
public:
	enum class Type {
		ADD, SUB, MUL, DIV
	};
	OperatorNode(char expr) {
		switch (expr) {
			case '+': type = Type::ADD; break;
			case '-': type = Type::SUB; break;
			case '*': type = Type::MUL; break;
			case '/': type = Type::DIV; break;
		}
		this->lchild = this->rchild = NULL;
	}
	Type getType() {
		return type;
	}
	void Accept(Visitor *v) {
		v->visit(this);
	}
private:
	Type type;
};

class AST {
public:
	AST(char* str) {
		int pos_priority = 1000, priority = 0, pos = -1;
		for (int i = 0; str[i]; i++) {
			int cur_priority = 1001;
			switch (str[i]) {
				//int cur_priority = 1a001;
				case '(': priority += 100; break;
				case ')': priority -= 100; break;
				case '+': 
				case '-': cur_priority = priority + 1; break;
				case '*':
				case '/': cur_priority = priority + 2; break;
			}
			if (pos_priority >= cur_priority) {
				pos_priority = cur_priority;
				pos = i;
			}
		}
		if (pos == -1) {
	//		char *aa = strdup(str);
			this->root = new NumberNode(str);
	//		free(aa);
		} else {
			char *str1 = strdup(str);
			AST t2(str1 + pos + 1);
			str1[pos] = 0;
			AST t1(str1);
			this->root = new OperatorNode(str[pos]);
			this->root->lchild = t1.getNode();
			this->root->rchild = t2.getNode();
			free(str1);
		}
	}

	Node *getNode() {
		return root;
	}
	void output();
private:
	Node *root;
};

class ASTVisitor : public Visitor {
public:
	void visit(NumberNode *node) {
		this->result = node->getNum();
	}
	void visit(OperatorNode *node) {
		OperatorNode::Type t = node->getType();
		ASTVisitor vst1, vst2;
		node->lchild->Accept(&vst1);
		node->rchild->Accept(&vst2);
		switch (t) {
			case OperatorNode::Type::SUB: this->result = vst1.result - vst2.result; break;
			case OperatorNode::Type::ADD: this->result = vst1.result + vst2.result; break;
			case OperatorNode::Type::MUL: this->result = vst1.result * vst2.result; break;
			case OperatorNode::Type::DIV: this->result = vst1.result / vst2.result; break;
		}
	}
	int getResult() {
		return result;
	}
private:
	int result;
};

void AST::output() {
		ASTVisitor vst;
		root->Accept(&vst);
		cout << vst.getResult() << endl;
	}
int main() {

	char *str = (char*)malloc(sizeof(char) * 1000);
	while (scanf("%[^\n]s", str) != EOF) {
		getchar();
		fflush(stdout);
		AST t(str);
		t.output();
	}
	return 0;
}
