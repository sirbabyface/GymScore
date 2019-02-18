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

FormTable::FormTable(MainWindow *infoWindow, QWidget *parent)
    : QWidget(parent),
      m_ui(new Ui::FormTable),
      m_infoWindow(infoWindow),
      m_previousPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)),
      m_currentRow(0),
      m_doc(Q_NULLPTR),
      m_sheet(Q_NULLPTR),
      m_model(Q_NULLPTR),
      m_settings(Q_NULLPTR)
{
    m_ui->setupUi(this);
    setWindowTitle(tr("Table Score"));
    setWindowIcon(QIcon(":/img/gymnast.png"));

    connect(m_ui->buttonOpen, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(m_ui->buttonSave, SIGNAL(clicked()), this, SLOT(saveFile()));
    connect(m_ui->buttonBlank, SIGNAL(clicked()), this, SLOT(blankInfoWindow()));
    connect(m_ui->buttonUpdate, SIGNAL(clicked()), this, SLOT(updateInfoWindow()));
    connect(m_ui->buttonLeft, SIGNAL(clicked()), this, SLOT(moveLeft()));
    connect(m_ui->buttonRight, SIGNAL(clicked()), this, SLOT(moveRight()));
    connect(m_ui->buttonRules, SIGNAL(clicked()), this, SLOT(loadRules()));
    connect(m_ui->tableView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(tableClicked(const QModelIndex&)));

    // For speeding testing
    loadRules(QDir::currentPath() + "/config/ginastica-centro.ini");
    loadFile(QDir::currentPath() + "/../2019-02-16-prova-artistica/Manha2.xlsx");
}

FormTable::~FormTable()
{
    delete m_ui;
}


void FormTable::closeEvent (QCloseEvent *event)
{
    if(m_model == Q_NULLPTR || !m_model->changed()) {
        QMessageBox::StandardButton resBtn = QMessageBox::question(this, QApplication::applicationName(),
                                                                   tr("Do you really want to exit?\n"),
                                                                   QMessageBox::No | QMessageBox::Yes,
                                                                   QMessageBox::No);
       if (resBtn == QMessageBox::Yes) {
           event->accept();
           m_infoWindow->close();
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
            m_infoWindow->close();
        }
        else if(saveFile()) {
            event->accept();
            m_infoWindow->close();
        }
        else {
            event->ignore();
        }
    }
}


void FormTable::setInfoWindow(MainWindow *win)
{
    m_infoWindow = win;
}

void FormTable::openFile()
{
    // Check if rules are load first
    if(m_settings == Q_NULLPTR) {
        QMessageBox::information(this, QApplication::applicationName(),
                                 tr("First load the rules to apply to the competition."));
        return;
    }

    QString filePath = QFileDialog::getOpenFileName(this, QApplication::applicationName() + " - " + tr("Open File"),
                                                    m_previousPath, "*.xlsx");
    if (filePath.isEmpty()) {
        return;
    }

    m_previousPath = QDir(filePath).absolutePath();
    loadFile(filePath);
}

/**
 * @brief Load XLSX file with information about the competition. Only the first
 *        sheet is loaded. The contents of the file are loaded except the first
 *        row, which will be used for the headers of table view.
 */
void FormTable::loadFile(const QString &filePath)
{
    m_doc = new QXlsx::Document(filePath);
    if(m_doc->sheetNames().size() == 0) {
        delete m_doc;
        m_sheet = Q_NULLPTR;
        QMessageBox::information(this, QApplication::applicationName(),
                                 tr("The file doesn't have any sheet. It is not valid to use."));
        return;
    }

    m_sheet = dynamic_cast<QXlsx::Worksheet *>(m_doc->sheet(m_doc->sheetNames().at(0)));
    if (m_sheet) {
        m_model = new QXlsx::SheetModel(m_sheet, m_settings, m_ui->tableView);
        m_ui->tableView->setModel(m_model);
        foreach (QXlsx::CellRange range, m_sheet->mergedCells()) {
            m_ui->tableView->setSpan(range.firstRow()-1, range.firstColumn()-1, range.rowCount(), range.columnCount());
        }

        connect(m_model, SIGNAL(dataChanged(QModelIndex, QModelIndex, QVector<int>)),
                this, SLOT(dataChanged(QModelIndex, QModelIndex, QVector<int>)));

    }
    m_ui->tableView->resizeColumnsToContents();
    m_ui->tableView->resizeRowsToContents();
}

bool FormTable::saveFile()
{
    if(m_doc == Q_NULLPTR) {
        QMessageBox::information(this, QApplication::applicationName(),
                                 tr("No file was loaded. Before saving a file should be opened."));
        return false;
    }

    QString filePath = QFileDialog::getSaveFileName(this, QApplication::applicationName() + " - " + tr("Save File"),
                                                    m_previousPath,
                                                    "*.xlsx");
    if (filePath.isEmpty()) {
        return false;
    }
    m_previousPath = QDir(filePath).absolutePath();
    if(m_doc->saveAs(filePath)) {
        QMessageBox::information(this, tr("File saved"), tr("File saved with success."));
        m_model->setChanged(false);
        return true;
    }
    else {
        QMessageBox::critical(this, tr("Error saving"), tr("Error saving file. The results not saved."));
        return false;
    }
}

void FormTable::moveLeft()
{
    m_ui->spinBox->setValue(m_ui->spinBox->value() - 1);
    updateInfoWindow();
}

void FormTable::moveRight()
{
    m_ui->spinBox->setValue(m_ui->spinBox->value() + 1);
    updateInfoWindow();
}

void FormTable::blankInfoWindow()
{
    m_infoWindow->blank();
}


void FormTable::updateInfoWindow()
{
    if(m_sheet == nullptr) {
        return;
    }

    int row = m_ui->spinBox->value();
    if(row > m_sheet->dimension().lastRow()) {
        QMessageBox::warning(this, tr("Invalid row"), tr("Invalid row. It doesn't exists on the table."));
        return;
    }

    m_infoWindow->loadInformation(row + 1, m_sheet, *m_settings);
}

/**
 * @brief Show dialog to select a ini file with rules
 */
void FormTable::loadRules()
{
    QString filePath = QFileDialog::getOpenFileName(this, QApplication::applicationName() + " - " + tr("Load Rules"),
                                                    QDir::currentPath() + "/config", "*.ini");
    if (filePath.isEmpty()) {
        return;
    }

    loadRules(filePath);
}

/**
 * @brief Loads the rules from file.
 */
void FormTable::loadRules(const QString &filePath)
{
    Settings *settings = new Settings(this);
    settings->load(filePath);
    if(settings->isLoaded()) {
        m_settings = settings;
        // update blank scene
        m_infoWindow->setupBlankScene(*m_settings);
        m_infoWindow->setupScene(*m_settings);

        m_ui->labelRules->setText(m_settings->competitionName());
    }
    else {
        delete settings;
    }
}

void FormTable::dataChanged(const QModelIndex &topLeft,
                            const QModelIndex &bottomRight,
                            const QVector<int> &roles)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);

    if(roles.contains(Qt::EditRole)) {
        m_ui->tableView->resizeColumnsToContents();

        int row = topLeft.row();
        int column = topLeft.column();

        moveToNextEditableCell(row, column);

    }

}

void FormTable::tableClicked(const QModelIndex &index)
{
    int column = index.column() + 1;
    if(m_settings->editableColumns().contains(column)) {
        m_ui->tableView->setCurrentIndex(index);
        m_ui->tableView->edit(index);
    }
}



void FormTable::moveToNextEditableCell(int row, int column)
{
    column++; // For the QModel the column start in 0

    QList<int> editable = m_settings->editableColumns();

    if(column == editable.last()) {
        // Already in last editable column don't do nothing
        return;
    }

    int pos = editable.indexOf(column);
    if(pos == -1) {
        return;
    }
    int nextColumn = editable.at(pos + 1) - 1; // QModel starts in 0

    QModelIndex nextIndex = m_model->index(row, nextColumn);
    m_ui->tableView->setCurrentIndex(nextIndex);
    m_ui->tableView->edit(nextIndex);
}
