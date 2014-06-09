#include "tools.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

Tool::Tool(string fileName)
{
    this->fileName = fileName;
}
Tool::~Tool()
{
}
ifstream* Tool::readFile(string fileName)
{
    ifstream *infile = new ifstream(fileName.data());
    if( !infile )
    {
        qDebug() << "cannot open input file "; // << fileName << endl;
        return NULL;
    }
    return infile;


}

void Tool::parseLog(string textline,QVector<QString> &outVector)
{
    int spaceNum = 0;
    char buf[20];

    //string tmp = textline.substr(0,16);
   // string tmp1 = textline.substr(16,string::npos);

   //qDebug() << QString(tmp1.data());
     outVector.clear();
   //qDebug() << QString(textline.data());
    for(int i=0; i < textline.length()-1; i++)
    {
         if(textline[i] == ' ' && textline[i+1] == ' ')
         {
              textline.erase(i, 1);
              i=i-1;
         }
         if(textline[i] == '[' && textline[i+1] == ' ')
         {
              textline.erase(i+1, 1);
         }
    }
        //qDebug() << QString(textline.data());
    QString tmpStr(textline.data());
    QStringList sl = tmpStr.split(" ");
    outVector.push_back(sl[0]+ " " + sl[1]+ " " + sl[2]); //date and time
    outVector.push_back(sl[3]); //hostname
    outVector.push_back(sl[4]); //service name
    QString level;
    QString msg="";
    if(sl[5].startsWith("[") || sl[5].startsWith("<"))
    {
         level=sl[5];

         if(sl[6].startsWith("[") || sl[6].startsWith("<"))
         {
             level=level+sl[6];
             for(int i=7; i < sl.length();i++)
                 msg=msg+sl[i];
         }
         else
         {
             for(int i=6; i < sl.length();i++)
                 msg=msg+sl[i];
         }
     }
     else
     {
         for(int i=5; i < sl.length();i++)
             msg=msg+sl[i];
     }
     //qDebug() << level;
     outVector.push_back(level); //level
     outVector.push_back(msg); //msg

}
QString Tool::parseTimeString(QString timestr)
{
    string month[] = {"Jan","Feb","Mar","Apr","May","Jun",
                      "Jul","Aug","Sep","Oct","Nov","Dec"};
    QStringList slist = timestr.split(" ");
   // qDebug() << slist[0] << " " << slist[1] << " " << slist[2] ;
    int index = 0;
    for( int i = 0; i < 12; i++ )
    {
        qDebug() << month[i].data();
        if( QString(month[i].data()) == slist[0] )
        {
            index = i+1;
            break;
        }
    }
    char t[8];
    string tmp;
    sprintf(t,"%d",index);
    tmp = t;
  //  qDebug() << QString("2014-").append(QString(tmp.data())).append(QString("-")).append(slist[1]);
    return QString("2014-").append(QString(tmp.data())).append(QString("-")).append(slist[1]).append(QString(" ")).append(slist[2]);

}

void Tool::parseLoginString(QVector<QString> loginVector,map<string,int> &loginInfo)
{

    if( loginVector[2].indexOf( "login" ) != -1 )
    {
        QStringList tmpList = loginVector[4].split(",");
        QStringList tmpList1 = tmpList[0].split("FOR");
        QString tmpName = tmpList1[1];
        //qDebug() << tmpList1[0].size();
        QString tmpDigit;
        for( int i = 0;i < tmpList1[0].size(); i++ )
        {
            if( tmpList1[0][i].isDigit())
            {
                tmpDigit.append(tmpList1[0][i]);
            }
        }
        loginInfo[tmpName.toStdString()] = tmpDigit.toInt();

    }
}

void Tool::getMaxValue(map<string,int> &tmpMap,int &maxValue)
{
     maxValue = 0;
     map<string,int>::const_iterator it = tmpMap.begin();
     while( it != tmpMap.end() )
     {
         if( it->second > maxValue )
         {
             maxValue = it->second;
         }
         it++;
     }
}
