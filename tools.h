#ifndef TOOLS_H
#define TOOLS_H

#include <QDebug>
#include <string>
#include <fstream>
#include <map>
using namespace std;
class Tool
{
public:
     Tool(string fileName);
     ~Tool();
     string fileName;
     ifstream* readFile(string fileName);
     static void getMaxValue(map<string,int> &tmpMap,int &maxValue);
     void parseLog(string textline,QVector<QString> &outVector);
     static QString parseTimeString(QString timestr);
     static void parseLoginString(QVector<QString> loginVector,map<string,int> &loginInfo);


};
#endif // TOOLS_H
