#include "server_node.h"

int create_tcp_socket(int portno)
{
    int sockfd; //Socket file descriptors and port number
    struct sockaddr_in serv_addr; ///two objects to store client and server address

    cout << "Hello there! This node is listening on port " << portno << " for incoming connections" << endl;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        error("ERROR opening socket");
    int enable = 1;

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        error("ERROR setsockopt(SO_REUSEADDR) failed");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    return sockfd;
}

void close_tcp_socket(int newsockfd, fd_set& masterfd)
{
    close(newsockfd);
    FD_CLR(newsockfd,&masterfd);
    printf("\nConnection on socket %i closed\n\n",newsockfd);
}

int listen_for_client(int sockfd)
{
    int newsockfd; //Socket file descriptor
    socklen_t clilen; //object clilen of type socklen_t
    struct sockaddr_in cli_addr; ///object to store client address

    //listen(sockfd,10);
    clilen = sizeof(cli_addr);
    if(-1==(newsockfd = accept(sockfd, NULL,NULL)))
    {
        if (EWOULDBLOCK != errno)
        {
            error("ERROR on accept");
        }
    }
    return newsockfd;
}

char *tcp_read(int newsockfd)
{
    int n;
    char* read_buffer = new char[256];
    bzero(read_buffer,256);

    n = read(newsockfd,read_buffer,255);

    if (n < 0)
        error("ERROR reading from socket");
    if ( n==0 )
        return NULL;

    return read_buffer;
}

int tcp_write(int newsockfd, char msg[WRITE_BUFFER_LENGTH])
{
    int n;
    n = write(newsockfd, msg, strlen(msg));//WRITE_BUFFER_LENGTH);
    if (n > 0)
    {
        printf("\nMSG SENT: \n\"%s\"\n", msg);
        return 1;
    }
    if (n < 0)
    {
        printf("\nERROR writing to socket\n\n");
        return -1;
    }

    return 0;
}

int  tcp_str_to_array(char* rcv_data, str_tok_buffer str_tok_buf_tmp)
{
    char* pch;
    int j=1;
    /// str_tok_buf_tmp = {NULL};
    if( (rcv_data == NULL) || (sizeof(rcv_data) == 0) )
    {
        printf("\nWARNING: cannot split array, received msg empty.\n");
        return -1;
    }
    pch = strtok(rcv_data,",");
    str_tok_buf_tmp[0].tok = pch;
    while(pch != NULL)
    {
        pch = strtok(NULL,",");
        str_tok_buf_tmp[j].tok = pch;
        ++j;
    }
    if( strcmp(str_tok_buf_tmp[j-2].tok,"<EOC>") == 0)
    {
        for(int l=0; l<COMMAND_UNIT_LENGTH; ++l)
        {
            if( str_tok_buf_tmp[l].tok == NULL)
                break;
            else
                printf("Data at index [%i]: %s\n",l,str_tok_buf_tmp[l].tok);
        }
        return 1;
    }
    else
    {
        printf("\nWARNING: <EOC> not received, received: %s\n",str_tok_buf_tmp[j-2].tok);
        return -1;
    }
}
