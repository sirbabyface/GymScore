#include "formtable.h"
#include "ui_formtable.h"
#include "mainwindow.h"

#include "3rdparty/qtxlsx/xlsxdocument.h"
#include "3rdparty/qtxlsx/xlsxworksheet.h"
#include "model/xlsxsheetmodel.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QStandardPaths>

FormTable::FormTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTable),
    infoWindow(Q_NULLPTR),
    previousPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)),
    currentRow(0),
    doc(Q_NULLPTR),
    sheet(Q_NULLPTR),
    model(Q_NULLPTR)
{
    ui->setupUi(this);
    setWindowTitle(tr("Table Score"));
    setWindowIcon(QIcon(":/img/gymnast.png"));

    connect(ui->buttonOpen, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(saveFile()));
    connect(ui->buttonBlank, SIGNAL(clicked()), this, SLOT(blankInfoWindow()));
    connect(ui->buttonUpdate, SIGNAL(clicked()), this, SLOT(updateInfoWindow()));
    connect(ui->buttonLeft, SIGNAL(clicked()), this, SLOT(moveLeft()));
    connect(ui->buttonRight, SIGNAL(clicked()), this, SLOT(moveRight()));

    //loadFile("C:/Utils/Gym/Docs/Tabela Manhã.xlsx");
}

FormTable::~FormTable()
{
    delete ui;
}


void FormTable::closeEvent (QCloseEvent *event)
{
    if(model == Q_NULLPTR || !model->changed()) {
        QMessageBox::StandardButton resBtn = QMessageBox::question(this, QApplication::applicationName(),
                                                                   tr("Do you really want to exit?\n"),
                                                                   QMessageBox::No | QMessageBox::Yes,
                                                                   QMessageBox::No);
       if (resBtn == QMessageBox::Yes) {
           event->accept();
           infoWindow->close();
       } else {
           event->ignore();
       }
    }
    else {
        // Need to ask for saving
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, QApplication::applicationName(),
                                                                    tr("The results are not saved. Do you to discard and exit?\n"),
                                                                    QMessageBox::Cancel | QMessageBox::Save | QMessageBox::Discard,
                                                                    QMessageBox::Save);
        if (resBtn == QMessageBox::Cancel) {
            event->ignore();
        } else if(resBtn == QMessageBox::Discard) {
            event->accept();
            infoWindow->close();
        }
        else if(saveFile()) {
            event->accept();
            infoWindow->close();
        }
        else {
            event->ignore();
        }
    }
}

void FormTable::setInfoWindow(MainWindow *win)
{
    infoWindow = win;
}

void FormTable::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, QApplication::applicationName() + " - " + tr("Save table"),
                                                    previousPath, "*.xlsx");
    if (filePath.isEmpty()) {
        return;
    }

    previousPath = QDir(filePath).absolutePath();
    loadFile(filePath);
}

void FormTable::loadFile(const QString &filePath)
{
    doc = new QXlsx::Document(filePath);
    if(doc->sheetNames().size() == 0) {
        delete doc;
        sheet = Q_NULLPTR;
        QMessageBox::information(this, QApplication::applicationName(), tr("The file doesn't any sheet. It is not valid to use."));
        return;
    }

    sheet = dynamic_cast<QXlsx::Worksheet *>(doc->sheet(doc->sheetNames().at(0)));
    if (sheet) {
        model = new QXlsx::SheetModel(sheet, ui->tableView);
        ui->tableView->setModel(model);
        foreach (QXlsx::CellRange range, sheet->mergedCells()) {
            ui->tableView->setSpan(range.firstRow()-1, range.firstColumn()-1, range.rowCount(), range.columnCount());
        }
    }
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

bool FormTable::saveFile()
{
    QString filePath = QFileDialog::getSaveFileName(this, QApplication::applicationName() + " - " + tr("Open File"),
                                                    previousPath,
                                                    "*.xlsx");
    if (filePath.isEmpty()) {
        return false;
    }
    previousPath = QDir(filePath).absolutePath();
    if(doc->saveAs(filePath)) {
        QMessageBox::information(this, tr("File saved"), tr("File saved with success."));
        model->setChanged(false);
        return true;
    }
    else {
        QMessageBox::critical(this, tr("Error saving"), tr("Error saving file. The results not saved."));
        return false;
    }
}

void FormTable::moveLeft()
{
    ui->spinBox->setValue(ui->spinBox->value() - 1);
    updateInfoWindow();
}

void FormTable::moveRight()
{
    ui->spinBox->setValue(ui->spinBox->value() + 1);
    updateInfoWindow();
}

void FormTable::blankInfoWindow()
{
    infoWindow->blank();
}


void FormTable::updateInfoWindow()
{
    if(sheet == nullptr) {
        return;
    }

    int row = ui->spinBox->value();
    if(row > sheet->dimension().lastRow()) {
        QMessageBox::warning(this, tr("Invalid row"), tr("Invalid row. It doesn't exists on the table."));
        return;
    }

    infoWindow->loadInformation(row + 1, sheet);
}


