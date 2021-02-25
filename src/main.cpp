#include "utils.h"
#include "master.h"
#include "paragraph.h"
#include "TextQueue.h"
#include "text_handler.h"
#include "worker.h"

std::mutex mtx;
std::vector<Line> result;

long P = sysconf(_SC_NPROCESSORS_CONF);

bool compare (Line la,Line lb)
{   
    return (la.id) < (lb.id) ;
}

void *worker_function(void *arg)
{

    WorkerThreqadARguments* wa= (WorkerThreqadARguments*) arg;
    
    int genre_id=wa->id;
    std::queue<Line*>* p_queue = wa->p_queue;

    while( !p_queue->empty())
    {
        Line* line=p_queue->front();
        line->text=handleText(genre_id,line->id,line->text);

        mtx.lock();
        result.push_back(*line);
        mtx.unlock();

        p_queue->pop();
    }

    pthread_exit(NULL);
}

void *master_function(void *arg)
{

    MasterThreadArguments* ma= ((MasterThreadArguments*)arg);
    int id=ma->id;
    std::ifstream infile( ma->fileName );
    Paragraph* paragraph=ma->paragraph;

    std::string line;
    
    while (std::getline(infile, line))
    {
        int genre=genreToID(line);
        if(genre==id)
        {
            paragraph->start();
        }
        else if(line == "")
        {
            paragraph->send(id);
        }
        else if(genre == UNDEFINED)
        {
            paragraph->consume(id,line);
        }
    }
    paragraph->send(id);

    int zero=0;
    MPI_Send( &zero , 1 , MPI_INT , id , 0 , MPI_COMM_WORLD );

    pthread_exit(NULL);
}

void send_result()
{
    int text_size=(int)result.size();
    MPI_Send( &text_size , 1 , MPI_INT , MASTER , 0 , MPI_COMM_WORLD );

    for(int i=0;i<text_size;i++)
    {
        int line_size=(int)result[i].text.size();
        MPI_Send( &line_size , 1 , MPI_INT , MASTER , 0 , MPI_COMM_WORLD );

        int line_id=result[i].id;
        MPI_Send( &line_id , 1 , MPI_INT , MASTER , 0 , MPI_COMM_WORLD );
        
        int genre_id=result[i].genre_id;
        MPI_Send( &genre_id , 1 , MPI_INT , MASTER , 0 , MPI_COMM_WORLD );
        
        MPI_Send( result[i].text.c_str() , line_size , MPI_CHAR , MASTER , 0 , MPI_COMM_WORLD);
    }
}

int main (int argc, char *argv[]) {

    int numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);

    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);

    int i;
    
    MPI_Status status;

    if (rank == MASTER)
    {
        pthread_t threads[MASTER_THREAD_COUNT];

        std::string fileName=argv[1];
        Paragraph paragraphs[MASTER_THREAD_COUNT];

        for (i = 0; i < MASTER_THREAD_COUNT; i++) {
            pthread_create(&threads[i], NULL, master_function, (void*) (new MasterThreadArguments(i+1,fileName,&paragraphs[i])) );
        }

        for (long id = 0; id < MASTER_THREAD_COUNT; id++) {
            pthread_join(threads[id], NULL);
        }

        for(int i=1;i<=4;i++)
        {
            int count;
            int line_size;
            int line_id;
            int genre_id;
            std::string s_line;
            
            MPI_Recv(&count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);

            for(int j=0;j<count;j++)
            {
                char s_line[LINE_LENGTH];
                memset(s_line,0,LINE_LENGTH);

                MPI_Recv(&line_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);

                MPI_Recv(&line_id, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                
                MPI_Recv(&genre_id, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                
                MPI_Recv(s_line, line_size, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
                
                std::string str_line(s_line);

                Line line(line_id,genre_id,str_line);
                result.push_back(line);
            }
            MPI_Barrier(MPI_COMM_WORLD);
        }

        std::sort(result.begin(), result.end(), compare);


        int genre=UNDEFINED;
        std::string prefix="";
        std::ofstream myfile;

        std::string s_filename(argv[1]);
        s_filename.replace(s_filename.end()-3,s_filename.end(),"out");

        FILE* fp = fopen(s_filename.c_str(), "w+");

        for(int i=0;i<result.size();i++)
        {
            Line line=result[i];
            if(line.genre_id!=genre)
            {
                genre=line.genre_id;
                fprintf(fp,"%s%s\n",prefix.c_str(),idToGenre(genre).c_str());
                prefix="\n";
            }
            fprintf(fp,"%s\n",line.text.c_str());
        }
        myfile.close();

    }
    else
    {
        int count;
        int line_size;
        int line_id;
        int genre_id;
        std::string line;
        TextQueue textQueue(P);

        while(1){

            // printf(REDB "%d waiting " NC "line count\n",rank);
            MPI_Recv(&count, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);
            // printf(YLL "[%d]" NC "count: %d\n",rank,count);

            if(count==0)
            {
                break;
            }

            for(int i=0;i<count;i++)
            {
                char s_line[LINE_LENGTH];
                memset(s_line,0,LINE_LENGTH);

                MPI_Recv(&line_size, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);
                MPI_Recv(&line_id, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);
                MPI_Recv(&genre_id, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);
                
                MPI_Recv(s_line, line_size, MPI_CHAR, MASTER, 0, MPI_COMM_WORLD, &status);
                
                // printf(YLL "[%d]" CYN " %d" NC " : %s\n",rank,line_id,s_line);
                std::string str_line(s_line);
                textQueue.push(new Line(line_id,genre_id,str_line));
            }
        }

        pthread_t threads[P];

        for(int i=0;i<P;i++)
        {
            pthread_create(&threads[i], NULL, worker_function, new WorkerThreqadARguments(rank,&(textQueue.qs[i])) );
        }
        for (long id = 0; id < P; id++) {
            pthread_join(threads[id], NULL);
        }

        if(rank==1)
        {
            send_result();
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if(rank==2)
        {
            send_result();
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if(rank==3)
        {
            send_result();
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if(rank==4)
        {
            send_result();
        }
        MPI_Barrier(MPI_COMM_WORLD);

    }
 
    MPI_Finalize();
}