//
//  main.cpp
//  hw2
//
//  Created by junyuan xiang on 15-11-5.
//  Copyright (c) 2015年 junyuan xiang. All rights reserved.
//

#include <iostream>
#include <vector>
#include <queue>
#include "Node.h"
#include <numeric>
#define minPlayer 0
#define maxPlayer 1
using namespace std;
static int count = 1;
static int alphaCount = 0;
static int betaCount = 0;
vector<int> convert(string b);
Node* exploreTree(Node &n);
int minMax(Node *n);
int alphaBeta(Node *n,int val,int val2);
void printNodeState(Node* n);
int main(int argc, const char * argv[])
{
    vector<int> b;
   
    if(argc!=2){
        cout<<"\nUsage: "<<argv[0]<<" board state\n"<<endl;
	}
    string temp;
    temp = argv[1];
    while(temp.length()!=9){
        cout<<"wrong board state,please enter new board state"<<endl;
        cin>>temp;
        
    }
    b = convert(temp);
    while(accumulate(b.begin(),b.end(),0) < 0){
        cout<<"wrong board state,'X' is the first player"<<endl;
        cin>>temp;
        b = convert(temp);
    }
    Node n(b);
    Node* root = exploreTree(n);
    int res = minMax(root);
    cout<<"Running without alpha-beta pruning:"<<endl;
    cout<<"Game Result:"<<res<<endl;
    cout<<"Moves considered without alpha-beta pruning:"<<::count<<endl<<endl;
    cout<<"--------------------------------------------"<<endl;
    ::count=0;
    cout<<"Running with alpha-beta pruning:"<<endl;
    res = alphaBeta(root,INT_MIN,INT_MAX);
    cout<<"Game Result:"<<res<<endl;
    cout<<"Moves considered with alpha-beta pruning:"<<::count<<endl;
    cout<<"Alpha cuts: "<<::alphaCount<<endl;
    cout<<"Beta cuts: "<<::betaCount<<endl;
    cout<<"--------------------------------------------"<<endl;
    return 0;
}

vector<int> convert(string b){
    vector<int> ans;
    for(int i = 0 ; i<b.length();i++){
        if(b[i]=='X')   ans.push_back(1);
        else if(b[i]=='O') ans.push_back(-1);
        else ans.push_back(0);
    }
    return ans;
}

Node* exploreTree(Node &n){
    Node *root = new Node(n);
    queue<Node*> myQue;
    myQue.push(root);
    while(!myQue.empty()){
        Node *temp = myQue.front();
        myQue.pop();
        if(temp->getPlayer()==minPlayer){
            if(temp->getResult() == 1)
            continue;
            for(int i = 0 ; i < temp->getBoard().size();i++){
                if(temp->getBoard()[i]==0){
                    vector<int> v(temp->getBoard());
                    v[i] = -1;
                    Node *child = new Node(v);
                    temp->addChild(child);
                    myQue.push(child);
                    
                }
            }
        }
        else if(temp->getPlayer()==maxPlayer){
            if(temp->getResult() == -1)
            continue;
            for(int i = 0 ; i < temp->getBoard().size();i++){
                if(temp->getBoard()[i]==0){
                    vector<int> v(temp->getBoard());
                    v[i] = 1;
                    Node *child = new Node(v);
                    temp->addChild(child);
                    myQue.push(child);
                   
                }
            }
        }
    
    
    }
    return root;
}

int minMax(Node *n){
    if(n->getChildren().empty()){
        return n->getResult();
    }
    int temp=0;
    int player = n->getPlayer();
    int ans = n->getPlayer()==maxPlayer?-1:2;
    for(int i = 0; i<n->getChildren().size();i++){
         ::count++;
        if(player==minPlayer){
            temp=minMax(n->getChildren()[i]);
            if(ans>temp){
                ans=temp;
            }
        }
        else{
            temp=minMax(n->getChildren()[i]);
            if(ans<temp){
                ans=temp;
            }
        
        }
    }
    
    
    return  ans;
}
int alphaBeta(Node *n,int val, int val2){
    if(n->getChildren().empty()){
        return n->getResult();
    }
    int alpha = INT_MIN;
    int beta = INT_MAX;
    //inherit alpha beta from parents node
    alpha = val ==INT_MIN?INT_MIN:val;
    beta  = val2 ==INT_MAX?INT_MAX:val2;
    int temp=0;
    int player = n->getPlayer();
    for(int i = 0; i<n->getChildren().size();i++){
        if(alpha >= beta){
            printNodeState(n);
            cout<<endl<<endl;
            continue;
            
        }
        else{
            //count the explore states.
            ::count++;
        }
        if(player==maxPlayer){
           //find child's value
            temp = alphaBeta(n->getChildren()[i],alpha,INT_MAX);
            //compare and update the alpha
            if(temp > alpha){
                alpha = temp;
            }
        }
        else{
           
            temp = alphaBeta(n->getChildren()[i],INT_MIN,beta);
            if(temp < beta){
                beta = temp;
            }
            
        }
    }
    //return the current node value
    int ans = player == maxPlayer?alpha:beta;
    return  ans;
}

void printNodeState(Node* n){
    
    for(int i = 0;i<n->getBoard().size();i++){
    if(i%3==0&&i!=0)
        cout<<endl;
    if(n->getBoard()[i]==1)
        cout<<"X";
     else if(n->getBoard()[i]==-1)
        cout<<"O";
     else
        cout<<"_";
     
    }
    if(n->getPlayer()==maxPlayer){
        cout<<"  Beta cut";
        ::betaCount++;
    }
    else {
         cout<<"  Alpha cut";
        ::alphaCount++;
    }

}
