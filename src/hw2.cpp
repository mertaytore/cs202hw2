/*
 * hw2.cpp
 * Mert Aytöre
 * 21400923
 * CS202-01
 * HW1
 */

#include "hw2.h"
#include <string>
#include <stdlib.h>
#include <fstream>
using namespace std;

NgramTree::NgramTree() {
	root = NULL;
}

NgramTree::~NgramTree() {
	root->deleteTree(root);
}

//for checking the complete tree
int pow(int a, int b){
	int sum = 1;
	for(int i = b; i > 0; i--){
		sum *= a;
	}
	if(b >= 0)
		return sum;
	else // so that a tree with height 1 counts as a complete tree
		return 1;
}

//is made according to dictionary
void NgramTree::addNgram(string ngram){
	//all to lowercase
	char s[ngram.length()];
	for(int i = 0; i < ngram.length(); i++){
		s[i] = tolower(ngram[i]);
	}
	//new string to be added
	string str = "";
	for(int i = 0; i < ngram.length(); i++){
		str += s[i];
	}
	//empty tree
	if(root == NULL)
		root = new NgramNode(str, NULL, NULL);

	else{
		NgramNode* ptr = root;
		while(ptr != NULL){
			//the string was already added
			if(str.compare(ptr->word) == 0){
				//so count of it is incremented
				ptr->count++;
				break;
			}
			else if(str.compare(ptr->word) < 0){
				// if left child empty, then add it
				// because it has lower priority
				if(ptr->leftChild == NULL){
					ptr->leftChild = new NgramNode(str, NULL, NULL);
					break;
				}
				// else further comparisons will be made with current node's left child
				else
					ptr = ptr->leftChild;
			}
			else{
				//then insert to right
				if(ptr->rightChild == NULL){
					ptr->rightChild = new NgramNode(str, NULL, NULL);
					break;
				}
				else
					ptr = ptr->rightChild;
			}
		}
	}
}

int  NgramTree::getTotalNgramCount(){
	int count = 0;
	NgramNode* ptr = root;

	ptr->countNgrams(count);
	return count;
}

void NgramTree::printNgramFrequencies(){
	NgramNode* ptr = root;
	cout << endl;
	ptr->printCount();
}

bool NgramTree::isComplete(){
	// stat = 2; every node in h-1 has 2 children so far
	int height = 0;
	int stat = 2;

	height = root->check(height);
	int curHeight = 1;

	// count:keeping track of all nodes with height h-1
	int count = 0;
	return root->checkComplete(curHeight, stat, height, count);
}

bool NgramTree::isFull(){
	int aCount, height = 0;
	aCount = this->getTotalNgramCount();
	height = root->check(height);
	//whole work done here
	if (pow(2,height)-1 == aCount)
		return true;
	else
		return false;
}

//read the inputFile and generate the tree by detecting ngrams with addNgram function
void NgramTree::generateTree(string fileName, int n){
	ifstream ff;
	ff.open(fileName.c_str());
	string w;

	while (ff >> w){
		if (w.length() >= n){

			for (int i = 0; i <= w.length() - n; i++)
				addNgram(w.substr(i,n));//substring for n size ngrams
		}
	}
}

void NgramNode::deleteTree(NgramNode*& node){
	if(node == NULL)
		cout << "";
	else if (node != NULL){
		deleteTree(node->leftChild);
		deleteTree(node->rightChild);
		delete node;
		node = NULL;
	}
}

void NgramNode::countNgrams(int& count){
	// traverse in preorder to sum count total recursively
	if(this != NULL){
		//if not null, count increment
		count++;
		leftChild->countNgrams(count);
		rightChild->countNgrams(count);
	}
}

void NgramNode::printCount(){
	// recursively prints current node's word and word count traversing the tree in in-order
	if(this != NULL)    {
		this->leftChild->printCount();

		printf("\"%s\" appears %d time(s)\n", this->word.c_str(), this->count);

		this->rightChild->printCount();
	}
}

//gives height
int NgramNode::check(int h){
	if(this != NULL){
		h++;
		h = max(leftChild->check(h), rightChild->check(h));
	}
	return h;
}

bool NgramNode::checkComplete(int curHeight, int &stat, int height, int &count){
	if(height == 0)
		return true;
	if(height == 2)
		return (leftChild != NULL && rightChild != NULL)
				|| (leftChild != NULL && rightChild == NULL);
	if(curHeight < height - 1 && this != NULL){
		this->leftChild->checkComplete(curHeight + 1, stat, height, count);
		this->rightChild->checkComplete(curHeight + 1, stat, height, count);
	}
	else if(this != NULL){
		//2 children
		if(stat == 2){
			if( (this->rightChild != NULL && this->leftChild != NULL))
				count++;
			//1 child
			else if( (this->leftChild != NULL && this->rightChild == NULL)){
				stat = 1;
				count++;
			}
			//nada
			else{
				count++;
				stat = 0;
			}
		}
		// only status 0 can come after status 1, so count is not incremented for other conditions
		else if(stat == 1){
			if( (this ->rightChild != NULL && this->leftChild != NULL)
					|| (this->leftChild != NULL && this->rightChild == NULL) )
			{
			}
			else {
				count++;
				stat = 0;
			}
		}
		// only status 0 can come after status 0, so count is not incremented for other conditions
		else{
			if( (this ->rightChild != NULL && this->leftChild != NULL)
					|| (this->leftChild != NULL && this->rightChild == NULL) ){}
			else
				count++;
		}
	}
	else
		return false;
	if(count == pow(2,height-2))
		return true;
	else
		return false;
}

int main(int argc, char **argv){
	NgramTree tree;
	string fileName(argv[1]);
	int n = atoi(argv[2]);
	tree.generateTree(fileName, n);

	cout << "\nTotal " << n << "-gram count: "  << tree.getTotalNgramCount() << endl;

	tree.printNgramFrequencies();
	cout << n << "-gram tree is complete: "  << (tree.isComplete() ? "Yes" : "No") << endl;

	//Before insertion of new n-grams
	cout << "\nTotal " << n << "-gram count: "  << tree.getTotalNgramCount() << endl;

	tree.addNgram("samp");
	tree.addNgram("samp");
	tree.addNgram("zinc");
	tree.addNgram("aatt");

	cout << "\nTotal " << n << "-gram count: "  << tree.getTotalNgramCount() << endl;

	tree.printNgramFrequencies();
	cout<< n << "-gram tree is complete: "  << (tree.isComplete() ? "Yes" : "No") << endl;
	cout<< n << "-gram tree is full: "  << (tree.isFull() ? "Yes" : "No") << endl;
	return 0;
}
