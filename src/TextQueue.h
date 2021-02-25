#ifndef TEXTQUEUE_TEMA3_APD
#define TEXTQUEUE_TEMA3_APD

#include "utils.h"
#include "paragraph.h"

class TextQueue
{
public:
    std::vector<std::queue<Line*>> qs;

    int line_count;
    int index;
    int p;

    TextQueue(int p)
    {
        this->p=p;

        for(int i=0;i<p;i++){
            std::queue<Line*> q;
            qs.push_back(q);
        }
        line_count=0;
        index=0;
    }

    void push(Line* line)
    {

        // printf( REDB "in push" CYN " %d " NC "with line:<%s>\n",index,line.c_str());

        qs[index].push(line);
        line_count++;

        if(line_count==20)
        {
            line_count=0;
            index= (index+1)%p;
        }
    }

};

#endif