//
//  Node.h
//  hw2
//
//  Created by junyuan xiang on 15-11-5.
//  Copyright (c) 2015年 junyuan xiang. All rights reserved.
//

#ifndef hw2_Node_h
#define hw2_Node_h
#include <vector>
using namespace std;

class Edge;

class Node{
private:
    vector<int> board;
    int player;
    int result;
    vector<Node*> children;
    int alpha;
    int beta;
    public:
    Node (vector<int> b);
    void addChild(Node *child);
    int checkWin();
    int getPlayer();
    int getResult();
    int getAlpha();
    int getBeta();
    void setAlpha(int);
    void setBeta(int);
    vector<int> getBoard();
    vector<Node*> getChildren();
};




#endif
