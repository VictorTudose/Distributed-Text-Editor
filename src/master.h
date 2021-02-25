#ifndef MASTER_TEMA3_APD
#define MASTER_TEMA3_APD

#include "utils.h"
#include "paragraph.h"

class MasterThreadArguments
{
public:
    int id;
    std::string fileName;
    Paragraph* paragraph;
    MasterThreadArguments(int id,std::string fileName,Paragraph* paragraph)
    {
        this->id=id;
        this->fileName=fileName;
        this->paragraph=paragraph;
    }
};

#endif