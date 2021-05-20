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
#include <queue>
//(15) 노드의 개수는 최대 100개이며, 링크 cost는 양수이며 최대 값은 100, 메시지 스트링의 최대 길이는 1000 byte로 가정하면 된다. Messages와 changes file은 100라인을 넘지 않는다.
#define MAXNODE 100
#define MAXCOST 100
#define MAXMESSAGE 1001
#define MAXLINE 100
#define INF 1000000000
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
queue<pair<int,int>> q;//queue for waiting list(sender, receiver)
void simulation(int linecnt){
    //현재 라우터 테이블을 기반으로 시뮬 값 출력
    for(int line=0;line<linecnt;line++){
        vector<int> history;
        int hop = 0;
        int cur = msg[line].s;
        while(cur!=msg[line].e){
            history.push_back(cur);
            cur = routerTable[cur][msg[line].e].next;
            if(cur==INF){
                hop = INF;
                break;
            }
            hop++;
        }
        if(hop!= INF){
            fout <<"from "<<msg[line].s<<" to "<<msg[line].e<<" cost "<<hop<<" hops";
            for (int i = 0; i < history.size(); i++){
              fout << history[i] << " "; //모든 값 출력 : 0 1 2 3 4
            }
            fout<<"massage "<<msg[line].message<<endl;
        }
        else{
            fout <<"from "<<msg[line].s<<" to "<<msg[line].e<<" cost "<<"infinite"<<" hops unreachable message "<<msg[line].message<<endl;
        }
    }
    fout <<endl;
    
}
void distvec(){
    //이웃 노드와 테이블 교환
    
    //initialize
    for(int cur=0;cur<numNodes;cur++){
        for(int dest=cur;dest<numNodes;dest++){
            if(dest==cur){//자기자신이 목적지일때
                routerTable[cur][dest].next=cur;
                routerTable[cur][dest].dist=0;
                continue;
            }
            if(network[cur][dest]>0){//이읏해 있다면
                q.push(make_pair(cur, dest));//교환 리스트에 넣음
                q.push(make_pair(dest, cur));//교환 리스트에 넣음
            }
            else{
                routerTable[cur][dest].dist = INF;
                routerTable[dest][cur].dist = INF;
                routerTable[cur][dest].next = INF;
                routerTable[dest][cur].next = INF;
            }
        }
    }
    
    while(!q.empty()){
        int sender = q.front().first;
        int receiver = q.front().second;
        q.pop();
        bool change_flag = false;
        //리시버는 샌더에게 테이블 받고 자신의 테이블에 반영
        for(int dest = 0;dest<numNodes;dest++){
            int poison  = routerTable[sender][dest].dist;
            if(routerTable[sender][dest].next == receiver){
                poison = INF;
            }
            if(routerTable[receiver][dest].dist<=(routerTable[receiver][sender].dist+poison)){
                if(routerTable[receiver][dest].dist==(routerTable[receiver][sender].dist+poison)){
                    //만약 같은 거리라면
                    if(routerTable[receiver][dest].next <= sender){
                        // 기존 테이블의 next가 더 작다면
                        //유지해야함
                        continue;
                    }
            
                }
                    //작은 거리라면
                    //업데이트해야함
                    change_flag = true;
                    routerTable[receiver][dest].dist =routerTable[receiver][sender].dist+poison;
                    routerTable[receiver][dest].next = sender;
            }
        }
        //변경사항
        if(change_flag){
            for(int dest = 0;dest<numNodes;dest++){
                if(network[receiver][dest]>0){
                    q.push(make_pair(receiver, dest));
                }
            }
        }
    }
}
void print_table(){
    for(int idx=0;idx<numNodes;idx++){
        for(int jdx=0;jdx<numNodes;jdx++){
            fout <<jdx<<' '<<routerTable[idx][jdx].next<<' '<<routerTable[idx][jdx].dist<<endl;
            //for test
            cout <<jdx<<' '<<routerTable[idx][jdx].next<<' '<<routerTable[idx][jdx].dist<<endl;
        }
        fout <<endl;
        //test
        cout <<endl;
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
        //set router-table
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
    simulation(linecnt);
    /*read change file*/
    while(!fin_changes.eof()){
        //change cost
        int s, e, cost;
        fin_changes >>s>>e>> cost;
        network[s][e] = cost;
        network[e][s] = cost;
        if(cost>0){
        //set router-table
        routerTable[s][e].dist= cost;
        routerTable[s][e].next= e;
        routerTable[e][s].dist= cost;
        routerTable[e][s].next= s;
        }
        
        //update router table
        distvec();
        print_table();
        //send message simulation
        simulation(linecnt);
        
    }//while file is end
    
    /*(5) end of program */
    fin_changes.close();
    //write output file
    fout.close();
    fprintf(stdout,"Complete. Output file written to output_dv.txt.\n");
    return 0;
}
