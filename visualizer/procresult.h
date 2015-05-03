#ifndef PROCRESULT
#define PROCRESULT

struct procresult
{
    double activity;
    double sending;
    double receiving;
    double activityPercent;
    double sendingPercent;
    double receivingPercent;
    procresult()
    {
        activity=0;
        sending=0;
        receiving=0;
        activityPercent=0;
        sendingPercent=0;
        receivingPercent=0;
    }
};
#endif // PROCRESULT

