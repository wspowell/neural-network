
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct node {
  int id;
  double signal;
  double temp;
};

struct link {
  int from_id;
  int to_id;
  
  double *from;
  double *to;
  
  double weight;
};

struct network {
  struct node *nodes;
  struct link *links;

  int node_count;
  int link_count;

  int node_cap;
  int link_cap;

  int compiled;
};

static struct node*
find_node(struct network *net, int id)
{
  if(net == NULL) {
    printf("???");
  }
  for(int k = 0; k < net->node_count; k++) {
    if(net->nodes[k].id == id)
      return &net->nodes[k];
  }
  return NULL;
}

static void
create_node(struct network *net, int id)
{
  if(find_node(net,id))
    return;

  struct node new_node;

  new_node.id = id;
  new_node.signal = 0.0;
  new_node.temp = 0.0;

  if(net->node_count == net->node_cap) {
    net->node_cap *= 3;
    net->node_cap /= 2;
    net->nodes = realloc(net->nodes, sizeof(struct node) * 
                         net->node_cap);
  }

  net->node_count++;
  net->nodes[net->node_count - 1] = new_node;

  net->compiled = 0;
}

static void
compile(struct network *net)
{
  net->node_cap = net->node_count;
  struct node *nodes = malloc(net->node_cap * sizeof(struct node));
  memcpy(nodes, net->nodes, net->node_cap * sizeof(struct node));
  free(net->nodes);
  net->nodes = nodes;

  net->link_cap = net->link_count;
  struct link *links = malloc(net->link_cap * sizeof(struct link));
  memcpy(links, net->links, net->link_cap * sizeof(struct link));
  free(net->links);
  net->links = links;

  for(int k = 0; k < net->link_count; k++) {
    struct link *link = &net->links[k];
    struct node *from_node = find_node(net, link->from_id);
    struct node *to_node = find_node(net, link->to_id);
    
    link->from = &from_node->signal;
    link->to = &to_node->temp;
  }

  net->compiled = 1;
}

static double
sigmoid(double signal)
{
  return 1.0 / (1.0 + exp(-signal));
}

void
network_create(struct network **netp)
{
  struct network *net = malloc(sizeof(struct network));

  net->node_count = 0;
  net->link_count = 0;
  net->compiled = 0;

  net->node_cap = 10;
  net->link_cap = 10;

  net->nodes = malloc(sizeof(struct node) * net->node_cap);
  net->links = malloc(sizeof(struct link) * net->link_cap);

  *netp = net;
}

void 
network_destroy(struct network *net)
{
  free(net->nodes);
  free(net->links);
  free(net);
}

void 
network_link(struct network *net, int from_id, int to_id, double weight)
{
  for(int k = 0; k < net->link_count; k++) {
    struct link *link = &net->links[k];

    if(link->from_id == from_id && link->to_id == to_id) {
      link->weight = weight;
      return;
    }
  }

  create_node(net, from_id);
  create_node(net, to_id);

  struct link new_link;
  new_link.from_id = from_id;
  new_link.to_id = to_id;
  new_link.weight = weight;
  new_link.from = NULL;
  new_link.to = NULL;

  if(net->link_count == net->link_cap) {
    net->link_cap *= 3;
    net->link_cap /= 2;

    net->links = realloc(net->links, sizeof(struct link) *
                         net->link_cap);
  }

  net->link_count++;

  net->links[net->link_count - 1] = new_link;
}

void
network_input(struct network *net, int id, double signal) {
  struct node *node = find_node(net, id);
  
  if(node)
    node->signal = signal;
}

double
network_output(struct network *net, int id) {
  struct node *node = find_node(net, id);
  
  if(node)
    return node->signal;
  else
    return 0.0;
}

void
network_activate(struct network *net) {
  if(!net->compiled) 
    compile(net);

  for(int k = 0; k < net->node_count; k++) {
    net->nodes[k].temp = 0.0;
  }

  for(int k = 0; k < net->link_count; k++) {
    struct link *link = &net->links[k];
    (*link->to) += (*link->from) * link->weight;
  }

  for(int k = 0; k < net->node_count; k++) {
    net->nodes[k].signal = sigmoid(net->nodes[k].temp);
  }
}
