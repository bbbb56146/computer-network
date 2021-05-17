//
//  distvec.cpp
//  networkHW
//
//  Created by 김세연 on 2021/05/17.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
//(15) 노드의 개수는 최대 100개이며, 링크 cost는 양수이며 최대 값은 100, 메시지 스트링의 최대 길이는 1000 byte로 가정하면 된다. Messages와 changes file은 100라인을 넘지 않는다.
#define MAXNODE 100
#define MAXCOST 100
#define MAXMESSAGE 1001
#define MAXLINE 100
using namespace std;
//output file open
ofstream fout("output_dv.txt");
int numNodes;
int network[MAXNODE][MAXNODE];//adjmatix
struct __massage{
    int s;
    int e;
    char message[MAXMESSAGE];
};
struct __massage msg[MAXLINE];
struct __routertable {
    int next = -1;
    int dist = 0;
    vector<int> history;
};
struct __routertable routerTable[MAXNODE][MAXNODE];//[cur][dest]

void exchange(int A, int B){
    //A.B table exchange;
    
}
void distvec(){
    //이웃 노드와 테이블 교환
    //split horizon
    for(int cur=0;cur<numNodes;cur++){
        for(int dest=cur;dest<numNodes;dest++){
            if(dest==cur){//자기자신이 목적지일때
                routerTable[cur][dest].next=cur;
                routerTable[cur][dest].dist=0;
                continue;
            }
            if(network[cur][dest]!=0){//이읏해 있다면
                //교환 (테이블 보냄)
                exchange(cur,dest);
            }
        }
    }
    
}
void print_table(){
    for(int idx=0;idx<numNodes;idx++){
        for(int jdx=0;jdx<numNodes;jdx++){
            fout <<jdx<<' '<<routerTable[idx][jdx].next<<' '<<routerTable[idx][jdx].dist<<endl;
        }
        fout <<endl;
    }
}
int main(int argc, char* argv[])
{
    if (argc != 4) {//(3) : # of argc check
        fprintf(stderr, "usage: distvec topologyfile messagesfile changesfile\n");
        exit(1);
    }
    
    /*(4) : input file open*/
    ifstream fin_topology(argv[1]);
    ifstream fin_message(argv[2]);
    ifstream fin_changes(argv[3]);
    if(fin_topology.fail()||fin_changes.fail()||fin_message.fail()){
        fprintf(stderr, "Error: open input file.\n");
        exit(1);
    }
    
    
    /*(6) read topology file*/
    //graph form as adjmatrix
    fin_topology >> numNodes;
    while(!fin_topology.eof()){
        int s, e, weight;
        fin_topology >>s>>e>>weight;
        network[s][e] = weight;
        network[e][s] = weight;
        //set table
        routerTable[s][e].dist= weight;
        routerTable[s][e].next= e;
        routerTable[e][s].dist= weight;
        routerTable[e][s].next= s;
    }//while file is end
    fin_topology.close();
    
    /*(7) read message file*/
    //
    int linecnt=0;
    char buffer[MAXMESSAGE];
    while(!fin_message.eof()){
        int s, e;
        fin_message >>s>>e>> buffer;
        msg[linecnt].s = s;
        msg[linecnt].e = e;
        strcpy(msg[linecnt].message,buffer);
    }//while file is end
    fin_message.close();
    
    /*make router table*/
    distvec();
    print_table();
    /*send message simulation*/
    
    /*read change file*/
    while(!fin_changes.eof()){
        //change cost
        int s, e, cost;
        fin_changes >>s>>e>> cost;
        network[s][e] = cost;
        network[e][s] = cost;
        
        //update router table
        distvec();
        print_table();
        //send message simulation
        
        
    }//while file is end
    
    /*(5) end of program */
    fin_changes.close();
    //write output file
    fout.close();
    fprintf(stdout,"Complete. Output file written to output_dv.txt.\n");
    return 0;
}
