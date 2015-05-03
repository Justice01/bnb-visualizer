#ifndef PROCRESULT
#define PROCRESULT

struct procresult
{
    double activity;
    double sending;
    double receiving;
    double activityPercent;
    procresult()
    {
        activity=0;
        sending=0;
        receiving=0;
        activityPercent=0;
    }
};
#endif // PROCRESULT

