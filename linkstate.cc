//
//  linkstate.cc
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
#define INF 1000000000
using namespace std;
//output file open
ofstream fout("output_ls.txt");
int numNodes;
int network[MAXNODE][MAXNODE];//adjmatix
bool N[MAXNODE][MAXNODE];//dijkstra
struct __massage{
    int s;
    int e;
    char message[MAXMESSAGE];
};
struct __massage msg[MAXLINE];
struct __routertable {
    int next;
    int dist;
    vector<int> history;
};
struct __routertable routerTable[MAXNODE][MAXNODE];//[cur][dest]
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
            for (int i = 0; i < (int)history.size(); i++){
              fout << history[i] << " "; //모든 값 출력 : 0 1 2 3 4
            }
            fout<<"massage"<<msg[line].message<<endl;
        }
        else{
            fout <<"from "<<msg[line].s<<" to "<<msg[line].e<<" cost "<<"infinite"<<" hops unreachable message"<<msg[line].message<<endl;
        }
    }
    fout <<endl;
    
}
bool check(int cur){
    bool ans = true;
    for(int dest=0;dest<numNodes;dest++){
        ans= ans&N[cur][dest];
        if(ans==false){
            break;
        }
    }
    return ans;
}
void linkstate(){
    cout << "LINK start";
    
    for(int cur=0;cur<numNodes;cur++){
        //initialize
        N[cur][cur]= true;
        for(int dest=0;dest<numNodes;dest++){
            if(dest==cur){//자기자신이 목적지일때
                routerTable[cur][dest].next=cur;
                routerTable[cur][dest].dist=0;
                continue;
            }
            if(network[cur][dest]>0){//이읏해 있다면
                continue;
            }
            else{
                routerTable[cur][dest].dist = INF;
                routerTable[dest][cur].dist = INF;
                routerTable[cur][dest].next = INF;
                routerTable[dest][cur].next = INF;
            }
        }
        
        //loop
       
        while(1){
            if(check(cur)){
                break;
            }
            pair<int,int> MIN = make_pair(-1, INF);
            //find w not in N s.t. routertable[w] is minimum;
            for(int dest = 0;dest <numNodes;dest++){
                if(N[cur][dest]){
                    continue;
                }
                if(routerTable[cur][dest].dist<MIN.second){
                    MIN.first = dest;
                    MIN.second = routerTable[cur][dest].dist;
                }
            }
            
            //update
            for(int dest = 0;dest <numNodes;dest++){
                if((network[MIN.first][dest]>0)&&(N[cur][dest]==false&&dest!=MIN.first)){
                    if (routerTable[cur][dest].dist >= MIN.second+routerTable[MIN.first][dest].dist){
                        if(routerTable[cur][dest].dist == MIN.second+routerTable[MIN.first][dest].dist){
                            if(routerTable[cur][dest].next <= MIN.first){
                                continue;
                            }
                        }
                        routerTable[cur][dest].dist = MIN.second+routerTable[MIN.first][dest].dist;
                        routerTable[cur][dest].next = MIN.first;
                        N[cur][MIN.first] = true;
                    }
                }
            }
            
        }
        
    }
    
  
}
void print_table(){
    for(int idx=0;idx<numNodes;idx++){
        for(int jdx=0;jdx<numNodes;jdx++){
            int tmp = routerTable[idx][jdx].dist;
            if (tmp == INF) {
                tmp = 0;
            }
            fout <<jdx<<' '<<routerTable[idx][jdx].next<<' '<<tmp<<endl;
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
        fprintf(stderr, "usage: linkstate topologyfile messagesfile changesfile\n");
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
    cout << "top";
    /*(7) read message file*/
     //
    int linecnt = 0;
    char buffer[MAXMESSAGE];
    while (!fin_message.eof()) {
        int s, e;
        fin_message >> s >> e;
        fin_message.getline(buffer, MAXMESSAGE);
        msg[linecnt].s = s;
        msg[linecnt].e = e;
        strcpy(msg[linecnt].message, buffer);
             cout << buffer << endl;
        memset(buffer, 0, sizeof(buffer));
        linecnt++;
        cout << linecnt << endl;
    }//while file is end
    cout << "end loop" << endl;
    fin_message.close();
    cout << "msg";
    /*make router table*/
    linkstate();
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
        linkstate();
        print_table();
        //send message simulation
        simulation(linecnt);
        
    }//while file is end
    
    /*(5) end of program */
    fin_changes.close();
    //write output file
    fout.close();
    fprintf(stdout,"Complete. Output file written to output_ls.txt.\n");
    return 0;
}
