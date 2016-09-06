#include "SPPoint.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct sp_point_t{
    double* data;
    int dim;
    int index;
};

SPPoint spPointCreate(double* data, int dim, int index){
    int i;
    if(data == NULL || dim <=0 || index < 0) return NULL;
    struct sp_point_t* res =(struct sp_point_t*) malloc(sizeof(struct sp_point_t));
    if (res == NULL){
        return NULL;
    }

    res->dim = dim;
    res->index = index;

    res->data = (double*)malloc(dim*sizeof(double));
    if (res->data == NULL){
        return NULL;
    }

    for (i = 0; i < dim; i++){
        res->data[i] = data[i];
    }

    return res;
}

SPPoint spPointCopy(SPPoint source){
    assert (source != NULL);
    struct sp_point_t* res = spPointCreate(source->data, source->dim, source->index);
    if(res==NULL) printf("CopyFailed!");
    return res;
}

void spPointDestroy(SPPoint point){
    if (point == NULL) return;
    free(point->data);
    free(point);
}

int spPointGetDimension(SPPoint point){
    assert (point != NULL);
    return point->dim;
}

int spPointGetIndex(SPPoint point){
    assert (point != NULL);
    return point->index;
}

double spPointGetAxisCoor(SPPoint point, int axis){
    assert (point != NULL && axis < point->dim);
    return point->data[axis];
}

double spPointL2SquaredDistance(SPPoint p, SPPoint q){
    assert (p!=NULL && q!=NULL && p->dim == q->dim);

    double res = 0;
    double diff;
    int i;

    for (i = 0; i < p->dim; i++){
        diff = p->data[i] - q->data[i];
        res += diff*diff;
    }

    return res;
}
//testing pointttttttttttttttttttttttttttt
void printPoint(SPPoint p){
    if(p==NULL) printf("NULLL\n");
    int i;
    printf("%d\t",p->index);
    //printf("%d\n",p->dim);
    for(i=0;i<p->dim;i++){
        printf("%f\t",spPointGetAxisCoor(p,i));

    }
    printf("\n");

}

