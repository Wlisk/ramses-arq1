### Compile (linux based):

```gcc main.c ramses.c random.c error.c generate.c -o ramses -Wall```

### Compile (windows):

```recommend the use of CodeBlocks IDE```

### Execute (linux based): 

```./ramses <filename>.mem```

# Description

This program is based on the specifics from the doc 'TrabalhoRamses_2022_1.pdf' (pt-BR).

It was created to help create random tests to be used as a test for the main college work. 

It basically receives a ramses .mem file as input and as output returns a new .mem file with random nodes (triples) at random positions from address x80 to address xEF. 


# Features: 

```<program> --help``` : Prints a help message.

[![program help](https://i.postimg.cc/Xq3DtGzm/Screenshot-from-2022-07-31-08-49-56.png)](https://postimg.cc/phGZFTxJ)
&nbsp;

```<program> <ramses-file>.mem -n <number>``` : Create N number of new ramses .mem file each randomly different.

&nbsp;

```<program> <ramses-file>.mem --print``` : Prints the memory of a ramses .mem file.

[![print .mem file](https://i.postimg.cc/sgxf67k1/Screenshot-from-2022-07-19-16-34-57.png)](https://postimg.cc/qNfdghMd)
&nbsp;

```<program> <ramses-file>.mem --stdout``` : Prints the generation information of each new ramses .mem file created. If ```--stdout``` is omitted, then a file new_\<number\>.info.txt with the information will be created.

[![print generation info nodes](https://i.postimg.cc/5tF411NV/Screenshot-from-2022-07-19-16-35-41.png)](https://postimg.cc/Z94tFGd7) 
[![print generation info memory](https://i.postimg.cc/4NBNQBFr/Screenshot-from-2022-07-19-16-36-11.png)](https://postimg.cc/tYnjXtb5)

&nbsp;

```<program> <ramses-file>.mem --daedalus``` : Prints the information about nodes in Daedalus assembler format and don't generate the new .mem files
