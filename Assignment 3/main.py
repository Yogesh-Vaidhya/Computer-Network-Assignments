class distance_vector_routing:
    def __init__(self, adjacency_list, nodes):      # Class constructor
        self.adjacency_list = adjacency_list
        self.nodes = nodes
        self.routing_table = [
            {
                'distance': [16 for _ in range(nodes)],
                'next_hop': [-1 for _ in range(nodes)],
            } for _ in range(nodes)
        ]
        for i in range(self.nodes):
            self.routing_table[i]['distance'][i] = 0
            self.routing_table[i]['next_hop'][i] = i
            for j in self.adjacency_list[i]:
                self.routing_table[i]['distance'][j] = 1
                self.routing_table[i]['next_hop'][j] = j

    def print_routing_table(self):         # Function for printing routing table
        for i in range(self.nodes):
            print("Routing table of node", i)
            print(self.routing_table[i])

    def update_routing_table(self):         # Function for updation of routing table
        flag = False
        for i in range(self.nodes):
            for j in self.adjacency_list[i]:
                for dest in range(self.nodes):
                    if self.routing_table[i]['next_hop'][dest] != j:
                        if self.routing_table[i]['distance'][dest] + 1 < self.routing_table[j]['distance'][dest]:
                            self.routing_table[j]['distance'][dest] = self.routing_table[i]['distance'][dest] + 1
                            self.routing_table[j]['next_hop'][dest] = i
                            flag = True

                    if self.routing_table[j]['next_hop'][dest] == i and self.routing_table[i]['distance'][dest] == 16:
                        self.routing_table[j]['distance'][dest] = 16
                        self.routing_table[j]['next_hop'][dest] = j
                        flag = True
        return flag

    def break_link(self, link):         # Function for link breaking and changing adj_list and routing table
        self.adjacency_list[link[0]].remove(link[1])
        self.adjacency_list[link[1]].remove(link[0])
        for dest in range(self.nodes):
            if self.routing_table[link[0]]['next_hop'][dest] == link[1]:
                self.routing_table[link[0]]['distance'][dest] = 16
                self.routing_table[link[0]]['next_hop'][dest] = link[0]

        for dest in range(self.nodes):
            if self.routing_table[link[1]]['next_hop'][dest] == link[0]:
                self.routing_table[link[1]]['distance'][dest] = 16
                self.routing_table[link[1]]['next_hop'][dest] = link[1]


if __name__ == '__main__':
    # File Handling
    input_file = open("input.txt", "r+")
    data = input_file.read().split('\n')
    nodes = int(data[0])        # Total number of routers present
    matrix = []
    for i in range(1, nodes + 1):
        ls = data[i].strip().split(' ')
        val = []
        for j in range(nodes):
            val.append(int(ls[j]))
        matrix.append(val)

    link = data[nodes + 1].strip().split(' ')
    link[0] = int(link[0])
    link[1] = int(link[1])

    adjacency_list = {}        # Adjacency List
    for i in range(nodes):
        val = []
        for j in range(nodes):
            if matrix[i][j] == 1:
                val.append(j)
        adjacency_list[i] = val
    input_file.close()

    time = 0                   # Time
    dv_routing = distance_vector_routing(adjacency_list, nodes)     # Object of the class distance_vector_routing
    print("Routing Table at end of time t =", time)
    dv_routing.print_routing_table()
    print('\n')

    while 1:
        if not dv_routing.update_routing_table():                   # Updating routing table
            break
        time += 1
        print("Routing Table at end of time t =", time)
        dv_routing.print_routing_table()
        print('\n')

    print("Answer1 : After time t =", time, "every node can reach every other node in the network.")
    print("Final Routing Table")
    dv_routing.print_routing_table()
    print("\n")

    # Link breaks b-c
    time += 1
    print("Link B-C breaks at time t =",time)
    print("\n")
    dv_routing.break_link(link)
    time+=1
    print("Routing Table at end of time t =", time, "after link b-c breaks")
    dv_routing.print_routing_table()
    print('\n')

    while 1:
        if not dv_routing.update_routing_table():       # Updating routing table
            break
        time += 1
        print("Routing Table at end of time t =", time)
        dv_routing.print_routing_table()
        print('\n')

    print("Answer2 : After time t =", time, "routing tables of all nodes get stabilised.")
    print("Updated routing tables of all nodes after the network stabilises.")
    dv_routing.print_routing_table()
    print("\n")