import os
import networkx
import matplotlib.pyplot as plt

with open('graph.txt') as file:
    lines = [line.rstrip() for line in file]

graph = []
G = networkx.Graph()

i = 0
for line in lines:
    graph.append(line.split(' '))
    G.add_node(i, label=i)
    i += 1

for i in range(len(graph)):
    for j in range(len(graph[i])):
        if (graph[i][j] != '-1') and (graph[i][j] != '0'):
            G.add_edge(i, j, weight=graph[i][j])

plt.figure(figsize=(10, 10))
pos = networkx.random_layout(G)
networkx.draw(G, pos, with_labels=True)
labels = networkx.get_edge_attributes(G, 'weight')
networkx.draw_networkx_edge_labels(G, pos, edge_labels=labels)

num = 0
file_name = f'graph{num}.png'
while os.path.exists(file_name):
    num += 1
    file_name = f'graph{num}.png'

plt.savefig(file_name)
