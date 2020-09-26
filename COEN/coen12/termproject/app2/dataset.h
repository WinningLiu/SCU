#ifdef DATASET_H
#define DATASET_H

typdef struct set SET;

SET *createDataSet(int maxElts);
void destroyDataSet(SET *sp);
int searchID(SET *sp, int ID);
void insertion(SET *sp, int ID, int age);
void deletion(SET *sp, int ID);

#endif
