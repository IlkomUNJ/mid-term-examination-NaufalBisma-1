#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QPainter>
#include <QMouseEvent>
#include <iostream>
#include <iomanip>
#include <QPixmap>
#include <array> // Masih disimpan untuk Array3x3, meskipun CustomMatrix menggunakan C-style array
#include "CustomMatrix.h"

using namespace std;
// Array3x3 tidak terpakai, tapi bisa dihapus jika tidak ada error. Untuk keamanan, biarkan saja.
using Array3x3 = std::array<std::array<bool, 3>, 3>;


class DrawingCanvas : public QWidget
{
    Q_OBJECT
private:
    const int WINDOW_WIDTH=600;
    const int WINDOW_HEIGHT=400;

public:
    explicit DrawingCanvas(QWidget *parent = nullptr);

    // Slot to clear all points from the canvas
    void clearPoints();
    void paintLines();
    void segmentDetection();

protected:
    // Overridden method to handle painting on the widget
    void paintEvent(QPaintEvent *event) override;

    // Overridden method to handle mouse clicks
    void mousePressEvent(QMouseEvent *event) override;

private:
    // A vector to store all the points drawn by the user
    QVector<QPoint> m_points;

    bool isPaintLinesClicked = false;

    // NEW: Vector untuk menyimpan posisi kandidat segmen yang terdeteksi
    QVector<QPoint> m_detection_candidates;
};
#endif // DRAWINGCANVAS_H
