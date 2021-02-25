#ifndef PARAGRAPH_TEMA2_APD
#define PARAGRAPH_TEMA2_APD

#include "utils.h"

class Line
{
public:
    std::string text;
    int id;
    int genre_id;

    Line(int line_id,int genre_id,std::string text)
    {
        this->text=text;
        this->id=line_id;
        this->genre_id=genre_id;
    }
};

class Paragraph
{
public:
    bool enable=false;
    std::vector<Line> lines;
    int line_count=0;

    void start()
    {
        enable=true;
    }
    void send(int id)
    {

			if(!enable)
				return;

        int text_size=(int)lines.size();
        MPI_Send( &text_size , 1 , MPI_INT , id , 0 , MPI_COMM_WORLD );
		// printf(CYNB "[sent->%d]" NC"\n",id);

        for(int i=0;i<lines.size();i++)
        {
            // printf(CYN "[send->%d]" NC  "%s\n",id,text[i].c_str());

            int line_size=(int)lines[i].text.size();
            MPI_Send( &line_size , 1 , MPI_INT , id , 0 , MPI_COMM_WORLD );
            int line_id=lines[i].id;
            MPI_Send( &line_id , 1 , MPI_INT , id , 0 , MPI_COMM_WORLD );
            
            int genre_id=lines[i].genre_id;
            MPI_Send( &genre_id , 1 , MPI_INT , id , 0 , MPI_COMM_WORLD );
            
            MPI_Send( lines[i].text.c_str() , line_size , MPI_CHAR , id , 0 , MPI_COMM_WORLD);
        }
        lines.clear();
        enable=false;
    }
    void consume(int genre_id,std::string line)
    {
        Line l_line(line_count++,genre_id,line);
        if(enable){
           lines.push_back(l_line);
        }
    }
};

#endif