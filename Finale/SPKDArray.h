typedef struct sp_kd_array
{
    int dim; //dim for each points
    int size; //number of points
    SPPoint* points;
    int** pointsMat;
} SPKDArray;

typedef struct split_res
{
    SPKDArray *kdLeft;
    SPKDArray *kdRight;
} SplitRes;

SPKDArray* init(SPPoint* arr,int size);
SPKDArray* spKDArrayCreate(SPPoint* points,int dim,int size);
int pntCmp(const void* pntA,const void* pntB);
SplitRes* split(SPKDArray *kdArr,int coor);
void printKDARR(SPKDArray *k);
