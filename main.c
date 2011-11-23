
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <neuro.h>

int main(int argc, char **argv)
{

  if(argc != 2) {
    err(1, "usage: %s <node count>", argv[0]);
  }

  network_t net;

  network_create(&net);

  int nodes = atoi(argv[1]);
  int links = nodes * nodes;
  int node_size = nodes * sizeof(int) * sizeof(double) * 2;
  int link_size = links * sizeof(int) * 2 * 
    sizeof(double) * sizeof(double*) * 2;
  double mb = (node_size + link_size) / (1024.0*1024.0);
  
  for(int a = 0; a < nodes; a++) {
    for(int b = a; b < nodes; b++) {
      network_link(net, a, b, 1.0);
    }
  }

  fflush(stdout);
  network_activate(net);

  double signal = network_output(net, 2);

  network_destroy(net);

  return 0;  
}
