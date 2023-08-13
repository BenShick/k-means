# k-means
K-means is a clustering algorhitm. Given n data points and k intial centroids (one for each cluster) in R^d, the algorithm classifies them into k clusters.

In each iteration:
1. Each data point is classified to the cluster in which the centroid is closest to point (the metric we used in our implementation is the standart norm).
2. The centroid of each cluster is updated to be the mean of all points in the cluster.

It can be shown that in each iteration the sum of distances of each point to the cenroid of its cluster is decreases (or stays the same) relative to the previous iteration.
One of the limitaions of this algorithm is that given some sets of data it can get stuck in a none optimum solution.

# about the program
I implemnted this algortihm with a fellow student. The program expects 3 arguments: number of clusters, maximum number of iterations, name of a txt file of the data points (each row in the file should be a vector in R^d).
