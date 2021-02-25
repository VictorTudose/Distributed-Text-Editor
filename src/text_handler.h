#ifndef TEXT_HANDLER_TEMA3_APD
#define TEXT_HANDLER_TEMA3_APD
#include "utils.h"

std::string consonants="qwrtypsdfghjklzxcvbnmQWRTYPSDFGHJKLZXCVBNM";
std::string letter="qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";

std::string handleText(int genre,int id,std::string input)
{
    std::string output="";

    bool en=true;
    static int wc=0;
    static int id_curr=-1;
    int count=0;

    std::stack<char> rev;

    if(id_curr!=id+1) {
        wc=0;
    }
    else
    {
        wc+=1;
        wc%=7;
    }

    id_curr=id;

    switch(genre)
    {
        case HORROR:

            for(int i=0;i<input.size();i++)
                {
                output+=input[i];
                if (consonants.find(input[i]) != std::string::npos)
                {
                    output+=tolower(input[i]);
                }
            }
            break;
        case COMEDY:

            for(int i=0;i<input.size();i++)
            {
                if (isalpha(input[i]))
                {
                    count++;
                }
                else{
                    count=0;
                }
                if(count%2==0)
                {
                    output+=toupper(input[i]);   
                }
                else
                {
                    output+=input[i];
                }
            }        
            break;
        
        case FANTASY:

            for(int i=0;i<input.size();i++)
            {
                if(en)
                {
                    output+=toupper(input[i]);
                    en=false;
                }
                else
                {
                    if(input[i]==' ')
                    {
                        en=true;
                    }
                    output+=input[i];
                }
            }   
            break;

        case SCIENCE_FICTION:

            for(int i=0;i<input.size();i++)
            {
                if(wc==6)
                {
                    if(input[i]==' '){
                        wc=0;
                        while(!rev.empty())
                        {
                            output+=rev.top();
                            rev.pop();
                        }
                        output+=' ';
                    }
                    else{
                        rev.push(input[i]);
                    }
                }
                else
                {
                    output+=input[i];
                    if(input[i]==' '){
                        wc++;
                    }
                }
            }

            break;
        
        default:
            break;
    }
    return output;
}

#endif