
/****************************************************************************
   
    OS/A65 Version 1.3.11
    Multitasking Operating System for 6502 Computers

    Copyright (C) 1989-1997 Andre Fachat 

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
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netdb.h>

#include "fstcp.h"
#include "oa1fs.h"


typedef struct {
	int	state;
	int	fd;
} File;

File files[MAXFILES];

void usage(void) {
	printf("Usage: fstcp [options] exported_directory\n"
		" options=\n"
		"   -ro		export read-only\n"
	);
	exit(1);
}

void do_cmd(char *buf, int fd) {
	int tfd, cmd;
	char retbuf[4];

	cmd = buf[FSP_CMD];
	tfd = buf[FSP_FD];

	printf("got cmd=%d, fd=%d, name=%s\n",cmd,tfd,buf+FSP_DATA);

	retbuf[FSP_CMD] = FS_REPLY;
	retbuf[FSP_DATA] = -22;
	retbuf[FSP_LEN] = 4;
	retbuf[FSP_FD] = tfd;

	write(fd, retbuf,4);	
	printf("write %02x %02x %02x %02x\n", retbuf[0], retbuf[1],
			retbuf[2], retbuf[3]);
}

int main(int argc, char *argv[]) {
	int sock, err;
	struct sockaddr_in serv_addr, client_addr;
	int client_addr_len;
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

	for(i=0;i<MAXFILES;i++) {
	  files[i].state = F_FREE;
	}

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
	  client_addr_len = sizeof(client_addr);
	  fd = accept(sock,(struct sockaddr*)&client_addr, &client_addr_len);
	  if(fd<0) {
	    fprintf(stderr, "Could not accept, errno=%d (%s)!\n",
						errno, strerror(errno));
	    return 2;
	  }
	  if(fork()==0) {
	    char buf[8192];
	    int wrp,rdp, plen;
	    int n;

	    close(sock);

	    printf("ok, got connection to %04lx, port %d\n", 
			client_addr.sin_addr.s_addr, client_addr.sin_port);

	    wrp = rdp = 0;

	    while((n=read(fd, buf+wrp, 8192-wrp))!=0) {
printf("read %d bytes: ",n);
for(i=0;i<n;i++) printf("%02x ",buf[wrp+i]); printf("\n");

	      if(n<0) {
		fprintf(stderr,"fstcp: read error %d (%s)\n",errno,strerror(errno));
		break;
	      }
	      wrp+=n;
	      if(wrp==8192 && rdp) {
		if(rdp!=wrp) {
		  memmove(buf, buf+rdp, wrp-rdp);
		}
		wrp -= rdp;
		rdp = 0;
	      }
printf("wrp=%d, rdp=%d, FSP_LEN=%d\n",wrp,rdp,FSP_LEN);
	      if(wrp-rdp > FSP_LEN) {
		plen = buf[rdp+FSP_LEN];
printf("wrp-rdp=%d, plen=%d\n",wrp-rdp,plen);
		if(wrp-rdp <= plen) {
		  do_cmd(buf+rdp, fd);
		}
	      }
	    }
	    exit(0);
	  }
	}
	
	close(sock);
	return 0;	
}

