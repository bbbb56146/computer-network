#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define MAXDATASIZE 1000
int main(int argc, char* argv[])
{
    if (argc != 4) {//(3) : # of argc check
        fprintf(stderr, "usage: linkstate topologyfile messagesfile changesfile\n");
        exit(1);
    }
    
    /*(4) : input file open*/
    FILE *fp_topology, *fp_message, *fp_changes;
    if(((fp_topology = fopen(argv[1],"r"))==NULL) ||((fp_message = fopen(argv[2],"r"))==NULL) ||((fp_changes = fopen(argv[3],"r"))==NULL)){
        fprintf(stderr, "Error: open input file.\n");
        exit(1);
    }
    
    /*(5) end of program */
    fprintf(stdout,"Complete. Output file written to output_ls.txt.\n");
    return 0;
}
