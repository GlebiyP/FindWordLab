#define NOMINMAX

#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

//#include <chrono>
//#include <execution>

#include <omp.h>
#include <windows.h>

std::string ReadFile(const std::string& FilePath)
{
    std::fstream in(FilePath);
    if (in.is_open())
    {
        std::ostringstream Stream;
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

    for (offset = 0; offset < TextLength - WordLength; ++offset)
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

int main()
{
    //using namespace std::chrono;

    std::string Word;
    std::cin >> Word;

    std::string Text = ReadFile("input.txt");

    const int ThreadCounter = 2;
    const int Incrementer = Text.length() / ThreadCounter;
    int counter = 0;
    double time = 0.0;

    auto Start = omp_get_wtime();
    //long long Start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    #pragma omp parallel for
    for (int i = 0; i < ThreadCounter; i++)
    {
        const auto IterStart = Text.data() + i * Incrementer;
        const int SegmentLength = std::min((int)(Text.data() + Text.length() - IterStart), Incrementer);

        counter += GetWordCount(_strdup(IterStart), SegmentLength, _strdup(Word.data()), Word.length());
    }

    //long long End = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto End = omp_get_wtime();

    std::cout << (End - Start) * 1000 << "ms\n";
    std::cout << counter << '\n';

    return 0;
}