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
static int count = 0;
static int alphaCount = 0;
static int betaCount = 0;
static int Rcount = 0;


vector<int> convert(string b);
Node* exploreTree(Node &n);
int minMax(Node *n);
int alphaBeta(Node *n,int val,int val2);
int alphaBetaKiller(Node *n,int val, int val2,vector<vector<int>> &k);
int alphaBetaKillerRotation(Node *n,int val, int val2,vector<vector<int>> &k,vector<vector<vector<int>>> &r);
int findDiff(vector<int> b1,vector<int>b2);
void printNodeState(Node* n);
int checkWin(vector<int> board,int player);
void printNodeState2(vector<int> board,int player);
void swap(int &i,int&j);
vector<int> rotation(vector<int> b,int degree);

int main(int argc, const char * argv[])
{
    vector<int> b;
    
    vector<vector<int>> killerTable(9);
    vector<vector<vector<int>>> R(9);
    
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
    ::count=0;
    ::alphaCount=0;
    ::betaCount=0;
    cout<<"Running with Killer Heuristic:"<<endl;
    res = alphaBetaKiller(root, INT_MIN, INT_MAX,killerTable);
    cout<<"Game Result:"<<res<<endl;
    cout<<"Moves considered with alpha-beta pruning:"<<::count<<endl;
    cout<<"Alpha cuts: "<<::alphaCount<<endl;
    cout<<"Beta cuts: "<<::betaCount<<endl;
    cout<<"--------------------------------------------"<<endl;
    ::count=0;
    ::alphaCount=0;
    ::betaCount=0;
    vector<vector<int>> killerTable2(9);
     cout<<"Running with Killer Heuristic and using rotation invariance:"<<endl;
    res = alphaBetaKillerRotation(root,INT_MIN, INT_MAX,killerTable2,R);
    cout<<"Game Result:"<<res<<endl;
    cout<<"Moves considered with alpha-beta pruning:"<<::count<<endl;
    cout<<"Alpha cuts: "<<::alphaCount<<endl;
    cout<<"Beta cuts: "<<::betaCount<<endl;
    cout<<"Rotation invariance invoked:"<<::Rcount<<endl;
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
                    child->addParent(temp);
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
                    child->addParent(temp);
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
     ::count++;
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



int findDiff(vector<int> b1,vector<int>b2){
    int position=-1;
    for(int i = 0;i<9;i++){
        if(b1[1]!=b2[i]) position=i;
    }
    return position;
}

int alphaBetaKiller(Node *n,int val, int val2,vector<vector<int>> &killerTable){
    ::count++;
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
    
    //try kill moves
    if(killerTable[n->getDepth()].size()!=0){
        for(int i = 0; i <killerTable[n->getDepth()].size();i++){
            vector<int> b(n->getParents()->getBoard());
            if(b[killerTable.at(n->getDepth()).at(i)]==0){
               b[killerTable.at(n->getDepth()).at(i)]= n->getParents()->getPlayer()==maxPlayer?1:-1;
            
            int res = checkWin(b, n->getPlayer());
            if(n->getPlayer()==maxPlayer){
                alpha = res;
            }
            else {
                beta  = res;
            }
            if(alpha >= beta){
            int ans = player == maxPlayer?alpha:beta;
            printNodeState2(b,player);
            cout<<endl;
            return  ans;
            }
            }
        }
    }
    
    
    for(int i = 0; i<n->getChildren().size();i++){
        if(alpha >= beta){
            int p=findDiff(n->getParents()->getBoard(),n->getBoard());
            killerTable.at(n->getDepth()).push_back(p);
            printNodeState(n);
            cout<<endl<<endl;
            continue;
            
        }
     
        if(player==maxPlayer){
            //find child's value
            temp = alphaBetaKiller(n->getChildren()[i],alpha,INT_MAX,killerTable);
            //compare and update the alpha
            if(temp > alpha){
                alpha = temp;
            }
        }
        else{
            
            temp = alphaBetaKiller(n->getChildren()[i],INT_MIN,beta,killerTable);
            if(temp < beta){
                beta = temp;
            }
            
        }
    }
    //return the current node value
    int ans = player == maxPlayer?alpha:beta;
    return  ans;
}
int alphaBetaKillerRotation(Node *n,int val, int val2,vector<vector<int>> &killerTable,vector<vector<vector<int>>> &r){
    ::count++;
    if(n->getChildren().empty()){
        return n->getResult();
    }
    //board rotation
    if(r[n->getDepth()].size()!=0){
        bool flag = false;
        for(int i = 0; i<r[n->getDepth()].size();i++){
            if(r[n->getDepth()][i]!=n->getBoard()){
                continue;
            }
            else {
                ::Rcount++;
                flag = true;
                break;
            }
        }
        if(flag){
            int ans = n->getResult();
            return ans;
        }
        else{
            
            vector<int> b90 = rotation(n->getBoard(),90);
            vector<int> b180 = rotation(n->getBoard(),180);
            vector<int> b270 = rotation(n->getBoard(),270);
            
            r[n->getDepth()].push_back(n->getBoard());
            r[n->getDepth()].push_back(b90);
            r[n->getDepth()].push_back(b180);
            r[n->getDepth()].push_back(b270);
        }
      
    }
    else{
        
    vector<int> b90 = rotation(n->getBoard(),90);
    vector<int> b180 = rotation(n->getBoard(),180);
    vector<int> b270 = rotation(n->getBoard(),270);
    
    r[n->getDepth()].push_back(n->getBoard());
    r[n->getDepth()].push_back(b90);
    r[n->getDepth()].push_back(b180);
    r[n->getDepth()].push_back(b270);
        
    }
    
    
    int alpha = INT_MIN;
    int beta = INT_MAX;
    //inherit alpha beta from parents node
    alpha = val ==INT_MIN?INT_MIN:val;
    beta  = val2 ==INT_MAX?INT_MAX:val2;
    int temp=0;
    int player = n->getPlayer();
    
    //try kill moves
    if(killerTable[n->getDepth()].size()!=0){
        for(int i = 0; i <killerTable[n->getDepth()].size();i++){
            vector<int> b(n->getParents()->getBoard());
            if(b[killerTable.at(n->getDepth()).at(i)]==0){
                b[killerTable.at(n->getDepth()).at(i)]= n->getParents()->getPlayer()==maxPlayer?1:-1;
                
                int res = checkWin(b, n->getPlayer());
                if(n->getPlayer()==maxPlayer){
                    alpha = res;
                }
                else {
                    beta  = res;
                }
                if(alpha >= beta){
                    int ans = player == maxPlayer?alpha:beta;
                    printNodeState2(b,player);
                    cout<<endl;
                    return  ans;
                }
            }
        }
    }
    
    
    for(int i = 0; i<n->getChildren().size();i++){
        if(alpha >= beta){
            int p=findDiff(n->getParents()->getBoard(),n->getBoard());
            killerTable.at(n->getDepth()).push_back(p);
            printNodeState(n);
            cout<<endl<<endl;
            continue;
            
        }
        
        if(player==maxPlayer){
            //find child's value
            temp = alphaBetaKillerRotation(n->getChildren()[i],alpha,INT_MAX,killerTable,r);
            //compare and update the alpha
            if(temp > alpha){
                alpha = temp;
            }
        }
        else{
            
            temp = alphaBetaKillerRotation(n->getChildren()[i],INT_MIN,beta,killerTable,r);
            if(temp < beta){
                beta = temp;
            }
            
        }
    }
    //return the current node value
    int ans = player == maxPlayer?alpha:beta;
    return  ans;
    
    
    
    

}

int checkWin(vector<int> board,int player){
    
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
        cout<<"  Beta cut" <<endl;
        ::betaCount++;
    }
    else {
        cout<<"  Alpha cut"<<endl;
        ::alphaCount++;
    }
    
}

void printNodeState2(vector<int> board,int player){
    
    for(int i = 0;i< board.size();i++){
        if(i%3==0&&i!=0)
            cout<<endl;
        if(board[i]==1)
            cout<<"X";
        else if(board[i]==-1)
            cout<<"O";
        else
            cout<<"_";
        
    }
    if(player==maxPlayer){
        cout<<"  Beta cut"<<endl;
        ::betaCount++;
    }
    else {
        cout<<"  Alpha cut"<<endl;
        ::alphaCount++;
    }
    
}

void swap(int &i,int&j){
    i = i^j;
    j = i^j;
    i = i^j;
}
vector<int> rotation(vector<int> b,int degree){
    switch (degree) {
        case 90:
            swap(b[0],b[6]);
            swap(b[1],b[3]);
            swap(b[2],b[0]);
            swap(b[3],b[7]);
            //swap(b[4],b[4]);
            swap(b[4],b[1]);
            swap(b[6],b[8]);
            swap(b[7],b[5]);
            swap(b[8],b[2]);
            break;
        case 180:
            swap(b[0],b[8]);
            swap(b[1],b[7]);
            swap(b[2],b[6]);
            swap(b[3],b[5]);
            //swap(b[4],b[4]);
            swap(b[4],b[3]);
            swap(b[6],b[2]);
            swap(b[7],b[1]);
            swap(b[8],b[0]);
            break;
        case 270:
            swap(b[0],b[2]);
            swap(b[1],b[5]);
            swap(b[2],b[8]);
            swap(b[3],b[1]);
            //swap(b[4],b[4]);
            swap(b[4],b[7]);
            swap(b[6],b[0]);
            swap(b[7],b[3]);
            swap(b[8],b[6]);
            break;
        default:
            break;
    }
    return b;
}

