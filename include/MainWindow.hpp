// MainWindow.hpp

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QWidget>
#include <QSlider>

#include "Viewer.hpp"
#include "VoxelGrid.hpp"
#include "VEF.hpp"

namespace Ui {
  class MainWindow;
}

// The main window for the application, it holds the menu as well as the viewer that will render the models.
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(VEF& voxels, QWidget *parent = 0);
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

protected:
  void keyPressEvent(QKeyEvent *e);


private:
  Ui::MainWindow *_ui;
  Viewer* _viewer;
  QMenu* _fileMenu;
  QAction* _openAction;
  QAction* _exportAsObjAction;
  QAction* _exitAction;

  void createMenu();
  void createActions();

private slots:
  void open();
  void exportAsObj();
  void exit();

};

#endif // MAINWINDOW_HPP
