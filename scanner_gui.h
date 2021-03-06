#ifndef SCANNER_GUI_H
#define SCANNER_GUI_H

#include <QScopedPointer>
#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRubberBand>
#include <QTcpSocket>
#include <QPainter>
#include <QWizard>
#include <QCloseEvent>
#include <QVector>
#include "videothread.h"
#include "instrument_thread.h"
#include "scanwizard.h"
#include "tool.h"

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class scanner_gui; }
QT_END_NAMESPACE

class scanner_gui : public QMainWindow
{
    Q_OBJECT

    public:
        scanner_gui();
        void init();
        ~scanner_gui();

    signals:
        void insthread_stop();
        void send_coord_to_wizard(QPoint, QRect);
        void cropped_image_coord();
        void height_measured();
        void scan_finished_to_wizard();
        void send_area_to_videothread(qint64);
        void stop_displaying_point();
        void allow_emit_pos(bool);
        void instruments_created();

    private slots:

        //Scan start stop
        void on_Start_scan_button_clicked();
        void stop_scan_button_clicked();
        void closeEvent(QCloseEvent *event) override;

        //Robot control buttons and settings
        void on_robot_connect_button_clicked();
        void on_Y_plus_button_pressed();
        void on_Y_minus_button_pressed();
        void on_X_plus_button_pressed();
        void on_X_minus_button_pressed();
        void on_Z_plus_pressed();
        void on_Z_minus_pressed();
        void on_home_button_clicked();
        void on_stepsize_xy_valueChanged(double arg1);
        void on_stepsize_z_valueChanged(double arg1);
        void on_scan_height_valueChanged(double arg1);
        void read_robot_msg();

        void on_Probe_dropdown_currentTextChanged(const QString &arg1);
        void tools_init();

        void robotBytesWritten(qint64);


        //Camera, Take and process the image
        void Take_img_button_clicked();
        void displayCapturedImage();
        void processCapturedImage(const QImage &img);
        void displayCroppedImage(QRect& rect);
        void resetCamera_button_clicked();
        void displayViewfinder();
        void cv_getframe(QImage);
        void cv_getcoord(bool, int, int, int, int, int, int);
        void cameraError(QString);
        void cameraConnected();
        void on_actionReset_Camera_triggered();
        void receive_scanheight_point(int, int);
        void throw_height_meas_error();

        //Scan settings
        void on_scan_settings_button_clicked();
        void get_sweep_points_amount(int);

        //Measurement instruments slots
        void SA_online(bool);
        void VNA_online(bool);
        void on_datasave_test_clicked();
        void get_trace_data(bool);
        void sa_dataread();
        void confirm_written_bytes(qint64);

        //Wizard slots
        void wizard_robot_to_origin(bool);
        void wizard_mark_background(int);
        void wizard_scan_control(bool);
        void send_to_top_pcb_edge();
        void ask_robot_for_cam_height();
        void ask_for_cam_h();

        void on_myfunction_clicked();

        void on_AddTool_clicked();
        void on_Tool_Tab_Closed(QVector<Tool*>);

private:
        //Widgets
        Ui::scanner_gui *ui;
        ScanWizard * wizard;

        //TCP sockets
        QTcpSocket *_socket_sa;
        QTcpSocket *_socket_vna;
        QTcpSocket *_socket_robot;

        // IP addresses
        const QString sa_ip_address = "192.168.11.4";
        const QString vna_ip_address = "192.168.11.6";
        const QString robot_ip_address = "192.168.11.2";

        //Measurement instruments
        Instrument_Thread * insthread;
        bool sa_connected_bool = false;
        bool vna_connected_bool = false;

        // Camera image sensor dimensions
        const float sensor_width = 4.54;
        const float sensor_height = 3.42;
        const float focal_lenght = 3.81;
        const uint16_t resolution_max_width = 4208;
        const uint16_t resolution_max_height = 3120;

        //Camera distances
        uint32_t camera_distance = 876;
        uint32_t real_height;
        uint32_t camera_distance_2 = 10000;

        //OpenCV and image processing
        cv::Point cv_robot_origin;
        cv::Mat cv_lastImage;
        QImage lastImage;
        QRect croppedOrigin;
        VideoThread* videothread;
        void video_thread_init();
        void instrument_thread_init();

        //PCB, Scan Area distances and sizes
        QPoint origin;
        QPoint pcb_corner;
        QRect pcb_size;
        QPoint scan_pcb_corner;
        QPoint scan_area_corner;
        QRect scan_area_size;
        QRect scan_area_size_px;
        QPoint scan_height_point;

        //Robot functions
        void robot_init();
        void send_robot_coordinates(bool);
        void set_scan_step_sizes();
        void start_scan();
        void stop_scan();

        //Data storage variables and functions
        QString datapath = "C:/Users/Near-field scanner/Documents/Near_Field_Scanner_GUI/datastorage/scan_data/";
        QString matlab_script_path = "C:/Users/Near-field scanner/Documents/Near_Field_Scanner_GUI/datastorage/";
        QString foldername = "SCAN_21_10_2020__15_34_11";
        QString current_scan_datapath;
        uint16_t scan_point = 0;
        ///int hm = 0;
        QVector<Tool*> Tools;


        std::vector<std::vector<std::vector<float>>> data_tensor;
        std::vector<std::vector<float>> temp2d;
        std::vector<float> freq;

        int sweep_points;
        int current_scan_point_x=-1;

        int scan_rows = 0;
        int scan_columns = 0;
        long int save_x = 0;
        long int save_y = 0;
        QByteArray b_data;
        QByteArray robot_raw_data;
        QByteArray f_data;
        int32_t bytes = 0;

        bool first_part = true;
        bool first_part_freq = true;
        bool this_time_already_done = false;

        bool run_scan_cam_h = false;

        bool y_comp = true;


        QTimer * timer2;
        //Minor variables
        QColor laststyle;
        bool time_for_amplitude = false;
        bool robot_first_run = true;
};

#endif // SCANNER_GUI_H
