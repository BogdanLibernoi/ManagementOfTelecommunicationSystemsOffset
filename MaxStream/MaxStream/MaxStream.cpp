#include <iostream>
#include <vector>
#include "Graph.h"
#include <fstream>
#include <string>
#include <list>
#include <iterator>
#include <algorithm>


auto GetWaysFrom(Graph& Net, int tempActive, std::string tempStr,
    int current)
{
    std::list< std::pair< int, std::string > > WaysFrom;
    current = 0; tempStr = ""; tempActive = 0;
    for (int i = 0; i < Net.Edges.size(); i++) {
        for (Pair pair : Net.Edges[i]) {

            if (current != i) {
                current = i;
                if (tempStr != "") {
                    tempStr[tempStr.size() - 2] = ' ';
                    WaysFrom.push_back(std::make_pair(tempActive, tempStr));
                    tempStr = "";
                }
            }
            if (current == i) {
                tempStr += "x" + std::to_string(i) + std::to_string(pair.first) + " + ";
                tempActive = i;
            }
        }
    }
    tempStr = tempStr.substr(0, tempStr.size() - 2);
    WaysFrom.push_back(std::make_pair(tempActive, tempStr));

    return WaysFrom;
}
auto GetWaysTo(Graph& Net, int tempActive, std::string tempStr,
    int current)
{
    std::list<std::pair<int, std::string >> waysTo;

    current = 0; tempStr = ""; tempActive = 0;
    for (int i = 0; i < Net.Edges.size(); i++) {
        for (Pair pair : Net.Edges[i]) {
            if (pair.first != tempActive) {
                current = pair.first;
                if (tempStr != "") {
                    tempStr[tempStr.size() - 2] = ' ';
                    waysTo.push_back(std::make_pair(tempActive, tempStr));
                    tempStr = "";
                }

            }
            if (pair.first == current) {
                tempStr += 'x' + std::to_string(i) + std::to_string(pair.first) + " + ";
                tempActive = current;
            }
        }
    }
    tempStr = tempStr.substr(0, tempStr.size() - 2);
    waysTo.push_back(std::make_pair(tempActive, tempStr));

    return waysTo;
}


void MakeLpSolve(Graph& Net) 
{
    std::ofstream stream;
    stream.open("solve.lp");

    std::string strMax;

    std::list<std::pair<std::string, int>> Sx;
    std::list<std::pair<std::string, int>> Dx;
    std::list<std::pair<std::string, int>> Xx;

    // "Последний" узел сети.
    int D = Net.Edges[Net.Edges.size() - 1][0].first;


    // Получаем пути xS?, x?D, X?? их велечины
    for (int i = 0; i < Net.Edges.size(); i++) {
        for (Pair pair : Net.Edges[i]) {
            if (i == 0) {
                Sx.push_back( 
                    std::make_pair(
                        "xS" + std::to_string(pair.first),
                         pair.second )
                );
            }
            if (pair.first == D) {
                Dx.push_back(
                    std::make_pair( 
                        ("x" + std::to_string(i) + "D"),
                        pair.second)
                    );
            }
            if (i != 0 && pair.first != D) {
                Xx.push_back(
                    std::make_pair(
                        ("x" + std::to_string(i) + std::to_string(pair.first)),
                        pair.second)
                        );
            }
        }
    }



    int tempActive = 0; std::string tempStr = "";
    int current = 0;

    // Пути из i
    std::list< std::pair< int, std::string > > WaysFrom = GetWaysFrom(Net, tempActive, tempStr, current);
    // Пути в  i
    std::list<std::pair<int, std::string >> WaysTo = GetWaysTo(Net, tempActive, tempStr, current);
    


    // Печатаем пути из S 
    strMax += "Max: " + WaysFrom.front().second +";";
    std::replace(strMax.begin(), strMax.end(), '0', 'S');
    stream << strMax + "\n\n";


    // Узнать о следующих путях к распечатке
    std::list<int> nextWays;
    for (auto const& next : WaysTo) {
        nextWays.push_back(next.first);
    }
    nextWays.sort();
    nextWays.unique();

    // Распечатать следующие пути
    std::list<std::string> strings;
        for (auto const& wayto : WaysTo) {
            for (auto const& wayfrom : WaysFrom) {
                if (wayto.first == wayfrom.first) {
                    strings.push_back(wayto.second + " = " + wayfrom.second + ";\n");
                }
            }

        }

    // Преобразуем первые и последние пути к правильному виду: 
    char Dchar = ((std::to_string(D))[0]);
    for (auto it = strings.begin(); it != strings.end(); it++) {
        std::replace( (*it).begin(), (*it).end(), '0', 'S');
        std::replace( (*it).begin(), (*it).end(), Dchar, 'D');
    }

    stream << "\n\n";
    for (auto const& str : strings) {
        stream << str + "\n";
    }

    

    stream << "\n\n";
    // Печатаем велечины рёбер (путей)
    for (auto const& str : Sx) {
        stream << str.first + " <= " + std::to_string(str.second) << ";\n";
    }
    for (auto const& str : Xx) {
        stream << str.first + " <= " + std::to_string(str.second) << ";\n";
    }
    for (auto const& str : Dx) {
        stream << str.first + " <= " + std::to_string(str.second) << ";\n";
    }



    stream.close();
}





int main()
{

    std::setlocale(LC_ALL, "rus");

    Graph* randGrapth = new Graph(10);
    std::cout << "Введите размер сети \n";
    int size;
    std::cin >> size;
    randGrapth->RandomCompletion(size);

    std::cout << "Рёбра данной сети и их вес: \n";
    for (int i = 0; i < (*randGrapth).Edges.size(); i++) {
        for (Pair pair : (*randGrapth).Edges[i]) {
            std::cout << "{" + std::to_string(i) + ", " + std::to_string(pair.first) + ", " + std::to_string(pair.second) + "}";
        }


    }
    std::cout << "\nХотите сделать lp файл данной сети?";
    std::cout << "\n Ожидание ввода: (1 - да , 2 - нет)";
    int choice;
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        MakeLpSolve(*randGrapth);
        std::cout << "Файл готов. Расположение: корневая папка проекта.";
    case 2:
        return 0;
    default:
        break;
    }


    // Сделать LP файл данной сети, содержащий решение задачи нахождения максимального потока
    

    
}
