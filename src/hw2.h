/*
 * hw2.h
 * Mert Aytöre
 * 21400923
 * CS202-01
 * HW1
 */

#ifndef SRC_NGRAMTREE_H_
#define SRC_NGRAMTREE_H_
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

class NgramNode{
public:
private:
    string word;
    int count;
    NgramNode* leftChild;
    NgramNode* rightChild;

    NgramNode(const string word, NgramNode* left, NgramNode* right) :
    	count(1), leftChild(NULL), rightChild(NULL){
        this->word = word;
        this->leftChild = left;
        this->rightChild = right;
    }
    void countNgrams(int& count);
    void printCount();
    int check(int height);
    bool checkComplete(int curHeight, int& status, int height, int &count);
    void deleteTree(NgramNode*& node);

    friend class NgramTree;
};

class NgramTree {
public:
	NgramTree();
	~NgramTree();
	void addNgram(string ngram);
	int  getTotalNgramCount();
	void printNgramFrequencies();
	bool isComplete();
	bool isFull();
	void generateTree(string fileName, int n);
private:
	NgramNode* root;
};

#endif /* SRC_NGRAMTREE_H_ */
