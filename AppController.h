#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QWidget>

class UIController;
class MemcacheClient;

class AppController : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(AppController)
public:
	AppController();
	~AppController();
	void run();

public slots:
	void mcConnect();
	void addItem();
	void getItem();
	void deleteItem();

private:
	UIController *ui_controller;
	MemcacheClient *memcache;
};

#endif // APPCONTROLLER_H
