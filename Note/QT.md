#### Qchart(相当于画板）

> QChart类管理图表系列、图例和轴的图形表示

QChartView(显示出画板)

> QChartView是一个可以显示图表的独立小部件

```c++
void QChart::addSeries(QAbstractSeries *series)
//将序列添加到图表中并获取其所有权,QLineSeries是QAbstractSeries的子类
```

#### QLineSeries

> 画曲线的类

```c++
//将给定的小部件设置为主窗口的中心小部件
void QMainWindow::setCentralWidget(QWidget *widget)
```