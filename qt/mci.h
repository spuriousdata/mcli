#ifndef MCI_H
#define MCI_H

#include <QtGui/QMainWindow>
#include <QModelIndex>

class StatData;
class GetData;

namespace Ui
{
	class McIClass;
}

class McI : public QMainWindow
{
	Q_OBJECT

public:
	McI(QWidget *parent = 0);
	Ui::McIClass *ui() const;
	void setInputEnabled(bool isEnabled);
	~McI();

public slots:
	void displayStats(QVector<StatData *> &data);
	void displayGet(QVector<GetData *> &data);
	void resizeTreeColumns(const QModelIndex& index);

private:
	Ui::McIClass *m_ui;
};

#endif // MCI_H
