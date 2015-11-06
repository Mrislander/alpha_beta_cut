//
//  node.cpp
//  hw2
//
//  Created by junyuan xiang on 15-11-5.
//  Copyright (c) 2015年 junyuan xiang. All rights reserved.
//

#include "node.h"
#include <vector>
#include <numeric>
#define minPlayer 0
#define maxPlayer 1
using namespace std;

Node::Node(vector<int> b){
    this->board=b;
    int sum = accumulate(b.begin(),b.end(),0);
    if (sum<=0)
    this->player = maxPlayer;
    else
    this->player = minPlayer;
    this->result = checkWin();
}
void Node::addChild(Node *child){
    this->children.push_back(child);
}
int Node::checkWin(){
    int result = 0;
    if(player==minPlayer){
        for(int i = 0;i<8;i+=3){
            if(board[i]+board[i+1]+board[i+2] == 3){ //check row
            result = 1;
            return result;
            }
        }
        for(int i = 0;i<3;i++){
            if(board[i]+board[i+3]+board[i+6] == 3){ //check col
                result = 1;
                return result;
            }
        }
        if(board[0]+board[4]+board[8] == 3) return 1;
        if(board[2]+board[4]+board[6] == 3) return 1;
        
    }
    else {
        for(int i = 0;i<8;i+=3){
            if(board[i]+board[i+1]+board[i+2] == -3){ //check row
                result = -1;
                return result;
            }
        }
        for(int i = 0;i<3;i++){
            if(board[i]+board[i+3]+board[i+6] == -3){ //check col
                result = -1;
                return result;
            }
        }
        if(board[0]+board[4]+board[8] == -3) return -1;
        if(board[2]+board[4]+board[6] == -3) return -1;

    }
    return result;
}

int Node:: getPlayer(){return this->player;};
int Node:: getResult(){return this->result;};
vector<int> Node:: getBoard(){ return this->board;};
vector<Node*> Node::getChildren(){return this->children;};



