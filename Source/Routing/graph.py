import os
import networkx
import matplotlib.pyplot as plt

with open('graph.txt') as file:
    lines = [line.rstrip() for line in file]

graph = []
G = networkx.Graph()

count = int(lines[0])
del lines[0]

types = lines[0].split(' ')
del lines[0]

names = []
for i in range(0, count):
    names.append(lines[i].split(' '))

del lines[0:count]

i = 0
for line in lines:
    graph.append(line.split(' '))
    G.add_node(i, label=names[i])
    i += 1

for i in range(len(graph)):
    for j in range(len(graph[i])):
        if (graph[i][j] != '-1') and (graph[i][j] != '0'):
            G.add_edge(i, j, weight=graph[i][j])

fig = plt.figure(figsize=(10, 10))
pos = networkx.random_layout(G)
networkx.draw(G, pos=pos, with_labels=False,node_color=types)
# networkx.draw_networkx_labels(G, pos, names)
labels = networkx.get_edge_attributes(G, 'weight')
networkx.draw_networkx_edge_labels(G, pos, edge_labels=labels)
fig.set_facecolor('#8cc4ff')

num = 0
file_name = f'graph{num}.png'
while os.path.exists(file_name):
    num += 1
    file_name = f'graph{num}.png'

fig.savefig(file_name)
