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
    for (int i = 0; i < Net.Edges.size(); i++) { // from i to D-1
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
    // Записываем все пути и их велечины
    std::vector<Edge> edges = {
        {0, 1, 16}, {0, 2, 14}, {0, 3, 16}, {1, 3, 15}, {2, 3, 16}, {2, 4, 15}, {3, 4, 10}, {4, 5, 8}, {3, 5, 10}, {4, 6, 10}, {5, 6, 5}, {3, 6, 10}, {6, 7, 20}
         };

    // Создаём Сеть (взвешенный ориентарованный граф)
    Graph* exampleGrapth =  new Graph(edges, 7);

    // Сделать LP файл данной сети, содержащий решение задачи нахождения максимального потока
    MakeLpSolve(*exampleGrapth);

    
}
