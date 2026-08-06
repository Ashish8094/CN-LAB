#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void calculateParity(int h[])
{
    h[1] = h[3]^h[5]^h[7]^h[9]^h[11];
    h[2] = h[3]^h[6]^h[7]^h[10]^h[11];
    h[4] = h[5]^h[6]^h[7]^h[12];
    h[8] = h[9]^h[10]^h[11]^h[12];
}

int main()
{
    int sock;
    struct sockaddr_in server;
    char msg[20];
    int h[13]={0}, data[9];
    char ch;
    int i,pos;

    printf("Enter ASCII character: ");
    scanf("%c",&ch);

    int ascii=(int)ch;

    for(i=8;i>=1;i--)
    {
        data[i]=ascii%2;
        ascii/=2;
    }

    h[3]=data[1];
    h[5]=data[2];
    h[6]=data[3];
    h[7]=data[4];
    h[9]=data[5];
    h[10]=data[6];
    h[11]=data[7];
    h[12]=data[8];

    calculateParity(h);

    printf("Generated Hamming Code: ");
    for(i=1;i<=12;i++)
        printf("%d",h[i]);
    printf("\n");

    printf("Enter error position (1-12, 0 for no error): ");
    scanf("%d",&pos);

    if(pos>=1 && pos<=12)
        h[pos]^=1;

    for(i=1;i<=12;i++)
        msg[i-1]=h[i]+'0';
    msg[12]='\0';

    sock=socket(AF_INET,SOCK_STREAM,0);

    server.sin_family=AF_INET;
    server.sin_port=htons(9000);
    server.sin_addr.s_addr=inet_addr("127.0.0.11");

    connect(sock,(struct sockaddr*)&server,sizeof(server));

    send(sock,msg,strlen(msg),0);

    close(sock);

    return 0;
}
