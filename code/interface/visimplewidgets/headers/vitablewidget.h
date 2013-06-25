#ifndef VITABLEWIDGET_H
#define VITABLEWIDGET_H

#include <QTableWidget>

class ViTableWidget;

class ViTableRow
{

	friend class ViTableWidget;

	public:

		ViTableRow();
		
		void add(int value);
		void add(double value, int decimalPlaces = 4);
		void add(QString text);
		void add(QWidget *widget);

		int count();

	protected:

		void addToTable(ViTableWidget *table);

	private:

		int mCount;
		QHash<int, QWidget*> mWidgets;
		QHash<int, QString> mStrings;

};

class ViTableWidget : public QTableWidget
{

	Q_OBJECT

	public slots:

		void addRow(ViTableRow *row);
		void addRow(QString text);

		void removeRow(int row);
		void removeRow(QWidget *widget);
		void removeSelectedRows();

	public:

		ViTableWidget(QWidget *parent = 0);

		void adjustHeight();

        void setItem(int row, int column, QString text, Qt::Alignment alignment = Qt::AlignCenter);
		void setItem(int row, int column, QWidget *widget, Qt::Alignment alignment = Qt::AlignCenter); //Takes ownership
        void setItem(int row, int column, QTableWidgetItem *item, Qt::Alignment alignment = Qt::AlignCenter); //Takes ownership

		QHash<int, QWidget*> mWidgets;
		int widgetRow(QWidget *widget);

	protected:

		virtual void paintEvent(QPaintEvent *event);

};

#endif
