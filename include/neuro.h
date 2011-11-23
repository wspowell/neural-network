
#ifndef NEURO_NETWORK_H
#define NEURO_NETWORK_H

typedef struct network *network_t;

void network_create(struct network **netp);
void network_destroy(struct network *net);
void network_link(struct network *net, int from_id, int to_id, double weight);
void network_input(struct network *net, int id, double signal);
double network_output(struct network *net, int id);
void network_activate(struct network *net);

#endif

