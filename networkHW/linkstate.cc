#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
//(15) 노드의 개수는 최대 100개이며, 링크 cost는 양수이며 최대 값은 100, 메시지 스트링의 최대 길이는 1000 byte로 가정하면 된다. Messages와 changes file은 100라인을 넘지 않는다.
#define MAXNODE 100
#define MAXCOST 100
#define MAXMESSAGE 1001
#define MAXLINE 100

int network[MAXNODE][MAXNODE];//adjmatix
struct __massage{
    int s;
    int e;
    char msg[MAXMESSAGE];
};
struct __massage list[MAXLINE];
using namespace std;
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
    int numNodes;
    fin_topology >> numNodes;
    while(!fin_topology.eof()){
        int s, e, weight;
        fin_topology >>s>>e>>weight;
        network[s][e] = weight;
        network[e][s] = weight;
    }//while file is end
    /*(7) read message file*/
    //graph form as adjmatrix
    
    char buffer[MAXMESSAGE];
    while(!fin_message.eof()){
        int s, e;
        fin_message >>s>>e>> buffer;
        
    }//while file is end
    
    
    /*(5) end of program */
    ofstream fout("output_ls.txt");
    //write output file
    fprintf(stdout,"Complete. Output file written to output_ls.txt.\n");
    fin_message.close();
    fin_changes.close();
    fin_topology.close();
    fout.close();
    return 0;
}
