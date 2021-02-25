# C++ Distributed Text Editor

Distributed text editor that takes a text divided in paragraphs and applies to each paragraph specific changes

The paragraphs have an associated genre that can be comedy,sci-fi,fantasy or horror

Structure of the system:
- 1 control node
- 4 worker nodes : each node has an associated genre and applies its specific changes to the text
  - horror: doubleing the consonats
  - comedy: making even-positioned letters uppercase
    - example: `eXaMpLe`
  - sci-fi : invert every seventh word
  - fantasy : make all words start with capital letter

How the application works :
1. The control node reads the file in 4 different threads
2. Every thread send the data to the worker node
3. After the worker node has all the data it process the text using maximum P threads
4. After the data was processed it is sent back to the control node that writes it in the text

### Worker node

The worker node uses more threads to apply changes to the paragraphs that it received from the control node

Every thread gets `C/P` or `C/P + 1` paragraphs, where `C` is the total number of paragraphs

They get the paragraphs cyclically meaning that thread 1 will get paragraph `1`, paragraph `P+1` , `2*P+1` , etc.

### Control node

The worker node sort back the text and writes in a file

### Mini Protocol between nodes

Sending a line over MPI
1. send 1 int : line_index
2. send 1 int : genre_id
3. send 1 int : line_size
4. send line_size chars: line_text

Sending a paragraph
1. send 1 int : line_count
2. send line_count lines