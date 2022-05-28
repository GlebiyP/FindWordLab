#define _USE_MATH_DEFINES

#include "mpi.h"
#include <cmath>
#include <windows.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;

string ReadFile(const string& FilePath) {
    fstream in(FilePath);
    if (in.is_open())
    {
        ostringstream Stream;
        Stream << in.rdbuf();
        return Stream.str();
    }
    else
    {
        return {};
    }
}

int GetWordCount(char* TextIter, int TextLength, char* WordIter, int WordLength)
{
    int counter = 0;
    int offset = 0;

    for (offset = 0; offset < TextLength - WordLength + 1; ++offset)
    {
        int TextOffset = 0;
        int WordOffset = 0;

        while (WordIter + WordOffset != WordIter + WordLength &&
            toupper(*(WordIter + WordOffset)) == toupper(*(TextIter + offset + TextOffset)))
        {
            ++WordOffset;
            ++TextOffset;
        }

        counter += (WordOffset == WordLength);
    }

    return counter;
}


int main(int argc, char* argv[])
{
    int Result;
    double startwtime = 0.0;
    double endwtime;

    int rc;
    if (rc = MPI_Init(&argc, &argv))
    {
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    int CurrentId{};
    int NumProcesses{};

    MPI_Comm_size(MPI_COMM_WORLD, &NumProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &CurrentId);

    string Text;
    string KeyWord = "he";

    char* realText = new char[0];
    int realTextLenght = 0;

    MPI_Status status;
    int value = 0;
    //cout << "Process id: " << CurrentId << endl;

    if (CurrentId == 0)
    {
        Text = ReadFile("input.txt");

        value = Text.length() / NumProcesses;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (CurrentId == 0) {
        startwtime = MPI_Wtime();
    }


    MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);


    char* workText = new char[value + 1];
    if (CurrentId == 0) {
        for (size_t i = 1; i < NumProcesses; i++)
        {
            MPI_Send(Text.c_str() + value * i, value, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
        workText = _strdup(Text.data());
    }
    else
    {
        MPI_Recv(workText, value, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    }


    int counter = GetWordCount(workText, value, _strdup(KeyWord.data()), KeyWord.length());
    //cout << "Process with id: " << CurrentId << " Find: " << KeyWord << " is " << counter << endl;


    MPI_Reduce(&counter, &Result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (CurrentId == 0)
    {
        endwtime = MPI_Wtime();
        std::cout << "Result: " << Result << '\n';
        cout.precision(6);
        std::cout << "Duration: " << (endwtime - startwtime) * 1000 << " ms\n";
    }

    MPI_Finalize();
    return 0;
}