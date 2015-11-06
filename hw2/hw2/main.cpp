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
#define minPlayer 0
#define maxPlayer 1
using namespace std;
static int count = 1;

vector<int> convert(string b);
Node* exploreTree(Node &n);
int minMax(Node *n);
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
    Node n(b);
    Node* root = exploreTree(n);
    int res = minMax(root);
    cout<<res;
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
                    ::count++;
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
                    ::count++;
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