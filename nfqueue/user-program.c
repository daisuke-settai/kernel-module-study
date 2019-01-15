#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/netfilter.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
#define QUEUE_ID 60000

static void print(const char *buf, int len)
{
  printf("print: len: %d\n", len);
  int i;
  for(i = 0; i < len; ++i) {
    if(i % 0x10 == 0){
      printf("\n");
      printf("0x%04x:  ", i);
    }
    printf("%02x", (unsigned char)buf[i]);
    if(i % 2 == 1)
      printf(" ");
  }
}

int dump(struct nfq_q_handle *qh, struct nfgenmsg *msg, struct nfq_data *nfdata, void *data)
{

  struct nfqnl_msg_packet_hdr *header;
  char *payload;
  int len;

  printf("dump\n");
  header = nfq_get_msg_packet_hdr(nfdata);
  printf("id: %d\n", ntohl(header->packet_id));
  len = nfq_get_payload(nfdata, (unsigned char **)&payload);
  print(payload, len);
  printf("string: %s\n", payload);

  return nfq_set_verdict(qh, ntohl(header->packet_id), NF_ACCEPT, 0, NULL);
}

int main(void)
{
  struct nfq_handle *nfqh;
  struct nfq_q_handle *nfqqh;
  char buf[0x10000];
  int len, fd;

  nfqh = nfq_open();

  if(nfq_unbind_pf(nfqh, PF_INET) < 0) {
    printf("unbind error\n");
  }

  if(nfq_bind_pf(nfqh, PF_INET) < 0) {
    printf("bind error\n");
  }

  nfqqh = nfq_create_queue(nfqh, QUEUE_ID, &dump, NULL);
  if(!nfqqh) {
    printf("error: nfq_create_queue\n");
  }


  nfq_set_mode(nfqqh, NFQNL_COPY_PACKET, sizeof(buf));

  fd = nfq_fd(nfqh);
  printf("start read from queue(loop)\n");
  while(1) {
    if((len = read(fd, buf, sizeof(buf))) < 0){
      printf("len: %d\n", len);
      break;
    } else {
      printf("handle\n");
      nfq_handle_packet(nfqh, buf, len);
      memset(buf, 0, sizeof(buf));
    }
  }

  nfq_unbind_pf(nfqh, PF_INET);
  nfq_close(nfqh);

  return 0;
}

