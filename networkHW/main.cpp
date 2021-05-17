#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define MAXDATASIZE 1000
int main(int argc, char* argv[])
{


    if (argc != 2 || strncmp(argv[1], "http://",7)) {//(2) : # of argc check, (3) : front of hostname is http://
        fprintf(stderr, "usage: http_client http://hostname[:port][/path/to/file]\n");
        exit(1);
    }
    argv[1] += 7;
    /*(4) : argv[1] parsing*/
    //sscanf(argv[1], "%[^/]/%s", host, path_to_file);

    //int ret = sscanf(host, "%[^:]:%d", hostname, &portnum);//get hostname, port number from host
    //sprintf(port, "%d", portnum);

    /*getaddrinfo - DNS return IP address*/
    //memset(&hints, 0, sizeof hints);
    //hints.ai_family = AF_UNSPEC;//ipv4 or ipv6
    //hints.ai_socktype = SOCK_STREAM;//TCP
    //if ((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
    //    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    //    return 1;
    //}
    /*make socket*/
    //if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
    //    perror("socket");
    //    return 2;
    //}
    /*(5) TCP connection fail*/
    //if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
    //    close(sockfd);
    //    perror("connect");
    //    exit(1);
    //}

    /*store IP address as sting s*/
    //inet_ntop(servinfo->ai_family, &((struct sockaddr_in*)servinfo->ai_addr) -> sin_addr, s, sizeof s);
    ////printf("client: connecting to %s\n", s);
    
    ///*test*/
    //printf("host : %s\n", host);
    //printf("hostname : %s\n", hostname);
    //printf("portnum : %d\n", portnum);
    //printf("port : %s\n", port);
    //printf("path to file : %s\n", path_to_file);



    //freeaddrinfo(servinfo);

    /*send HTTP request message*/
    //sprintf(buf, "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n", path_to_file, host);
//    printf("GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n", path_to_file, host);
    //if (send(sockfd, buf, strlen(buf), 0) == -1) {
    //    perror("send");
    //    close(sockfd);
    //    exit(1);
    //}//

    /*receive HTTP response message*/
    //if ((numbytes = recv(sockfd, buf, (sizeof buf) - 1, 0)) == -1) {
    //    perror("recv");
    //    close(sockfd);
    //    exit(1);
    //}
    //buf[numbytes] = '\0';
    //print status code
    //strstrªÁøÎ
    //char* ptr;
    //ptr = strstr(buf, "\r\n");
//char temp[32];
    //strncpy(temp, buf, ptr - buf);
    //temp[ptr - buf] = '\0';
    //printf("%s\n", temp);
    //µ•¿Ã≈Õ øµø™ √£±‚
    //char* dataptr = strstr(ptr, "\r\n\r\n");
    //dataptr += 4;//
    //header : content-length √£±‚
    //strnicmp ªÁøÎ
    //int strnicmp(const char *string1, const char *string2, int n);
    //∞∞¿∏∏È 0 ∏Æ≈œ«‘
    //strstrªÁøÎ«ÿ \r\n¿∏∑Œ ≥™¥Æ
    char* oldptr;
    int content_length = -1;
    //while (ptr < dataptr) {
    //    oldptr = ptr;
    //    /ptr = strstr(ptr, "\r\n");
    //    ptr += 2;//ptr¿∫ ¥Ÿ¿Ω¡Ÿ Ω√¿€¡° ∞°∏Æ≈¥
        if (!strncasecmp(oldptr, "content-length", 14)) {
            sscanf(oldptr, "%*[^:]:%d", &content_length);
    //        break;
        }
    //}
    //if (content_length == -1) {
    //    fprintf(stderr, "Content-Length not specified.\n");
    //    return 1;
    //}

    //dataptr µ•¿Ã≈Õ ∫Œ∫– √‚∑¬
    //int recieve_size = strlen(dataptr);
    FILE* output;
    //output = fopen("20171614.out", "w");
    //fprintf(output, "%s", dataptr);

    while (recieve_size < content_length) {//ø¯«œ¥¬ content-length±Ó¡ˆ ∞Ëº” πﬁ¿Ω
        memset(buf, 0, sizeof buf);//initialize
        /*receive HTTP response message*/
        if ((numbytes = recv(sockfd, buf, (sizeof buf) - 1, 0)) == -1) {
            perror("recv");
            close(sockfd);
            exit(1);
        }
        buf[numbytes] = '\0';
        recieve_size += numbytes;

        fprintf(output, "%s", buf);
    }
    fclose(output);
    printf("%d bytes written to 20171614.out\n", recieve_size);
    close(sockfd);
    return 0;
}
