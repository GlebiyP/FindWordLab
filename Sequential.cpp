#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <chrono>

std::string ReadFile(const std::string& FilePath) {
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

int WordCounter(char* TextIter, int TextLength, char* WordIter, int WordLength)
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
    using namespace std::chrono;

    std::cout << "Enter a word\n";
    std::string Word;
    std::cin >> Word;

    std::string Text = ReadFile("input.txt");

    auto Start = steady_clock::now();
    int counter = WordCounter(_strdup(Text.data()), Text.length(), _strdup(Word.data()), Word.length());
    auto End = steady_clock::now();

    std::cout << duration_cast<microseconds>(End - Start).count() / 1000.0 << "ms\n";
    std::cout << counter << '\n';

    return 0;
}