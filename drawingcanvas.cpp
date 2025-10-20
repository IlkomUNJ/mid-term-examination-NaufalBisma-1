#include "drawingcanvas.h"
#include <vector>
#include <cstring>

const bool PATTERN_THICK_CENTER[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

const std::vector<CustomMatrix> IDEAL_WINDOWS = {
    CustomMatrix(PATTERN_THICK_CENTER)
};
// ------------------------------------------------

DrawingCanvas::DrawingCanvas(QWidget *parent)  {
    setMinimumSize(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
    setStyleSheet("background-color: white; border: 1px solid gray;");
}

void DrawingCanvas::clearPoints(){
    m_points.clear();
    m_detection_candidates.clear();
    update();
}

void DrawingCanvas::paintLines(){
    isPaintLinesClicked = true;
    update();
}

void DrawingCanvas::segmentDetection(){
    QPixmap pixmap = this->grab();
    QImage image = pixmap.toImage();

    m_detection_candidates.clear();
    cout << "Detection started. Canvas size: " << image.width() << "x" << image.height() << endl;

    // Loop untuk geser jendela 3x3 (i, j adalah pusat jendela)
    for(int i = 1; i < image.width()-1;i++){
        for(int j = 1; j < image.height()-1;j++){
            bool local_window[3][3] = {false};
            bool is_non_empty = false; // Flag untuk Milestone 1

            // Ambil piksel di jendela 3x3
            for(int m=-1;m<=1;m++){
                for(int n=-1;n<=1;n++){
                    QRgb rgbValue = image.pixel(i+m, j+n);
                    bool is_active = (rgbValue != 0xffffffff);
                    local_window[m+1][n+1] = is_active;
                    if(is_active) is_non_empty = true;
                }
            }

            // Hanya proses jendela yang tidak kosong
            if (!is_non_empty) continue;

            CustomMatrix detected_matrix(local_window);

            bool is_match = false;
            for (const CustomMatrix& ideal_pattern : IDEAL_WINDOWS) {
                if (detected_matrix == ideal_pattern) {
                    is_match = true;
                    break;
                }
            }

            if (is_match) {
                // Posisi i, j adalah pusat piksel yang cocok dengan pola.
                // Inilah koordinat yang harus ditandai.
                m_detection_candidates.append(QPoint(i, j));
            }
        }
    }

    cout << "Detection complete. Found " << m_detection_candidates.size() << " candidates." << endl;
    update();
}

void DrawingCanvas::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 1 & 2: Draw points (Blue) and lines (Red)
    QPen pen(Qt::blue, 5);
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::blue));
    for (const QPoint& point : std::as_const(m_points)) { painter.drawEllipse(point, 3, 3); }

    if(isPaintLinesClicked){
        pen.setColor(Qt::red);
        pen.setWidth(4);
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);
        for(int i=0;i<m_points.size()-1;i+=2){ painter.drawLine(m_points[i], m_points[i+1]); }
        isPaintLinesClicked = false;
        pen.setColor(Qt::blue);
        painter.setPen(pen);
    }

    // 3. Draw detection candidates (Small Rectangle of Purple Color)
    // Penandaan dilakukan di setiap piksel yang cocok (i, j)
    const QColor purpleColor(128, 0, 128); // Pure Purple (Ungu)
    QPen purplePen(purpleColor, 1);
    QBrush purpleBrush(purpleColor); // Mengisi kotak dengan warna ungu

    painter.setPen(purplePen);
    painter.setBrush(purpleBrush);

    const int RECT_SIZE = 3; // Ukuran kotak penanda SANGAT KECIL (3x3 pixels)

    for (const QPoint& center : std::as_const(m_detection_candidates)) {
        // center.x() dan center.y() adalah koordinat pusat piksel yang cocok.
        painter.drawRect(
            center.x() - RECT_SIZE/2,
            center.y() - RECT_SIZE/2,
            RECT_SIZE,
            RECT_SIZE
            );
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event) {
    m_points.append(event->pos());
    update();
}
