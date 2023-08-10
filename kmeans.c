#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int checkK(char *k, int N){
    /* if K invalid, return 0. if K valid returns K cast to int */
    char *temp = k;
    int k1;
    if ( k == NULL){
        return 0;
    }
    while (*k) {
        if ( (int)*k < 48 || (int)*k > 57)
            return 0;
        ++k;
    }
    k1 = atoi(temp);
    if (k1 >= N || k1 <= 1)
        return 0;
    return k1;
}

int checkIter(char *iter){
    /* if iter invalid, returns 0. if iter valid, returns iter cast to int */
    char *temp = iter;
    int iter1;
    while (*iter) {
        if ( (int)*iter < 48 || (int)*iter > 57)
            return 0;
        iter++;
    }
    iter1 = atoi(temp);
    if (iter1 >= 1000 || iter1 <= 1)
        return 0;
    return iter1;
}

struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};

void convertVectorToArray(struct cord *head, int vectorSize, double *arr){
    /* turns list of coordinates to an array of doubles */
    int i;
    for (i=0; i<vectorSize; i++){
        arr[i] = head->value;
        head = head->next;
    }
}

double distance(double *vec1, double *vec2,int sizeVec) {
    /* returns the distance between two arrays */
    double sum = 0;
    double distance;
    int i;
    for (i=0; i<sizeVec; i++){
        sum += (vec1[i] - vec2[i]) * (vec1[i] - vec2[i]);
    }
    distance = sqrt(sum);
    return distance;
}

void multiplyByScalar(double scalar, double *vec,int sizeVec){
    /* multiply all elements in vec by scalar */
    int i;
    for (i=0; i<sizeVec; i++){
        vec[i] = scalar*vec[i];
    }
}

void sumArrays(double *arr1, double  *arr2,int sizeVec){
    /* sum arr1 and arr2 into arr1 */
    int i;
    for (i=0; i<sizeVec; i++){
        arr1[i] = arr1[i]+arr2[i];
    }
}

double** Kmeans(double **dataPoints, int iter, int k, int N, int vectorSize) {
    /* implementation of Kmeans algorithm, return array of centroids, if memory allocation failed return NULL */
    double epsilon = 0.001, dist, minDist;
    int iterations, counter, flag, i, j, minK;
    double *classifies, *counterClasses;
    double **sumOfVectors;
    double **centroids = calloc(k, sizeof(double *));

    if (centroids == NULL)
        return NULL;

    iterations = 0;
    counter = 0;
    flag = 1;

    for (i=0; i<k; i++){
        centroids[i] = calloc(vectorSize,sizeof (double));
        if (centroids[i] == NULL) {
            for (j=0; j<i; j++){
                free(centroids[i]);
            }
            free(centroids);
            return NULL;
        }
        for (j=0; j<vectorSize; j++){
            centroids[i][j] = dataPoints[i][j];
        }
    }
    classifies = calloc(N, sizeof(double));
    if (classifies == NULL) {
        for (i=0; i<k; i++){
            free(centroids[i]);
        }
        free(centroids);
        return NULL;
    }
    sumOfVectors = calloc(k,sizeof (double *));
    if (sumOfVectors == NULL) {
        for (i=0; i<k; i++){
            free(centroids[i]);
        }
        free(centroids);
        free(classifies);
        return NULL;
    }
    for (i=0; i<k; i++){
        sumOfVectors[i] = calloc(vectorSize,sizeof (double ));
        if (sumOfVectors[i] == NULL) {
            for (j=0; j<i; j++){
                free(sumOfVectors[i]);
            }
            free(sumOfVectors);
            for (j=0; j<k; j++){
                free(centroids[j]);
            }
            free(centroids);
            free(classifies);
            return NULL;
        }
    }
    counterClasses = calloc(k, sizeof(double));
    if(counterClasses == NULL) {
        for (j=0; j<k; j++){
            free(centroids[j]);
            free(sumOfVectors[i]);
        }
        free(centroids);
        free(classifies);
        free(sumOfVectors);
        return NULL;
    }
    while (iterations<iter && flag == 1){
        for (i=0; i<k; i++){
            counterClasses[i] = 0;
            for (j=0; j<vectorSize; j++){
                sumOfVectors[i][j] = 0;
            }
        }
        for(i=0; i<N; i++){
            classifies[i] = 0;
        }
        for(i=0; i<N; i++){
            minDist = distance(centroids[0], dataPoints[i],vectorSize);
            minK = 0;
            for(j=0; j<k; j++){
                dist = distance(centroids[j], dataPoints[i],vectorSize);
                if (dist < minDist){
                    minDist = dist;
                    minK = j;
                }
            }
            classifies[i] = minK;
            counterClasses[minK] += 1;
            sumArrays(sumOfVectors[minK],dataPoints[i],vectorSize);
        }
        counter = 0;
        for(i=0 ;i<k; i++){
            double scalar = (double)(1/(double)(counterClasses[i]));
            multiplyByScalar(scalar,sumOfVectors[i],vectorSize);
            if (distance(sumOfVectors[i],centroids[i],vectorSize) < epsilon){
                counter += 1;
            }
            for (j=0; j<vectorSize; j++){
                centroids[i][j] = sumOfVectors[i][j];
            }
        }
        if(counter == k){
            flag = 0;
        }
        iterations += 1;
    }
    free(classifies);
    free(counterClasses);
    for (i=0; i<k; i++){
        free(sumOfVectors[i]);
    }
    free(sumOfVectors);
    return centroids;
}

void freeLinkedList(struct vector *head_vec, struct cord *head_cord){
    struct vector *curr_vec, *temp_vec;
    struct cord *curr_cord, *temp_cord;
    curr_vec = head_vec;
    while (curr_vec != NULL){
        curr_cord = curr_vec->cords;
        while (curr_cord != NULL)
        {
            temp_cord = curr_cord;
            curr_cord = curr_cord->next;
            free(temp_cord);
        }
        temp_vec = curr_vec;
        curr_vec = curr_vec->next;
        free(temp_vec);
    }
    free(head_cord);
}


int main(int argc, char *argv[]) {

    struct vector *head_vec, *curr_vec, *temp;
    struct cord *head_cord, *curr_cord;
    int N=0, vectorSize =1, first=1, i, j,flag=1, iter=0, k;
    double n;
    char c;
    double **dataPoints, **centroids;
    char *pointerIter, *pointerk;
    double *array;

    head_cord = malloc(sizeof(struct cord));
    if(head_cord == NULL){
        printf("An Error Has Occurred\n");
        return 1;
    }
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    if (head_vec == NULL){
        free(head_cord);
        printf("An Error Has Occurred\n");
        return 1;
    }
    curr_vec = head_vec;
    curr_vec->next = NULL;

    while (scanf("%lf%c", &n, &c) == 2)
    {
        if (c == '\n')
        {
            first =0;
            N ++;
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            if (curr_vec->next == NULL) {
                printf("An Error Has Occurred\n");
                freeLinkedList(head_vec,head_cord);
                return 1;
            }
            curr_vec = curr_vec->next;
            curr_vec->cords =NULL;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            if (head_cord == NULL) {
                printf("An Error Has Occurred\n");
                freeLinkedList(head_vec,head_cord);
                return 1;
            }
            curr_cord = head_cord;
            curr_cord->next = NULL;
            continue;
        }
        if (first == 1) {
            vectorSize++;
        }
        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord));
        if (curr_cord->next == NULL){
            printf("An Error Has Occurred\n");
            freeLinkedList(head_vec,head_cord);
            return 1;
        }
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
    }

    dataPoints = calloc(N,sizeof (double *));
    if (dataPoints == NULL) {
        freeLinkedList(head_vec,head_cord);
        printf("An Error Has Occurred\n");
        return 1;
    }
    temp = head_vec;
    for (i=0; i<N; i++){
        dataPoints[i] = calloc(vectorSize,sizeof(double));
        if (dataPoints[i] == NULL) {
            printf("An Error Has Occurred\n");
            freeLinkedList(head_vec,head_cord);
            for (j=0; j<i; j++){
                free(dataPoints[j]);
            }
            free(dataPoints);
            return 1;
        }
        array = dataPoints[i];
        convertVectorToArray(temp->cords,vectorSize,array);
        temp = temp->next;
    }

    freeLinkedList(head_vec,head_cord);

    if (argc == 3) {
        pointerk = argv[1];
        k = checkK(pointerk,N);
        pointerIter = argv[2];
        iter = checkIter(pointerIter);
    }
    else{
        if (argc == 2) {
            pointerk = argv[1];
            k = checkK(pointerk,N);
            iter = 200;
        }
        else {
            printf("An Error Has Occurred\n");
            for (i=0; i<N; i++){
                free(dataPoints[i]);
            }
            free(dataPoints);
            return 1;
        }
    }

    if(k == 0){
        printf("Invalid number of clusters!\n");
        flag = 0;
    }
    if(iter == 0){
        printf("Invalid maximum iteration!\n");
        flag = 0;
    }
    if(flag == 0){
        for (i=0; i<N; i++){
            free(dataPoints[i]);
        }
        free(dataPoints);
        return 1;
    }

    centroids = Kmeans(dataPoints,iter,k,N,vectorSize);

    if (centroids == NULL) {
        printf("An Error Has Occurred\n");
        for (j=0; j<i; j++){
            free(dataPoints[j]);
        }
        free(dataPoints);
        return 1;
    }
    for(i=0 ; i<k; i++){
        for(j=0; j<vectorSize;j++){
            if (j <= vectorSize-2) {
                printf("%.4f,", centroids[i][j]);
            }
            else{
                printf("%.4f", centroids[i][j]);
            }
        }
        printf("\n");
    }
    for (i=0; i<k; i++){
        free(centroids[i]);
    }
    free(centroids);
    for (i=0; i<N; i++){
        free(dataPoints[i]);
    }
    free(dataPoints);

    return 0;
}