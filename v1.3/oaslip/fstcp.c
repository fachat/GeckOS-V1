
/****************************************************************************
   
    OS/A65 Version 1.3.11
    Multitasking Operating System for 6502 Computers

    Copyright (C) 1989-1996 Andre Fachat 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

****************************************************************************/

/*
 * This file is a server FSTCP filesystem implementation, to be
 * used with the FSTCP program on an OS/A65 computer. 
 *
 * usage: 
 *   fstcp [options] exported_directory
 *
 *   options:
 * 	-ro	export read-only
 */

#define	PORT	8090


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netdb.h>


void usage(void) {
	printf("Usage: fstcp [options] exported_directory\n"
		" options=\n"
		"   -ro		export read-only\n"
	);
	exit(1);
}

int main(int argc, char *argv[]) {
/*	char text[]="test-string\n";*/
	int sock, err;
/*	char *hname;*/
	struct sockaddr_in serv_addr, client_addr;
	int client_addr_len;
/*	struct hostent host, *p;*/
	int port=PORT;
	int fd;
	int i, ro=0;
	char *dir;

	i=1;
	while(i<argc && argv[i][0]=='-') {
	  switch(argv[i][1]) {
	    case '?':
		usage();
		break;
	    case 'r':
		if(argv[i][2]=='o') {
		  ro=1;
		}
		break;
	  }
	  i++;
	}

	if(i!=argc-1) {
	  usage();
	}

	dir = argv[i++];

	printf("port=%d\n",port);
	
	sock = socket(AF_INET, SOCK_STREAM, 0);

	printf("sock=%d\n",sock);

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(port);
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);

	err = bind(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if(err<0) {
	  fprintf(stderr, "Could not bind!\n");
	  return 2;
	}

	err = listen(sock, 1);
	if(err<0) {
	  fprintf(stderr, "Could not listen!\n");
	  return 2;
	}

	while(1) {
	  fd = accept(sock,(struct sockaddr*)&client_addr, &client_addr_len);
	  if(fd<0) {
	    fprintf(stderr, "Could not accept, errno=%d (%s)!\n",
						errno, strerror(errno));
	    return 2;
	  }
	  if(fork()==0) {
	    char buf[8192];
	    int n;
	    close(sock);

	    printf("ok, got connection to %04lx, port %d\n", 
			client_addr.sin_addr.s_addr, client_addr.sin_port);
	    while((n=read(fd, buf, 8192))!=0) {
	      if(n<0) {
		fprintf(stderr,"fstcp: read error %d (%s)\n",errno,strerror(errno));
		break;
	      }
	      for(i=0;i<n;i++) {
		if(!(i%16)) printf("\n%04x:",i);
		printf("%02x ",buf[i]);
	      }
	      printf("\n");
	    }
	    exit(0);
	  }
	}
	
	close(sock);
	return 0;	
}

