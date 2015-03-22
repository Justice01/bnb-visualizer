#ifndef PROCESSOR
#define PROCESSOR

struct processor
{
    int * activity;

    processor(){activity=NULL;}

    ~processor(){ if (activity!=NULL) delete [] activity; }
};

#endif // PROCESSOR

