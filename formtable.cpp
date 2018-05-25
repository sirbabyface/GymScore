#include "formtable.h"
#include "ui_formtable.h"
#include "mainwindow.h"

#include "3rdparty/qtxlsx/xlsxdocument.h"
#include "3rdparty/qtxlsx/xlsxworksheet.h"
#include "model/xlsxsheetmodel.h"

#include <QMessageBox>
#include <QFileDialog>

FormTable::FormTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTable),
    infoWindow(Q_NULLPTR),
    currentRow(0),
    doc(Q_NULLPTR),
    sheet(Q_NULLPTR)
{
    ui->setupUi(this);
    setWindowTitle(tr("Gymnastic Table Score"));

    connect(ui->buttonOpen, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(saveFile()));
    connect(ui->buttonBlank, SIGNAL(clicked()), this, SLOT(blankInfoWindow()));
    connect(ui->buttonUpdate, SIGNAL(clicked()), this, SLOT(updateInfoWindow()));
    connect(ui->buttonLeft, SIGNAL(clicked()), this, SLOT(moveLeft()));
    connect(ui->buttonRight, SIGNAL(clicked()), this, SLOT(moveRight()));

    loadFile("C:/Utils/Gym/Docs/Tabela Manhã.xlsx");
}

FormTable::~FormTable()
{
    delete ui;
}

void FormTable::setInfoWindow(MainWindow *win)
{
    infoWindow = win;
}

void FormTable::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open xlsx file", QString(), "*.xlsx");
    if (filePath.isEmpty()) {
        return;
    }

    loadFile(filePath);
}

void FormTable::loadFile(const QString &filePath)
{
    doc = new QXlsx::Document(filePath);
    if(doc->sheetNames().size() == 0) {
        delete doc;
        sheet = Q_NULLPTR;
        QMessageBox::information(this, tr("Information"), tr("The file doesn't have 1 sheet. It is not valid"));
        return;
    }

    sheet = dynamic_cast<QXlsx::Worksheet *>(doc->sheet(doc->sheetNames().at(0)));
    if (sheet) {
        ui->tableView->setModel(new QXlsx::SheetModel(sheet, ui->tableView));
        foreach (QXlsx::CellRange range, sheet->mergedCells()) {
            ui->tableView->setSpan(range.firstRow()-1, range.firstColumn()-1, range.rowCount(), range.columnCount());
        }
    }
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

void FormTable::saveFile()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save results", QString(), "*.xlsx");
    if (filePath.isEmpty()) {
        return;
    }
    if(doc->saveAs(filePath)) {
        QMessageBox::information(this, tr("File saved"), tr("File saved with success."));
    }
    else {
        QMessageBox::critical(this, tr("Error saving"), tr("Error saving file. The results not saved."));
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
    int row = ui->spinBox->value();
    if(row > sheet->dimension().lastRow()) {
        QMessageBox::warning(this, tr("Invalid row"), tr("Invalid row. It doesn't exists on the table."));
        return;
    }

    infoWindow->loadInformation(row + 1, sheet);
}


