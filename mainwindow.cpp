#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "tools.h"
#include "loginPlot.h"
#include <QDebug>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QFileDialog>
#include <QComboBox>
#include <QToolBar>
#include <QToolBar>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_symbol.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_plot_histogram.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setTableViewHeader();
    ui->tableView->setSortingEnabled(true);
    //addContent();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setTableViewHeader()
{
    log_model = new QStandardItemModel();
    log_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("等级")));
    log_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("日期与时间")));
    log_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("主机名")));
    log_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("服务名称")));
    log_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("消息内容")));

    ui->tableView->setModel(log_model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑单元表格
    ui->tableView->setColumnWidth(0,50);
    ui->tableView->setColumnWidth(1,110);
    ui->tableView->setColumnWidth(2,100);
    ui->tableView->setColumnWidth(3,100);
    ui->tableView->setColumnWidth(4,300);
}

bool MainWindow::addContent()
{

    bool flag = false;
    QString levelStr[] = {"<emerg>","<alert>","<crit>","<error>[1400737688.966608]","<error>[1400737835.491674]","<err>","[warning]","<notice>","<info>","<debug>","[critical]","<warn>","[warning][GLib-GObject]","[warning][Gtk]","[critical][GLib-GObject]","[warning][guestinfo]"};
    Tool tool(filePath.toStdString());
    ifstream *fs = tool.readFile(tool.fileName);
    string textline;
    int i = 0;
    log_model->clear();
    //qDebug() << tool.parseString("Jun 5 06:05:43");
    while( getline(*fs,textline,'\n'))
    {

        tool.parseLog(textline,infoVector);
       // Tool::parseLoginString(infoVector,loginErrorCount);
       // qDebug() << tool.parseString(infoVector[0]);
      //  qDebug() << searchType << " " << infoVector[3];
        if( isFind(infoVector,searchType,keyWord))
        {
            //qDebug() << infoVector[3];
            flag = true;
            if(infoVector[3] != "" && infoVector[3] != "[")
            {
                if(isContain(infoVector[3],levelStr) == true)
                {
                    if( infoVector[3] == "[warning][GLib-GObject]" || infoVector[3] == "[warning][Gtk]" || infoVector[3] == "[warning][guestinfo]")
                    {
                       log_model->setItem(i,0,new QStandardItem("[warning]"));
                    }
                    else if( infoVector[3] == "[critical][GLib-GObject]" )
                    {
                        log_model->setItem(i,0,new QStandardItem("[critical]"));
                    }
                    else if(infoVector[3] == "<error>[1400737835.491674]")
                    {
                        log_model->setItem(i,0,new QStandardItem("[error]"));
                    }
                    else if(infoVector[3] == "<error>[1400737688.966608]")
                    {
                        log_model->setItem(i,0,new QStandardItem("[error]"));
                    }
                    else
                    {
                        log_model->setItem(i,0,new QStandardItem(infoVector[3]));
                    }
                }
                else
                {
                    log_model->setItem(i,0,new QStandardItem(QString("<无>")));
                }
            }
            else
            {
                log_model->setItem(i,0,new QStandardItem(QString("<无>")));
            }
            qDebug() << infoVector[0];
            log_model->setItem(i,1,new QStandardItem(infoVector[0]));
            log_model->setItem(i,2,new QStandardItem(infoVector[1]));
            log_model->setItem(i,3,new QStandardItem(infoVector[2]));
            log_model->setItem(i,4,new QStandardItem(infoVector[4]));

            i++;
        }
    }
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑单元表格
    ui->tableView->setColumnWidth(0,80);
    ui->tableView->setColumnWidth(1,110);
    ui->tableView->setColumnWidth(2,100);
    ui->tableView->setColumnWidth(3,100);
    ui->tableView->setColumnWidth(4,300);

    return flag;

}
void MainWindow::on_pushButton_clicked()
{
    //start button
   keyWord = ui->lineEdit->text();
   bool isSuccess = addContent();
   if( isSuccess == false )
   {
        QMessageBox::information(this,tr("Tips"),tr("您查找的日志内容没有找到，请重新输入关键字或者选择查询方式!!\n"),
                                 QMessageBox::Ok);
   }
   ui->lineEdit->clear();
}

void MainWindow::on_tongji_tabBarClicked(int index)
{
    //展示统计柱状图

}

void MainWindow::on_radioButton_clicked()
{
    //表示按时间搜索
    searchType = 1;
    qDebug() << "radioButton is clicked" << searchType;
}

void MainWindow::on_radioButton_2_clicked()
{
    //表示等级
    searchType = 2;
    qDebug() << "radioButton is clicked" << searchType;
}

void MainWindow::on_radioButton_3_clicked()
{
    //表示按用户
    searchType = 3;
    qDebug() << "radioButton is clicked" << searchType;
}

void MainWindow::on_radioButton_4_clicked()
{
    //按服务名称关键字
    searchType = 4;
}

bool MainWindow::isContain(QString str,QString strArr[])
{
    for(int i = 0; i < 17; i++)
    {
        if( strArr[i] == str)
        {
            return true;
        }
    }
    return false;
}

bool MainWindow::isFind(QVector<QString> infoVector1,int searchType1,QString keyWord1)
{
    if( searchType1 == 1 ) //by time
    {
        if(  keyWord1 != "" )
        {
            int  index = Tool::parseTimeString(infoVector1[0]).indexOf(keyWord1);
            if( index == -1 )
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    if( searchType1 == 2  )//by level
    {
        if( infoVector1[3] == "[warning][guestinfo]")
        {
            infoVector1[3] = "[warning]";
        }
        if(  keyWord1 != "" )
        {
            int  index = infoVector1[3].toUpper().indexOf(keyWord1.toUpper());
            if( index == -1 )
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    if( searchType1 == 4 ) //by service
    {
        if(  keyWord1 != "" )
        {
            int  index = infoVector1[2].toUpper().indexOf(keyWord1.toUpper());
            if( index == -1 )
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }

    return false;

}



void MainWindow::on_actionAbout_triggered()
{
    Dialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionOpen_triggered()
{
    filePath = QFileDialog::getOpenFileName(this,
                     "Open Document",
                      QDir::currentPath(),
                     "Document files (*.doc *.rtf,*.txt);All files(*.*)");
    if( !filePath.isNull() )
    {
         count = 0;
         QMessageBox::information(this, "选择文件", "文件载入成功！", QMessageBox::Ok);
    }
}

void MainWindow::setShowStaticsGraph(map<string,int> &tmpMap)
{
    /*int maxValue1 = 0 ;
    Tool::getMaxValue(loginErrorCount,maxValue1);

    QwtPlot *plot = new QwtPlot(ui->scrollAreaWidgetContents_2);
    plot->setTitle( "统计柱状图" );
    plot->setCanvasBackground( Qt::blue );
    plot->setAxisScale( QwtPlot::yLeft, 0, maxValue1+5 );
    plot->setAxisScale(QwtPlot::xBottom,0,sizeof(tmpMap)*2);
    //plot->setAxisScale(QwtPlot::xBottom,0,1);
    plot->setAutoFillBackground(true);
    plot->insertLegend( new QwtLegend() );
    plot->setAxisTitle( QwtPlot::yLeft, "the count of errors" );
    plot->setAxisTitle( QwtPlot::xBottom, "the user logined" );
   // plot->setStyle(QwtPlotHistogram::Columns);
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setPalette( Qt::green );
    canvas->setBorderRadius( 10 );
    plot->setCanvas( canvas );

    QwtPlotHistogram *histogram = new QwtPlotHistogram();

    histogram->setTitle("ErrorCount");
    histogram->setPen(Qt::blue,4);
    histogram->setBrush(Qt::green);
    histogram->setRenderHint(QwtPlotItem::RenderAntialiased,false);

    //int values[]= {};
    int i = 0;
    map<string,int>::const_iterator it = tmpMap.begin();
    //int values[] = {3,9,6,12};
    QVector<QwtIntervalSample> samples( 1 );
    while( it != tmpMap.end() )
    {
        QwtInterval interval( double( i ), i + 1 );
        interval.setBorderFlags( QwtInterval::ExcludeMaximum );
        //interval.setBorderFlags(QwtInterval::ExcludeMinimum);
        samples[i] = QwtIntervalSample( it->second, interval );
        histogram->setData( new QwtIntervalSeriesData( samples ) );
        it++;
        i++;
    }
   // histogram->Rtti_PlotIntervalCurve;

   // histogram->setData( new QwtIntervalSeriesData( samples ) );

    QwtColumnSymbol *symbol = new QwtColumnSymbol( QwtColumnSymbol::Box );
    symbol->setFrameStyle( QwtColumnSymbol::Raised );
    symbol->setLineWidth( 3 );
    symbol->setPalette( QPalette( histogram->pen().color() ) );
    histogram->setSymbol( symbol );
    //histogram->setStyle(QwtPlotHistogram::Lines);

    histogram->attach(plot);
    plot->resize( 500, 400 );
    plot->show();
    */
    int maxValue1 = 0 ;
    Tool::getMaxValue(loginErrorCount,maxValue1);
    //画图工具
    QwtPlotHistogram *m_histogram;
    QwtPlot *m_plot;
    QVector<QwtPlotHistogram*> m_plotVec;
    //QwtPlotGrid *m_grid;
    m_plot = new QwtPlot(QwtText("用户登录错误统计图"),ui->scrollAreaWidgetContents_2);
    //m_plot->resize(this->width(),this->height());

    m_plot->setAxisTitle( QwtPlot::yLeft,"登陆错误次数" );//设置y轴的标题
    m_plot->setAxisTitle( QwtPlot::xBottom, "登录用户" );//设置x轴的标题
    m_plot->setAxisScale(QwtPlot::yLeft,0,maxValue1+5);//设置y轴最大尺度
    m_plot->setAxisScale(QwtPlot::xBottom,0,tmpMap.size()*2);//设置x轴最大尺度

   // m_grid = new QwtPlotGrid;
   // m_grid->attach(m_plot);
    //右上方的用户说明，对应柱状图
    m_plot->insertLegend(new QwtLegend(),QwtPlot::RightLegend);
    /*
    m_histogram = new QwtPlotHistogram("0");
    m_plotVec.push_back(m_histogram);
    m_histogram = new QwtPlotHistogram("1");
    m_plotVec.push_back(m_histogram);
    m_histogram = new QwtPlotHistogram("2");
    m_plotVec.push_back(m_histogram);
    */
    int i = 0;

    //tmpMap是存储所有用户的map，存储格式为:用户：次数
    map<string,int>::const_iterator it = tmpMap.begin();
    int red = 0;
    int green = 0;
    int blue = 0;
    //这段主要设置柱状图的数据,通过setdata方法
    while( it != tmpMap.end() )
    {
        m_histogram = new QwtPlotHistogram(QString((it->first).data()));
        m_plotVec.push_back(m_histogram);
        red = rand()%255+10;
        green = rand()%255+10;
        blue = rand()%255+10;
        QVector<QwtIntervalSample> samples(1);
        QwtInterval interval(double(i*2),i*2+1);
        interval.setBorderFlags(QwtInterval::ExcludeMaximum);
        samples[0] = QwtIntervalSample(it->second, interval);
        m_plotVec[i]->setBrush(QColor(red,green,blue));
        m_plotVec[i]->setData(new QwtIntervalSeriesData(samples));
        m_plotVec[i]->attach(m_plot);
        it++;
        i++;
    }
    /*
    for(uint i=0;i<3;i++)
    {
        QVector<QwtIntervalSample> samples(1);
        QwtInterval interval(double(i*2),i*2+1);
        interval.setBorderFlags(QwtInterval::ExcludeMaximum);
        if(i==0)
        {
            samples[0] = QwtIntervalSample(32, interval);
            m_plotVec[i]->setBrush(QColor(125,0,0));
        }
        if(i==1)
        {
            samples[0] = QwtIntervalSample(12, interval);
            m_plotVec[i]->setBrush(QColor(0,125,0));
        }
        if(i==2)
        {
            samples[0] = QwtIntervalSample(44, interval);
            m_plotVec[i]->setBrush(QColor(0,0,125));
        }
            m_plotVec[i]->setData(new QwtIntervalSeriesData(samples));
            m_plotVec[i]->attach(m_plot);
    }
    */
    m_plot->resize( 550, ui->scrollAreaWidgetContents_2->height() );
    m_plot->show();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    QVector<QString> tmpVector;
    if( index == 1 && count == 0 )
    {
         count++;
         Tool tool(filePath.toStdString());
         ifstream *fs = tool.readFile(tool.fileName);
         string textline;
         int i = 0;
         qDebug() << "Hello";
         while( getline(*fs,textline,'\n'))
         {
             qDebug() << "Hello";
             tool.parseLog(textline,tmpVector);
             tool.parseLoginString(tmpVector,loginErrorCount);
         }
         map<string,int>::const_iterator it = loginErrorCount.begin();
         while( it != loginErrorCount.end() )
         {
             qDebug() << (it->first).data() << "->" << it->second;
             it++;
         }

         setShowStaticsGraph(loginErrorCount);
    }

}
