/****************************************************************************
 * lsrv2.c  
 * License server server program version 2 - features ticket recycling
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/errno.h>

#define	MSGLEN		128

#define RECLAIM_INTERVAL 5		/* reclaim every 60 seconds */
int main(int ac, char *av[])
{
	struct sockaddr client_addr;
	socklen_t addrlen;
	struct ipTicket
	{
		char      buf[MSGLEN];
	}
	ipTicket ipTicket1;
	int	  ret, sock;
	void	  ticket_reclaim();	/* version 2 addition */
	void shut_down();
	void print_tickets();
	unsigned  time_left;

	sock = setup();
	signal(SIGALRM, ticket_reclaim); /* run ticket reclaimer */
	signal(SIGHUP, print_tickets);  /* install handler */
	signal (SIGQUIT, shut_down);   /* install handler */
	alarm(RECLAIM_INTERVAL);	 /* after this delay     */

	while(1) {
		addrlen = sizeof(client_addr);
		ret = recvfrom(sock,ipTicket1,MSGLEN,0,
				(struct sockaddr *)&client_addr,&addrlen);
		if ( ret != -1 ){
			buf[ret] = '\0';
			narrate("GOT:", ipTicket1.buf, &client_addr);
			time_left = alarm(0);
			handle_request(ipTicket1.buf,&client_addr,addrlen);
			alarm(time_left);
		}
		else if ( errno != EINTR )
			perror("recvfrom");
	}
}