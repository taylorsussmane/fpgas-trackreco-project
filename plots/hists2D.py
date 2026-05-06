import matplotlib.pyplot as plt
import csv
import numpy as np

# Read in Data
eventID = []
layerID = []
x = []
y = []
z = []
t = []

with open('../trackerInputs_SingleHitEvents.csv', mode='r') as file:
#with open('trackerInputs_events.csv', mode='r') as file:
    reader = csv.reader(file)
    next(reader) #skipping header row
    for row in reader:
        eventID.append(row[0])
        layerID.append(row[1])
        x.append(row[2])
        y.append(row[3])
        z.append(row[4])
        t.append(row[5])

# Convert Lists to Arrays
arr_eventID = np.array([int(i) for i in eventID])
arr_layerID = np.array([int(i) for i in layerID]) 
arr_x = np.array([float(i) for i in x])
arr_y = np.array([float(i) for i in y])
arr_z = np.array([float(i) for i in z])
arr_t = np.array([float(i) for i in t])

print("type(arr_eventID) = ", type(arr_eventID))
print("len(arr_eventID) = ", len(arr_eventID))
print("len(arr_x) = ", len(arr_x))
print("len(arr_y) = ", len(arr_y))
print("len(arr_z) = ", len(arr_z))
print("len(arr_t) = ", len(arr_t))

# Create a grid of 2x3 (2 rows, 3 columns)
fig, axes = plt.subplots(2,3)#, sharex=True, sharey=True)
fig.suptitle('Event 0, xy-plane')

# Make xy arrays per layer
xlayer0 = []
ylayer0 = []
zlayer0 = []
xlayer1 = []
ylayer1 = []
zlayer1 = []
xlayer2 = []
ylayer2 = []
zlayer2 = []
xlayer3 = []
ylayer3 = []
zlayer3 = []
xlayer4 = []
ylayer4 = []
zlayer4 = []

# Access plots using coordinates [row, col]
# Filling the hist using event_ID
for i in range(len(arr_eventID)):
    #if i < 3: print(i)
    #if i == 0: print("i == 0")
    if arr_eventID[i]==0 and arr_layerID[i]==0:
        xlayer0.append(arr_x[i])
        ylayer0.append(arr_y[i])
        zlayer0.append(arr_z[i])
        print("Layer 0 (x,y) = (", arr_x[i], ",", arr_y[i],")")
        print("Filled Layer 0")
    if arr_eventID[i]==0 and arr_layerID[i]==1:
        xlayer1.append(arr_x[i])
        ylayer1.append(arr_y[i])
        zlayer1.append(arr_z[i])
        print("Filled Layer 1")
    if arr_eventID[i]==0 and arr_layerID[i]==2:
        xlayer2.append(arr_x[i])
        ylayer2.append(arr_y[i])
        zlayer2.append(arr_z[i])
        print("Filled Layer 2")
    if arr_eventID[i]==0 and arr_layerID[i]==3:
        xlayer3.append(arr_x[i])
        ylayer3.append(arr_y[i])
        zlayer3.append(arr_z[i])
        print("Filled Layer 3")
    if arr_eventID[i]==0 and arr_layerID[i]==4:
        xlayer4.append(arr_x[i])
        ylayer4.append(arr_y[i])
        zlayer4.append(arr_z[i])
        print("Filled Layer 4")

# Find Min/Max of xyz points
xmax = np.maximum.reduce([xlayer0, xlayer1, xlayer2, xlayer3, xlayer4])
xmin = np.minimum.reduce([xlayer0, xlayer1, xlayer2, xlayer3, xlayer4])

ymax = np.maximum.reduce([ylayer0, ylayer1, ylayer2, ylayer3, ylayer4])
ymin = np.minimum.reduce([ylayer0, ylayer1, ylayer2, ylayer3, ylayer4])

zmax = np.maximum.reduce([zlayer0, zlayer1, zlayer2, zlayer3, zlayer4])
zmin = np.minimum.reduce([zlayer0, zlayer1, zlayer2, zlayer3, zlayer4])

#print("y max = ", xmax)
#print("y min = ", xmin)

# Fill Histograms
#axes[0,0].set_xlim(xmin, xmax)
#axes[0,0].set_ylim(ymin, ymax)
axes[0,0].hist2d(xlayer0, ylayer0, bins=(50,50),cmap=plt.cm.jet) #Layer ID 1
axes[0,0].set_title('Layer ID 0')
axes[0,1].hist2d(xlayer1, ylayer1, bins=(50,50),cmap=plt.cm.jet) #Layer ID 1
axes[0,1].set_title('Layer ID 1')
axes[0,2].hist2d(xlayer2, ylayer2, bins=(50,50),cmap=plt.cm.jet) #Layer ID 2
axes[0,2].set_title('Layer ID 2')
axes[1,0].hist2d(xlayer3, ylayer3, bins=(50,50),cmap=plt.cm.jet) #Layer ID 3
axes[1,0].set_title('Layer ID 3')
axes[1,1].hist2d(xlayer4, ylayer4, bins=(50,50),cmap=plt.cm.jet) #Layer ID 4
axes[1,1].set_title('Layer ID 4')

# Visualize Plots
fig.supylabel('x-position')
fig.supylabel('y-position')
plt.tight_layout()
plt.savefig('Event0_1Hitxy.pdf')

plt.close()

# New figure showing yz-plane
# Create a grid of 2y3 (2 rows, 3 columns)
fig, axes = plt.subplots(2,3)#, sharex=True, sharey=True)
fig.suptitle('Event 0, xz-plane')

# Fill histograms
#axes[0,0].set_xlim(xmin,xmax)
#axes[0,0].set_ylim(zmin, zmax)
axes[0,0].hist2d(xlayer0, zlayer0, bins=(50,50),cmap=plt.cm.jet) #Layer ID 0
axes[0,0].set_title('Layer ID 0')
axes[0,1].hist2d(xlayer1, zlayer1, bins=(50,50),cmap=plt.cm.jet) #Layer ID 1
axes[0,1].set_title('Layer ID 1')
axes[0,2].hist2d(xlayer2, zlayer2, bins=(50,50),cmap=plt.cm.jet) #Layer ID 2
axes[0,2].set_title('Layer ID 2')
axes[1,0].hist2d(xlayer3, zlayer3, bins=(50,50),cmap=plt.cm.jet) #Layer ID 3
axes[1,0].set_title('Layer ID 3')
axes[1,1].hist2d(xlayer4, zlayer4, bins=(50,50),cmap=plt.cm.jet) #Layer ID 4
axes[1,1].set_title('Layer ID 4')

# Visualize Plots
fig.supylabel('x-position')
fig.supylabel('z-position')
plt.tight_layout()
plt.savefig('Event0_1Hitxz.pdf')

plt.close()


