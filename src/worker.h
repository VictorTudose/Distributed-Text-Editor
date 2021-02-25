#ifndef WORKER_TEMA3_APD
#define WORKER_TEMA3_APD

#include "paragraph.h"

class WorkerThreqadARguments{

public:

    int id;
    std::queue<Line*>* p_queue;

    WorkerThreqadARguments(int id,std::queue<Line*>* p_queue)
    {
        this->id=id;
        this->p_queue=p_queue;
    }
};

#endif