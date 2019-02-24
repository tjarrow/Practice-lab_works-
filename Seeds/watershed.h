#ifndef WATERSHED_H
#define WATERSHED_H

#include <QMainWindow>
#include <vector>

void watershed(QString path,int color, int color_2, int& amount, std::vector<double>& length, std::vector<double>& width, std::vector<double>& area);


#endif // WATERSHED_H

